using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace socketXBOXControllerForSWARMS
{
    class SWARMS_Sockets_client
    {

        public delegate void SWARMSStatusChangedDelegate(String status);
        public delegate void SWARMSReceivedDataDelegate(String data);

        SWARMSStatusChangedDelegate statusDelegate;
        SWARMSReceivedDataDelegate dataDelegate;

        private Socket _serverSocket;
        private int _port;

        public SWARMS_Sockets_client(int port) { _port = port; }

        private class ConnectionInfo
        {
            public Socket Socket;
            public Thread Thread;
        }

        ConnectionInfo connection;

        public void disconnect()
        {

            if (_serverSocket != null)
            {
                if (_serverSocket.Connected)
                {
                    _serverSocket.Disconnect(true);
                    _serverSocket.Shutdown(SocketShutdown.Both);
                }
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

                ConnectionInfo clientInfo = connection;
                Socket clientSocket = clientInfo.Socket;

                ipString = clientSocket.ToString();
  

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

            //SetupServerSocket();

            statusDelegate("Started");
        }

        public void connectToServer(String ipAddress, String port)
        {
            
        }

        private void SetupClientSocket()
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
  /*              Socket socket = _serverSocket.Accept();
                ConnectionInfo connection = new ConnectionInfo();
                connection.Socket = socket;

                // Create the thread for the receives.
                connection.Thread = new Thread(ProcessConnection);
                connection.Thread.IsBackground = true;
                connection.Thread.Start(connection);

                // Store the socket
                lock (_connections) _connections.Add(connection);
               
                statusDelegate("Connected"); */
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


                statusDelegate("Disconnected");
            }
        }
    }
}
