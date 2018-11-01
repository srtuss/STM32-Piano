#ifndef _FILTER_H_
#define _FILTER_H_

typedef struct filter_t {
	float hp;
	float lp;
	float bp;
	float kQ; ///< Q coefficient
	float kF; ///< cutoff-frequency coefficient
} filter_s;

extern void filter_init(filter_s *s);
extern void filter_clock(filter_s* s, float in);

#endif
