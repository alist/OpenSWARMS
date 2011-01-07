using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;
using Input = Microsoft.Xna.Framework.Input; // to provide shorthand to clear up ambiguities

namespace socketXBOXControllerForSWARMS
{
    public partial class SWARMSControlWindow : Form
    {
	    bool connected = false; // is the system connected
	    int servo_location = 0;
	    int motor_speed = 0;
	    int motor_speed_min = -100;//actual 44;
	    int motor_speed_max = 100; //actual 59;
	    int servo_dir_min = -100;
	    int servo_dir_max = 100;
	    double last_sensor1_value = 0;
	    double last_sensor2_value = 0;
	    double last_sensor3_value = 0;
	    double last_sensor4_value = 0;
	    double last_sensor5_value = 0;
	    double last_sensor6_value = 0;
	    double last_sensor7_value = 0;
	    bool servo_needs_upd = false;
	    bool motor_needs_upd = false;
	    int button_pressed = 0;

	    float rawRightTrigger = 0;
	    float rawLeftTrigger = 0;
	    float rawRightThumbstickX = 0;
	    float rawRightThumbstickY = 0;
	    float rawLeftThumbstickX = 0;
	    float rawLeftThumbstickY = 0;

	    int XBOXSuggestedScaledSpeedValue;
	    int XBOXSuggestedScaledServoValue;

	    GamePadState gamePadState;
	    GamePadState previousState;
	    PlayerIndex playerIndex = PlayerIndex.One;

	    Socket comSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
       // System.Net.IPEndPoint remotePoint = new System.Net.IPEndPoint(localhost,
	    public SWARMSControlWindow()
	    {
		    InitializeComponent();
	    }

	    private void SWARMSControlWindow_Load(object sender, EventArgs e)
	    {
		    this.ddlController.SelectedIndex = 0;
		    this.controllerTimer.Start();
	    }

	    private void stopButton_Click(object sender, EventArgs e)
	    {

	    }

	    private void ipAddressBox_KeyDown(object sender, KeyEventArgs e)
	    {
		    // Cycle through the mask fields  
		    if(e.KeyCode == System.Windows.Forms.Keys.Tab)  
		    {  
			    int pos = ipAddressBox.SelectionStart;  
			    int max = (ipAddressBox.MaskedTextProvider.Length - ipAddressBox.MaskedTextProvider.EditPositionCount);  
			    int nextField = 0;  

			    for(int i = 0; i < ipAddressBox.MaskedTextProvider.Length; i++)  
			    {  
				    if(!ipAddressBox.MaskedTextProvider.IsEditPosition(i) && (pos + max) >= i)  
					    nextField = i;  
			    }  
			    nextField += 1;  

			    // We're done, enable the TabStop property again  
			    if(pos == nextField)  
				    ipAddressBox_Leave(this, e);  

			    ipAddressBox.SelectionStart = nextField;  
		    }
	    }

	    private void ipAddressBox_Enter(object sender, EventArgs e)
	    {
		    // Resets the cursor when we enter the textbox  
		    ipAddressBox.SelectionStart = 0;
		    // Disable the TabStop property to prevent the form and its controls to catch the Tab key  
		    foreach (Control c in this.Controls)
			    c.TabStop = false;  
	    }

	    private void ipAddressBox_Leave(object sender, EventArgs e)
	    {
		    // Resets the cursor when we leave the textbox  
		    ipAddressBox.SelectionStart = 0;
		    // Enable the TabStop property so we can cycle through the form controls again  
		    foreach (Control c in this.Controls)
		    c.TabStop = true;  
	    }

	    private void ddlController_SelectedIndexChanged(object sender, EventArgs e)
	    {
	       switch (this.ddlController.SelectedIndex)
				    {
					    case 0: playerIndex = PlayerIndex.One; break;
					    case 1: playerIndex = PlayerIndex.Two; break;
					    case 2: playerIndex = PlayerIndex.Three; break;
					    case 3: playerIndex = PlayerIndex.Four; break;
					    default: playerIndex = PlayerIndex.One; break;
				    }
	    }

	    private void scaleControllerValuesToImplement()
	    {
		    float scaledRightTrigger = rawRightTrigger * 100;
		    float scaledLeftTrigger = rawLeftTrigger * 100;
		    float scaledRightThumbstickX = rawRightThumbstickX * 1; //not used
		    float scaledRightThumbstickY = rawRightThumbstickY * 100;
		    float scaledLeftThumbstickX = rawLeftThumbstickX * 100;
		    float scaledLeftThumbstickY = rawLeftThumbstickY * 1;

		    int suggestedMotorValuePerTriggers = 0;
		    int suggestedMotorValuePerRightThumbstick = 0;
		    int suggestedServoValuePerLeftThumbstick = 0;

		    suggestedMotorValuePerRightThumbstick = suggestedMotorValuePerRightThumbstick + (int)scaledRightThumbstickY;
		    suggestedServoValuePerLeftThumbstick = suggestedServoValuePerLeftThumbstick + (int)scaledLeftThumbstickX;
		    if (scaledRightTrigger > scaledLeftTrigger)
		    {
			    suggestedMotorValuePerTriggers = suggestedMotorValuePerTriggers + (int)scaledRightTrigger;
		    }
		    else
		    {
			    suggestedMotorValuePerTriggers = suggestedMotorValuePerTriggers - (int)scaledLeftTrigger;
		    }

		    if (suggestedMotorValuePerTriggers != 0)
		    {
			    XBOXSuggestedScaledSpeedValue = suggestedMotorValuePerTriggers;
		    }
		    else
		    {
			    XBOXSuggestedScaledSpeedValue = suggestedMotorValuePerRightThumbstick;
		    }
    		
		    XBOXSuggestedScaledServoValue = suggestedServoValuePerLeftThumbstick;

		    this.suggestedMotorSpeedLabel.Text = XBOXSuggestedScaledSpeedValue.ToString();
		    this.suggestedServoLocationLabel.Text = XBOXSuggestedScaledServoValue.ToString();

	    }

	    private void UpdateControllerState()
	    {
		    //Get the new gamepad state and save the old state.
		    this.previousState = this.gamePadState;
		    this.gamePadState = GamePad.GetState(this.playerIndex);
		    //If the controller is not connected, let the user know
		    this.lblNotConnected.Visible = !this.gamePadState.IsConnected;
		    //I personally prefer to only update the buttons if their state has been changed. 
		    if (!this.gamePadState.Buttons.Equals(this.previousState.Buttons))
		    {
			    /*
			    this.buttonA.Checked = (this.gamePadState.Buttons.A == Input.ButtonState.Pressed);
			    this.buttonB.Checked = (this.gamePadState.Buttons.B == Input.ButtonState.Pressed);
			    this.buttonX.Checked = (this.gamePadState.Buttons.X == Input.ButtonState.Pressed);
			    this.buttonY.Checked = (this.gamePadState.Buttons.Y == Input.ButtonState.Pressed);
			    this.buttonLeftShoulder.Checked = (this.gamePadState.Buttons.LeftShoulder == Input.ButtonState.Pressed);
			    this.buttonRightShoulder.Checked = (this.gamePadState.Buttons.RightShoulder == Input.ButtonState.Pressed);
			    this.buttonStart.Checked = (this.gamePadState.Buttons.Start == Input.ButtonState.Pressed);
			    this.buttonBack.Checked = (this.gamePadState.Buttons.Back == Input.ButtonState.Pressed);
			    this.buttonLeftStick.Checked = (this.gamePadState.Buttons.LeftStick == Input.ButtonState.Pressed);
			    this.buttonRightStick.Checked = (this.gamePadState.Buttons.RightStick == Input.ButtonState.Pressed);
			     */
		    }
		    if (!this.gamePadState.DPad.Equals(this.previousState.DPad))
		    {
			    /*
			    this.buttonUp.Checked = (this.gamePadState.DPad.Up == Input.ButtonState.Pressed);
			    this.buttonDown.Checked = (this.gamePadState.DPad.Down == Input.ButtonState.Pressed);
			    this.buttonLeft.Checked = (this.gamePadState.DPad.Left == Input.ButtonState.Pressed);
			    this.buttonRight.Checked = (this.gamePadState.DPad.Right == Input.ButtonState.Pressed);
    		
			     */
		    }

		    //Update the position of the thumb sticks
		    //since the thumbsticks can return a number between -1.0 and +1.0 I had to shift (add 1.0)
		    //and scale (mutiplication by 100/2, or 50) to get the numbers to be in the range of 0-100
		    //for the progress bar
		    /*
		    this.x1Position.Value = (int)((this.gamePadState.ThumbSticks.Left.X + 1.0f) * 100.0f / 2.0f);
		    this.y1Position.Value = (int)((this.gamePadState.ThumbSticks.Left.Y + 1.0f) * 100.0f / 2.0f);
		    this.x2position.Value = (int)((this.gamePadState.ThumbSticks.Right.X + 1.0f) * 100.0f / 2.0f);
		    this.y2position.Value = (int)((this.gamePadState.ThumbSticks.Right.Y + 1.0f) * 100.0f / 2.0f);
		     * */
		    rawRightThumbstickX = this.gamePadState.ThumbSticks.Right.X;
		    rawRightThumbstickY = this.gamePadState.ThumbSticks.Right.Y;
		    rawLeftThumbstickX = this.gamePadState.ThumbSticks.Left.X;
		    rawLeftThumbstickY = this.gamePadState.ThumbSticks.Left.Y;


		    //The triggers return a value between 0.0 and 1.0.  I only needed to scale these values for
		    //the progress bar
		    /*
		    this.leftTriggerPosition.Value = (int)((this.gamePadState.Triggers.Left * 100));
		    this.rightTriggerPosition.Value = (int)(this.gamePadState.Triggers.Right * 100);
		     * */
		    rawLeftTrigger = (this.gamePadState.Triggers.Left);
		    rawRightTrigger = (this.gamePadState.Triggers.Right);


	    }
    	
	    private bool startSocketConnection(bool startingNotStoping){
			    if (startingNotStoping){
				    if (comSocket.Connected){
					    return true;
				    }else {
                        string portString = ipAddressBox.Text.Replace(" ","");
                        try
                        {
                            comSocket.Connect(portString, System.Convert.ToInt16(portMaskedTextBox.Text)); //update this
                        }
                        catch (Exception e)
                        {
                            connectionErrorsLabel.Text = e.Message;
                        }


                        if (comSocket.Connected)
                        {
                            connectionErrorsLabel.Visible = false;
                            updateSocketFromController.Enabled = true; 
                            return true;
                        }
                        else
                        {
                            connectionErrorsLabel.Visible = true;
                            return false;
                        }
                        
				    }
    			
                }else {
			    if (comSocket.Connected){
				    comSocket.Disconnect(true);
			    }
                comSocket = null;
                comSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                updateSocketFromController.Enabled = false;
			    return true;
		    }

	    }

	    private bool updateMotor(int value, int motor)
	    {
		    String toSendString;
		    char commandChar;
		    String commandString; 
		    String payloadString;
		    char payloadLength;
		    String lengthString;
    		
		    if (motor == 1) //drive
		    {
		       // mainControl.setServoLocation(value);
			    //serial stuff
    			
			    //pseudo code -- needs to be redone -- especially conversions
			    commandChar = (char)20;
			    commandString = System.Convert.ToString(commandChar);
			    payloadString = System.Convert.ToString(value);
			    payloadLength = (char)payloadString.Length;
			    lengthString = System.Convert.ToString(payloadLength);
    			
			    toSendString = System.String.Concat(commandString, lengthString, payloadString);
                labelProjectMotor.Text = payloadString;    			
			    beginSendingPacket(toSendString);

		    }
		    else if (motor == 2) //speed
		    {
			    commandChar = (char)19;
			    commandString = System.Convert.ToString(commandChar);
			    payloadString = System.Convert.ToString(value);
			    payloadLength = (char)payloadString.Length;
			    lengthString = System.Convert.ToString(payloadLength);
    			
			    toSendString = System.String.Concat(commandString, lengthString, payloadString);
                labelProjetedServo.Text = payloadString;
			    beginSendingPacket(toSendString);
		    }
		    else
			    return false;

		    return true;
	    }

	    private void controllerTimer_Tick(object sender, EventArgs e)
	    {
		    this.UpdateControllerState();
		    scaleControllerValuesToImplement();
	    }
    	
	    private void updateUIForConnectionStatus(bool connected){
            if (connected)
            {
                socketConnectButton.Text = "Disconnect ('C')";
                labelConnectionState.Text = "Connected to " + System.Net.IPAddress.Parse(((System.Net.IPEndPoint)comSocket.RemoteEndPoint).Address.ToString()) + ":" + ((System.Net.IPEndPoint)comSocket.RemoteEndPoint).Port.ToString();
            }
            else {
                socketConnectButton.Text = "Connect ('C')";
                labelConnectionState.Text = "Not Connected";
            }
	    }
    	
	    private bool beginSendingPacket(String sendingData){
		    if (comSocket.Connected){
			    //comSocket.SendAsync(sendingData);	 //figure out sending data
                byte[] byteArray = new System.Text.UTF8Encoding(true).GetBytes(sendingData);

                try
                {
                    comSocket.Send(byteArray);
                }
                catch (Exception e)
                {
                    connectionErrorsLabel.Text = e.Message;
                    connectionErrorsLabel.Visible = true;
                    if (this.startSocketConnection(false))
                    {
                        updateUIForConnectionStatus(false);
                    }
                    return false;
                }
                return true;
		    }
		    return false;
	    }

	    private void socketConnectButton_Click(object sender, EventArgs e)
	    {
            if (!comSocket.Connected)
            {
                if (this.startSocketConnection(true))
                {
                    updateUIForConnectionStatus(true);
                }
                else
                {
                    //connect failed.
                }
            }
            else
            {
                if (this.startSocketConnection(false))
                {
                    updateUIForConnectionStatus(false);
                }
                else
                {
                    //connect failed.
                }
            }
         }
        
	    private void updateSocketFromController_Tick(object sender, EventArgs e)
	    {
            if (servo_location != XBOXSuggestedScaledServoValue)
            {
                updateMotor(XBOXSuggestedScaledServoValue, 1);
                servo_location = XBOXSuggestedScaledServoValue;
            }
            else
            {
                updateMotor(XBOXSuggestedScaledSpeedValue, 2);
                motor_speed = XBOXSuggestedScaledSpeedValue;
            }
	    }

        
        private void updateUIForXBOXController(){
    	
	    }
    	private void updateUIForSocketCommunication(){
		    if (comSocket.Connected){
    			
		    }else {
			    updateUIForConnectionStatus(false);
		    }
        }

        private void ipAddressBox_MaskInputRejected(object sender, MaskInputRejectedEventArgs e)
        {

        }
  
    }
}
