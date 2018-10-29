extern "C"
{
#include "voice.h"
#include "reverb.h"
}

#include <algorithm>
#include "wavWriter.h"
#include "serialPort.h"
#include "protocol.h"

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


#include "audioEndpoint.h"

void synthTest()
{
	SerialPort serial;
	Protocol protocol;
	serial.Open("COM4");
	AudioEndpoint audioEndpoint;

	audioEndpoint.Open();

	reverbTest();

	voice_s voice;
	voice_init(&voice);

	reverb_s reverb;
	reverb_init(&reverb);

	int numActiveVoices = 0;

	while (1)
	{
		//audioEndpoint.Wait();

		size_t numFramesAvail = audioEndpoint.NumAvailable();
		float *buffer = (float*)audioEndpoint.GetBuffer(numFramesAvail);
		int frameAlign = 8;
		for (int pos = 0; pos < numFramesAvail; ++pos)
		{
			float left = voice_clock(&voice) * .5f;
			float right = left;

			auto revb = reverb_clock(&reverb, REVERBSAMPLE_FROM_FLOAT(left * .15f));
			left += REVERBSAMPLE_TO_FLOAT(revb.left);
			right += REVERBSAMPLE_TO_FLOAT(revb.right);

			buffer[pos * 2] = left;
			buffer[pos * 2 + 1] = right;
		}
		audioEndpoint.ReleaseBuffer(numFramesAvail);



		if (serial.GetAvailableRead() > 0)
		{
			if (protocol.Next((char)serial.ReadByte()))
			{
				int key, state;
				if (sscanf(protocol.Get().c_str(), "%i %i", &key, &state) != 2)
					throw new std::exception();
				//printf("%i %i\n", key, state);
				if (state == 1)
				{
					voice_setGate(&voice, key);
					++numActiveVoices;
				}
				else
				{
					--numActiveVoices;
					if (numActiveVoices == 0)
						voice_clearGate(&voice, key);
				}
			}
		}

		_sleep(0);

	}
}

int main()
{
	reverbTest();
	//synthTest();

    return 0;
}