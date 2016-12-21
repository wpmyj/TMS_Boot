using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Windows.Forms;
namespace TMS_CAN_UPDATE
{

    class CanUpdateManager
    {
        
        SerialRcvState rcvState = SerialRcvState.INIT;
        
        SerialPort serial = null;
        ParamTimer cmdTimeoutTimer = null;
        
        public delegate void CommonEventHandler(Dictionary<String,Object> ev);
        public enum EventType {
            TRANSMIT_BLK,
            CMD_SEND_FAILED,
            PRINT,
            FILE_TRANSER_COMPLETE,
            FILE_TRANSER_FAILED,
            SERIAL_CANNOT_USE
        };

        // 基于上面的委托定义事件
        public event CommonEventHandler CommonEvent;
        
        enum SerialRcvState
        {
            INIT,
            FLAG1,
            FLAG2,
            COMPLETE
        };
        private enum IAP_CMD
        {
            GET_SOFTWARE_VERSION = 0,
            PROGRAM_READY,
            PROGRAMING,
            REBOOT,
            FINAL_FUNC_CODE
        }
        
        public CanUpdateManager()
        {
            serial = new SerialPort();
            serial.BaudRate = 115200;
            serial.DataReceived += serial_DataReceived;
            cmdTimeoutTimer = new ParamTimer();
            cmdTimeoutTimer.AutoReset = false;
            cmdTimeoutTimer.Elapsed +=cmdTimeoutTimer_Elapsed;
            
        }

        void cmdTimeoutTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            ParamTimer timer  = (ParamTimer)sender;
            int timeoutCount = (int)timer.Param["TimeoutCount"];
            if ((int)timer.Param["funcCode"] == (int)IAP_CMD.GET_SOFTWARE_VERSION)
            {
                Dictionary<string, object> dir = new Dictionary<string, object>();
                dir.Add("success", false);
                dir.Add("m_id", timer.Param["m_id"]);
                dir.Add("s_id", timer.Param["s_id"]);
                getDevVerHandler(dir);
                return;
            }
            if (++timeoutCount >= 3)//超时3次
            {
                timeoutCount = 0;
                Dictionary<string, object> dir = new Dictionary<string, object>();
                dir.Add("m_id", (int)timer.Param["m_id"]);
                dir.Add("EventType", EventType.CMD_SEND_FAILED);
                dir.Add("s_id", (int)timer.Param["s_id"]);
                dir.Add("funcCode", (int)timer.Param["funcCode"]);
                CommonEvent(dir);
            }
            else {
                int m_id = (int)timer.Param["m_id"];
                int s_id = (int)timer.Param["s_id"];
                int funcCode = (int)timer.Param["funcCode"];
                byte[] cmdVal = (byte[])timer.Param["cmdVal"];
                SendCmd(m_id, s_id, funcCode, cmdVal);
            }
            timer.Param["TimeoutCount"] = timeoutCount;
            
        }
        FileTransfer fileTransfer = null;
        /*****************************************************
        块大小要结合CAN包形式
        can包:
        sdflxxxxxxxx...xxxx
        * 一字节源节点+一字节目的节点+一字节功能吗+一字节长度+长度数据
        * can包要发往canopen数据字典要嘛发给CMD字典(8字节)要嘛发给SEG字典(256字节)
        * 文件块格式2字节cnt+2字节offset+文件块+2字节块CRC(最后一块后两字节为整个文件CRC)
        * 协议字段(6字节)+文件块字段<=256
        * 文件块=cnt+offset+文件段内容<=248
        * 所以文件分片最大248
        **********************************************************/
        private void TransmitNextBlk(int dts_mNode, int dts_sNode)
        {
            if (fileTransfer == null)
                return;
            SendCmd(dts_mNode, dts_sNode, (int)IAP_CMD.PROGRAMING, fileTransfer.NextBlk());
            Dictionary<String, Object> dir = new Dictionary<string, object>();
            dir.Add("EventType", EventType.TRANSMIT_BLK);
            dir.Add("TotalBlk", fileTransfer.TotalBlk);
            dir.Add("Count", fileTransfer.Count);
            CommonEvent(dir);
        }
        private void processedReponse(int dts_mNode, int dts_sNode, int funcCode, UInt32 cmdVal)
        {
            cmdTimeoutTimer.Stop();
            if (funcCode == (int)IAP_CMD.PROGRAM_READY)
            {
                if (cmdVal == 1)//准备就绪，开始传输bin
                {
                    TransmitNextBlk(dts_mNode, dts_sNode);
                }
            }
            else if (funcCode == (int)IAP_CMD.PROGRAMING)
            {
                if (cmdVal == 0)//校验通过
                {
                    byte[] val = { 0, 0, 0, 0 };
                    SendCmd(dts_mNode, dts_sNode, (int)IAP_CMD.REBOOT, val);
                    Dictionary<String, Object> dir = new Dictionary<String, Object>();
                    dir.Add("EventType", EventType.FILE_TRANSER_COMPLETE);
                    CommonEvent(dir);
                }
                else if (cmdVal == 1)//写入成功，请求下一块
                {
                    TransmitNextBlk(dts_mNode, dts_sNode);
                    
                }
                else
                {//IAP过程异常
                    if (cmdVal == 2)
                    {
                        MessageBox.Show("块擦除失败", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                    }
                    else if (cmdVal == 3)
                    {
                        MessageBox.Show("块写入失败", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                    }
                    else if (cmdVal == 4)
                    {
                        MessageBox.Show("块校验错误", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                    }
                    else if (cmdVal == 5)
                    {
                        MessageBox.Show("文件校验错误", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                    }
                    Dictionary<String, Object> dir = new Dictionary<String, Object>();
                    dir.Add("EventType", EventType.FILE_TRANSER_FAILED);
                    CommonEvent(dir);
                }
            }
            else if (funcCode == (int)IAP_CMD.GET_SOFTWARE_VERSION)
            {
                int m_id = dts_mNode;
                int s_id = dts_sNode;
                string app = (cmdVal & 0x08000000) == 0 ? "APP" : "BOOT";
                string ver = (cmdVal & 0x00ffffff).ToString("X");
                Dictionary<string, object> dir = new Dictionary<string, object>();
                dir.Add("app", app);
                dir.Add("ver", ver);
                dir.Add("success",true);
                dir.Add("m_id", m_id);
                dir.Add("s_id", s_id);
                getDevVerHandler(dir);                
            }
        }
        private void parseCmd(byte[] _cmdBuff)
        {
            UInt16 crc = System.BitConverter.ToUInt16(_cmdBuff, 8);
            if (crc == Tools.CRC16(_cmdBuff, 0, 8))
            {
                int dts_mNode = _cmdBuff[2] >> 4;
                int dts_sNode = _cmdBuff[2] & 0x0f;
                int funcCode = _cmdBuff[3];
                UInt32 cmdVal = System.BitConverter.ToUInt32(_cmdBuff, 4);
                processedReponse(dts_mNode, dts_sNode, funcCode, cmdVal);
            }
            else
            {
                MessageBox.Show("UART 响应 CRC 校验错误！", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            }
        }
        public String[] GetPortNames()
        { 
            return SerialPort.GetPortNames();
        }
        byte[] rcv_buff = new byte[0];
        void serial_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Byte[] buff = new Byte[serial.BytesToRead];

            try
            {
                int cnt = serial.Read(buff, 0, buff.Length);
                System.Console.WriteLine("接收数据(" + rcvState.ToString() + ") ：" + Tools.bytes_to_str(buff));
                //int i = 0;
                //while (cnt-- != 0)
                //{
                //    System.Console.WriteLine(buff[i++].ToString("X"));
                //}
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            }
            byte[] print_buff = new byte[0];
            
            while (buff.Length != 0)
            {
                if (rcvState == SerialRcvState.INIT)
                {
                    //System.Console.WriteLine("进入INIT处理：");
                    int pos = Array.IndexOf(buff, (byte)0xaf);
                    if (pos == -1)//没找到0xaf
                    {
                        //System.Console.WriteLine("没找到0xaf.");
                        Array.Resize(ref print_buff, print_buff.Length + buff.Length);
                        Array.Copy(buff, 0, print_buff, print_buff.Length - buff.Length, buff.Length);
                        Array.Resize(ref buff, 0);
                    }
                    else//找到0xaf
                    {

                        Array.Resize(ref print_buff, print_buff.Length + pos);
                        Array.Copy(buff, 0, print_buff, print_buff.Length - pos, pos);
                        Array.Copy(buff, pos, buff, 0, buff.Length - pos);
                        Array.Resize(ref buff, buff.Length - pos);
                        //System.Console.WriteLine("找到0xaf buff: " + bytes_to_str(buff));
                        if (buff.Length == 1)//0xaf在末尾
                        {
                            //System.Console.WriteLine("0xaf在末尾: " );
                            rcvState = SerialRcvState.FLAG1;
                            Array.Resize(ref rcv_buff, 1);
                            rcv_buff[0] = 0xaf;
                            Array.Resize(ref buff, 0);
                        }
                        else if (buff[1] != 0xfa)//0xaf不在末尾，但是后一项不是0xfa
                        {
                            //状态不变，排出0xaf并进入下个循环重新寻找
                            Array.Resize(ref print_buff, print_buff.Length + 1);
                            print_buff[print_buff.Length - 1] = 0xaf;
                            Array.Copy(buff, 1, buff, 0, buff.Length - 1);
                            Array.Resize(ref buff, buff.Length - 1);
                            //System.Console.WriteLine("0xaf不在末尾，但是后一项不是0xfa buff : " + bytes_to_str(buff));
                        }
                        else if (buff.Length < 10)//匹配0xaf和0xfa但不足10字节
                        {
                            rcvState = SerialRcvState.FLAG2;
                            Array.Resize(ref rcv_buff, rcv_buff.Length + buff.Length);
                            Buffer.BlockCopy(buff, 0, rcv_buff, rcv_buff.Length - buff.Length, buff.Length);
                            Array.Resize(ref buff, 0);
                            //System.Console.WriteLine("匹配0xaf和0xfa但不足10字节 " );
                        }
                        else//匹配0xaf和0xfa且足够10字节
                        {
                            rcvState = SerialRcvState.COMPLETE;
                            Array.Resize(ref rcv_buff, 10);
                            Buffer.BlockCopy(buff, 0, rcv_buff, 0, 10);
                            Buffer.BlockCopy(buff, 10, buff, 0, buff.Length - 10);
                            Array.Resize(ref buff, buff.Length - 10);
                            //System.Console.WriteLine("匹配0xaf和0xfa且足够10字节 buff : " + bytes_to_str(buff));
                        }
                    }
                }
                else if (rcvState == SerialRcvState.FLAG1)
                {
                    //System.Console.WriteLine("进入FLAG1处理 : ");
                    if (buff[0] != 0xfa)//后一项不是0xfa
                    {
                        //System.Console.WriteLine("后一项不是0xfa : " + bytes_to_str(buff));
                        //状态复位，追加0xaf到打印缓冲
                        rcvState = SerialRcvState.INIT;
                        Array.Resize(ref print_buff, print_buff.Length + 1);
                        print_buff[print_buff.Length - 1] = 0xaf;
                    }
                    else if (buff.Length < 9)//匹配0xaf和0xfa但不足10字节
                    {
                        rcvState = SerialRcvState.FLAG2;
                        Array.Resize(ref rcv_buff, rcv_buff.Length + buff.Length);
                        Buffer.BlockCopy(buff, 0, rcv_buff, rcv_buff.Length - buff.Length, buff.Length);
                        Array.Resize(ref buff, 0);
                    }
                    else//匹配0xaf和0xfa且足够10字节
                    {
                        rcvState = SerialRcvState.COMPLETE;
                        Array.Resize(ref rcv_buff, rcv_buff.Length + 9);
                        Buffer.BlockCopy(buff, 0, rcv_buff, rcv_buff.Length - 9, 9);
                        Buffer.BlockCopy(buff, 9, buff, 0, buff.Length - 9);
                        Array.Resize(ref buff, buff.Length - 9);
                    }
                }
                else if (rcvState == SerialRcvState.FLAG2)
                {
                    if (buff.Length + rcv_buff.Length < 10)//匹配0xaf和0xfa但不足10字节
                    {
                        //状态不变
                        System.Console.WriteLine("buff len : " + buff.Length + " rcv buff len:" + rcv_buff.Length);
                        Array.Resize(ref rcv_buff, rcv_buff.Length + buff.Length);
                        Buffer.BlockCopy(buff, 0, rcv_buff, rcv_buff.Length - buff.Length, buff.Length);
                        Array.Resize(ref buff, 0);
                    }
                    else//匹配0xaf和0xfa且足够10字节
                    {
                        rcvState = SerialRcvState.COMPLETE;
                        int pre_len = rcv_buff.Length;
                        Array.Resize(ref rcv_buff, 10);
                        Buffer.BlockCopy(buff, 0, rcv_buff, pre_len, rcv_buff.Length - pre_len);
                        Buffer.BlockCopy(buff, rcv_buff.Length - pre_len, buff, 0, buff.Length - rcv_buff.Length + pre_len);
                        Array.Resize(ref buff, buff.Length - rcv_buff.Length + pre_len);
                    }
                }

                if (rcvState == SerialRcvState.COMPLETE)
                {
                    rcvState = SerialRcvState.INIT;
                    parseCmd(rcv_buff);
                    Array.Resize(ref rcv_buff, 0);
                }
            }
            if (print_buff.Length > 0)
            {
                Dictionary<String, Object> dir = new Dictionary<String, Object>();
                dir.Add("EventType", EventType.PRINT);
                dir.Add("STR", System.Text.Encoding.Default.GetString(print_buff));
                CommonEvent(dir);
                Array.Resize(ref print_buff, 0);
            }
        }
        public bool Transmit(String filePath,int m_id, int s_id)
        {
            String fileName = System.IO.Path.GetFileNameWithoutExtension(filePath);
            String devName = XmlManager.GetDevNameFromXml(m_id,s_id);
            String fileNamePrefix = fileName.Substring(0, devName.Length);
            if (devName != fileNamePrefix)
            {
                MessageBox.Show("烧写文件与设备不匹配！", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                return false;
            }
            fileTransfer = new FileTransfer(filePath);
            Byte[] cmdVal = { 0, 0, 0, 0 };
            return SendCmd(m_id, s_id, (int)IAP_CMD.PROGRAM_READY, cmdVal);
        }
        /*
        public void Transmit(String filePath)
        {
            this.filePath = filePath;
            Byte[] cmdVal = { 0, 0, 0, 0 };
            for (int i = 0; i < treeView.Nodes.Count; i++)
            {
                if (treeView.Nodes[i].Checked == true)
                {
                    string[] strs = treeView.Nodes[i].Text.Split(' ');
                    int m_id = int.Parse(strs[0]);
                    int s_id = int.Parse(strs[1]);
                    SendCmd(m_id, s_id, (int)IAP_CMD.PROGRAM_READY, cmdVal);
                    treeView.Nodes[i].Checked = false;
                    break;
                }
                else
                {
                    for (int j = 0; j < treeView.Nodes[i].Nodes.Count; j++)
                    {
                        if (treeView.Nodes[i].Nodes[j].Checked == true)
                        {
                            string[] strs = treeView.Nodes[i].Nodes[j].Text.Split(' ');
                            int m_id = int.Parse(strs[0]);
                            int s_id = int.Parse(strs[1]);
                            SendCmd(m_id, s_id, (int)IAP_CMD.PROGRAM_READY, cmdVal);
                            treeView.Nodes[i].Nodes[j].Checked = true;
                            i = treeView.Nodes.Count;//跳出外层
                            break;
                        }
                    }
                }
            }
        }
         * */
        public bool isValidID(int m_id, int s_id)
        {
            return m_id >= 1 && m_id <= 12 && s_id >= 0 && s_id <= 3;
        }
        private bool SendCmd(int mNode, int sNode, int funcCode, Byte[] cmdVal)
        {
            Byte[] buff = new Byte[8 + cmdVal.Length];
            UInt16 len = (UInt16)buff.Length;
            buff[0] = 0xAF;
            buff[1] = 0xFA;
            Byte[] bytes = System.BitConverter.GetBytes(len);
            Buffer.BlockCopy(bytes, 0, buff, 2, 2);
            buff[4] = (Byte)((mNode << 4) | sNode);
            buff[5] = (Byte)funcCode;
            Buffer.BlockCopy(cmdVal, 0, buff, 6, cmdVal.Length);

            Buffer.BlockCopy(cmdVal, 0, buff, 6, cmdVal.Length);
            UInt16 crc = Tools.CRC16(buff, 0, (UInt32)(buff.Length - 2));
            bytes = System.BitConverter.GetBytes(crc);
            Buffer.BlockCopy(bytes, 0, buff, buff.Length - 2, 2);
            try
            {
                serial.Write(buff, 0, buff.Length);
            }
            catch (Exception)
            {
                Dictionary<String, Object> ev = new Dictionary<String, Object>();
                ev.Add("EventType", EventType.SERIAL_CANNOT_USE);
                CommonEvent(ev);
                return false;
            }
            Dictionary<String,Object> dict = new Dictionary<String,Object>();
            dict.Add("m_id",mNode);
            dict.Add("s_id",sNode);
            dict.Add("funcCode", funcCode);
            dict.Add("cmdVal", cmdVal);
            dict.Add("TimeoutCount", 0);
            cmdTimeoutTimer.Start(4000, dict);//超时时间必须大于sdo超时等待
            return true;
        }
        public void Close()
        {
            serial.Close();
        }
        public bool Open(String serialName)
        {
            serial.PortName = serialName;
            try
            {
                serial.Open();
            }
            catch (Exception)
            {
                MessageBox.Show("串口打开失败，请确认串口是否存在！", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                Dictionary<String, Object> dir = new Dictionary<String, Object>();
                dir.Add("EventType", EventType.FILE_TRANSER_FAILED);
                CommonEvent(dir);
                return false;
            }
            return true;
        }
        public delegate bool CommonHandler(Dictionary<string, object> devNodeDir);
        CommonHandler getDevVerHandler = null;
        public bool GetDevVerCallback(int m_id, int s_id, CommonHandler handler)
        {
            getDevVerHandler = handler;
            Byte[] cmdVal = { 0, 0, 0, 0 };
            return SendCmd(m_id, s_id, (int)IAP_CMD.GET_SOFTWARE_VERSION, cmdVal);
        }
    }
}
