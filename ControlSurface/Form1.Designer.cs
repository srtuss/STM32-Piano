namespace ControlSurface
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if(disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.knobEchoAmount = new UI.Knob();
            this.knobReverbAmount = new UI.Knob();
            this.knobSeqEnable = new UI.Knob();
            this.knobOscPulseWidth = new UI.Knob();
            this.knobVoiceNumUnison = new UI.Knob();
            this.knobFilterReso = new UI.Knob();
            this.knobFilterCutoff = new UI.Knob();
            this.SuspendLayout();
            // 
            // knobEchoAmount
            // 
            this.knobEchoAmount.BeamColor = System.Drawing.Color.FromArgb(((int)(((byte)(248)))), ((int)(((byte)(4)))), ((int)(((byte)(137)))));
            this.knobEchoAmount.KnobStyle = UI.KnobStyle.RotaryDial;
            this.knobEchoAmount.Location = new System.Drawing.Point(552, 53);
            this.knobEchoAmount.Maximum = 1F;
            this.knobEchoAmount.Minimum = 0F;
            this.knobEchoAmount.Name = "knobEchoAmount";
            this.knobEchoAmount.Size = new System.Drawing.Size(70, 70);
            this.knobEchoAmount.Snap = false;
            this.knobEchoAmount.TabIndex = 6;
            this.knobEchoAmount.Value = 0F;
            // 
            // knobReverbAmount
            // 
            this.knobReverbAmount.BeamColor = System.Drawing.Color.FromArgb(((int)(((byte)(248)))), ((int)(((byte)(4)))), ((int)(((byte)(137)))));
            this.knobReverbAmount.KnobStyle = UI.KnobStyle.RotaryDial;
            this.knobReverbAmount.Location = new System.Drawing.Point(465, 53);
            this.knobReverbAmount.Maximum = 1F;
            this.knobReverbAmount.Minimum = 0F;
            this.knobReverbAmount.Name = "knobReverbAmount";
            this.knobReverbAmount.Size = new System.Drawing.Size(70, 70);
            this.knobReverbAmount.Snap = false;
            this.knobReverbAmount.TabIndex = 5;
            this.knobReverbAmount.Value = 0F;
            // 
            // knobSeqEnable
            // 
            this.knobSeqEnable.BeamColor = System.Drawing.Color.FromArgb(((int)(((byte)(248)))), ((int)(((byte)(4)))), ((int)(((byte)(137)))));
            this.knobSeqEnable.KnobStyle = UI.KnobStyle.RotaryDial;
            this.knobSeqEnable.Location = new System.Drawing.Point(389, 53);
            this.knobSeqEnable.Maximum = 1F;
            this.knobSeqEnable.Minimum = 0F;
            this.knobSeqEnable.Name = "knobSeqEnable";
            this.knobSeqEnable.Size = new System.Drawing.Size(70, 70);
            this.knobSeqEnable.Snap = false;
            this.knobSeqEnable.TabIndex = 4;
            this.knobSeqEnable.Value = 0F;
            // 
            // knobOscPulseWidth
            // 
            this.knobOscPulseWidth.BeamColor = System.Drawing.Color.FromArgb(((int)(((byte)(248)))), ((int)(((byte)(4)))), ((int)(((byte)(137)))));
            this.knobOscPulseWidth.KnobStyle = UI.KnobStyle.RotaryDial;
            this.knobOscPulseWidth.Location = new System.Drawing.Point(303, 53);
            this.knobOscPulseWidth.Maximum = 1F;
            this.knobOscPulseWidth.Minimum = 0F;
            this.knobOscPulseWidth.Name = "knobOscPulseWidth";
            this.knobOscPulseWidth.Size = new System.Drawing.Size(70, 70);
            this.knobOscPulseWidth.Snap = false;
            this.knobOscPulseWidth.TabIndex = 3;
            this.knobOscPulseWidth.Value = 0F;
            // 
            // knobVoiceNumUnison
            // 
            this.knobVoiceNumUnison.BeamColor = System.Drawing.Color.FromArgb(((int)(((byte)(248)))), ((int)(((byte)(4)))), ((int)(((byte)(137)))));
            this.knobVoiceNumUnison.KnobStyle = UI.KnobStyle.RotaryDial;
            this.knobVoiceNumUnison.Location = new System.Drawing.Point(215, 53);
            this.knobVoiceNumUnison.Maximum = 1F;
            this.knobVoiceNumUnison.Minimum = 0F;
            this.knobVoiceNumUnison.Name = "knobVoiceNumUnison";
            this.knobVoiceNumUnison.Size = new System.Drawing.Size(70, 70);
            this.knobVoiceNumUnison.Snap = false;
            this.knobVoiceNumUnison.TabIndex = 2;
            this.knobVoiceNumUnison.Value = 0F;
            // 
            // knobFilterReso
            // 
            this.knobFilterReso.BeamColor = System.Drawing.Color.FromArgb(((int)(((byte)(248)))), ((int)(((byte)(4)))), ((int)(((byte)(137)))));
            this.knobFilterReso.KnobStyle = UI.KnobStyle.RotaryDial;
            this.knobFilterReso.Location = new System.Drawing.Point(129, 53);
            this.knobFilterReso.Maximum = 1F;
            this.knobFilterReso.Minimum = 0F;
            this.knobFilterReso.Name = "knobFilterReso";
            this.knobFilterReso.Size = new System.Drawing.Size(70, 70);
            this.knobFilterReso.Snap = false;
            this.knobFilterReso.TabIndex = 1;
            this.knobFilterReso.Value = 0F;
            // 
            // knobFilterCutoff
            // 
            this.knobFilterCutoff.BeamColor = System.Drawing.Color.FromArgb(((int)(((byte)(248)))), ((int)(((byte)(4)))), ((int)(((byte)(137)))));
            this.knobFilterCutoff.KnobStyle = UI.KnobStyle.RotaryDial;
            this.knobFilterCutoff.Location = new System.Drawing.Point(44, 53);
            this.knobFilterCutoff.Maximum = 1F;
            this.knobFilterCutoff.Minimum = 0F;
            this.knobFilterCutoff.Name = "knobFilterCutoff";
            this.knobFilterCutoff.Size = new System.Drawing.Size(70, 70);
            this.knobFilterCutoff.Snap = false;
            this.knobFilterCutoff.TabIndex = 0;
            this.knobFilterCutoff.Value = 0F;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(70)))));
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.knobEchoAmount);
            this.Controls.Add(this.knobReverbAmount);
            this.Controls.Add(this.knobSeqEnable);
            this.Controls.Add(this.knobOscPulseWidth);
            this.Controls.Add(this.knobVoiceNumUnison);
            this.Controls.Add(this.knobFilterReso);
            this.Controls.Add(this.knobFilterCutoff);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private UI.Knob knobFilterCutoff;
        private UI.Knob knobFilterReso;
        private UI.Knob knobVoiceNumUnison;
        private UI.Knob knobOscPulseWidth;
        private UI.Knob knobSeqEnable;
        private UI.Knob knobReverbAmount;
        private UI.Knob knobEchoAmount;
    }
}

