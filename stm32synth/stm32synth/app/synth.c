#include "synth.h"
#include "macro.h"

void synth_init(synth_s *s) {
	for(int i = 0; i < SYNTH_MAX_VOICES; ++i) {
		voice_init(&s->voices[i]);
	}
	echo_init(&s->echo);
	reverb_init(&s->reverb);
}

synth_stereo_s synth_clock(synth_s *s) {
	
	synth_stereo_s mix = {0, 0};

	for(int i = 0; i < SYNTH_MAX_VOICES; ++i)
	{
		voice_stereo_s v = voice_clock(&s->voices[i]);
		mix.left += v.left;
		mix.right += v.right;
	}

	mix.left /= 10;
	mix.right /= 10;

	echo_stereo_s echo = echo_clock(&s->echo,
		ECHOSAMPLE_FROM_FLOAT(mix.left),
		ECHOSAMPLE_FROM_FLOAT(mix.right));

	mix.left += ECHOSAMPLE_TO_FLOAT(echo.left);
	mix.right += ECHOSAMPLE_TO_FLOAT(echo.right);

	reverb_stereo_s revb = reverb_clock(&s->reverb,
		REVERBSAMPLE_FROM_FLOAT(mix.left * .08f));

	//wetMix.left = 0;
	//wetMix.right = 0;

	mix.left += REVERBSAMPLE_TO_FLOAT(revb.left);
	mix.right += REVERBSAMPLE_TO_FLOAT(revb.right);

	/*float mid = wetMix.left + wetMix.right;
	float side = wetMix.right - wetMix.left;

	wetMix.left = mid * .25f + side;
	wetMix.right = mid * .25f - side;*/

	return mix;
}

static int numOn = 0;

static void synth_startVoice(synth_s *s, int key) {
	int best = 0, found = 0;
	float bestV = 10000;
	for(int i = 0; i < SYNTH_MAX_VOICES; ++i)
	{
		if(s->voices[i].state == 0)
		{
			s->voices[i].keyID = key;
			voice_setGate(&s->voices[i], key);
			found = 1;
			++numOn;
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
		++numOn;
	}
}

static void synth_stopVoice(synth_s *s, int key) {
	for(int i = 0; i < SYNTH_MAX_VOICES; ++i)
	{
		if(s->voices[i].state != 0 && s->voices[i].keyID == key)
		{
			voice_clearGate(&s->voices[i], key);
			--numOn;
		}
	}
}

void synth_keyChange(synth_s *s, int key, int state) {

	const int8_t chord[] = { 0/*, 12, 24, 24 +3*/ };

	for(int i = 0; i < ARRAY_LEN(chord); ++i) {
		if(state == 1) {
			synth_startVoice(s, key + chord[i]);
		}
		else if(state == 0) {
			synth_stopVoice(s, key + chord[i]);
		}
	}

}
