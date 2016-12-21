using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;
namespace TMS_CAN_UPDATE
{
    public partial class CanUpdateWin : Form
    {
        enum WorkState
        {
            INIT,
            GET_SOFT_VERSION_LIST,
            WAIT_PROGRAM_READY,
            PROGRAMING,
            WAIT_REBOOT,
        };
        WorkState workState = WorkState.INIT;
        Stopwatch watch = new Stopwatch();
        private void TransitionState(WorkState newState)
        {
            if (newState == WorkState.INIT)
            {
                workState = newState;

                comboBox1.Enabled = true;
                serialSwitchBtn.Enabled = true;
                treeView1.Enabled = true;
                checkBox1.Enabled = true;
                groupBox1.Enabled = true;
                UpdateDevBtn.Enabled = true;
                pathTextBox.Enabled = true;
                updateBtn.Enabled = true;
                browseBtn.Enabled = true;
                DevInfoConfBtn.Enabled = true;

                progressBar1.Value = 0;
                watch.Reset();
            }
            else if (newState == WorkState.GET_SOFT_VERSION_LIST)
            {
                workState = newState;

                comboBox1.Enabled = false;
                serialSwitchBtn.Enabled = false;
                treeView1.Enabled = false;
                checkBox1.Enabled = false;
                groupBox1.Enabled = false;
                UpdateDevBtn.Enabled = false;
                pathTextBox.Enabled = false;
                updateBtn.Enabled = false;
                browseBtn.Enabled = false;
                DevInfoConfBtn.Enabled = false;
            }
            else if (newState == WorkState.PROGRAMING)
            {
                workState = newState;

                comboBox1.Enabled = false;
                serialSwitchBtn.Enabled = false;
                treeView1.Enabled = false;
                checkBox1.Enabled = false;
                groupBox1.Enabled = false;
                UpdateDevBtn.Enabled = false;
                pathTextBox.Enabled = false;
                updateBtn.Enabled = false;
                browseBtn.Enabled = false;
                DevInfoConfBtn.Enabled = false;
                
                watch.Reset();
                watch.Start();
            }
        }
        Byte[] cmdBuff = new Byte[10];//响应包长
    
        CanUpdateManager canUpdateManager = null;
        private Dictionary<string, Object>[,] devInfoTable = new Dictionary<string, Object>[15, 5];
        public CanUpdateWin()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            canUpdateManager = new CanUpdateManager();
            canUpdateManager.CommonEvent += canUpdateManager_CommonEvent;
            this.treeView1.CheckBoxes = true;
            
            String[] strs = canUpdateManager.GetPortNames();
            foreach (String str in strs)
            {
                comboBox1.Items.Add(str);
            }
            if (strs.Length != 0)
            {
                comboBox1.SelectedIndex = 0;
            }
            XmlManager.LoadDevOnlineFromXml(devInfoTable);
            updateDevTreeView(devInfoTable);
        }


        void canUpdateManager_CommonEvent(Dictionary<string, object> ev)
        {
            CanUpdateManager.EventType evType = (CanUpdateManager.EventType)ev["EventType"];
            
            if (evType == CanUpdateManager.EventType.CMD_SEND_FAILED)
            {
                string str = "cmd send failed : m_id "+ev["m_id"]+" s_id "+ev["s_id"]+" funcCode "+ev["funcCode"];
                textBox1.AppendText(str);
                TransitionState(WorkState.INIT);
            }
            else if (evType == CanUpdateManager.EventType.FILE_TRANSER_COMPLETE)
            {
                if (!updateOneDevFromTreeView(pathTextBox.Text))
                {
                    MessageBox.Show("设备更新已完成！", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                    TransitionState(WorkState.INIT);
                }
                
            }
            else if (evType == CanUpdateManager.EventType.SERIAL_CANNOT_USE)
            {
                MessageBox.Show("串口发送异常，请确认串口已打开！", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                TransitionState(WorkState.INIT);
            }
            else if (evType == CanUpdateManager.EventType.PRINT)
            {
                textBox1.AppendText((string)ev["STR"]);
            }
            else if (evType == CanUpdateManager.EventType.TRANSMIT_BLK)
            {
                Dictionary<String, Object> dir = new Dictionary<string, object>();
                int offset = (int)ev["TotalBlk"] - (int)ev["Count"];
                progressBar1.Value = offset * 100 / (int)ev["TotalBlk"];
                String str = watch.Elapsed.Minutes.ToString("00") + ":" + watch.Elapsed.Seconds.ToString("00") + ":" + watch.Elapsed.Milliseconds.ToString("000");
                str += " offset " + offset.ToString() + " total " + ((int)ev["TotalBlk"]).ToString()+"\n";
                textBox1.AppendText(str);
            }
            else if (evType == CanUpdateManager.EventType.FILE_TRANSER_COMPLETE)
            {
                if (checkBox1.Checked == true)//文件传输完成，传输下一个设备
                {
                    if (!updateOneDevFromTreeView(pathTextBox.Text))//没有下一个设备
                    {
                        TransitionState(WorkState.INIT);
                    }
                }
                else
                {
                    TransitionState(WorkState.INIT);
                }
                    
            }
            else if (evType == CanUpdateManager.EventType.FILE_TRANSER_FAILED)
            {
                MessageBox.Show("设备升级失败!", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                TransitionState(WorkState.INIT);
            }
            
        }
        private void serialSwitchBtn_Click(object sender, EventArgs e)
        {
            if (serialSwitchBtn.Text == "关闭")
            {
                canUpdateManager.Close();
                serialSwitchBtn.Text = "打开";
                return;
            }
            if (comboBox1.Text == "")
            {
                MessageBox.Show("请选择串口!", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                return;
            }
            if (canUpdateManager.Open(comboBox1.Text))
            {
                serialSwitchBtn.Text = "关闭";
            }
                
        }
        private bool updateOneDevFromTreeView(String filePath)
        {
            for (int i = 0; i < treeView1.Nodes.Count; i++)
            {
                int m_id = int.Parse(treeView1.Nodes[i].Text.Split(' ')[0]);
                if (treeView1.Nodes[i].Checked == true)
                {
                    treeView1.Nodes[i].Checked = false;
                    return canUpdateManager.Transmit(filePath, m_id, 0);
                }
                else
                {
                    for (int j = 0; j < treeView1.Nodes[i].Nodes.Count; j++)
                    {
                        if (treeView1.Nodes[i].Nodes[j].Checked == true)
                        {
                            int s_id = int.Parse(treeView1.Nodes[i].Nodes[j].Text.Split(' ')[0]);
                            treeView1.Nodes[i].Nodes[j].Checked = false;
                            return canUpdateManager.Transmit(filePath, m_id, s_id);
                        }
                    }
                }
            }
            return false;
        }
        private void updateBtn_Click(object sender, EventArgs e)
        {
            
            int m_id = int.Parse(mIndexTextBox.Text);
            int s_id = int.Parse(sIndexTextBox.Text);
            if(!File.Exists(pathTextBox.Text))
            {
                MessageBox.Show("BIN文件不存在!", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                return;
            }
            if (checkBox1.Checked == true)
            {

                if (!canUpdateManager.isValidID(m_id, s_id))
                {
                    MessageBox.Show("请输入有效的主设备ID和自设备ID!", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                    return;
                }
                if(canUpdateManager.Transmit(pathTextBox.Text, m_id, s_id))
                    TransitionState(WorkState.PROGRAMING);
            }
            else 
            {
                if(updateOneDevFromTreeView(pathTextBox.Text))
                    TransitionState(WorkState.PROGRAMING);
            }
            
            
        }
        
        

        private void browseBtn_Click(object sender, EventArgs e)
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
        


        private void button1_Click_2(object sender, EventArgs e)
        {
            textBox1.Clear();
        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
            {
                groupBox1.Enabled = true;
            }
            else
            {
                groupBox1.Enabled = false;
            }
        }

        private void DevInfoConfBtn_Click(object sender, EventArgs e)
        {
            new DevInfoWin().ShowDialog(this);
        }
        Dictionary<string, object>[,] devDepTable = null;
        private void UpdateDevBtn_Click(object sender, EventArgs e)
        {
            treeView1.Nodes.Clear();
            for (int i = 0; i < devInfoTable.GetLength(0); i++)
                for (int j = 0; j < devInfoTable.GetLength(1); j++)
                    devInfoTable[i, j] = null;
            devDepTable = XmlManager.GetDevDevployFromXml();
            if(gerDevVerHandler(null))
                TransitionState(WorkState.GET_SOFT_VERSION_LIST);
        }
        void updateDevTreeView(Dictionary<string, object>[,] devInfoTable)
        {
            treeView1.Nodes.Clear();
            for (int i = 0; i < devInfoTable.GetLength(0); i++)
            {
                if(devInfoTable[i,0] != null)
                {
                    string devName = XmlManager.GetDevNameFromXml(i, 0);
                    devName = devName == null ? "----" : devName;
                    TreeNode mNode = treeView1.Nodes.Add((i << 4).ToString(), i.ToString() + " " + devName + " " + devInfoTable[i, 0]["app"] + " " + devInfoTable[i, 0]["ver"]);
                    for (int j = 1; j < devInfoTable.GetLength(1); j++)
                    {
                        if(devInfoTable[i,j] != null)
                        {
                            devName = XmlManager.GetDevNameFromXml(i, j);
                            devName = devName == null ? "----" : devName;
                            mNode.Nodes.Add((i * 16 + j).ToString(), j.ToString() + " " + devName + " " + devInfoTable[i, j]["app"] + " " + devInfoTable[i, j]["ver"]);
                        }
                        
                    }    
                }
            }
        }
        public delegate void UpdateDevTreeView(Dictionary<string, object>[,] devNodeDir);
        private bool gerDevVerHandler(Dictionary<string, object> devNodeDir)//获得设备版本
        {
            int m_id = 0, s_id = 0;
            int next_m_id = 0, next_s_id = 0;
            if (devNodeDir != null)
            {
                m_id = (int)devNodeDir["m_id"];
                s_id = (int)devNodeDir["s_id"];
                if ((bool)devNodeDir["success"] == true)
                {
                    devInfoTable[m_id, s_id] = devNodeDir;
                    this.Invoke(new UpdateDevTreeView(updateDevTreeView), devInfoTable);
                } 
            }
            for (int i = m_id; i < devDepTable.GetLength(0); i++)
                for (int j = (i == m_id ? s_id+1:0); j < devDepTable.GetLength(1); j++)
                {
                    if (devDepTable[i, j] != null )
                    {
                        next_m_id = i;
                        next_s_id = j;
                        textBox1.AppendText("获取版本信息 m_id:" + next_m_id + " s_id:" + next_s_id + "\n");
                        return this.canUpdateManager.GetDevVerCallback(next_m_id, next_s_id, gerDevVerHandler);
                    }
                }
            TransitionState(WorkState.INIT);//没有下一个版本信息
            XmlManager.SaveDevOnlineToXml(devInfoTable);
            return false;
            
        }
    }
}
