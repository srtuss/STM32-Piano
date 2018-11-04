using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace ControlSurface
{
    class Pipe
    {
        Socket listenSocket;
        Socket socket;

        public void Open()
        {
            listenSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            listenSocket.Bind(new IPEndPoint(IPAddress.Parse("127.0.0.9"), 25565));
            listenSocket.Listen(100);
        }

        public void Close()
        {
            listenSocket.Close();
        }

        public void Service()
        {
            if(listenSocket.Poll(0, SelectMode.SelectRead))
            {
                socket = listenSocket.Accept();
            }
        }

        public bool IsOpen => socket != null;

        public void Write(string msg)
        {
            socket.Send(Encoding.ASCII.GetBytes(msg));
        }
    }
}
