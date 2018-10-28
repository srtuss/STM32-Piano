#pragma once

#include <audioclient.h>
#include <mmdeviceapi.h>
#include <stdexcept>

extern const CLSID CLSID_MMDeviceEnumerator;
extern const IID IID_IMMDeviceEnumerator;
extern const IID IID_IAudioClient;
extern const IID IID_IAudioRenderClient;
 
// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }
#define THROW_ON_ERROR(hr) if(FAILED(hr)) throw new std::exception("error")

class AudioEndpoint
{
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioRenderClient *pRenderClient = NULL;
	WAVEFORMATEX *pwfx = NULL;
	UINT32 bufferFrameCount;
	UINT32 numFramesPadding;
	BYTE *pData;
	DWORD flags = 0;
	HANDLE waitEvent;

public:

	size_t NumAvailable()
	{
		// See how much buffer space is available.
		HRESULT hr = pAudioClient->GetCurrentPadding(&numFramesPadding);
		THROW_ON_ERROR(hr);

		return bufferFrameCount - numFramesPadding;
	}

	unsigned char* GetBuffer(size_t num)
	{
		BYTE *data;
		HRESULT hr = pRenderClient->GetBuffer(num, &data);
		if(FAILED(hr))
			throw new std::exception();
		return data;
	}

	void ReleaseBuffer(size_t num)
	{
		HRESULT hr = pRenderClient->ReleaseBuffer(num, flags);
		if(FAILED(hr))
			throw new std::exception();
	}

	void Close()
	{
		CoTaskMemFree(pwfx);
		SAFE_RELEASE(pEnumerator)
			SAFE_RELEASE(pDevice)
			SAFE_RELEASE(pAudioClient)
			SAFE_RELEASE(pRenderClient)
			CloseHandle(waitEvent);
	}

	// wait until more data is needed
	void Wait()
	{
		WaitForSingleObject(waitEvent, INFINITE);
	}

	void Open()
	{
		CoInitialize(NULL);

		HRESULT hr;
		hr = CoCreateInstance(
			CLSID_MMDeviceEnumerator, NULL,
			CLSCTX_ALL, IID_IMMDeviceEnumerator,
			(void**)&pEnumerator);
		EXIT_ON_ERROR(hr)

			hr = pEnumerator->GetDefaultAudioEndpoint(
				eRender, eConsole, &pDevice);
		EXIT_ON_ERROR(hr)

			hr = pDevice->Activate(
				IID_IAudioClient, CLSCTX_ALL,
				NULL, (void**)&pAudioClient);
		EXIT_ON_ERROR(hr)


		REFERENCE_TIME devDefaultPeriod, devMinimumPeriod;
		pAudioClient->GetDevicePeriod(&devDefaultPeriod, &devMinimumPeriod);


			hr = pAudioClient->GetMixFormat(&pwfx);
		EXIT_ON_ERROR(hr)

			hr = pAudioClient->Initialize(
				AUDCLNT_SHAREMODE_SHARED,
				AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
				devMinimumPeriod,
				0,
				pwfx,
				NULL);
		EXIT_ON_ERROR(hr)

		waitEvent = CreateEvent(0, FALSE, FALSE, 0);
		pAudioClient->SetEventHandle(waitEvent);

			// Tell the audio source which format to use.
//			hr = pMySource->SetFormat(pwfx);
//		EXIT_ON_ERROR(hr)

			// Get the actual size of the allocated buffer.
			hr = pAudioClient->GetBufferSize(&bufferFrameCount);
		EXIT_ON_ERROR(hr)

			hr = pAudioClient->GetService(
				IID_IAudioRenderClient,
				(void**)&pRenderClient);
		EXIT_ON_ERROR(hr)

			/*// Grab the entire buffer for the initial fill operation.
			hr = pRenderClient->GetBuffer(bufferFrameCount, &pData);
		EXIT_ON_ERROR(hr)

			// Load the initial data into the shared buffer.
//			hr = pMySource->LoadData(bufferFrameCount, pData, &flags);
//		EXIT_ON_ERROR(hr)

			hr = pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
		EXIT_ON_ERROR(hr)*/

			// Calculate the actual duration of the allocated buffer.
			hnsActualDuration = (double)REFTIMES_PER_SEC *
			bufferFrameCount / pwfx->nSamplesPerSec;

		hr = pAudioClient->Start();  // Start playing.
		EXIT_ON_ERROR(hr)

			Exit:;
	}
};