using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace TMS_CAN_UPDATE
{
    public partial class DevInfoWin : Form
    {
        public DevInfoWin()
        {
            InitializeComponent();
        }
        string xmlFile = "../../TMSDeviceDeploy.xml";
        private void Form2_Load(object sender, EventArgs e)
        {
            XmlDocument xmldoc = new XmlDocument();
            try
            {
                xmldoc.Load(xmlFile);
                XmlElement root = xmldoc.DocumentElement;
                XmlNodeList nodeList_m = root.ChildNodes;
                for (int i = 0; i < nodeList_m.Count; i++)
                {
                    string str = nodeList_m[i].Attributes["id"].Value + " " + nodeList_m[i].Attributes["name"].Value;
                    int key = int.Parse(nodeList_m[i].Attributes["id"].Value) << 4;
                    treeView1.Nodes.Add(key.ToString(), str);
                    XmlNodeList nodeList_s = nodeList_m[i].ChildNodes;
                    for (int j = 0; j < nodeList_s.Count; j++)
                    {
                        str = nodeList_s[j].Attributes["id"].Value + " " + nodeList_s[j].Attributes["name"].Value;
                        key = (int.Parse(nodeList_m[i].Attributes["id"].Value) << 4) + (int.Parse(nodeList_s[j].Attributes["id"].Value));
                        treeView1.Nodes[i].Nodes.Add(key.ToString(), str);
                    }
                }
            }
            catch (System.IO.FileNotFoundException)
            {
                MessageBox.Show("找不到配置文件\"TMSDeviceDeploy.xml\"", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            }
            finally
            {
                xmldoc.Clone();
            }
            
        }
        private void button4_Click(object sender, EventArgs e)//保存
        {
            XmlDocument xmlDoc = new XmlDocument();
            //创建类型声明节点    
            XmlNode node = xmlDoc.CreateXmlDeclaration("1.0", "utf-8", "");
            xmlDoc.AppendChild(node);    
            //创建根节点
            XmlNode root = xmlDoc.CreateElement("root");    
            xmlDoc.AppendChild(root);    
            //创建主设备和子设备节点
            for(int i = 0;i < treeView1.Nodes.Count;i++)
            {
                string[] strs = treeView1.Nodes[i].Text.Split(' ');
                node = xmlDoc.CreateNode(XmlNodeType.Element, "main", null);
                XmlAttribute attr = xmlDoc.CreateAttribute("id", null);
                attr.Value = strs[0];
                node.Attributes.Append(attr);
                attr = xmlDoc.CreateAttribute("name",null);
                attr.Value = strs[1];
                node.Attributes.Append(attr);
                root.AppendChild(node);
                for(int j=0;j<treeView1.Nodes[i].Nodes.Count;j++)
                {
                    strs = treeView1.Nodes[i].Nodes[j].Text.Split(' ');
                    XmlNode s_node = xmlDoc.CreateNode(XmlNodeType.Element, "sub", null);
                    attr = xmlDoc.CreateAttribute("id", null);
                    attr.Value = strs[0];
                    s_node.Attributes.Append(attr);
                    attr = xmlDoc.CreateAttribute("name", null);
                    attr.Value = strs[1];
                    s_node.Attributes.Append(attr);
                    node.AppendChild(s_node);
                }
            }
            try    
            {    
                xmlDoc.Save(xmlFile);    
            }    
            catch (System.IO.FileNotFoundException)
            {
                MessageBox.Show("找不到配置文件\"TMSDeviceDeploy.xml\"", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString(), "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            }
            finally
            {
                xmlDoc.Clone();
                Close();
            }    
                
        }    
            
        public string paraTo = null;
        private void button1_Click(object sender, EventArgs e)//添加主设备
        {
            int index = -1;
            if (treeView1.Nodes.Count == 0)
                index = 0;
            index = treeView1.Nodes.IndexOf(treeView1.SelectedNode);
            if (index == -1)
            {
                MessageBox.Show("请选择一个主设备", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                return;
            }    
            new AddDevWin().ShowDialog(this);
            if (paraTo != null)
            {

                int key = int.Parse(paraTo.Split(' ')[0]) << 4;
                treeView1.Nodes.Insert(index+1, key.ToString(),paraTo);
                paraTo = null;
            }
                
        }
        private void button2_Click(object sender, EventArgs e)//添加子设备
        {
            int index = -1;
            index = treeView1.Nodes.IndexOf(treeView1.SelectedNode);
            if (index == -1)
            {
                MessageBox.Show("请选择一个主设备", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                return;
            }
            paraTo = treeView1.SelectedNode.Name;
            new AddDevWin().ShowDialog(this);
            if (paraTo != null)
            {
                int key = int.Parse(treeView1.SelectedNode.Name) | int.Parse(paraTo.Split(' ')[0]);
                treeView1.SelectedNode.Nodes.Add( key.ToString(), paraTo, (treeView1.Nodes.Count << 4).ToString());
                paraTo = null;
            }
        }

        private void button3_Click(object sender, EventArgs e)//删除设备
        {
            if (treeView1.SelectedNode == null)
            {
                MessageBox.Show("请选择一个设备", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            }
            int key = int.Parse(treeView1.SelectedNode.Name);
            System.Console.WriteLine("key "+key);
            if ((key & 0x0f) != 0)//选中子节点
            {
                treeView1.Nodes.Find((key&0xf0).ToString(),false)[0].Nodes.Remove(treeView1.SelectedNode);
            }
            else
            {
                treeView1.Nodes.Remove(treeView1.SelectedNode);
            }
            
        }

        private void button5_Click(object sender, EventArgs e)
        {
            Close();
        }



        
    }
}
