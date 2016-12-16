using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TMS_CAN_UPDATE
{
    public partial class Form3 : Form
    {
        public Form3()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int number;
            if (int.TryParse(textBox1.Text,out number) == false)
            {
                MessageBox.Show("请输入正确的COB_ID。", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                return;
            }
            if (textBox2.Text.Trim()== "")
            {
                MessageBox.Show("请输入正确的设备名称。", "Warning", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                return;
            }
            ((Form2)Owner).paraTo = textBox1.Text + " " + textBox2.Text;
            Close();
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
