#include "dacTest.h"

#include "dac.h"

int test[128] = {};

void dacTest_init() {

    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
    HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
    //HAL_DACEx_TriangleWaveGenerate(&hdac, DAC_CHANNEL_1, 0x10000);

    for(int i = 0; i < 100; ++i) {
        test[i] = rand();
    }


}

void dacTest_handler() {
    static int x = 0x7FFF, y = 0;
    static int z = 0x7FF, w = 0;

    //HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x800 + (x >> 5) + (x >> 7) + (z >> 2));
    //HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x800 + (y >> 5) + (y >> 7) + (w >> 2));

    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, test, 100 * 4, DAC_ALIGN_12B_R);

    y -= x >> 4;
    x += y >> 4;
    w -= z >> 4;
    z += w >> 3;
}
