#include "voice.h"
#include <math.h>
#include <stdlib.h>

#define SAMPLERATE 44100

void envelope_init(envelope_s *s) {
	s->state = ADSR_STATE_ATTACK;
	s->v = 0;
	s->kA = 500.f / SAMPLERATE;
	s->kR = .99f;
}

void envelope_clock(envelope_s *s) {
	switch(s->state) {
	case ADSR_STATE_ATTACK:
		s->v += s->kA;
		if(s->v > 1) {
			s->v = 1;
			s->state = ADSR_STATE_SUSTAIN;
		}
		break;
	case ADSR_STATE_RELEASE:
		s->v *= s->kR;
		break;
	}
}

void voice_init(voice_s *s)
{
	s->state = 0;
	filter_init(&s->filters[0]);
	filter_init(&s->filters[1]);
	envelope_init(&s->envelope);
	envelope_init(&s->fltEnvelope);
}

void voice_setGate(voice_s *s, int note)
{
	float dt = 1;

	for(int i = 0; i < VOICE_MAX_OSC; ++i) {
		float fbase = exp2((note - 69 + 48) / 12.) * 440.f / SAMPLERATE;

		fbase *= dt;
		dt *= 1.002f;

		s->oscPan[i] = rand() / (float)RAND_MAX;

		osc_init(&s->oscs[i], fbase);
	}
	s->state = 1;
	envelope_init(&s->envelope);
	envelope_init(&s->fltEnvelope);
}

void voice_clearGate(voice_s *s, int note)
{
	s->envelope.state = ADSR_STATE_RELEASE;
}

voice_stereo_s voice_clock(voice_s *s)
{
	voice_stereo_s output = {0, 0};

	if(!s->state)
		return output;

	for(int i = 0; i < VOICE_MAX_OSC; ++i) {
		float v = osc_clock(&s->oscs[i]);
		output.left += v * s->oscPan[i];
		output.right += v * (1 - s->oscPan[i]);
	}

	s->filters[0].kF = s->fltEnvelope.v;
	s->filters[1].kF = s->fltEnvelope.v;
	envelope_clock(&s->fltEnvelope);

	filter_clock(&s->filters[0], output.left);
	filter_clock(&s->filters[1], output.right);
	output.left = s->filters[0].lp;
	output.right = s->filters[1].lp;

	output.left *= s->envelope.v * 4.f / VOICE_MAX_OSC;
	output.right *= s->envelope.v * 4.f / VOICE_MAX_OSC;

	envelope_clock(&s->envelope);

	if(s->envelope.state == ADSR_STATE_RELEASE && s->envelope.v < .01f)
		s->state = 0;

	return output;
}
