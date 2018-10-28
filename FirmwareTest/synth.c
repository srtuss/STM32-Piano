#include "synth.h"

void synth_init(synth_s *s)
{
	voice_init(s);
}

synth_stereo_s synth_clock(synth_s *s)
{
	auto revb = reverb_clock(&reverb, REVERBSAMPLE_FROM_FLOAT(left * .15f));
	left += REVERBSAMPLE_TO_FLOAT(revb.left);
	right += REVERBSAMPLE_TO_FLOAT(revb.right);
}