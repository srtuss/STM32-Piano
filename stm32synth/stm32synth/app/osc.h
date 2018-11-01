#ifndef _OSC_H_
#define _OSC_H_

typedef struct osc_t{
	float t;
	float tInc;
} osc_s;

extern void osc_init(osc_s *s, float tInc);
extern float osc_clock(osc_s *s);

#endif