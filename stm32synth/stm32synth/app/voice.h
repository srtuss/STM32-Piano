#ifndef _VOICE_H_
#define _VOICE_H_

#include "osc.h"
#include "filter.h"

#define VOICE_MAX_OSC 3
#define VOICE_MAX_FILTERS 2

enum adsr_state_e {
	ADSR_STATE_ATTACK,
	ADSR_STATE_DECAY,
	ADSR_STATE_SUSTAIN,
	ADSR_STATE_RELEASE
};

typedef struct envelope_t {
	float v;
	int state;
	float kA; ///< Attack coefficient
	float kD; ///< Decay coefficient
	float kR; ///< Release coefficient
	float kS; ///< Sustain level
} envelope_s;

typedef struct modulation_t {
	float gain;
	float bias;
} modulation_s;

typedef struct voice_t
{
	int state;
	int keyID; ///< set by the user
	osc_s oscs[VOICE_MAX_OSC];
	float oscPan[VOICE_MAX_OSC];
	filter_s filters[VOICE_MAX_FILTERS];
	envelope_s envelope, fltEnvelope;
	modulation_s fltModulation;
} voice_s;

typedef struct voice_stereo_t
{
	float left, right;
} voice_stereo_s;

extern void voice_init(voice_s *s);
extern voice_stereo_s voice_clock(voice_s *s);
extern void voice_setGate(voice_s *s, int note);
extern void voice_clearGate(voice_s *s, int note);

#endif
