using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace UI
{
    public partial class Button : Control
    {
        bool state;

        public Button()
        {
            DoubleBuffered = true;
            Paint += Button_Paint;
            MouseDown += Button_MouseDown;
        }

        public bool Value
        {
            get { return state; }
            set { state = value; if(ValueChanged != null) ValueChanged.Invoke(this, null); Invalidate(); }
        }

        public event EventHandler ValueChanged;

        private void Button_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;

            var rled = new Rectangle(ClientRectangle.Left + ClientRectangle.Width / 2 - 4, ClientRectangle.Top + 10 - 4, 7, 7);

            var r2 = Rectangle.Inflate(rled, 1, 1);
            e.Graphics.FillEllipse(new LinearGradientBrush(r2, Color.FromArgb(50, 0, 0, 0), Color.FromArgb(50, 255, 255, 255), LinearGradientMode.Vertical), r2);

            var r = new RectangleF(rled.Left + .25f, rled.Top + .25f, rled.Width - .5f, rled.Height - .5f);
            e.Graphics.FillEllipse(new SolidBrush(state ? Color.FromArgb(0xFE, 0xB9, 0x00) : Color.FromArgb(0x50, 0x50, 0x50)), r);
            e.Graphics.DrawEllipse(new Pen(Color.Black), r);

            if(state)
            {
                var rshine = Rectangle.Inflate(rled, 7, 7);
                GraphicsPath gp = new GraphicsPath();
                gp.AddEllipse(rshine);
                PathGradientBrush pgb = new PathGradientBrush(gp);
                pgb.CenterColor = Color.FromArgb(90, 0xFE, 0xB9, 0x00);
                pgb.SurroundColors = new Color[] { Color.FromArgb(0, 0xFE, 0xB9, 0x00) };
                e.Graphics.FillRectangle(pgb, rshine);
            }

            e.Graphics.SmoothingMode = SmoothingMode.None;
            e.Graphics.FillEllipse(new SolidBrush(Color.White), (int)r.X + 3, (int)r.Y + 1, 2, 2);
            e.Graphics.DrawEllipse(new Pen(Color.White), (int)r.X + 2, (int)r.Y + 3, 3, 2);
        }

        private void Button_MouseDown(object sender, MouseEventArgs e)
        {
            state = !state;
            if(ValueChanged != null)
                ValueChanged.Invoke(this, null);
            Invalidate();
        }
    }
}
