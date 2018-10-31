#include "synth.h"

void synth_init(synth_s *s)
{
	voice_init(&s->voice);
	reverb_init(&s->reverb);
}

synth_stereo_s synth_clock(synth_s *s)
{
	float dryMix = voice_clock(&s->voice);

	synth_stereo_s wetMix = {dryMix, dryMix};

	reverb_stereo_s revb = reverb_clock(&s->reverb, REVERBSAMPLE_FROM_FLOAT(dryMix * .15f));
	wetMix.left += REVERBSAMPLE_TO_FLOAT(revb.left);
	wetMix.right += REVERBSAMPLE_TO_FLOAT(revb.right);

	return wetMix;
}

void synth_keyChange(synth_s *s, int key, int state) {
	static int numOn = 0;

	if(state == 1) {
		++numOn;
		voice_setGate(&s->voice, key);
	}
	else if(state == 0) {
		if(--numOn <= 0)
			voice_clearGate(&s->voice, key);
	}

}
