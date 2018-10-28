#include <string.h>
#include "mainApp.h"
#include "matrix.h"
#include "synth.h"
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;
synth_s synth;

void mainApp_init(void) {
	matrix_init();
	synth_init(&synth);
}

void mainApp_handler(void) {
	matrix_handler();
}

void mainApp_sendMessage(const char *str) {
	HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 500);
}

void mainApp_keyChange(int key, int state) {
	synth_keyChange(&synth, key, state);
}
