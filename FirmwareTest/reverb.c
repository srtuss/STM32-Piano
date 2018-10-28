#include "reverb.h"

#include <string.h>
#include <stdio.h>

static reverbsample_t delay_getCurrent(rvbdelay_s *s)
{
	return s->buffer[s->at];
}

static reverbsample_t delay_getOffs(rvbdelay_s *s, int offset)
{
	offset = s->at + offset;
	offset %= s->length;
	return s->buffer[offset];
}

static reverbsample_t delay_clock(rvbdelay_s *s, reverbsample_t in)
{
	reverbsample_t res = s->buffer[s->at];

	s->buffer[s->at] = in;

	if(++s->at >= s->length)
		s->at = 0;

	return res;
}

//#define ARITH_SAT
#define RVB_FRACT_MUL(s, f) (((int)(s) * (int)(1024 * (f))) >> 10)

static reverbsample_t revb_add(int a, int b)
{
	int res = a + b;
#ifdef ARITH_SAT
	if(res > REVERBSAMPLE_MAX)
		res = REVERBSAMPLE_MAX;
	else if(res < REVERBSAMPLE_MIN)
		res = REVERBSAMPLE_MIN;
#else
	if(res > REVERBSAMPLE_MAX || res < REVERBSAMPLE_MIN)
		abort();
#endif
	return res;
}

static reverbsample_t revb_subtract(int a, int b)
{
	int res = a - b;
#ifdef ARITH_SAT
	if(res > REVERBSAMPLE_MAX)
		res = REVERBSAMPLE_MAX;
	else if(res < REVERBSAMPLE_MIN)
		res = REVERBSAMPLE_MIN;
#else
	if(res > REVERBSAMPLE_MAX || res < REVERBSAMPLE_MIN)
		abort();
#endif
	return res;
}

static reverbsample_t delay_allpassClock(rvbdelay_s *s, reverbsample_t in)
{
	reverbsample_t tap = delay_getCurrent(s);
	reverbsample_t nextTap = revb_add(in, (tap >> 1));
	reverbsample_t out = tap - (nextTap >> 1);
	delay_clock(s, nextTap);
	return out;
}

reverb_stereo_s reverb_clock(reverb_s *s, reverbsample_t in)
{
	in = delay_clock(&s->predelay, in);

	srand(123456);
	reverb_stereo_s result = { 0, 0 };
	for(int i = 0; i < 4; ++i)
	{
		s->cur += in;
		s->cur = delay_allpassClock(&s->delays[i * 3], s->cur);
		s->cur = delay_allpassClock(&s->delays[i * 3 + 1], s->cur);
		s->cur = delay_clock(&s->delays[i * 3 + 2], s->cur);
		result.left += delay_getOffs(&s->delays[i * 3 + 2], rand());
		result.right += delay_getOffs(&s->delays[i * 3 + 2], rand());
		s->cur = RVB_FRACT_MUL(s->cur, .8);
	}

	return result;
}

#define DLT(v) ((v) * RVB_MAX_DELAYBUFFER)

const static int delayLengths[] =
{
	DLT(0.0821132958749669),
	DLT(0.0758567940494999),
	DLT(0.071788697870657),
	DLT(0.03962452930745),
	DLT(0.112479877901124),
	DLT(0.0574113072058072),
	DLT(0.07680989754962),
	DLT(0.106478579890313),
	DLT(0.116303867112087),
	DLT(0.09111474765869),
	DLT(0.0709115685709063),
	DLT(0.099106837008878)
};

void reverb_init(reverb_s *s)
{
	memset(s->delaybuffer, 0, sizeof(s->delaybuffer));
	memset(s->predelayBuffer, 0, sizeof(s->predelayBuffer));

	s->predelay.at = 0;
	s->predelay.buffer = s->predelayBuffer;
	s->predelay.length = RVB_MAX_PREDELAYBUFFER;

	int pos = 0;
	for(int i = 0; i < RVB_MAX_DELAYS; ++i)
	{
		s->delays[i].at = 0;
		s->delays[i].length = delayLengths[i];
		s->delays[i].buffer = s->delaybuffer + pos;

		pos += delayLengths[i];
	}
	if(pos >= RVB_MAX_DELAYBUFFER)
		abort();
}