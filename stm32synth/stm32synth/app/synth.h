#ifndef _SYNTH_H_
#define _SYNTH_H_

#include "voice.h"
#include "reverb.h"

typedef struct synth_t
{
	voice_s voice;
	reverb_s reverb;
} synth_s;

typedef struct synth_stereo_t
{
	float left, right;
} synth_stereo_s;

extern void synth_init(synth_s *s);
extern synth_stereo_s synth_clock(synth_s *s);
extern void synth_keyChange(synth_s *s, int key, int state);

#endif
