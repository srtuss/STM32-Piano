using Audiolib;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace StmPiano
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        AudioContext ac;
        Timer timer = new Timer { Interval = 1000 / 30 };
        VstNode vst, vstBass;
        StmDevice stmDevice = new StmDevice();
        const string pathNexus = @"X:\Music\Steinberg\Vstplugins\Nexus2\Nexus.dll";
        const string pathSylenth = @"X:\Music\Steinberg\Vstplugins\Sylenth1.dll";

        private void Form1_Load(object sender, EventArgs e)
        {
            ac = new AudioContext();

            vst = new VstNode(ac, pathSylenth);
            vst.OpenEditor(panel1.Handle);
            vst.Connect(ac.Destination);
            try
            {
                vst.LoadProgram("mel_state.fxp");
            }
            catch(Exception ee)
            {
                
            }
            vstBass = new VstNode(ac, pathSylenth);
            vstBass.OpenEditor(panel2.Handle);
            vstBass.Connect(ac.Destination);

            try
            {
                vstBass.LoadProgram("bass_state.fxp");
            }
            catch(Exception ee)
            {

            }

            timer.Tick += Timer_Tick;
            timer.Start();

            stmDevice.KeyChange += StmDevice_KeyChange;
            stmDevice.Open();
        }

        private void StmDevice_KeyChange(object sender, KeyChangeEventArgs e)
        {
            if(e.Key > 11)
                vst.SendMidi(0, new byte[] { 0x90, (byte)(e.Key + 12 * 4), (byte)(e.Value > 0 ? 127 : 0) });
            else
                vstBass.SendMidi(0, new byte[] { 0x90, (byte)(e.Key + 12 * 4), (byte)(e.Value > 0 ? 127 : 0) });
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            vst.SaveProgram("mel_state.fxp");
            vstBass.SaveProgram("bass_state.fxp");
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            vst.EditorService();
            vstBass.EditorService();
        }
    }
}
