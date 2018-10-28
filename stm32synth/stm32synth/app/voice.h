#ifndef _VOICE_H_
#define _VOICE_H_

typedef struct voice_t
{
	int state;
	float phase;
	float phaseInc;
} voice_s;

extern void voice_init(voice_s *s);
extern float voice_clock(voice_s *s);
extern void voice_setGate(voice_s *s, int note);
extern void voice_clearGate(voice_s *s, int note);

#endif