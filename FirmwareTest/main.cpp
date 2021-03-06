extern "C"
{
#include "synth.h"
}

#include <algorithm>
#include <thread>

#include "wavWriter.h"
#include "serialPort.h"
#include "protocol.h"
#include "audioContext.h"
#include "tcpSocket.h"

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

static inline float saturate(float x)
{
	if(x > 1)
		return 1;
	else if(x < -1)
		return -1;
	return x;
}

class App
{
	SerialPort serial;
	Protocol protocol, controlProtocol;
	TcpSocket control;
	std::shared_ptr<AudioContext> audioContext;

	synth_s synth;

	void SerialFrameReceived(const std::string &txt)
	{
		int key, state;
		if(sscanf(protocol.Get().c_str(), "%i %i", &key, &state) != 2)
			throw std::exception("frame error");
		//printf("%i %i\n", key, state);

		synth_keyChange(&synth, key + 24, state);
	}

	void Process(audioFrame_t frames[], int numFrames)
	{
		for(int i = 0; i < 100 && serial.GetAvailableRead() > 0; ++i)
		{
			int c = serial.ReadByte();
			if(c < 0)
				break;
			//if(protocol.Next((char)serial.ReadByte()))
			if(protocol.Next(c))
			{
				SerialFrameReceived(protocol.Get());
			}
		}

		while(control.Poll(1))
		{
			char buf[1];
			control.Read(buf, 1);
			if(controlProtocol.Next(buf[0]))
			{
				int index;
				float value;
				auto frame = controlProtocol.Get();
				if(sscanf(frame.c_str(), "%d;%f", &index, &value) == 2)
				{
					synth_setParameter(&synth, index, value);
				}
				//printf(" >%s\n", controlProtocol.Get().c_str());
			}
		}

		for(int i = 0; i < numFrames; ++i)
		{
			auto s = synth_clock(&synth);

			s.left = saturate(s.left);
			s.right = saturate(s.right);

			frames[i].left = s.left;
			frames[i].right = s.right;
		}
	}

public:
	void Load()
	{
		serial.Open("COM4");

		printf("trying to connect to contro surface...");

		control.Connect(IpEndpoint("127.0.0.9", 25565));

		//reverbTest();

		synth_init(&synth);

		audioContext.reset(new AudioContext());
		audioContext->ProcessCB = [] (audioFrame_t frames[], int numFrames, void* context) { ((App*)context)->Process(frames, numFrames); };
		audioContext->userContext = this;
	}

	void Closing()
	{
		audioContext->Close();
	}
};



#include <conio.h>
#include "resource.h"

void synthTest()
{
	try
	{
		auto icon = (LONG)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(GetConsoleWindow(), WM_SETICON, ICON_BIG, icon);
		SendMessage(GetConsoleWindow(), WM_SETICON, ICON_SMALL, icon);

		App app;

		app.Load();

		while(!kbhit())
		{
			_sleep(0);

		}

		app.Closing();
	}
	catch(const std::exception& e)
	{
		printf("\n\nan exception occured:\n\n%s", e.what());
	}

}

int main()
{
	reverbTest();
	synthTest();

    return 0;
}