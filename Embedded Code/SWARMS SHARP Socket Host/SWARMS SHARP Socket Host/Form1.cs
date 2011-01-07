using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SWARMS_SHARP_Socket_Host;

namespace SWARMS_SHARP_Socket_Host
{
    public partial class Form1 : Form
    {
        SWARMS_Socket_Host mainServer = new SWARMS_Socket_Host(2234);
        SWARMSLibUSB mainSWARMS = new SWARMSLibUSB();

        //for now this is for UI refresh
        String statusChangedNotification;
        String dataRecievedNotification;

        public Form1()
        {
            InitializeComponent();
        }

        public void setupServer()
        {
            if (! mainServer.isConnected())
            {
                //mainServer = new SWARMS_Socket_Host(2234);
                mainServer.Start(new SWARMS_Socket_Host.SWARMSStatusChangedDelegate(this.statusChangedNotifg), new SWARMS_Socket_Host.SWARMSReceivedDataDelegate(this.dataRecievedNotify));
            }
        }
        public void shutdownServer()
        {
            if (mainServer != null)
            {
                if (mainServer.isConnected())
                {
                    mainServer.disconnect();
                   //disconnect or somtin
                }
            }
        }

        private void connectAllButton_Click(object sender, EventArgs e)
        {
            this.setupServer();
            connectSWARMS(true);
        }

        private void setupUIForServerRunning(bool running)
        {
            if (running)
            {
                this.connectAllButton.Enabled = false;
                this.startWebsocketListeningButton.Text = "Stop Websocket Server";
                this.hostPortLabel.Text = mainServer.ipAndPortString();
            }
            else
            {
                this.connectAllButton.Enabled = true;
                this.startWebsocketListeningButton.Text = "Start Websocket Listening";
                this.hostPortLabel.Text = "Not Connected";
                this.connectedWithLabel.Text = "Not Connected";
            }
        }

        private void dataRecievedNotify(String received)
        {
            if (received.Substring(2, 1) == "2")
            {
               
            }
            dataRecievedNotification = received.Substring(0, 2 + (int)System.Convert.ToChar(received.Substring(1, 1)));//received.Substring(1, 1)

            //also handle stuff on this thread
        }
        private void statusChangedNotifg(String status)
        {
            statusChangedNotification = status;
        }

        private void startWebsocketListeningButton_Click(object sender, EventArgs e)
        {
            if (mainServer.isConnected() == false)
            {
                this.setupServer();
            }
            else
            {
                this.shutdownServer();
            }
        }

        private void asyncUIRefresh_Tick(object sender, EventArgs e)
        {
            if (statusChangedNotification != null)
            {
                if (statusChangedNotification.Equals("Started") || statusChangedNotification.Equals("Already Started"))
                {
                    this.setupUIForServerRunning(true);
                }
                else if (statusChangedNotification.Equals("Shut Down"))
                {
                    this.setupUIForServerRunning(false);
                }
                else if (statusChangedNotification.Equals("Connected"))
                {
                    //need to find what system device is connected to...
                    connectedWithLabel.Text = this.mainServer.connectedClientIPString();
                    socketConnectedLabel.Text = "Connected";
                }
                else if (statusChangedNotification.Equals("Disconnected"))
                {
                    //need to find what system device is connected to...
                    connectedWithLabel.Text = "Not Connected";
                    socketConnectedLabel.Text = "Socket is Disconnected";
                }

                statusChangedNotification = null;
            }
            if (dataRecievedNotification != null)
            {
                String returnString = mainSWARMS.syncronousReadWrite(dataRecievedNotification);

                if (returnString != null)
                {
                    this.SWARMSDataIncommingRichText.Text = this.SWARMSDataIncommingRichText.Text.Insert(0, returnString);
                }
                this.SWARMSDataOutgoingRichText.Text = this.SWARMSDataOutgoingRichText.Text.Insert(0, dataRecievedNotification);
                this.SocketDataIncommingRichText.Text = this.SocketDataIncommingRichText.Text.Insert(0, dataRecievedNotification); 
                dataRecievedNotification = null;
            }
        }

        public bool connectSWARMS(bool connectNotDisconnect)
        {
            if (connectNotDisconnect)
            {
                if (mainSWARMS.openDevice())
                {
                    connectSwarmsUSBButton.Text = "Disconnect SWARMS USB";
                    return true;
                }
                else
                {
                    connectSwarmsUSBButton.Text = "Connect SWARMS USB";
                    return false;
                }
            }
            else
            {
                mainSWARMS.closeDevice();

                connectSwarmsUSBButton.Text = "Connect SWARMS USB";

                return true;
            }
        }

        private void connectSwarmsUSBButton_Click(object sender, EventArgs e)
        {
            if (mainSWARMS.isConnected())
            {
                connectSWARMS(false);
            }
            else
            {
                connectSWARMS(true);
            }
        }

        private void testSWARMSConnectivityButton_Click(object sender, EventArgs e)
        {
            if (mainSWARMS.isConnected())
            {
            String toSendString;
		    char commandChar;
		    String commandString; 
		    String payloadString;
		    char payloadLength;
		    String lengthString;
    		
    			
			    //pseudo code -- needs to be redone -- especially conversions
			    commandChar = (char)20;
			    commandString = System.Convert.ToString(commandChar);
                payloadString = "-100";//System.Convert.ToString("-100");
			    payloadLength = (char)payloadString.Length;
			    lengthString = System.Convert.ToString(payloadLength);
                		
			    toSendString = System.String.Concat(commandString, lengthString, payloadString);

                String returnString =  mainSWARMS.syncronousReadWrite (toSendString);
            }
        }
    }
}
