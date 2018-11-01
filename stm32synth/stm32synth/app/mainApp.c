#include <string.h>
#include "mainApp.h"
#include "matrix.h"
#include "synth.h"
#include "dacTest.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
#include "dac.h"

extern UART_HandleTypeDef huart2;
typedef struct dac_sample_t {
	uint16_t left, right;
} dac_sample_s;
#define MAX_DAC_SAMPLES 8

synth_s synth;
int8_t smpbufHave = 0, smpbufPtr = 0;
dac_sample_s smpbuf[MAX_DAC_SAMPLES];

int8_t transpose = 0;

void mainApp_init(void) {
	matrix_init();
	synth_init(&synth);
	dacTest_init();

	HAL_TIM_Base_Start_IT(&htim2);
}

void mainApp_handler(void) {
	matrix_handler();
	//dacTest_handler();


	uint32_t now = HAL_GetTick();
	static uint32_t lastCheck = 0;
	static uint8_t timeout = 0;

	if(now != lastCheck) {
		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) {
			if(timeout == 0) {
				if(++transpose >= 12)
					transpose = 0;
				//mainApp_sendMessage("button");
			}
			timeout = 50;
		}
		if(timeout > 0)
			--timeout;
		lastCheck = now;
	}

	if(smpbufHave < 8) {
		HAL_GPIO_WritePin(DEBUG0_GPIO_Port, DEBUG0_Pin, 1);

		synth_stereo_s nextSample = synth_clock(&synth);

		if(nextSample.left > 1)
			nextSample.left = 1;
		else if(nextSample.left < -1)
			nextSample.left = -1;
		if(nextSample.right > 1)
			nextSample.right = 1;
		else if(nextSample.right < -1)
			nextSample.right = -1;

		smpbuf[smpbufPtr].left = 0x800 + (int)(0x300 * nextSample.left);
		smpbuf[smpbufPtr].right = 0x800 + (int)(0x300 * nextSample.right);

		if(++smpbufPtr >= MAX_DAC_SAMPLES)
			smpbufPtr -= MAX_DAC_SAMPLES;
		++smpbufHave;

		HAL_GPIO_WritePin(DEBUG0_GPIO_Port, DEBUG0_Pin, 0);
	}
	//dacTest_put(nextSample.left, nextSample.right);
}

void mainApp_sendMessage(const char *str) {
	HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 500);
}

void mainApp_keyChange(int key, int state) {
	synth_keyChange(&synth, key + transpose, state);

	char buf[64];
	sprintf(buf, "~%i %i\r\n", key, state);
	mainApp_sendMessage(buf);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	//dacTest_put(nextSample.left, nextSample.right);
	if(smpbufHave > 0) {
		int8_t rp = smpbufPtr - smpbufHave;
		if(rp < 0)
			rp += MAX_DAC_SAMPLES;

		dac_sample_s tmp = smpbuf[rp];
		--smpbufHave;

		HAL_DACEx_DualSetValue(&hdac, DAC_ALIGN_12B_R, tmp.left, tmp.right);
	}

	HAL_GPIO_TogglePin(DEBUG1_GPIO_Port, DEBUG1_Pin);


	//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, nextLeft);
	//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, nextRight);
}
