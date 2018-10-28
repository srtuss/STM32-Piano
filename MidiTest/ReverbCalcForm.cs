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

namespace StmPiano
{
    public partial class ReverbCalcForm : Form
    {
        public ReverbCalcForm()
        {
            InitializeComponent();

            int numDelays = 12;
            var rnd = new Random();
            double[] delayTimes = new double[numDelays];
            for(int i = 0; i < numDelays; ++i)
                delayTimes[i] = rnd.NextDouble() * .7f + .3f;

            double sum = 0;
            for(int i = 0; i < delayTimes.Length; ++i)
                sum += delayTimes[i];
            for(int i = 0; i < delayTimes.Length; ++i)
                delayTimes[i] /= sum;

            sum = 0;
            for(int i = 0; i < delayTimes.Length; ++i)
                sum += delayTimes[i];

            CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;
            string s = "";
            for(int i = 0; i < numDelays; ++i)
                s += "DLL(" + delayTimes[i] + "),\r\n";
            textBox1.Text = s;
        }
    }
}
