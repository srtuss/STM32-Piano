#include <string.h>
#include "echo.h"

static void delay_init(echo_delay_s *s, echo_sample_t *buffer, int length) {
	s->at = 0;
	s->buffer = buffer;
	s->length = length;
	memset(s->buffer, 0, length * sizeof(buffer[0]));
}

static echo_sample_t delay_getCurrent(echo_delay_s *s) {
	return s->buffer[s->at];
}

static echo_sample_t delay_getOffs(echo_delay_s *s, int offset) {
	offset = s->at + offset;
	offset %= s->length;
	return s->buffer[offset];
}

static echo_sample_t delay_clock(echo_delay_s *s, echo_sample_t in) {
	echo_sample_t res = s->buffer[s->at];

	s->buffer[s->at] = in;

	if(++s->at >= s->length)
		s->at = 0;

	return res;
}

void echo_init(echo_s* s) {
	delay_init(&s->delays[0], &s->delaybufferA, ECHO_MAX_DELAYBUFFER - 1000);
	delay_init(&s->delays[1], &s->delaybufferB, ECHO_MAX_DELAYBUFFER);
}

echo_stereo_s echo_clock(echo_s *s, echo_sample_t left, echo_sample_t right) {
	echo_stereo_s res;
	res.left = delay_clock(&s->delays[0], (left + delay_getCurrent(&s->delays[1]) >> 1));
	res.right = delay_clock(&s->delays[1], (right + delay_getCurrent(&s->delays[0]) >> 1));
	return res;
}
