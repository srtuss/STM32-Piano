#ifndef _SYNTH_H_
#define _SYNTH_H_

#include "voice.h"
#include "echo.h"
#include "reverb.h"

#define SYNTH_MAX_VOICES 6

typedef struct sequencer_t {
	int _tmp;
} sequencer_s;

typedef struct synth_t {
	voice_s voices[SYNTH_MAX_VOICES];
	echo_s echo;
	reverb_s reverb;
	float reverbAmt;
	float echoAmt;
	int8_t oscWaveform;
	float oscPulseWidth;


	control_s control;

	int numVoicesOn;
	int numDown;
	int monoKey;

	// sequencer
	uint8_t seqEnable;
	int bpm;
	int seqLastKey;
	int smpPerStep;
	int subStep;
	int stepPos;
	int seqLen;
} synth_s;

typedef struct synth_stereo_t {
	float left, right;
} synth_stereo_s;

enum synthParams_e {
	ParamFilterCutoff,
	ParamFilterReso,
	ParamVoiceNumUnison,
	ParamOscPulseWidth,
	ParamSeqEnable,
	ParamReverbAmount,
	ParamEchoAmount
};

extern void synth_init(synth_s *s);
extern synth_stereo_s synth_clock(synth_s *s);
extern void synth_keyChange(synth_s *s, int key, int state);
extern void synth_setParameter(synth_s *s, int index, float value);
extern float synth_getParameter(synth_s *s, int index);

#endif
