#include "reverb.h"
#include "trace.h"
#include <string.h>

static void delay_init(rvbdelay_s *s, rvbsample_t *buffer, int length)
{
	s->at = 0;
	s->buffer = buffer;
	s->length = length;
	//memset(s->buffer, 0, length * sizeof(rvbsample_t)); // this is done for all buffers at once, in reverb_init()
}

static rvbsample_t delay_getCurrent(rvbdelay_s *s)
{
	return s->buffer[s->at];
}

static rvbsample_t delay_getOffs(rvbdelay_s *s, int offset)
{
	offset = s->at + offset;
	offset %= s->length;
	return s->buffer[offset];
}

static rvbsample_t delay_clock(rvbdelay_s *s, rvbsample_t in)
{
	rvbsample_t res = s->buffer[s->at];

	s->buffer[s->at] = in;

	if(++s->at >= s->length)
		s->at = 0;

	return res;
}

//#define ARITH_SAT
#define RVB_FRACT_MUL(s, f) (((int)(s) * (int)(1024 * (f))) >> 10)

static rvbsample_t revb_add(int a, int b)
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

static rvbsample_t revb_subtract(int a, int b)
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

static rvbsample_t delay_allpassClock(rvbdelay_s *s, rvbsample_t in)
{
	rvbsample_t tap = delay_getCurrent(s);
	rvbsample_t nextTap = revb_add(in, (tap >> 1));
	rvbsample_t out = tap - (nextTap >> 1);
	delay_clock(s, nextTap);
	return out;
}

const static uint16_t randoms[] = {
	9977, 22818, 10150, 16017, 7706,
	20368, 21548, 8141, 828, 19946,
	30493, 26435, 32067, 19160, 10894,
	3199, 5151, 8919, 15384, 15754,
	14659, 20190, 6375, 27312, 13401,
	28226, 16398, 16262, 18484, 10199,
	12061, 11564, 9222, 9816, 31990,
	27088, 29907, 13320, 5852, 2419
};

reverb_stereo_s reverb_clock(reverb_s *s, rvbsample_t in)
{
	in = delay_clock(&s->predelay, in);

	int rs = 0;

	reverb_stereo_s result = { 0, 0 };
	for(int i = 0; i < 4; ++i)
	{
		s->cur += in;
		s->cur = delay_allpassClock(&s->delays[i * 3], s->cur);
		s->cur = delay_allpassClock(&s->delays[i * 3 + 1], s->cur);
		s->cur = delay_clock(&s->delays[i * 3 + 2], s->cur);
		result.left += delay_getOffs(&s->delays[i * 3 + 2], randoms[rs++]);
		result.right += delay_getOffs(&s->delays[i * 3 + 2], randoms[rs++]);
		s->cur = RVB_FRACT_MUL(s->cur, .8);
	}

	return result;
}

#define DLT(v) (uint16_t)((v) * RVB_MAX_DELAYBUFFER)

const static uint16_t delayLengths[] =
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
	s->cur = 0;
	delay_init(&s->predelay, s->predelayBuffer, RVB_MAX_PREDELAYBUFFER);

	int pos = 0;
	for(int i = 0; i < RVB_MAX_DELAYS; ++i)
	{
		delay_init(&s->delays[i], s->delaybuffer + pos, delayLengths[i]);

		pos += delayLengths[i];
	}
	if(pos >= RVB_MAX_DELAYBUFFER)
		abort();

	memset(s->delaybuffer, 0, sizeof(s->delaybuffer));
	memset(s->predelayBuffer, 0, sizeof(s->predelayBuffer));
}