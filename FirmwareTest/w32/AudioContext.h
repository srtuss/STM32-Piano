#pragma once

#include "audioEndpoint.h"
#include <thread>

class AudioNode
{

};

class CodeProcessorNode
{

};

struct audioFrame_t
{
	float left, right;
};

class AudioContext
{
	AudioEndpoint audioEndpoint;
	std::shared_ptr<std::thread> processingThread;
	bool shutdown;
	void ProcessingThread();
	static void ProcessingThreadStart(AudioContext *thisPtr)
	{
		thisPtr->ProcessingThread();
	}
public:
	//std::function<void(audioFrame_t[], int)> ProcessCB;
	void (*ProcessCB)(audioFrame_t frames[], int numFrames, void *context);
	void *userContext;
	AudioContext();
	~AudioContext();
	void Close();
};

