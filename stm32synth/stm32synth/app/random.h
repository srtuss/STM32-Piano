/*
 * random.h
 *
 *  Created on: 01.11.2018
 *      Author: srtuss-tower
 */

#ifndef _RANDOM_H_
#define _RANDOM_H_

static uint32_t __rnd = 1;

static float randomFloat() {
	__rnd *= 48271;
	uint32_t a = (__rnd & 0x7FFFFF) | 0x40000000;
	return (*(float*)&a) - 3.f;
}

#endif /* _RANDOM_H_ */
