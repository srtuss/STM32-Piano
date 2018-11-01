#include "synth.h"

void synth_init(synth_s *s) {
	for(int i = 0; i < SYNTH_MAX_VOICES; ++i)
	{
		voice_init(&s->voices[i]);
	}
	reverb_init(&s->reverb);
}

synth_stereo_s synth_clock(synth_s *s) {
	
	synth_stereo_s wetMix = {0, 0};

	for(int i = 0; i < SYNTH_MAX_VOICES; ++i)
	{
		voice_stereo_s v = voice_clock(&s->voices[i]);
		wetMix.left += v.left;
		wetMix.right += v.right;
	}

	wetMix.left /= 3;
	wetMix.right /= 3;

	reverb_stereo_s revb = reverb_clock(&s->reverb, REVERBSAMPLE_FROM_FLOAT(wetMix.left * .08f));
	wetMix.left += REVERBSAMPLE_TO_FLOAT(revb.left);
	wetMix.right += REVERBSAMPLE_TO_FLOAT(revb.right);

	return wetMix;
}

void synth_keyChange(synth_s *s, int key, int state) {
	static int numOn = 0;

	if(state == 1) {
		++numOn;
		
		int best = 0, found = 0;
		float bestV = 10000;
		for(int i = 0; i < SYNTH_MAX_VOICES; ++i)
		{
			if(s->voices[i].state == 0)
			{
				s->voices[i].keyID = key;
				voice_setGate(&s->voices[i], key);
				found = 1;
				break;
			}
			if(s->voices[i].envelope.state == ADSR_STATE_RELEASE) {
				if(s->voices[i].envelope.v < bestV) {
					bestV = s->voices[i].envelope.v;
					best = i;
				}
			}
		}
		if(!found) {
			s->voices[best].keyID = key;
			voice_setGate(&s->voices[best], key);
		}
	}
	else if(state == 0) {
		for(int i = 0; i < SYNTH_MAX_VOICES; ++i)
		{
			if(s->voices[i].state != 0 && s->voices[i].keyID == key)
			{
				voice_clearGate(&s->voices[i], key);
			}
		}

		--numOn;
		//if(numOn <= 0)
		//	voice_clearGate(&s->voices[0], key);
	}

}
