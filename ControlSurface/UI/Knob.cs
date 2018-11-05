using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UI
{
    public enum KnobStyle
    {
        RotaryDial,
        Slider
    }

    public enum KnobValueMode
    {
        Positive,
        NegativePositive
    }

    public partial class Knob : Control
    {
        float fvalue;

        [Category("Appearance")]
        [Browsable(true)]
        public KnobStyle KnobStyle { get; set; } = KnobStyle.RotaryDial;

        [Category("Appearance")]
        [Browsable(true)]
        public float Minimum { get; set; } = 0;

        [Category("Appearance")]
        [Browsable(true)]
        public float Maximum { get; set; } = 1;

        [Category("Appearance")]
        [Browsable(true)]
        public bool Snap { get; set; } = false;

        [Category("Appearance")]
        [Browsable(true)]
        public Color BeamColor { get; set; } = Color.FromArgb(0xF8, 0x04, 0x89);

        [Category("Appearance")]
        [Browsable(true)]
        public float Value
        {
            get { return fvalue; }
            set { fvalue = value; if(ValueChanged != null) ValueChanged.Invoke(this, null); Invalidate(); }
        }

        public float ValueRelative
        {
            get { return (fvalue - Minimum) / (Maximum - Minimum); }
        }



        public event EventHandler ValueChanged;

        public Knob()
        {
            Paint += Knob_Paint;
            MouseMove += Knob_MouseMove;
            MouseDown += Knob_MouseDown;
            MouseCaptureChanged += Knob_MouseCaptureChanged;
            DoubleBuffered = true;
        }

        void PaintRotaryDial(PaintEventArgs e)
        {
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
            float d = Math.Min(Width, Height);
            RectangleF r = new RectangleF(0, 0, d, d);
            GraphicsPath gp = new GraphicsPath();
            gp.AddArc(RectangleF.Inflate(r, -6, -6), 45 + 90, 270);
            gp.AddArc(RectangleF.Inflate(r, -8, -8), 45, -270);
            e.Graphics.FillPath(new SolidBrush(Color.FromArgb(0x5B, 0x5D, 0x62)), gp);

            float thickness = 4;
            float full = 270;
            int nslices = 25;
            float slice = (full + full / nslices - thickness) / nslices;

            for(int i = 0; i < nslices; ++i)
            {
                float a = i * slice + 90 + 45;
                gp = new GraphicsPath();
                gp.AddArc(r, a, thickness);
                gp.AddArc(RectangleF.Inflate(r, -3, -3), a + thickness, -thickness);
                e.Graphics.FillPath(i == 0 || i == nslices - 1 || i == nslices / 2 ?
                    new SolidBrush(Color.White) :
                    new SolidBrush(Color.FromArgb(0x5B, 0x5D, 0x62)), gp);
            }

            e.Graphics.FillEllipse(new SolidBrush(Color.FromArgb(0x12, 0x13, 0x18)),
                RectangleF.Inflate(r, -11, -11));
            e.Graphics.FillEllipse(new LinearGradientBrush(new PointF(r.Left, r.Top), new PointF(r.Left, r.Bottom), Color.FromArgb(0xB5, 0xB6, 0xB7), Color.FromArgb(0x40, 0x40, 0x47)),
                RectangleF.Inflate(r, -12, -12));
            e.Graphics.FillEllipse(new SolidBrush(Color.FromArgb(0x2B, 0x2C, 0x30)),
                RectangleF.Inflate(r, -14, -14));

            float pvalue = fvalue;

            float angMid = full * Minimum / (Maximum - Minimum);


            
            gp = new GraphicsPath();
            float angArcStart = 135 - angMid;
            float angArcEnd = 135 + (full * ValueRelative);
            gp.AddArc(RectangleF.Inflate(r, -6, -6), angArcStart, angArcEnd - angArcStart);
            gp.AddArc(RectangleF.Inflate(r, -8, -8), angArcEnd, angArcStart - angArcEnd);

            e.Graphics.FillPath(new SolidBrush(BeamColor), gp);


            e.Graphics.TranslateTransform((r.Left + r.Right) * .5f, (r.Bottom + r.Top) * .5f);
            e.Graphics.RotateTransform(-135 + full * ValueRelative);
            e.Graphics.FillRectangle(new SolidBrush(Color.White), -2, 7 - r.Height / 2, 4, 12);
        }

        void PaintSlider(PaintEventArgs e)
        {
            var r = ClientRectangle;
            Point mid = new Point(r.Width / 2, r.Height / 2);

            var rbar = new Rectangle(mid.X - 2, r.Top, 3, r.Height);

            // track/hole
            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(0x0D, 0x0D, 0x10)), Rectangle.Inflate(rbar, 0, -1));
            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(0x3D, 0x3F, 0x45)), rbar.Left, rbar.Bottom - 1, rbar.Width, 1);
            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(0x19, 0x19, 0x1B)), rbar.Left, rbar.Top, rbar.Width, 1);


            // ticks
            const int numTicks = 11;
            for(int i = 0; i < numTicks; ++i)
            {
                Rectangle rtick = new Rectangle(mid.X - 14, r.Top + 1 + i * (r.Height - 4) / (numTicks - 1), 6, 1);
                if(i == 0 || i + 1 == numTicks)
                    rtick.Height *= 2;

                e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(0x39, 0x3A, 0x41)), rtick);
                rtick.X += 21;
                e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(0x39, 0x3A, 0x41)), rtick);
            }

            // cap
            e.Graphics.TranslateTransform(mid.X, 5 + (int)((1 - Value) * (r.Height - 11)));

            Rectangle rcap = new Rectangle(-9, -2, 17, 5);
            GraphicsPath gp = new GraphicsPath();
            gp.AddLine(rcap.Left + 1, rcap.Top - 2, rcap.Right - 1, rcap.Top - 2);
            gp.AddLine(rcap.Right, rcap.Top, rcap.Right, rcap.Bottom);
            gp.AddLine(rcap.Right - 1, rcap.Bottom + 2, rcap.Left + 1, rcap.Bottom + 2);
            gp.AddLine(rcap.Left, rcap.Bottom, rcap.Left, rcap.Top);
            gp.CloseFigure();

            // cap shadow
            /*PathGradientBrush pgb = new PathGradientBrush(gp);
            pgb.CenterColor = Color.FromArgb(200, 0, 0, 0);
            pgb.SurroundColors = new Color[] { Color.FromArgb(0, 0, 0, 0) };

            Rectangle rcapshadow = new Rectangle(rcap.Left - 1, rcap.Bottom, rcap.Width + 2, 8);
            var gradshadow = new LinearGradientBrush(rcapshadow, Color.FromArgb(200, 0, 0, 0), Color.FromArgb(0, 0, 0, 0), LinearGradientMode.Vertical);
            gradshadow.GammaCorrection = true;

            e.Graphics.FillRectangle(
            gradshadow,
            //pgb,
            rcapshadow);*/


            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
            e.Graphics.PixelOffsetMode = PixelOffsetMode.Half;





            e.Graphics.DrawPath(new Pen(Color.FromArgb(120, 0, 0, 0), 2), gp);


            e.Graphics.FillPath(new LinearGradientBrush(gp.GetBounds(),
                Color.FromArgb(0x37, 0x38, 0x3C),
                Color.FromArgb(0x1E, 0x1F, 0x23), LinearGradientMode.Vertical), gp);

            e.Graphics.FillRectangle(
                new SolidBrush(Color.FromArgb(0x50, 0x51, 0x53)),
                //new LinearGradientBrush(rcap, Color.FromArgb(0x50, 0x51, 0x53), Color.FromArgb(0xAF, 0xB0, 0xB1), LinearGradientMode.Horizontal),
                rcap);

            e.Graphics.DrawRectangle(new Pen(Color.FromArgb(50, 255, 255, 255)), -8, -2, 16, 4);


        }

        private void Knob_Paint(object sender, PaintEventArgs e)
        {
            if(KnobStyle == KnobStyle.RotaryDial)
                PaintRotaryDial(e);
            else if(KnobStyle == KnobStyle.Slider)
                PaintSlider(e);

            //e.Graphics.DrawString(Text, new Font("Verdana", 5), new SolidBrush(Color.FromArgb(0x93, 0x94, 0x99)), 15, r.Bottom - 7);
        }

        int lastMouseY = 0;
        Point mouseDragstart;

        float forceAccum;
        int forceAccumPol;
        bool snapped = false;

        float snapvalue = 0;

        private void Knob_MouseMove(object sender, MouseEventArgs e)
        {
            if(DesignMode)
                return;

            if(Capture)
            {
                float force = e.Y - lastMouseY;
                float nextfvalue = fvalue;

                nextfvalue -= force * (Maximum - Minimum) * .006f;

                if(snapped)
                {
                    if(force != 0)
                    {
                        if(forceAccumPol != Math.Sign(force))
                        {
                            forceAccumPol = Math.Sign(force);
                            forceAccum = 0;
                        }
                        forceAccum += Math.Abs(force);
                    }

                    if(forceAccum > 80)
                    {
                        snapped = false;
                    }
                    else
                        nextfvalue = snapvalue;
                }
                else if(Snap)
                {
                    float a = fvalue, b = nextfvalue;
                    if(b < a)
                    {
                        var t = a; a = b; b = t;
                    }

                    if(snapvalue >= a && snapvalue < b)
                    {
                        snapped = true;
                        nextfvalue = snapvalue;
                        forceAccum = 0;
                        forceAccumPol = 0;
                    }
                }


                if(nextfvalue > Maximum)
                    nextfvalue = Maximum;
                if(nextfvalue < Minimum)
                    nextfvalue = Minimum;

                if(nextfvalue != fvalue)
                {
                    fvalue = nextfvalue;

                    if(ValueChanged != null)
                        ValueChanged.Invoke(this, null);

                    Invalidate();
                }
                
                lastMouseY = e.Y;
            }
        }

        private void Knob_MouseDown(object sender, MouseEventArgs e)
        {
            if(DesignMode)
                return;

            Capture = true;
            lastMouseY = e.Y;

            Cursor.Hide();

            mouseDragstart = Cursor.Position;
        }

        private void Knob_MouseCaptureChanged(object sender, EventArgs e)
        {
            if(DesignMode)
                return;

            if(!Capture)
            {
                Cursor.Position = mouseDragstart;
                Cursor.Show();
            }
        }
    }

}
