#include "synth.h"
#include "macro.h"

#define SAMPLERATE 44100

static void synth_startVoice(synth_s *s, int key);

static void synth_stopVoice(synth_s *s, int key);

void synth_init(synth_s *s) {
	for(int i = 0; i < SYNTH_MAX_VOICES; ++i) {
		voice_init(&s->voices[i]);
	}
	echo_init(&s->echo);
	reverb_init(&s->reverb);
	s->reverbAmt = .08f;
	s->echoAmt = .5f;
	s->oscWaveform = WAVE_PULSE;
	s->oscPulseWidth = .5f;

	s->seqEnable = 1;

	s->numVoicesOn = 0;
	s->bpm = 120;
	s->smpPerStep = SAMPLERATE * 60 / 4 / s->bpm;
	s->subStep = 0;
	s->stepPos = 0;
	s->seqLen = 8;
	s->monoKey = -1;
	s->numDown = 0;
	s->seqLastKey = -1;

	memset(&s->control, 0, sizeof(s->control));
}

synth_stereo_s synth_clock(synth_s *s) {
	
	synth_stereo_s mix = {0, 0};

	if(s->seqEnable) {
		const static char seq[] = {
			0, 0, 12, 0, 0, 12, 0, 12,
			0, 0, 12, 0, 0, 12, 0, 10};

		if(s->subStep <= 0)
		{
			if(s->monoKey != -1) {
				if(s->seqLastKey != -1)
					synth_stopVoice(s, s->seqLastKey);

				s->seqLastKey = s->monoKey + seq[s->stepPos];
				synth_startVoice(s, s->seqLastKey);
			}
			if(++s->stepPos >= s->seqLen)
				s->stepPos = 0;

			s->subStep += s->smpPerStep;
		}
		--s->subStep;
	}

	for(int i = 0; i < SYNTH_MAX_VOICES; ++i)
	{
		voice_stereo_s v = voice_clock(&s->voices[i], &s->control);
		mix.left += v.left;
		mix.right += v.right;
	}

	mix.left /= 5;
	mix.right /= 5;

	echo_stereo_s echo = echo_clock(&s->echo,
		ECHOSAMPLE_FROM_FLOAT(mix.left),
		ECHOSAMPLE_FROM_FLOAT(mix.right));

	mix.left += ECHOSAMPLE_TO_FLOAT(echo.left) * s->echoAmt;
	mix.right += ECHOSAMPLE_TO_FLOAT(echo.right) * s->echoAmt;

	reverb_stereo_s revb = reverb_clock(&s->reverb,
		REVERBSAMPLE_FROM_FLOAT(mix.left * s->reverbAmt));

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

static void synth_startVoice(synth_s *s, int key) {
	int best = 0, found = 0;
	float bestV = 10000;
	for(int i = 0; i < SYNTH_MAX_VOICES; ++i)
	{
		voice_s *voice = &s->voices[i];

		if(voice->state == 0)
		{
			voice->keyID = key;
			voice_setGate(voice, key);

			for(int j = 0; j < voice->numOscs; ++j) {
				voice->oscs[j].wave = s->oscWaveform;
				voice->oscs[j].pulseWidth = s->oscPulseWidth;
			}

			found = 1;
			++s->numVoicesOn;
			break;
		}
		if(voice->envelope.state == ADSR_STATE_RELEASE) {
			if(voice->envelope.v < bestV) {
				bestV = voice->envelope.v;
				best = i;
			}
		}
	}
	if(!found) {
		s->voices[best].keyID = key;
		voice_setGate(&s->voices[best], key);
		++s->numVoicesOn;
	}
}

static void synth_stopVoice(synth_s *s, int key) {
	for(int i = 0; i < SYNTH_MAX_VOICES; ++i)
	{
		if(s->voices[i].state != 0 && s->voices[i].keyID == key)
		{
			voice_clearGate(&s->voices[i], key);
			--s->numVoicesOn;
		}
	}
}

void synth_keyChange(synth_s *s, int key, int state) {

	const int8_t chord[] = { 0/*, 12, 24, 24 +3*/ };


	if(state) {
		++s->numDown;
		s->monoKey = key;
	}
	else {
		if(--s->numDown <= 0) {
			s->monoKey = -1;
		}
	}

	if(s->seqEnable) {
		if(s->monoKey == -1 && s->seqLastKey != -1) {
			synth_stopVoice(s, s->seqLastKey);
			s->seqLastKey = -1;
		}
	}
	else {
		for(int i = 0; i < ARRAY_LEN(chord); ++i) {
			if(state == 1) {
				synth_startVoice(s, key + chord[i]);
			}
			else if(state == 0) {
				synth_stopVoice(s, key + chord[i]);
			}
		}
	}
}

#define _verify_param(v)

void synth_setParameter(synth_s *s, int index, float value)
{
	switch(index) {
	case ParamFilterCutoff:
		s->control.cutoffBase = value;
		break;
	case ParamFilterReso:
		s->control.resoBase = value;
		break;
	case ParamOscPulseWidth:
		s->control.pulseWidth = value;
		break;
	case ParamSeqEnable:
		if(value > .5f)
			s->seqEnable = 1;
		else
			s->seqEnable = 0;
		break;
	case ParamReverbAmount:
		s->reverbAmt = value;
		break;
	case ParamEchoAmount:
		s->echoAmt = value;
		break;
	}
}

float synth_getParameter(synth_s *s, int index)
{
	switch(index) {
	case ParamSeqEnable:
		return s->seqEnable;
	case ParamReverbAmount:
		return s->reverbAmt;
	case ParamEchoAmount:
		return s->echoAmt;
	}
	return -1;
}