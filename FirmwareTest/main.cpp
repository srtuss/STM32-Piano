extern "C"
{
#include "synth.h"
}

#include <algorithm>
#include "wavWriter.h"
#include "serialPort.h"
#include "protocol.h"
#include "audioEndpoint.h"

void reverbTest()
{
	WavWriter wavWriter("output\\out.wav");
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
}

synth_s synth;

void process(float *dst, int numFrames)
{
	for(int i = 0; i < numFrames; ++i)
	{
		auto s = synth_clock(&synth);

		if(s.left > 1)
			s.left = 1;
		else if(s.left < -1)
			s.left = -1;
		if(s.right > 1)
			s.right = 1;
		else if(s.right < -1)
			s.right = -1;

		dst[i * 2] = s.left;
		dst[i * 2 + 1] = s.right;
	}
}

void synthTest()
{
	SerialPort serial;
	Protocol protocol;
	serial.Open("COM4");
	AudioEndpoint audioEndpoint;

	serial.Write("ready", 5);

	//reverbTest();

	synth_init(&synth);

	audioEndpoint.Open();

	while(1)
	{
		//audioEndpoint.Wait();

		size_t numFramesAvail = audioEndpoint.NumAvailable();
		if(numFramesAvail > 0)
		{
			unsigned char *buffer = audioEndpoint.GetBuffer(numFramesAvail);
			int frameAlign = 8;
			process((float*)buffer, numFramesAvail);
			audioEndpoint.ReleaseBuffer(numFramesAvail);
			//printf("%i\n", numFramesAvail);
		}


		if(serial.GetAvailableRead() > 0)
		{
			if(protocol.Next((char)serial.ReadByte()))
			{
				int key, state;
				if(sscanf(protocol.Get().c_str(), "%i %i", &key, &state) != 2)
					throw new std::exception();
				//printf("%i %i\n", key, state);
				synth_keyChange(&synth, key, state);
			}
		}

		_sleep(0);

	}
}

int main()
{
	reverbTest();
	synthTest();

    return 0;
}