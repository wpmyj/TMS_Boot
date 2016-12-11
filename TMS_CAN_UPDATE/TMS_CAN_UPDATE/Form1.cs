using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Diagnostics;
namespace TMS_CAN_UPDATE
{
    public partial class Form1 : Form
    {
        private enum IAP_CMD
        {
            GET_ONLINE_TABLE = 0,
            PROGRAM_READY,
            PROGRAMING,
            REBOOT,
            FINAL_FUNC_CODE
        }
        private void parseCmd(byte[] _cmdBuff)
        {
            UInt16 crc = System.BitConverter.ToUInt16(_cmdBuff, 8);
            if (crc == CRC16(_cmdBuff, 0, 8))
            {
                int dts_mNode = _cmdBuff[2] >> 4;
                int dts_sNode = _cmdBuff[2] & 0x0f;
                int funcCode = _cmdBuff[3];
                UInt32 cmdVal = System.BitConverter.ToUInt32(_cmdBuff, 4);
                processedReponse(dts_mNode, dts_sNode, funcCode, cmdVal);
            }
            else {
                MessageBox.Show("UART 响应 CRC 校验错误！", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            }
        }
        private void processedReponse(int dts_mNode, int dts_sNode, int funcCode, UInt32 cmdVal)
        {
            if (funcCode == (int)IAP_CMD.PROGRAM_READY )
            {
                if(cmdVal == 1)//准备就绪，开始传输bin
                {
                    sendFile(pathTextBox.Text);
                }        
            }
            else if (funcCode == (int)IAP_CMD.PROGRAMING )
            {
                if(cmdVal == 0)//校验通过
                {
                    FileBlkCnt = 0;
                    FileBlkOffset = 0;
                    watch.Stop();
                    progressBar1.Value = 0;
                    byte[] val = { 0, 0, 0, 0 };
                    SendCmd(dts_mNode, dts_sNode, (int)IAP_CMD.REBOOT, val);
                    //MessageBox.Show("传输已完成 , 耗时:" + ((float)(watch.ElapsedMilliseconds)/1000).ToString(), "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                }
                else if(cmdVal == 1)//写入成功，请求下一块
                {
                    sendFile(pathTextBox.Text);
                }
                else{//IAP过程异常
                    if (cmdVal == 2)
                    {
                        MessageBox.Show("块擦除失败", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                    }
                    else if(cmdVal == 3)
                    {
                        MessageBox.Show("块写入失败", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                    }
                    else if(cmdVal == 4)
                    {
                        MessageBox.Show("块校验错误", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                    }
                    else if(cmdVal == 5)
                    {
                        MessageBox.Show("文件校验错误", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                    }
                    FileBlkCnt = 0;
                    FileBlkOffset = 0;
                }
            }

        }
        Stopwatch watch = new Stopwatch();
        const int FILE_BLK_MAX_SIZE = 244;//必须4字节对齐
        int FileBlkCnt = 0;
        int FileBlkOffset = 0;
        long fileLen = 0;
        byte[] fileBuff;
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
        private void sendFile(String name)
        {
            UInt16 crc = 0;
            if (FileBlkCnt == 0)
            {
                watch.Reset();
                watch.Start();
                System.IO.FileStream fsRead = null;
                try
                {
                    fsRead = new System.IO.FileStream(name, System.IO.FileMode.Open);
                }
                catch (Exception )
                {
                    MessageBox.Show("文件打开异常，请检查文件是否存在！", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                    return;
                }
                fileLen = fsRead.Length;
                FileBlkCnt = (int)(fileLen) / FILE_BLK_MAX_SIZE;
                if ((int)(fileLen) % FILE_BLK_MAX_SIZE != 0)
                    FileBlkCnt++;
                FileBlkOffset = 0;
                fileBuff = new byte[fileLen + 2];
                fsRead.Read(fileBuff, 0, (int)fileLen);
                crc = CRC16(fileBuff, 0, (UInt32)fileLen);
                Byte[] bytes = System.BitConverter.GetBytes(crc);
                Buffer.BlockCopy(bytes, 0, fileBuff, (int)fileLen, 2);
                fsRead.Close();
                progressBar1.Value = 0;
            }
            FileBlkOffset++;
            progressBar1.Value = FileBlkOffset * 100 / FileBlkCnt;
            String str = watch.Elapsed.ToString(@"ss\:fff");
            str = String.Format("{0} File block {1:D},{2:D}\r\n", str, FileBlkOffset, FileBlkCnt);
            textBox1.AppendText(str);
            int fileBlkLen = FileBlkOffset < FileBlkCnt ? FILE_BLK_MAX_SIZE : (int)fileLen % FILE_BLK_MAX_SIZE;
            byte[] buff = new byte[fileBlkLen + 6];
            Byte[] tmp = System.BitConverter.GetBytes((UInt16)FileBlkCnt);
            Buffer.BlockCopy(tmp, 0, buff, 0, 2);
            tmp = System.BitConverter.GetBytes((UInt16)FileBlkOffset);
            Buffer.BlockCopy(tmp, 0, buff, 2, 2);

            try
            {
                Buffer.BlockCopy(fileBuff, (FileBlkOffset - 1) * FILE_BLK_MAX_SIZE, buff, 4, fileBlkLen);
                if (FileBlkOffset == FileBlkCnt)
                {
                    crc = CRC16(fileBuff, 0, (UInt32)fileLen);
                }
                else
                {
                    crc = CRC16(buff, 4, (UInt32)fileBlkLen);
                }
                Byte[] bytes = System.BitConverter.GetBytes(crc);
                Buffer.BlockCopy(bytes, 0, buff, (int)fileBlkLen + 4, 2);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            }
            SendCmd(int.Parse(mIndexTextBox.Text), int.Parse(sIndexTextBox.Text), (int)IAP_CMD.PROGRAMING, buff);
            //System.Console.WriteLine("send FileBlkOffset : " + FileBlkOffset);
            if (FileBlkOffset == FileBlkCnt)
            {
                FileBlkCnt = 0;
                FileBlkOffset = 0;
                fileLen = 0;
            }
        }
        private void SendCmd(int mNode, int sNode, int funcCode, Byte[] cmdVal)
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
            UInt16 crc = CRC16(buff, 0, (UInt32)(buff.Length - 2));
            bytes = System.BitConverter.GetBytes(crc);
            Buffer.BlockCopy(bytes, 0, buff, buff.Length - 2, 2);
            try
            {
                serial.Write(buff, 0, buff.Length);
            }
            catch (Exception )
            {
                MessageBox.Show("串口发送异常，请确认串口已打开！", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            }
            //int i = buff.Length, j = 0;
            //System.Console.WriteLine("func :" + funcCode + "  " + FileBlkOffset);
            //while (i-- != 0)
            //    System.Console.Write(buff[j++].ToString("X") + " ");
            //System.Console.WriteLine("");
        }
        
        Byte[] cmdBuff = new Byte[10];//响应包长
        int cmdLen = 0;//已接受响应包长
        void serial_DataReceived1(object sender, SerialDataReceivedEventArgs e)
        {
            Byte[] buff = new Byte[serial.BytesToRead];
            //System.Console.WriteLine("接收数据[] " + FileBlkOffset);
            try {
                int cnt = serial.Read(buff, 0, buff.Length);
                //int i = 0;
                while (cnt-- != 0)
                {
                    //System.Console.WriteLine(buff[i++].ToString("X"));
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            }
            if (cmdLen > 0 && cmdLen != 10)
            {
                int nowLen = buff.Length < 10 - cmdLen ? buff.Length : 10 - cmdLen;
                Buffer.BlockCopy(buff, 0, cmdBuff, cmdLen, nowLen);
                cmdLen += nowLen;
                Byte[] tmp = new Byte[buff.Length - nowLen];
                Buffer.BlockCopy(buff, nowLen, tmp, 0, tmp.Length);
                buff = tmp;
            }
            if (cmdLen == 10)
            {
                parseCmd(cmdBuff);
                cmdLen = 0;
            }
            Byte[] printBuff = null;
            while (buff.Length !=0)
            {
                int i ;
                for (i = 0; i < buff.Length && buff[i] != 0xaf; i++);
                if (i != buff.Length)
                {
                    cmdLen = buff.Length - i < 10 ? buff.Length - i : 10;
                    printBuff = new Byte[buff.Length - cmdLen];
                    Buffer.BlockCopy(buff, 0, printBuff, 0, i);
                    Buffer.BlockCopy(buff, i, cmdBuff, 0, cmdLen);
                    Buffer.BlockCopy(buff, i + cmdLen, printBuff, i, printBuff.Length - i);
                    if (cmdLen == 10)
                    {
                        parseCmd(cmdBuff);
                        cmdLen = 0;
                    }
                    buff = printBuff;
                    continue;
                }
                else
                {
                    printBuff = buff;
                    if (printBuff.Length != 0)
                    {
                        textBox1.AppendText(System.Text.Encoding.Default.GetString(printBuff));
                        //foreach (Byte b in printBuff)
                        //{
                        //    System.Console.WriteLine("{0:X} \r\n", b);
                        //}
                        //System.Console.Write("--------------------\r\n");
                    }
                    return;
                }
            }
        }
        enum serial_rcv_state{
            INIT,
            FLAG1,
            FLAG2,
            COMPLETE
        };
        String bytes_to_str(byte[] bytes)
        {
            String str = "";
            for (int i = 0; i < bytes.Length-1; i++)
            {
                str += bytes[i].ToString("X") + " ";
            }
            str += bytes[bytes.Length - 1].ToString("X");
            return str;
        }
        serial_rcv_state rcv_state = serial_rcv_state.INIT;
        byte[] rcv_buff = new byte[0];
        void serial_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Byte[] buff = new Byte[serial.BytesToRead];
            
            try
            {
                int cnt = serial.Read(buff, 0, buff.Length);
                System.Console.WriteLine("接收数据("+rcv_state.ToString()+") ：" + bytes_to_str(buff));
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
            while(buff.Length!=0)
            {
                if (rcv_state == serial_rcv_state.INIT)
                {
                    //System.Console.WriteLine("进入INIT处理：");
                    int pos = Array.IndexOf(buff, (byte)0xaf);
                    if (pos == -1)//没找到0xaf
                    {
                        //System.Console.WriteLine("没找到0xaf.");
                        Array.Resize(ref print_buff,print_buff.Length+buff.Length);
                        Array.Copy(buff, 0, print_buff, print_buff.Length-buff.Length,buff.Length);
                        Array.Resize(ref buff, 0); 
                    }
                    else//找到0xaf
                    {
                        
                        Array.Resize(ref print_buff, print_buff.Length + pos);
                        Array.Copy(buff,0,print_buff, print_buff.Length - pos, pos);
                        Array.Copy(buff, pos, buff, 0, buff.Length - pos);
                        Array.Resize(ref buff, buff.Length-pos);
                        //System.Console.WriteLine("找到0xaf buff: " + bytes_to_str(buff));
                        if (buff.Length == 1)//0xaf在末尾
                        {
                            //System.Console.WriteLine("0xaf在末尾: " );
                            rcv_state = serial_rcv_state.FLAG1;
                            Array.Resize(ref rcv_buff, 1);
                            rcv_buff[0] = 0xaf;
                            Array.Resize(ref buff,0);
                        }
                        else if (buff[1] != 0xfa)//0xaf不在末尾，但是后一项不是0xfa
                        {
                            //状态不变，排出0xaf并进入下个循环重新寻找
                            Array.Resize(ref print_buff,print_buff.Length+1);
                            print_buff[print_buff.Length - 1] = 0xaf;
                            Array.Copy(buff,1,buff,0,buff.Length-1);
                            Array.Resize(ref buff,buff.Length-1);
                            //System.Console.WriteLine("0xaf不在末尾，但是后一项不是0xfa buff : " + bytes_to_str(buff));
                        }
                        else if (buff.Length < 10)//匹配0xaf和0xfa但不足10字节
                        {
                            rcv_state = serial_rcv_state.FLAG2;
                            Array.Resize(ref rcv_buff, rcv_buff.Length + buff.Length);
                            Buffer.BlockCopy(buff, 0, rcv_buff, rcv_buff.Length - buff.Length, buff.Length);
                            Array.Resize(ref buff, 0);
                            //System.Console.WriteLine("匹配0xaf和0xfa但不足10字节 " );
                        }
                        else//匹配0xaf和0xfa且足够10字节
                        {
                            rcv_state = serial_rcv_state.COMPLETE;
                            Array.Resize(ref rcv_buff, 10);
                            Buffer.BlockCopy(buff, 0, rcv_buff, 0, 10);
                            Buffer.BlockCopy(buff, 10, buff, 0, buff.Length - 10);
                            Array.Resize(ref buff,buff.Length-10);
                            //System.Console.WriteLine("匹配0xaf和0xfa且足够10字节 buff : " + bytes_to_str(buff));
                        }
                    }
                }
                else if (rcv_state == serial_rcv_state.FLAG1)
                {
                    //System.Console.WriteLine("进入FLAG1处理 : ");
                    if (buff[0] != 0xfa)//后一项不是0xfa
                    {
                        //System.Console.WriteLine("后一项不是0xfa : " + bytes_to_str(buff));
                        //状态复位，追加0xaf到打印缓冲
                        rcv_state = serial_rcv_state.INIT;
                        Array.Resize(ref print_buff, print_buff.Length + 1);
                        print_buff[print_buff.Length - 1] = 0xaf;
                    }
                    else if (buff.Length < 9)//匹配0xaf和0xfa但不足10字节
                    {
                        rcv_state = serial_rcv_state.FLAG2;
                        Array.Resize(ref rcv_buff, rcv_buff.Length + buff.Length);
                        Buffer.BlockCopy(buff, 0, rcv_buff, rcv_buff.Length - buff.Length, buff.Length);
                        Array.Resize(ref buff, 0);
                    }
                    else//匹配0xaf和0xfa且足够10字节
                    {
                        rcv_state = serial_rcv_state.COMPLETE;
                        Array.Resize(ref rcv_buff, rcv_buff.Length + 9);
                        Buffer.BlockCopy(buff, 0, rcv_buff, rcv_buff.Length - 9, 9);
                        Buffer.BlockCopy(buff, 9, buff, 0,buff.Length- 9);
                        Array.Resize(ref buff, buff.Length - 9);
                    }
                }
                else if (rcv_state == serial_rcv_state.FLAG2)
                {
                    if (buff.Length + rcv_buff.Length < 10)//匹配0xaf和0xfa但不足10字节
                    {
                        //状态不变
                        Array.Resize(ref rcv_buff, rcv_buff.Length + buff.Length);
                        Buffer.BlockCopy(buff, 0, rcv_buff, rcv_buff.Length - buff.Length, buff.Length);
                        Array.Resize(ref buff, 0);
                    }
                    else//匹配0xaf和0xfa且足够10字节
                    {
                        rcv_state = serial_rcv_state.COMPLETE;
                        int pre_len = rcv_buff.Length;
                        Array.Resize(ref rcv_buff, 10);
                        Buffer.BlockCopy(buff, 0, rcv_buff, pre_len, rcv_buff.Length-pre_len);
                        Buffer.BlockCopy(buff, rcv_buff.Length - pre_len, buff, 0, buff.Length-rcv_buff.Length+pre_len);
                        Array.Resize(ref buff, buff.Length - rcv_buff.Length + pre_len);
                    }
                }

                if (rcv_state == serial_rcv_state.COMPLETE)
                {
                    rcv_state = serial_rcv_state.INIT;
                    parseCmd(rcv_buff);
                    Array.Resize(ref rcv_buff,0);
                }
            }
            if (print_buff.Length > 0)
            {
                textBox1.AppendText(System.Text.Encoding.Default.GetString(print_buff));
                Array.Resize(ref print_buff, 0);
            }
        }
        SerialPort serial = new SerialPort();
        public Form1()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            serial.BaudRate = 115200;
            String[] strs = SerialPort.GetPortNames();
            foreach (String str in strs)
            {
                comboBox1.Items.Add(str);
            }
            if (strs.Length != 0)
            {
                comboBox1.SelectedIndex = 0;
            }
            closeBtn.Enabled = false;
            serial.DataReceived += serial_DataReceived;
        }
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            
            

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (comboBox1.Text == "")
            {
                MessageBox.Show("请选择串口!", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                return;
            }
            serial.PortName = comboBox1.Text;
            try {
                serial.Open();
            }
            catch(Exception ){
                MessageBox.Show("串口打开失败，请确认串口是否存在！", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            }
            openBtn.Enabled = false;
            closeBtn.Enabled = true;
        }



        private void button2_Click(object sender, EventArgs e)
        {
            serial.Close();
            closeBtn.Enabled = false;
            openBtn.Enabled = true;
        }

        private void updateBtn_Click(object sender, EventArgs e)
        {
            FileBlkCnt = 0;
            //sendFile(pathTextBox.Text);
            Byte[] cmdVal = {0,0,0,0};
            SendCmd(int.Parse(mIndexTextBox.Text), int.Parse(sIndexTextBox.Text), (int)IAP_CMD.PROGRAM_READY, cmdVal);

        }
        
        

        private void updateBtn1_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                this.pathTextBox.Text = dialog.FileName;
            }
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
        }
        Byte[] auchCRCHi ={
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
            0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
            0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
            0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
            0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
            0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
            0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
            0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
            0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
            0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
            0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
            0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
            0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
            0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
            0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
            0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
            0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
            0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
            };
        Byte[] auchCRCLo={
            0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
            0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
            0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
            0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
            0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
            0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
            0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
            0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
            0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
            0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
            0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
            0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
            0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
            0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
            0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
            0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
            0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
            0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
            0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
            0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
            0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
            0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
            0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
            0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
            0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
            0x43, 0x83, 0x41, 0x81, 0x80, 0x40
            };

        UInt16 CRC16(Byte[] puchMsg,int offset, UInt32 usDataLen)
        {
            Byte uchCRCHi = 0xFF ;
            Byte uchCRCLo = 0xFF ;
            Byte uIndex ;
            //String str = watch.Elapsed.ToString(@"ss\:fff");
            //long ticks = watch.ElapsedTicks;
            //long fre = Stopwatch.Frequency;
            //str = String.Format("CRC start : {0:D}(us) --- len : {1:D}\r\n", ticks * 1000000 / fre, usDataLen);
            //textBox1.AppendText(str);
            for (int i = offset; i < offset+usDataLen; i++)
            {
                uIndex = (Byte)(uchCRCHi ^ puchMsg[i]);
	            uchCRCHi = (Byte)(uchCRCLo ^ auchCRCHi[uIndex]);
	            uchCRCLo = auchCRCLo[uIndex];
            }
            //ticks = watch.ElapsedTicks;
            //str = String.Format("CRC stop : {0:D}(us)\r\n", ticks * 1000000 / fre);
            //textBox1.AppendText(str);
            return (UInt16)(uchCRCHi << 8 | uchCRCLo) ;
        }

        private void button1_Click_2(object sender, EventArgs e)
        {
            textBox1.Clear();
        }
    }
}
