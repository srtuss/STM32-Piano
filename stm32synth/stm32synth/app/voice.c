#include "voice.h"
#include <math.h>

#define SAMPLERATE 44100

void voice_init(voice_s *s)
{
	s->phase = 0;
	s->state = 0;
}

void voice_setGate(voice_s *s, int note)
{
	s->phaseInc = exp2((note - 69 + 48) / 12.) * 440.f / SAMPLERATE;
	s->state = 1;
}

void voice_clearGate(voice_s *s, int note)
{
	s->state = 0;
}

static inline float blep(float t, float dt)
{
	// 0 <= t < 1
	if(t < dt)
	{
		t /= dt;
		return t + t - t*t - 1.f;
	}
	// -1 < t < 0
	else if(t > 1.f - dt)
	{
		t = (t - 1.f) / dt;
		return t*t + t + t + 1.f;
	}
	// 0 otherwise
	else
		return 0.f;
}

float voice_clock(voice_s *s)
{
	float output = 0;

	if(!s->state)
		return 0;

	output += (s->phase - .5f) * 2.f - blep(s->phase, s->phaseInc);

	s->phase += s->phaseInc;
	while(s->phase >= 1)
		s->phase -= 1;

	return output;
}