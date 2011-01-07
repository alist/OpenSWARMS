namespace socketXBOXControllerForSWARMS
{
    partial class SWARMSControlWindow
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
            this.panel2 = new System.Windows.Forms.Panel();
            this.proximityVibrateEnabled = new System.Windows.Forms.CheckBox();
            this.enableControllerCheckbox = new System.Windows.Forms.CheckBox();
            this.lblNotConnected = new System.Windows.Forms.Label();
            this.ddlController = new System.Windows.Forms.ComboBox();
            this.label23 = new System.Windows.Forms.Label();
            this.suggestedServoLocationLabel = new System.Windows.Forms.Label();
            this.suggestedMotorSpeedLabel = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.socketConnectButton = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label9 = new System.Windows.Forms.Label();
            this.driveByKeyboardCheckbox = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.labelProjectMotor = new System.Windows.Forms.Label();
            this.labelProjetedServo = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.labelConnectionState = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.labelLastCommand = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.stopButton = new System.Windows.Forms.Button();
            this.ipAddressBox = new System.Windows.Forms.MaskedTextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.portMaskedTextBox = new System.Windows.Forms.MaskedTextBox();
            this.controllerTimer = new System.Windows.Forms.Timer(this.components);
            this.updateSocketFromController = new System.Windows.Forms.Timer(this.components);
            this.connectionErrorsLabel = new System.Windows.Forms.Label();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.SystemColors.ControlDark;
            this.panel2.Controls.Add(this.proximityVibrateEnabled);
            this.panel2.Controls.Add(this.enableControllerCheckbox);
            this.panel2.Controls.Add(this.lblNotConnected);
            this.panel2.Controls.Add(this.ddlController);
            this.panel2.Controls.Add(this.label23);
            this.panel2.Controls.Add(this.suggestedServoLocationLabel);
            this.panel2.Controls.Add(this.suggestedMotorSpeedLabel);
            this.panel2.Controls.Add(this.label22);
            this.panel2.Controls.Add(this.label21);
            this.panel2.Controls.Add(this.label13);
            this.panel2.Location = new System.Drawing.Point(12, 12);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(265, 107);
            this.panel2.TabIndex = 65;
            // 
            // proximityVibrateEnabled
            // 
            this.proximityVibrateEnabled.AutoSize = true;
            this.proximityVibrateEnabled.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.proximityVibrateEnabled.Enabled = false;
            this.proximityVibrateEnabled.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.proximityVibrateEnabled.Location = new System.Drawing.Point(28, 70);
            this.proximityVibrateEnabled.Name = "proximityVibrateEnabled";
            this.proximityVibrateEnabled.Size = new System.Drawing.Size(103, 17);
            this.proximityVibrateEnabled.TabIndex = 61;
            this.proximityVibrateEnabled.Text = "Proximity Vibrate";
            this.proximityVibrateEnabled.UseVisualStyleBackColor = true;
            // 
            // enableControllerCheckbox
            // 
            this.enableControllerCheckbox.AutoSize = true;
            this.enableControllerCheckbox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.enableControllerCheckbox.Checked = true;
            this.enableControllerCheckbox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.enableControllerCheckbox.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.enableControllerCheckbox.Location = new System.Drawing.Point(5, 53);
            this.enableControllerCheckbox.Name = "enableControllerCheckbox";
            this.enableControllerCheckbox.Size = new System.Drawing.Size(126, 17);
            this.enableControllerCheckbox.TabIndex = 61;
            this.enableControllerCheckbox.Text = "Enable Controller (\'E\')";
            this.enableControllerCheckbox.UseVisualStyleBackColor = true;
            // 
            // lblNotConnected
            // 
            this.lblNotConnected.AutoSize = true;
            this.lblNotConnected.ForeColor = System.Drawing.Color.Red;
            this.lblNotConnected.Location = new System.Drawing.Point(80, 91);
            this.lblNotConnected.Name = "lblNotConnected";
            this.lblNotConnected.Size = new System.Drawing.Size(131, 13);
            this.lblNotConnected.TabIndex = 60;
            this.lblNotConnected.Text = "Controller Is Disconnected";
            this.lblNotConnected.Visible = false;
            // 
            // ddlController
            // 
            this.ddlController.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ddlController.FormattingEnabled = true;
            this.ddlController.Items.AddRange(new object[] {
            "Player 1",
            "Player 2",
            "Player 3",
            "Player 4"});
            this.ddlController.Location = new System.Drawing.Point(83, 25);
            this.ddlController.Name = "ddlController";
            this.ddlController.Size = new System.Drawing.Size(132, 21);
            this.ddlController.TabIndex = 59;
            this.ddlController.SelectedIndexChanged += new System.EventHandler(this.ddlController_SelectedIndexChanged);
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label23.Location = new System.Drawing.Point(140, 74);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(63, 13);
            this.label23.TabIndex = 58;
            this.label23.Text = "Sug. Servo:";
            this.label23.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // suggestedServoLocationLabel
            // 
            this.suggestedServoLocationLabel.AutoSize = true;
            this.suggestedServoLocationLabel.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.suggestedServoLocationLabel.Location = new System.Drawing.Point(203, 74);
            this.suggestedServoLocationLabel.Name = "suggestedServoLocationLabel";
            this.suggestedServoLocationLabel.Size = new System.Drawing.Size(13, 13);
            this.suggestedServoLocationLabel.TabIndex = 58;
            this.suggestedServoLocationLabel.Text = "0";
            this.suggestedServoLocationLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // suggestedMotorSpeedLabel
            // 
            this.suggestedMotorSpeedLabel.AutoSize = true;
            this.suggestedMotorSpeedLabel.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.suggestedMotorSpeedLabel.Location = new System.Drawing.Point(203, 54);
            this.suggestedMotorSpeedLabel.Name = "suggestedMotorSpeedLabel";
            this.suggestedMotorSpeedLabel.Size = new System.Drawing.Size(13, 13);
            this.suggestedMotorSpeedLabel.TabIndex = 58;
            this.suggestedMotorSpeedLabel.Text = "0";
            this.suggestedMotorSpeedLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label22.Location = new System.Drawing.Point(140, 53);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(62, 13);
            this.label22.TabIndex = 58;
            this.label22.Text = "Sug. Motor:";
            this.label22.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label21.Location = new System.Drawing.Point(26, 28);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(51, 13);
            this.label21.TabIndex = 58;
            this.label21.Text = "Controller";
            this.label21.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label13.Location = new System.Drawing.Point(91, 5);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(111, 17);
            this.label13.TabIndex = 57;
            this.label13.Text = "XBOX Controller";
            // 
            // socketConnectButton
            // 
            this.socketConnectButton.Location = new System.Drawing.Point(283, 12);
            this.socketConnectButton.Name = "socketConnectButton";
            this.socketConnectButton.Size = new System.Drawing.Size(227, 36);
            this.socketConnectButton.TabIndex = 66;
            this.socketConnectButton.Text = "Connect (\'C\')";
            this.socketConnectButton.UseVisualStyleBackColor = true;
            this.socketConnectButton.Click += new System.EventHandler(this.socketConnectButton_Click);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Location = new System.Drawing.Point(410, 130);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(100, 100);
            this.panel1.TabIndex = 68;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(307, 130);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(101, 17);
            this.label9.TabIndex = 67;
            this.label9.Text = "Key-Drive Box:";
            // 
            // driveByKeyboardCheckbox
            // 
            this.driveByKeyboardCheckbox.AutoSize = true;
            this.driveByKeyboardCheckbox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.driveByKeyboardCheckbox.ForeColor = System.Drawing.Color.DarkBlue;
            this.driveByKeyboardCheckbox.Location = new System.Drawing.Point(319, 150);
            this.driveByKeyboardCheckbox.Name = "driveByKeyboardCheckbox";
            this.driveByKeyboardCheckbox.Size = new System.Drawing.Size(85, 17);
            this.driveByKeyboardCheckbox.TabIndex = 61;
            this.driveByKeyboardCheckbox.Text = "Enabled (\'K\')";
            this.driveByKeyboardCheckbox.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(316, 114);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(199, 13);
            this.label1.TabIndex = 69;
            this.label1.Text = "Press in box to support keyboard control.";
            // 
            // labelProjectMotor
            // 
            this.labelProjectMotor.AutoSize = true;
            this.labelProjectMotor.Location = new System.Drawing.Point(152, 206);
            this.labelProjectMotor.Name = "labelProjectMotor";
            this.labelProjectMotor.Size = new System.Drawing.Size(13, 13);
            this.labelProjectMotor.TabIndex = 79;
            this.labelProjectMotor.Text = "0";
            // 
            // labelProjetedServo
            // 
            this.labelProjetedServo.AutoSize = true;
            this.labelProjetedServo.Location = new System.Drawing.Point(152, 191);
            this.labelProjetedServo.Name = "labelProjetedServo";
            this.labelProjetedServo.Size = new System.Drawing.Size(13, 13);
            this.labelProjetedServo.TabIndex = 78;
            this.labelProjetedServo.Text = "0";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(16, 191);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(89, 13);
            this.label6.TabIndex = 77;
            this.label6.Text = "Projected Speed:";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(16, 206);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(130, 13);
            this.label15.TabIndex = 76;
            this.label15.Text = "Projected Servo Location:";
            // 
            // labelConnectionState
            // 
            this.labelConnectionState.AutoSize = true;
            this.labelConnectionState.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelConnectionState.Location = new System.Drawing.Point(108, 130);
            this.labelConnectionState.Name = "labelConnectionState";
            this.labelConnectionState.Size = new System.Drawing.Size(91, 15);
            this.labelConnectionState.TabIndex = 73;
            this.labelConnectionState.Text = "Not Connected!";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(14, 130);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(72, 15);
            this.label8.TabIndex = 72;
            this.label8.Text = "Connection:";
            // 
            // labelLastCommand
            // 
            this.labelLastCommand.AutoSize = true;
            this.labelLastCommand.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelLastCommand.Location = new System.Drawing.Point(108, 154);
            this.labelLastCommand.Name = "labelLastCommand";
            this.labelLastCommand.Size = new System.Drawing.Size(37, 15);
            this.labelLastCommand.TabIndex = 71;
            this.labelLastCommand.Text = "None";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(14, 154);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(94, 15);
            this.label10.TabIndex = 70;
            this.label10.Text = "Last Command:";
            // 
            // stopButton
            // 
            this.stopButton.Location = new System.Drawing.Point(273, 173);
            this.stopButton.Name = "stopButton";
            this.stopButton.Size = new System.Drawing.Size(131, 49);
            this.stopButton.TabIndex = 80;
            this.stopButton.Text = "STOP! (\'S\')";
            this.stopButton.UseVisualStyleBackColor = true;
            this.stopButton.Click += new System.EventHandler(this.stopButton_Click);
            // 
            // ipAddressBox
            // 
            this.ipAddressBox.Location = new System.Drawing.Point(381, 54);
            this.ipAddressBox.Mask = "##0\\.#00\\.#00\\.#00";
            this.ipAddressBox.Name = "ipAddressBox";
            this.ipAddressBox.Size = new System.Drawing.Size(90, 20);
            this.ipAddressBox.TabIndex = 81;
            this.ipAddressBox.Text = "1921681  2";
            this.ipAddressBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ipAddressBox_KeyDown);
            this.ipAddressBox.MaskInputRejected += new System.Windows.Forms.MaskInputRejectedEventHandler(this.ipAddressBox_MaskInputRejected);
            this.ipAddressBox.Leave += new System.EventHandler(this.ipAddressBox_Leave);
            this.ipAddressBox.Enter += new System.EventHandler(this.ipAddressBox_Enter);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(295, 55);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(80, 17);
            this.label2.TabIndex = 67;
            this.label2.Text = "IP Address:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(337, 81);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(38, 17);
            this.label3.TabIndex = 67;
            this.label3.Text = "Port:";
            // 
            // portMaskedTextBox
            // 
            this.portMaskedTextBox.Location = new System.Drawing.Point(381, 80);
            this.portMaskedTextBox.Mask = "####";
            this.portMaskedTextBox.Name = "portMaskedTextBox";
            this.portMaskedTextBox.Size = new System.Drawing.Size(37, 20);
            this.portMaskedTextBox.TabIndex = 81;
            this.portMaskedTextBox.Text = "2234";
            // 
            // controllerTimer
            // 
            this.controllerTimer.Interval = 20;
            this.controllerTimer.Tick += new System.EventHandler(this.controllerTimer_Tick);
            // 
            // updateSocketFromController
            // 
            this.updateSocketFromController.Interval = 200;
            this.updateSocketFromController.Tick += new System.EventHandler(this.updateSocketFromController_Tick);
            // 
            // connectionErrorsLabel
            // 
            this.connectionErrorsLabel.AutoSize = true;
            this.connectionErrorsLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.connectionErrorsLabel.ForeColor = System.Drawing.Color.DeepPink;
            this.connectionErrorsLabel.Location = new System.Drawing.Point(9, 230);
            this.connectionErrorsLabel.Name = "connectionErrorsLabel";
            this.connectionErrorsLabel.Size = new System.Drawing.Size(239, 15);
            this.connectionErrorsLabel.TabIndex = 73;
            this.connectionErrorsLabel.Text = "This box will hold many errors in the future.";
            this.connectionErrorsLabel.Visible = false;
            // 
            // SWARMSControlWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(520, 252);
            this.Controls.Add(this.portMaskedTextBox);
            this.Controls.Add(this.ipAddressBox);
            this.Controls.Add(this.stopButton);
            this.Controls.Add(this.labelProjectMotor);
            this.Controls.Add(this.labelProjetedServo);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.connectionErrorsLabel);
            this.Controls.Add(this.labelConnectionState);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.labelLastCommand);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.driveByKeyboardCheckbox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.socketConnectButton);
            this.Controls.Add(this.panel2);
            this.Name = "SWARMSControlWindow";
            this.Text = "SWARMS Socket Controller";
            this.Load += new System.EventHandler(this.SWARMSControlWindow_Load);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.CheckBox proximityVibrateEnabled;
        private System.Windows.Forms.CheckBox enableControllerCheckbox;
        private System.Windows.Forms.Label lblNotConnected;
        private System.Windows.Forms.ComboBox ddlController;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Label suggestedServoLocationLabel;
        private System.Windows.Forms.Label suggestedMotorSpeedLabel;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Button socketConnectButton;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.CheckBox driveByKeyboardCheckbox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label labelProjectMotor;
        private System.Windows.Forms.Label labelProjetedServo;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label labelConnectionState;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label labelLastCommand;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.MaskedTextBox ipAddressBox;
        private System.Windows.Forms.Button stopButton;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.MaskedTextBox portMaskedTextBox;
        private System.Windows.Forms.Timer controllerTimer;
        private System.Windows.Forms.Timer updateSocketFromController;
        private System.Windows.Forms.Label connectionErrorsLabel;
    }
}

