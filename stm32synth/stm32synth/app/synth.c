#include "synth.h"

void synth_init(synth_s *s)
{
	voice_init(&s->voice);
}

synth_stereo_s synth_clock(synth_s *s)
{
	float dryMix = voice_clock(&s->voice);

	synth_stereo_s wetMix = {0, 0};

	reverb_stereo_s revb = reverb_clock(&s->reverb, REVERBSAMPLE_FROM_FLOAT(dryMix * .15f));
	wetMix.left += REVERBSAMPLE_TO_FLOAT(revb.left);
	wetMix.right += REVERBSAMPLE_TO_FLOAT(revb.right);

	return wetMix;
}

void synth_keyChange(synth_s *s, int key, int state) {
	if(state == 1)
		voice_setGate(&s->voice, key);
	else if(state == 0)
		voice_clearGate(&s->voice, key);
}
