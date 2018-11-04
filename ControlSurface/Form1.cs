using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ControlSurface
{
    public partial class Form1 : Form
    {
        Pipe pipe = new Pipe();
        Timer tick = new Timer { Interval = 1000 / 30 };
        string[] fwControlNames;

        public Form1()
        {
            InitializeComponent();

            fwControlNames = NameConverter.Do();
            foreach(var y in fwControlNames)
                Console.WriteLine(y);
        }

        void LabelControlsAutomatically()
        {
            List<Control> toAdd = new List<Control>();

            foreach(var control in Controls)
            {
                if(control is UI.Knob knob)
                {
                    var label = new Label
                    {
                        Left = knob.Left - 10,
                        Top = knob.Top - 20,
                        Width = knob.Width + 20,
                        TextAlign = ContentAlignment.TopCenter,
                        Text = knob.Name.Replace("knob", ""),
                        ForeColor = Color.White,
                        Font = new Font("Calibri", 10f, FontStyle.Regular, GraphicsUnit.Point)
                    };

                    toAdd.Add(label);
                }
            }

            foreach(var control in toAdd)
                Controls.Add(control);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            LabelControlsAutomatically();

            foreach(var control in Controls)
            {
                if(control is UI.Knob knob)
                {
                    knob.ValueChanged += Knob_ValueChanged;
                }
            }

            pipe.Open();

            tick.Tick += Tick_Tick;
            tick.Start();
        }

        private void Knob_ValueChanged(object sender, EventArgs e)
        {
            var knob = (UI.Knob)sender;
            if(pipe.IsOpen)
            {
                var xx = knob.Name.Replace("knob", "");
                int index = Array.FindIndex(fwControlNames, (o) => o.Equals(xx, StringComparison.OrdinalIgnoreCase));

                pipe.Write("~" + index + ";" + knob.Value.ToString(CultureInfo.InvariantCulture) + "\r\n");
            }
        }

        private void Tick_Tick(object sender, EventArgs e)
        {
            pipe.Service();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            var window = MessageBox.Show(
            "Close the window?",
            "Are you sure?",
            MessageBoxButtons.YesNo);

            e.Cancel = (window == DialogResult.No);
        }
    }
}
