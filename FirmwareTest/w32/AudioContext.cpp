#include "audioContext.h"

void AudioContext::ProcessingThread()
{
	audioEndpoint.Open();

	while(!shutdown)
	{
		audioEndpoint.Wait();

		size_t numFramesAvail = audioEndpoint.NumAvailable();
		if(numFramesAvail > 0)
		{
			unsigned char *buffer = audioEndpoint.GetBuffer(numFramesAvail);
			int frameAlign = 8;
			//processFrame((float*)buffer, numFramesAvail);

			audioFrame_t *dst = (audioFrame_t*)buffer;

			if(ProcessCB)
				ProcessCB(dst, numFramesAvail, userContext);

			audioEndpoint.ReleaseBuffer(numFramesAvail);
			//printf("%i\n", numFramesAvail);
		}

		_sleep(0);

	}
}

void AudioContext::Close()
{
	if(!shutdown)
	{
		shutdown = true;
		processingThread->join();
	}
}

AudioContext::AudioContext()
{
	shutdown = false;
	processingThread.reset(new std::thread(ProcessingThreadStart, this));
}


AudioContext::~AudioContext()
{
	Close();
}
