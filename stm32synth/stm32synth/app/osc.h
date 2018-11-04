#ifndef _OSC_H_
#define _OSC_H_

enum wave_type_e {
	WAVE_SAW,
	WAVE_PULSE
};

typedef struct osc_t {
	float t;
	float tInc;
	float pulseWidth;
	unsigned char wave;
} osc_s;

extern void osc_init(osc_s *s, float tInc);
extern float osc_clock(osc_s *s);

#endif
