using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StmPiano
{
    class StmDevice
    {
        SerialPort serialPort;
        SerialProtocol protocol = new SerialProtocol();
        public void Open()
        {
            serialPort = new SerialPort("COM4", 115200);
            serialPort.DataReceived += SerialPort_DataReceived;
            serialPort.Open();
        }

        public event EventHandler<KeyChangeEventArgs> KeyChange;

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            for(int c; (c = serialPort.ReadByte()) >= 0; )
            {
                var frame = protocol.NextRecv((byte)c);
                if(frame != null)
                {
                    var ss = frame.Split(' ');
                    KeyChange?.Invoke(this, new KeyChangeEventArgs { Key = int.Parse(ss[0]), Value = int.Parse(ss[1]) });
                }
            }
        }
    }

    public class KeyChangeEventArgs : EventArgs
    {
        public int Key;
        public int Value;
    }

    enum ProtocolState
    {
        WaitStart,
        Data
    }

    class SerialProtocol
    {
        ProtocolState protocolState;
        List<byte> protocolRxBuffer = new List<byte>();
        byte protocolLastRx;

        public string NextRecv(byte b)
        {
            switch(protocolState)
            {
                case ProtocolState.WaitStart:
                    if(b == '~')
                    {
                        protocolState = ProtocolState.Data;
                    }
                    break;
                case ProtocolState.Data:
                    if(protocolLastRx == '\r' && b == '\n')
                    {
                        var ret = Encoding.ASCII.GetString(protocolRxBuffer.ToArray());
                        protocolRxBuffer.Clear();
                        protocolState = ProtocolState.WaitStart;
                        return ret;
                    }
                    protocolRxBuffer.Add(b);
                    break;
            }
            protocolLastRx = b;
            return null;
        }
    }
}
