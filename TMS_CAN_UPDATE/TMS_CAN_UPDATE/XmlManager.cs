using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Windows.Forms;
namespace TMS_CAN_UPDATE
{
    class XmlManager
    {
        static String xmlFile = "../../TMSDeviceDeploy.xml";
        public static string GetDevNameFromXml(int m_id, int s_id)
        {
            string devName = null;
            XmlDocument xmldoc = new XmlDocument();
            try
            {
                xmldoc.Load(xmlFile);
                XmlElement root = xmldoc.DocumentElement;
                XmlNodeList nodeList_m = root.ChildNodes;
                for (int i = 0; i < nodeList_m.Count; i++)
                {
                    if (m_id.ToString() == nodeList_m[i].Attributes["id"].Value)
                    {
                        if (s_id == 0)
                        {
                            devName = nodeList_m[i].Attributes["name"].Value;
                            break;
                        }
                        else
                        {
                            XmlNodeList nodeList_s = nodeList_m[i].ChildNodes;
                            for (int j = 0; j < nodeList_s.Count; j++)
                            {
                                if (s_id.ToString() == nodeList_s[j].Attributes["id"].Value)
                                {
                                    devName = nodeList_s[j].Attributes["id"].Value;
                                    break;
                                }
                            }
                            break;
                        }
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
            return devName;
        }
        public static Dictionary<string, object>[,] GetDevDevployFromXml()
        {
            Dictionary<string, object>[,] dev = new Dictionary<string, object>[15, 5];
            XmlDocument xmldoc = new XmlDocument();
            try
            {
                xmldoc.Load(xmlFile);
                XmlElement root = xmldoc.DocumentElement;
                XmlNodeList nodeList_m = root.ChildNodes;
                for (int i = 0; i < nodeList_m.Count; i++)
                {
                    int m_id = int.Parse(nodeList_m[i].Attributes["id"].Value);
                    dev[m_id, 0] = new Dictionary<string, object>();
                    dev[m_id, 0]["name"] = nodeList_m[i].Attributes["name"].Value;
             
                    XmlNodeList nodeList_s = nodeList_m[i].ChildNodes;
                    for (int j = 0; j < nodeList_s.Count; j++)
                    {
                        int s_id = int.Parse(nodeList_s[j].Attributes["id"].Value);
                        dev[m_id, s_id] = new Dictionary<string, object>();
                        dev[m_id, s_id]["name"] = nodeList_s[j].Attributes["name"].Value;
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
            return dev;
        }
    }
}
