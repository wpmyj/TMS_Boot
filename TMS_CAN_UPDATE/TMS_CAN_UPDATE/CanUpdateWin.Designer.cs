namespace TMS_CAN_UPDATE
{
    partial class CanUpdateWin
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.serialSwitchBtn = new System.Windows.Forms.Button();
            this.mIndexTextBox = new System.Windows.Forms.TextBox();
            this.sIndexTextBox = new System.Windows.Forms.TextBox();
            this.updateBtn = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.pathTextBox = new System.Windows.Forms.TextBox();
            this.browseBtn = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.DevInfoConfBtn = new System.Windows.Forms.Button();
            this.UpdateDevBtn = new System.Windows.Forms.Button();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(53, 47);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(80, 20);
            this.comboBox1.TabIndex = 0;
            // 
            // serialSwitchBtn
            // 
            this.serialSwitchBtn.Location = new System.Drawing.Point(200, 44);
            this.serialSwitchBtn.Name = "serialSwitchBtn";
            this.serialSwitchBtn.Size = new System.Drawing.Size(57, 23);
            this.serialSwitchBtn.TabIndex = 1;
            this.serialSwitchBtn.Text = "打开";
            this.serialSwitchBtn.UseVisualStyleBackColor = true;
            this.serialSwitchBtn.Click += new System.EventHandler(this.serialSwitchBtn_Click);
            // 
            // mIndexTextBox
            // 
            this.mIndexTextBox.Location = new System.Drawing.Point(41, 56);
            this.mIndexTextBox.Name = "mIndexTextBox";
            this.mIndexTextBox.Size = new System.Drawing.Size(53, 21);
            this.mIndexTextBox.TabIndex = 3;
            this.mIndexTextBox.Text = "1";
            // 
            // sIndexTextBox
            // 
            this.sIndexTextBox.Location = new System.Drawing.Point(111, 56);
            this.sIndexTextBox.Name = "sIndexTextBox";
            this.sIndexTextBox.Size = new System.Drawing.Size(51, 21);
            this.sIndexTextBox.TabIndex = 4;
            this.sIndexTextBox.Text = "0";
            // 
            // updateBtn
            // 
            this.updateBtn.Location = new System.Drawing.Point(430, 400);
            this.updateBtn.Name = "updateBtn";
            this.updateBtn.Size = new System.Drawing.Size(75, 23);
            this.updateBtn.TabIndex = 5;
            this.updateBtn.Text = "烧写";
            this.updateBtn.UseVisualStyleBackColor = true;
            this.updateBtn.Click += new System.EventHandler(this.updateBtn_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(41, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 6;
            this.label1.Text = "主设备ID";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(109, 21);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 12);
            this.label2.TabIndex = 7;
            this.label2.Text = "子设备ID";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(51, 23);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 12);
            this.label3.TabIndex = 8;
            this.label3.Text = "串口号";
            // 
            // pathTextBox
            // 
            this.pathTextBox.Location = new System.Drawing.Point(305, 363);
            this.pathTextBox.Name = "pathTextBox";
            this.pathTextBox.Size = new System.Drawing.Size(200, 21);
            this.pathTextBox.TabIndex = 9;
            this.pathTextBox.Text = "G:\\宝洪辰工作\\八通线\\can升级\\代码\\CCUProject\\Project\\CCU.bin";
            // 
            // browseBtn
            // 
            this.browseBtn.Location = new System.Drawing.Point(305, 400);
            this.browseBtn.Name = "browseBtn";
            this.browseBtn.Size = new System.Drawing.Size(75, 23);
            this.browseBtn.TabIndex = 10;
            this.browseBtn.Text = "浏览";
            this.browseBtn.UseVisualStyleBackColor = true;
            this.browseBtn.Click += new System.EventHandler(this.browseBtn_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(53, 492);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox1.Size = new System.Drawing.Size(452, 136);
            this.textBox1.TabIndex = 11;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(430, 464);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 12;
            this.button1.Text = "清空";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_2);
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(305, 429);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(200, 24);
            this.progressBar1.TabIndex = 13;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.mIndexTextBox);
            this.groupBox1.Controls.Add(this.sIndexTextBox);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Enabled = false;
            this.groupBox1.Location = new System.Drawing.Point(305, 75);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(200, 100);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(305, 53);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(72, 16);
            this.checkBox1.TabIndex = 15;
            this.checkBox1.Text = "指定地址";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // DevInfoConfBtn
            // 
            this.DevInfoConfBtn.Location = new System.Drawing.Point(305, 464);
            this.DevInfoConfBtn.Name = "DevInfoConfBtn";
            this.DevInfoConfBtn.Size = new System.Drawing.Size(94, 23);
            this.DevInfoConfBtn.TabIndex = 17;
            this.DevInfoConfBtn.Text = "设备信息配置";
            this.DevInfoConfBtn.UseVisualStyleBackColor = true;
            this.DevInfoConfBtn.Click += new System.EventHandler(this.DevInfoConfBtn_Click);
            // 
            // UpdateDevBtn
            // 
            this.UpdateDevBtn.Location = new System.Drawing.Point(305, 198);
            this.UpdateDevBtn.Name = "UpdateDevBtn";
            this.UpdateDevBtn.Size = new System.Drawing.Size(104, 23);
            this.UpdateDevBtn.TabIndex = 18;
            this.UpdateDevBtn.Text = "刷新设备列表";
            this.UpdateDevBtn.UseVisualStyleBackColor = true;
            this.UpdateDevBtn.Click += new System.EventHandler(this.UpdateDevBtn_Click);
            // 
            // treeView1
            // 
            this.treeView1.Location = new System.Drawing.Point(53, 78);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(246, 408);
            this.treeView1.TabIndex = 19;
            // 
            // CanUpdateWin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(550, 640);
            this.Controls.Add(this.treeView1);
            this.Controls.Add(this.UpdateDevBtn);
            this.Controls.Add(this.DevInfoConfBtn);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.browseBtn);
            this.Controls.Add(this.pathTextBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.updateBtn);
            this.Controls.Add(this.serialSwitchBtn);
            this.Controls.Add(this.comboBox1);
            this.Name = "CanUpdateWin";
            this.Text = "CAN升级";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Button serialSwitchBtn;
        private System.Windows.Forms.TextBox mIndexTextBox;
        private System.Windows.Forms.TextBox sIndexTextBox;
        private System.Windows.Forms.Button updateBtn;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox pathTextBox;
        private System.Windows.Forms.Button browseBtn;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.Button DevInfoConfBtn;
        private System.Windows.Forms.Button UpdateDevBtn;
        private System.Windows.Forms.TreeView treeView1;
    }
}

