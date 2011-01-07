
using System;
using System.IO;
using System.Text;
using System.Windows.Forms;
using LibUsbDotNet;
using LibUsbDotNet.LibUsb;
using LibUsbDotNet.Main;
using LibUsbDotNet.LudnMonoLibUsb;
using EC = LibUsbDotNet.Main.ErrorCode;



namespace SWARMS_SHARP_Socket_Host
{


    class SWARMSLibUSB
    {
//        typedef usb_dev_handle *(*usb_open_t)(struct usb_device *dev);

  //      [DllImport("libusb0.dll", CharSet = CharSet.Unicode)]
       // public static extern int usb_bulk_write(usb_dev_handle* dev, int ep, char* bytes, int size, int timeout);

        int MY_VID = 0xf4D8; //0xf4D8
        int MY_PID = 0x0f04;   //0x0f04

        private UsbDevice mUsbDevice;
        private UsbEndpointReader mEpReader;
        private UsbEndpointWriter mEpWriter;
        private string mLogFileName = String.Empty;
        private FileStream mLogFileStream;
        private UsbRegDeviceList mRegDevices;

        public void initSystem(){

        }
        public void closeDevice()
        {
            if (mUsbDevice != null)
            {
                if (mUsbDevice.IsOpen)
                {


                    if (mEpReader != null)
                    {
                        mEpReader.DataReceivedEnabled = false;
                        mEpReader.DataReceived -= mEp_DataReceived;
                        mEpReader.Dispose();
                        mEpReader = null;
                    }
                    if (mEpWriter != null)
                    {
                        mEpWriter.Abort();
                        mEpWriter.Dispose();
                        mEpWriter = null;
                    }

                    // If this is a "whole" usb device (libusb-win32, linux libusb)
                    // it will have an IUsbDevice interface. If not (WinUSB) the 
                    // variable will be null indicating this is an interface of a 
                    // device.
                    IUsbDevice wholeUsbDevice = mUsbDevice as IUsbDevice;
                    if (!ReferenceEquals(wholeUsbDevice, null))
                    {
                        // Release interface #0.
                        wholeUsbDevice.ReleaseInterface(0);
                    }
                    mUsbDevice.Close();
                    mUsbDevice = null;
                }

            }
        }

        private void mEp_DataReceived(object sender, EndpointDataEventArgs e) { OnDataReceived(sender, e); }

        private void OnDataReceived(object sender, EndpointDataEventArgs e)
        {
           
        }

        public bool writeCommand(String toSend)
        {
            byte[] bytesToWrite = Encoding.UTF8.GetBytes(toSend);

            int uiTransmitted;
            return (mEpWriter.Write(bytesToWrite, 0,64, 500, out uiTransmitted) == ErrorCode.None);
   
        }

        public bool isConnected()
        {
            if (mUsbDevice == null) return false;
            return mUsbDevice.IsOpen;
        }


        public bool openDevice()
        {
            bool bRtn = false;
            closeDevice();

            #region integration needed

            mRegDevices = UsbDevice.AllDevices;
            GC.Collect();

            foreach (UsbRegistry regDevice in mRegDevices)
            {
                // add the Vid, Pid, and usb device description to the dropdown display.
                // NOTE: There are many more properties available to provide you with more device information.
                // See the LibUsbDotNet.Main.SPDRP enumeration.

                if (regDevice.Vid == MY_VID && regDevice.Pid == MY_PID)
                {
                    regDevice.Open(out mUsbDevice);

                    IUsbDevice wholeUsbDevice = mUsbDevice as IUsbDevice;
                    if (!ReferenceEquals(wholeUsbDevice, null))
                    {
                        // This is a "whole" USB device. Before it can be used, 
                        // the desired configuration and interface must be selected.

                        // Select config #1
                        wholeUsbDevice.SetConfiguration(1);

                        // Claim interface #0.
                        wholeUsbDevice.ClaimInterface(0);

                        bRtn = true;
                    }

                }
            }
            #endregion


                if (bRtn)
                {
                    mEpReader = mUsbDevice.OpenEndpointReader((ReadEndpointID)0x81);
                    mEpWriter = mUsbDevice.OpenEndpointWriter((WriteEndpointID)0x01);
                    
                    /*mEpReader = mUsbDevice.OpenEndpointReader((ReadEndpointID)(epNum | 0x81));
                    mEpWriter = mUsbDevice.OpenEndpointWriter((WriteEndpointID)0x01); */
                    mEpReader.DataReceived += mEp_DataReceived;
                    mEpReader.Flush();
                }
            

            if (bRtn)
            {
                //tsStatus.Text = "Device Opened.";
            }
            else
            {
              //  tsStatus.Text = "Device Failed to Opened!";
                if (!ReferenceEquals(mUsbDevice, null))
                {
                    if (mUsbDevice.IsOpen) mUsbDevice.Close();
                    mUsbDevice = null;
                }
            }

            return bRtn;
        }

        public String syncronousReadWrite(String output)
        {
            if (!this.isConnected())
            {
                return null;
            }
            if (!writeCommand(output)) return null;
            
            byte[] readBuffer = new byte[64];

            int uiTransmitted;
            ErrorCode eReturn;
            if ((eReturn = mEpReader.Read(readBuffer, 0,64,200, out uiTransmitted)) == ErrorCode.None){
                String recieved = System.Text.ASCIIEncoding.UTF8.GetString(readBuffer);
                return recieved;
            }
            
            return null;

        }

        #region Nested Types

        private delegate void OnDataReceivedDelegate(object sender, EndpointDataEventArgs e);

        private delegate void UsbErrorEventDelegate(object sender, UsbError e);

        #endregion
    }
}
