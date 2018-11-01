#include "osc.h"

#include <stdlib.h>

void osc_init(osc_s *s, float tInc) {
	s->t = (float)rand() / RAND_MAX;
	s->tInc = tInc;
}

static inline float blep(float t, float dt)
{
	// 0 <= t < 1
	if(t < dt)
	{
		t /= dt;
		return t + t - t * t - 1.f;
	}
	// -1 < t < 0
	else if(t > 1.f - dt)
	{
		t = (t - 1.f) / dt;
		return t * t + t + t + 1.f;
	}
	// 0 otherwise
	else
		return 0.f;
}

float osc_clock(osc_s *s) {
	float output = 0;
	output += s->t * 2 - 1 - blep(s->t, s->tInc);

	s->t += s->tInc;
	while(s->t >= 1)
		s->t -= 1;

	return output;
}