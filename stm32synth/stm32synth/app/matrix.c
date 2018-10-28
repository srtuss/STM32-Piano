#include "matrix.h"
#include "stm32f4xx_hal.h"
#include "mainApp.h"

#define MATRIX_NUM_ROWS (5)
#define MATRIX_NUM_COLS (8)
#define MATRIX_NUM_KEYS (MATRIX_NUM_ROWS * MATRIX_NUM_COLS)

static uint8_t keyStates[MATRIX_NUM_KEYS];

static GPIO_TypeDef* outPorts[] = {
	MATRIX_O0_GPIO_Port,
	MATRIX_O1_GPIO_Port,
	MATRIX_O2_GPIO_Port,
	MATRIX_O3_GPIO_Port,
	MATRIX_O4_GPIO_Port
};

static uint16_t outPins[] = {
	MATRIX_O0_Pin,
	MATRIX_O1_Pin,
	MATRIX_O2_Pin,
	MATRIX_O3_Pin,
	MATRIX_O4_Pin
};

static GPIO_TypeDef* inPorts[] = {
	MATRIX_I0_GPIO_Port,
	MATRIX_I1_GPIO_Port,
	MATRIX_I2_GPIO_Port,
	MATRIX_I3_GPIO_Port,
	MATRIX_I4_GPIO_Port,
	MATRIX_I5_GPIO_Port,
	MATRIX_I6_GPIO_Port,
	MATRIX_I7_GPIO_Port
};

static uint16_t inPins[] = {
	MATRIX_I0_Pin,
	MATRIX_I1_Pin,
	MATRIX_I2_Pin,
	MATRIX_I3_Pin,
	MATRIX_I4_Pin,
	MATRIX_I5_Pin,
	MATRIX_I6_Pin,
	MATRIX_I7_Pin
};

static uint8_t rowCnt = 0;
static uint32_t lastCheck = 0;

void matrix_init(void) {

}

void matrix_handler(void) {
	uint32_t now = HAL_GetTick();
	if(now > lastCheck + 1)
	{
		lastCheck = now;
		for(uint8_t i = 0; i < MATRIX_NUM_COLS; ++i) {
			int j = i + MATRIX_NUM_COLS * rowCnt;
			int v = HAL_GPIO_ReadPin(inPorts[i], inPins[i]);
			if(v != keyStates[j])
			{
				keyStates[j] = v;

				mainApp_keyChange(j, v);

				char buf[64];
				sprintf(buf, "~%i %i\r\n", j, v);
				mainApp_sendMessage(buf);
			}
		}
		HAL_GPIO_WritePin(outPorts[rowCnt], outPins[rowCnt], 0);
		if(++rowCnt >= MATRIX_NUM_ROWS)
			rowCnt = 0;
		HAL_GPIO_WritePin(outPorts[rowCnt], outPins[rowCnt], 1);
	}
}
