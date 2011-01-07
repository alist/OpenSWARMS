using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace SWARMS_SHARP_Socket_Host
{
    class SWARMS_Socket_Host
    {
        public delegate void SWARMSStatusChangedDelegate(String status);
        public delegate void SWARMSReceivedDataDelegate(String data);

        SWARMSStatusChangedDelegate statusDelegate;
        SWARMSReceivedDataDelegate dataDelegate;

        private Socket _serverSocket;
        private int _port;

        public SWARMS_Socket_Host(int port) { _port = port; }

        private class ConnectionInfo
        {
            public Socket Socket;
            public Thread Thread;
        }

        private Thread _acceptThread;
        private List<ConnectionInfo> _connections =  new List<ConnectionInfo>();

        public void disconnect(){

            if (_serverSocket != null)
            {
                if (_serverSocket.Connected)
                {
                    _serverSocket.Disconnect(true);
                    _serverSocket.Shutdown(SocketShutdown.Both);
                }
                _acceptThread.Abort();
                _serverSocket.Close();
                _serverSocket = null;
            }
            statusDelegate("Shut Down");
        }
        public bool isConnected()
        {
            if (_serverSocket != null)
             return (_serverSocket.IsBound);
            return false;
        }

        public string connectedClientIPString()
        {
            String ipString = null;
            lock (_connections)
            {
 
                ConnectionInfo clientInfo = _connections.First();
                IPEndPoint clientEndpoint = (IPEndPoint) clientInfo.Socket.RemoteEndPoint;

                ipString = System.Net.IPAddress.Parse(((System.Net.IPEndPoint)clientEndpoint).Address.ToString()) + ":" + ((System.Net.IPEndPoint)clientEndpoint).Port.ToString();

            }

            return ipString;
        }

        public string ipAndPortString()
        {
            if (_serverSocket == null)
            {
                return null;
            }

            string returnString = _serverSocket.LocalEndPoint.ToString();

            return returnString;

        }

        public void Start(SWARMSStatusChangedDelegate delegate1, SWARMSReceivedDataDelegate delegate2)
        {
            if (_serverSocket != null)
            {
                statusDelegate("Already Started");
                return;
            }
            statusDelegate = delegate1;
            dataDelegate = delegate2;

            SetupServerSocket();
            _acceptThread = new Thread(AcceptConnections);
            _acceptThread.IsBackground = true;
            _acceptThread.Start();

            statusDelegate("Started");
        }


        private void SetupServerSocket()
        {
            // Resolving local machine information
            IPHostEntry localMachineInfo =
                Dns.GetHostEntry(Dns.GetHostName());
            IPEndPoint myEndpoint = new IPEndPoint(
                localMachineInfo.AddressList[0], _port);

            // Create the socket, bind it, and start listening
            _serverSocket = new Socket(myEndpoint.Address.AddressFamily,
                SocketType.Stream, ProtocolType.Tcp);
            _serverSocket.Bind(myEndpoint);
            _serverSocket.Listen((int)SocketOptionName.MaxConnections);
        }
        private void AcceptConnections()
        {
            while (_serverSocket.IsBound)
            {
                // Accept a connection
                Socket socket = _serverSocket.Accept();
                ConnectionInfo connection = new ConnectionInfo();
                connection.Socket = socket;

                // Create the thread for the receives.
                connection.Thread = new Thread(ProcessConnection);
                connection.Thread.IsBackground = true;
                connection.Thread.Start(connection);

                // Store the socket
                lock (_connections) _connections.Add(connection);

                statusDelegate("Connected");
            }
        }

        private void ProcessConnection(object state)
        {
            ConnectionInfo connection = (ConnectionInfo)state;
            byte[] buffer = new byte[255];
            try
            {
                while (true)
                {
                    int bytesRead = connection.Socket.Receive(buffer);
                    if (bytesRead > 0)
                    {
                        //pass them to delegate, then to swarms

                        String recieved = System.Text.ASCIIEncoding.UTF8.GetString(buffer);
                        dataDelegate(recieved);
                        /*
                        lock (_connections)
                        {
                            foreach (ConnectionInfo conn in _connections)
                            {
                                if (conn != connection)
                                {
                                    conn.Socket.Send(
                                        buffer, bytesRead, SocketFlags.None);
                                }
                            }
                        }
                         */
                    }
                    else if (bytesRead == 0) return;
                }
            }
            catch (SocketException exc)
            {
                Console.WriteLine("Socket exception: " + exc.SocketErrorCode);
            }
            catch (Exception exc)
            {
                Console.WriteLine("Exception: " + exc);
            }
            finally
            {
                connection.Socket.Close();
                lock (_connections) _connections.Remove(connection);

                statusDelegate("Disconnected");
            }
        }

    }
}
