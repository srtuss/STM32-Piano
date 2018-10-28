//
// a simple realistic reverberator
// based on the Spin Semiconductor Knowledge Base:
// http://www.spinsemi.com/knowledge_base/effects.html
//

#ifndef _REVERB_H_
#define _REVERB_H_

#include <stdint.h>

typedef int16_t rvbsample_t;

typedef struct reverb_stereo_t
{
	rvbsample_t left, right;
} reverb_stereo_s;

#define REVERBSAMPLE_MAX			(32767)
#define REVERBSAMPLE_MIN			(-32768)
#define REVERBSAMPLE_TO_FLOAT(v)	((v) / 32768.f)
#define REVERBSAMPLE_FROM_FLOAT(v)	((rvbsample_t)((v) * 32767.99f))
#define	RVB_MAX_DELAYS				(12)

#define RVB_MAX_DELAYBUFFER			(1024 * 32)
#define RVB_MAX_PREDELAYBUFFER		(1024 * 4)

typedef struct rvbdelay_t
{
	rvbsample_t *buffer;
	int length;
	int at;
} rvbdelay_s;

typedef struct reverb_t
{
	rvbsample_t cur;
	rvbsample_t delaybuffer[RVB_MAX_DELAYBUFFER];
	rvbsample_t predelayBuffer[RVB_MAX_PREDELAYBUFFER];
	rvbdelay_s delays[RVB_MAX_DELAYS];
	rvbdelay_s predelay;
} reverb_s;

extern void reverb_init(reverb_s *s);
extern reverb_stereo_s reverb_clock(reverb_s *s, rvbsample_t in);

#endif
