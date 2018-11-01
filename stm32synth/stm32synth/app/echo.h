#ifndef _ECHO_H_
#define _ECHO_H_

#include <stdint.h>


#define ECHO_16BIT

#ifdef ECHO_16BIT
typedef int16_t echo_sample_t;
#define ECHO_MAX_DELAYBUFFER		(11000)
#define ECHOSAMPLE_MAX				(32767)
#define ECHOSAMPLE_MIN				(-32768)
#define ECHOSAMPLE_TO_FLOAT(v)		((v) / 32768.f)
#define ECHOSAMPLE_FROM_FLOAT(v)	((echo_sample_t)((v) * 32767.99f))
#else
typedef int8_t echo_sample_t;
#define ECHO_MAX_DELAYBUFFER		(22050)
#define ECHOSAMPLE_MAX				(127)
#define ECHOSAMPLE_MIN				(-128)
#define ECHOSAMPLE_TO_FLOAT(v)		((v) / 128.f)
#define ECHOSAMPLE_FROM_FLOAT(v)	((echo_sample_t)((v) * 127.99f))
#endif

typedef struct echo_delay_t
{
	echo_sample_t *buffer;
	int length;
	int at;
} echo_delay_s;

typedef struct echo_t {
	echo_sample_t delaybufferA[ECHO_MAX_DELAYBUFFER - 500];
	echo_sample_t delaybufferB[ECHO_MAX_DELAYBUFFER];
	echo_delay_s delays[2];
} echo_s;

typedef struct echo_stereo_t {
	echo_sample_t left, right;
} echo_stereo_s;

extern void echo_init(echo_s* s);
extern echo_stereo_s echo_clock(echo_s *s, echo_sample_t left, echo_sample_t right);

#endif