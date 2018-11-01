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

#ifdef _WIN32
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
#else
#define revb_add(a, b) ((rvbsample_t)((a)+(b)))
#define revb_subtract(a, b) ((rvbsample_t)((a)-(b)))
#endif

static rvbsample_t delay_allpassClock(rvbdelay_s *s, rvbsample_t in)
{
	rvbsample_t tap = delay_getCurrent(s);
	rvbsample_t nextTap = revb_add(in, (tap >> 1));
	rvbsample_t out = tap - (nextTap >> 1);
	delay_clock(s, nextTap);
	return out;
}

reverb_stereo_s reverb_clock(reverb_s *s, rvbsample_t in)
{
	in = delay_clock(&s->predelay, in);

	int rs = 0;

	reverb_stereo_s result = { 0, 0 };
	for(int i = 0; i < RVB_STAGES; ++i)
	{
		rvbstage_s *stage = &s->stages[i];

		s->cur += in;
		s->cur = delay_allpassClock(&stage->delay[0], s->cur);
		s->cur = delay_allpassClock(&stage->delay[1], s->cur);
		s->cur = delay_clock(&stage->delay[2], s->cur);
		result.left += delay_getOffs(&stage->delay[2], stage->tapOffsets[0]);
		result.right += delay_getOffs(&stage->delay[2], stage->tapOffsets[1]);
		s->cur = RVB_FRACT_MUL(s->cur, RVB_LOOP_FEEDBACK);
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
	srand(123456);

	s->cur = 0;
	delay_init(&s->predelay, s->predelayBuffer, RVB_MAX_PREDELAYBUFFER);

	int pos = 0, index = 0;
	for(int i = 0; i < RVB_STAGES; ++i)
	{
		rvbstage_s *stage = &s->stages[i];
		for(int j = 0; j < RVB_DELAYS_PER_STAGE; ++j)
		{
			delay_init(&stage->delay[j], s->delaybuffer + pos, delayLengths[index]);
			pos += delayLengths[index];
			++index;
		}
		stage->tapOffsets[0] = rand() % stage->delay[2].length;
		stage->tapOffsets[1] = rand() % stage->delay[2].length;
	}
	if(pos >= RVB_MAX_DELAYBUFFER)
		abort();

	memset(s->delaybuffer, 0, sizeof(s->delaybuffer));
	memset(s->predelayBuffer, 0, sizeof(s->predelayBuffer));
}
