using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace UI
{
    public class Piano : Control
    {
        Bitmap bmp;
        const int maxOcataves = 5;
        float[] velocities = new float[maxOcataves * 12];
        int offset = 36;

        public event EventHandler<KeyInputEvent> VelocityChanged;
        public event EventHandler<KeyInputEvent> PianoKeyDown;
        public event EventHandler<KeyInputEvent> PianoKeyUp;

        public Piano()
        {
            //File.WriteAllText("keys.txt", System.Convert.ToBase64String(File.ReadAllBytes("..\\..\\keys.png")));
            bmp = new Bitmap(new MemoryStream(Convert.FromBase64String("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")));
            DoubleBuffered = true;
        }

        int KeyFromPoint(Point p)
        {
            p.X -= 5;
            int octave = p.X / 131;
            p.X -= octave * 131;
            int[] ww = new int[] { 19, 18, 19, 19, 19, 18, 19, 19 };
            int[] wv = new int[] { 0, 2, 4, 5, 7, 9, 11 };
            int[] bw = new int[] { 13, 23, 33, 21, 20, 34 };
            int[] bv = new int[] { 1, 3, 6, 8, 10 };
            if(p.X < 0)
                return -1;
            if(p.Y < 52) // black keys
            {
                for(int i = 0, x2 = p.X; i < bw.Length; ++i)
                {
                    x2 -= bw[i];
                    if(x2 >= 0 && x2 < 7)
                        return octave * 12 + bv[i];
                    
                }
            }
            // white keys
            for(int i = 0, x2 = p.X; i < ww.Length; ++i)
            {
                if(x2 < ww[i + 1])
                    return octave * 12 + wv[i];
                x2 -= ww[i];
            }
            return -1;
        }
        int selectedIndex;

        protected override void OnMouseUp(MouseEventArgs e)
        {
            if(selectedIndex >= 0)
            {
                velocities[selectedIndex] = 0;
                if(VelocityChanged != null)
                    VelocityChanged.Invoke(this, new KeyInputEvent { Note = selectedIndex + offset, Velocity = velocities[selectedIndex] });

                PianoKeyUp?.Invoke(this, new KeyInputEvent { Note = selectedIndex + offset });

                Invalidate();
            }
            base.OnMouseUp(e);
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            selectedIndex = KeyFromPoint(e.Location);
            if(selectedIndex >= 0)
            {
                velocities[selectedIndex] = 1;
                if(VelocityChanged != null)
                    VelocityChanged.Invoke(this, new KeyInputEvent { Note = selectedIndex + offset, Velocity = velocities[selectedIndex] });

                PianoKeyDown?.Invoke(this, new KeyInputEvent { Note = selectedIndex + offset });

                Invalidate();
            }
            base.OnMouseDown(e);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            int numOctaves = maxOcataves;

            // keys:  5,3  131x74  4,3
            e.Graphics.DrawImage(bmp, 0, 0, new Rectangle(0, 0, 5, 80), GraphicsUnit.Pixel);
            for(int i = 0; i < numOctaves; ++i)
            {
                e.Graphics.DrawImage(bmp, 5 + i * 131, 0, new Rectangle(5, 0, 131, 80), GraphicsUnit.Pixel);
            }
            e.Graphics.DrawImage(bmp, 5 + numOctaves * 131, 0, new Rectangle(136, 0, 5, 80), GraphicsUnit.Pixel);

            int[] ww = new int[] { 19, 18, 19, 19, 19, 18, 19 };
            int[] wv = new int[] { 0, 2, 4, 5, 7, 9, 11 };
            int[] bw = new int[] { 17, 23, 33, 21, 20 };
            int[] bv = new int[] { 1, 3, 6, 8, 10 };

            for(int j = 0; j < numOctaves; ++j)
            {
                for(int i = 0, x = 0; i < 7; x += ww[i], i++)
                {
                    if(velocities[j * 12 + wv[i]] > 0)
                        e.Graphics.DrawImage(bmp, 5 + x + j * 131, 0, new Rectangle(5 + x, 80, 18, 80), GraphicsUnit.Pixel);
                }
            }

            for(int j = 0; j < numOctaves; ++j)
            {
                for(int i = 0, x = 0; i < 5; i++)
                {
                    x += bw[i];
                    e.Graphics.DrawImage(bmp, 131 * j + x, 0, new Rectangle(17, velocities[j * 12 + bv[i]] > 0 ? 80 : 0, 9, 52), GraphicsUnit.Pixel);
                }
            }
            base.OnPaint(e);
        }

        public void SetVelocity(int n, float v)
        {
            n -= offset;
            if(n >= 0 && n < velocities.Length)
            {
                float value = Math.Min(Math.Max(v, 0), 1);
                if(value != velocities[n])
                {
                    velocities[n] = value;
                    VelocityChanged?.Invoke(this, new KeyInputEvent { Note = n + offset, Velocity = velocities[n] });
                    Invalidate();
                }
            }
        }
    }

    public class KeyInputEvent : EventArgs
    {
        public int Note;
        public float Velocity;
    }
}