namespace SWARMS_SHARP_Socket_Host
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.hostPortLabel = new System.Windows.Forms.Label();
            this.connectedWithLabel = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.SocketDataOutgoingRichText = new System.Windows.Forms.RichTextBox();
            this.SocketDataIncommingRichText = new System.Windows.Forms.RichTextBox();
            this.socketConnectedLabel = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.SWARMSDataOutgoingRichText = new System.Windows.Forms.RichTextBox();
            this.SWARMSDataIncommingRichText = new System.Windows.Forms.RichTextBox();
            this.SWARMSConnectedLabel = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.resetServerButton = new System.Windows.Forms.Button();
            this.startWebsocketListeningButton = new System.Windows.Forms.Button();
            this.testSWARMSConnectivityButton = new System.Windows.Forms.Button();
            this.connectSwarmsUSBButton = new System.Windows.Forms.Button();
            this.connectAllButton = new System.Windows.Forms.Button();
            this.asyncUIRefresh = new System.Windows.Forms.Timer(this.components);
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // hostPortLabel
            // 
            this.hostPortLabel.AutoSize = true;
            this.hostPortLabel.Location = new System.Drawing.Point(121, 293);
            this.hostPortLabel.Name = "hostPortLabel";
            this.hostPortLabel.Size = new System.Drawing.Size(67, 13);
            this.hostPortLabel.TabIndex = 76;
            this.hostPortLabel.Text = "Not Running";
            // 
            // connectedWithLabel
            // 
            this.connectedWithLabel.AutoSize = true;
            this.connectedWithLabel.Location = new System.Drawing.Point(54, 270);
            this.connectedWithLabel.Name = "connectedWithLabel";
            this.connectedWithLabel.Size = new System.Drawing.Size(79, 13);
            this.connectedWithLabel.TabIndex = 75;
            this.connectedWithLabel.Text = "Not Connected";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(46, 291);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(60, 15);
            this.label7.TabIndex = 78;
            this.label7.Text = "Host Port:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(10, 251);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(96, 15);
            this.label3.TabIndex = 77;
            this.label3.Text = "Connected With:";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ControlDark;
            this.panel1.Controls.Add(this.SocketDataOutgoingRichText);
            this.panel1.Controls.Add(this.SocketDataIncommingRichText);
            this.panel1.Controls.Add(this.socketConnectedLabel);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.label5);
            this.panel1.Controls.Add(this.label6);
            this.panel1.Location = new System.Drawing.Point(509, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(271, 268);
            this.panel1.TabIndex = 74;
            // 
            // SocketDataOutgoingRichText
            // 
            this.SocketDataOutgoingRichText.Location = new System.Drawing.Point(26, 155);
            this.SocketDataOutgoingRichText.Name = "SocketDataOutgoingRichText";
            this.SocketDataOutgoingRichText.Size = new System.Drawing.Size(237, 78);
            this.SocketDataOutgoingRichText.TabIndex = 87;
            this.SocketDataOutgoingRichText.Text = "";
            // 
            // SocketDataIncommingRichText
            // 
            this.SocketDataIncommingRichText.Location = new System.Drawing.Point(26, 47);
            this.SocketDataIncommingRichText.Name = "SocketDataIncommingRichText";
            this.SocketDataIncommingRichText.Size = new System.Drawing.Size(237, 78);
            this.SocketDataIncommingRichText.TabIndex = 87;
            this.SocketDataIncommingRichText.Text = "";
            // 
            // socketConnectedLabel
            // 
            this.socketConnectedLabel.AutoSize = true;
            this.socketConnectedLabel.ForeColor = System.Drawing.Color.Red;
            this.socketConnectedLabel.Location = new System.Drawing.Point(81, 246);
            this.socketConnectedLabel.Name = "socketConnectedLabel";
            this.socketConnectedLabel.Size = new System.Drawing.Size(120, 13);
            this.socketConnectedLabel.TabIndex = 60;
            this.socketConnectedLabel.Text = "Socket is Disconnected";
            this.socketConnectedLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label4.Location = new System.Drawing.Point(12, 139);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(50, 13);
            this.label4.TabIndex = 57;
            this.label4.Text = "Outgoing";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label5.Location = new System.Drawing.Point(12, 31);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(58, 13);
            this.label5.TabIndex = 57;
            this.label5.Text = "Incomming";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label6.Location = new System.Drawing.Point(97, 12);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(84, 17);
            this.label6.TabIndex = 57;
            this.label6.Text = "Server Data";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.SystemColors.ControlDark;
            this.panel2.Controls.Add(this.SWARMSDataOutgoingRichText);
            this.panel2.Controls.Add(this.SWARMSDataIncommingRichText);
            this.panel2.Controls.Add(this.SWARMSConnectedLabel);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Controls.Add(this.label1);
            this.panel2.Controls.Add(this.label13);
            this.panel2.Location = new System.Drawing.Point(228, 12);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(271, 268);
            this.panel2.TabIndex = 73;
            // 
            // SWARMSDataOutgoingRichText
            // 
            this.SWARMSDataOutgoingRichText.Location = new System.Drawing.Point(26, 155);
            this.SWARMSDataOutgoingRichText.Name = "SWARMSDataOutgoingRichText";
            this.SWARMSDataOutgoingRichText.Size = new System.Drawing.Size(237, 78);
            this.SWARMSDataOutgoingRichText.TabIndex = 87;
            this.SWARMSDataOutgoingRichText.Text = "";
            // 
            // SWARMSDataIncommingRichText
            // 
            this.SWARMSDataIncommingRichText.Location = new System.Drawing.Point(26, 47);
            this.SWARMSDataIncommingRichText.Name = "SWARMSDataIncommingRichText";
            this.SWARMSDataIncommingRichText.Size = new System.Drawing.Size(237, 78);
            this.SWARMSDataIncommingRichText.TabIndex = 87;
            this.SWARMSDataIncommingRichText.Text = "";
            // 
            // SWARMSConnectedLabel
            // 
            this.SWARMSConnectedLabel.AutoSize = true;
            this.SWARMSConnectedLabel.ForeColor = System.Drawing.Color.Red;
            this.SWARMSConnectedLabel.Location = new System.Drawing.Point(62, 246);
            this.SWARMSConnectedLabel.Name = "SWARMSConnectedLabel";
            this.SWARMSConnectedLabel.Size = new System.Drawing.Size(135, 13);
            this.SWARMSConnectedLabel.TabIndex = 60;
            this.SWARMSConnectedLabel.Text = "SWARMS is Disconnected";
            this.SWARMSConnectedLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label2.Location = new System.Drawing.Point(12, 139);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(50, 13);
            this.label2.TabIndex = 57;
            this.label2.Text = "Outgoing";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label1.Location = new System.Drawing.Point(12, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(58, 13);
            this.label1.TabIndex = 57;
            this.label1.Text = "Incomming";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label13.Location = new System.Drawing.Point(83, 12);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(103, 17);
            this.label13.TabIndex = 57;
            this.label13.Text = "SWARMS Data";
            this.label13.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // resetServerButton
            // 
            this.resetServerButton.Location = new System.Drawing.Point(12, 212);
            this.resetServerButton.Name = "resetServerButton";
            this.resetServerButton.Size = new System.Drawing.Size(197, 36);
            this.resetServerButton.TabIndex = 70;
            this.resetServerButton.Text = "Reset Server";
            this.resetServerButton.UseVisualStyleBackColor = true;
            // 
            // startWebsocketListeningButton
            // 
            this.startWebsocketListeningButton.Location = new System.Drawing.Point(12, 170);
            this.startWebsocketListeningButton.Name = "startWebsocketListeningButton";
            this.startWebsocketListeningButton.Size = new System.Drawing.Size(197, 36);
            this.startWebsocketListeningButton.TabIndex = 69;
            this.startWebsocketListeningButton.Text = "Start Websocket Listening";
            this.startWebsocketListeningButton.UseVisualStyleBackColor = true;
            this.startWebsocketListeningButton.Click += new System.EventHandler(this.startWebsocketListeningButton_Click);
            // 
            // testSWARMSConnectivityButton
            // 
            this.testSWARMSConnectivityButton.Location = new System.Drawing.Point(12, 128);
            this.testSWARMSConnectivityButton.Name = "testSWARMSConnectivityButton";
            this.testSWARMSConnectivityButton.Size = new System.Drawing.Size(197, 36);
            this.testSWARMSConnectivityButton.TabIndex = 72;
            this.testSWARMSConnectivityButton.Text = "Test SWARMS Connectivity";
            this.testSWARMSConnectivityButton.UseVisualStyleBackColor = true;
            this.testSWARMSConnectivityButton.Click += new System.EventHandler(this.testSWARMSConnectivityButton_Click);
            // 
            // connectSwarmsUSBButton
            // 
            this.connectSwarmsUSBButton.Location = new System.Drawing.Point(13, 86);
            this.connectSwarmsUSBButton.Name = "connectSwarmsUSBButton";
            this.connectSwarmsUSBButton.Size = new System.Drawing.Size(196, 36);
            this.connectSwarmsUSBButton.TabIndex = 71;
            this.connectSwarmsUSBButton.Text = "Connect Swarms USB";
            this.connectSwarmsUSBButton.UseVisualStyleBackColor = true;
            this.connectSwarmsUSBButton.Click += new System.EventHandler(this.connectSwarmsUSBButton_Click);
            // 
            // connectAllButton
            // 
            this.connectAllButton.Location = new System.Drawing.Point(12, 12);
            this.connectAllButton.Name = "connectAllButton";
            this.connectAllButton.Size = new System.Drawing.Size(210, 67);
            this.connectAllButton.TabIndex = 68;
            this.connectAllButton.Text = "Connect Net and SWARMS";
            this.connectAllButton.UseVisualStyleBackColor = true;
            this.connectAllButton.Click += new System.EventHandler(this.connectAllButton_Click);
            // 
            // asyncUIRefresh
            // 
            this.asyncUIRefresh.Enabled = true;
            this.asyncUIRefresh.Tick += new System.EventHandler(this.asyncUIRefresh_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(794, 318);
            this.Controls.Add(this.hostPortLabel);
            this.Controls.Add(this.connectedWithLabel);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.resetServerButton);
            this.Controls.Add(this.startWebsocketListeningButton);
            this.Controls.Add(this.testSWARMSConnectivityButton);
            this.Controls.Add(this.connectSwarmsUSBButton);
            this.Controls.Add(this.connectAllButton);
            this.Name = "Form1";
            this.Text = "SWARMS LibUsb Socket Server";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label hostPortLabel;
        private System.Windows.Forms.Label connectedWithLabel;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.RichTextBox SocketDataOutgoingRichText;
        private System.Windows.Forms.RichTextBox SocketDataIncommingRichText;
        private System.Windows.Forms.Label socketConnectedLabel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.RichTextBox SWARMSDataOutgoingRichText;
        private System.Windows.Forms.RichTextBox SWARMSDataIncommingRichText;
        private System.Windows.Forms.Label SWARMSConnectedLabel;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Button resetServerButton;
        private System.Windows.Forms.Button startWebsocketListeningButton;
        private System.Windows.Forms.Button testSWARMSConnectivityButton;
        private System.Windows.Forms.Button connectSwarmsUSBButton;
        private System.Windows.Forms.Button connectAllButton;
        private System.Windows.Forms.Timer asyncUIRefresh;
    }
}

