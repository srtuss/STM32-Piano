extern "C"
{
#include "voice.h"
#include "reverb.h"
}

#include <algorithm>
#include "wavWriter.h"

int main()
{
	WavWriter wavWriter("out.wav");
	reverb_s reverb;

	reverb_init(&reverb);

	int signalLength = 3000;

	for(int i = 0; i < 300000; ++i)
	{
		float dry = i < signalLength ? sinf(i / 6.f) * std::min(1.f, i * .01f) * exp(std::max(i - signalLength + 400, 0) * -.01f) * .25f : 0;

		auto out =
			//REVERBSAMPLE_FROM_FLOAT(sinf(i / 100.f));
			//reverb_clock(&reverb, i == 0 ? REVERBSAMPLE_MAX >> 8 : 0);
			reverb_clock(&reverb, REVERBSAMPLE_FROM_FLOAT(dry));
		wavWriter.Write(
			REVERBSAMPLE_TO_FLOAT(out.left) + dry * 2,
			REVERBSAMPLE_TO_FLOAT(out.right) + dry * 2);
	}

    return 0;
}