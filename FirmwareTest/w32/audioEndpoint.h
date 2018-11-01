/**
 * @file audioEndpoint.h
 * @author srtuss
 * @date 1 Nov 2018
 * @brief audio endpoint header
 *
 */

#pragma once

#include <audioclient.h>
#include <mmdeviceapi.h>
#undef min
#undef max

 /**
  * @brief Audio-endpoint device
  */
class AudioEndpoint
{
	REFERENCE_TIME hnsActualDuration;
	IMMDeviceEnumerator *pEnumerator = __nullptr;
	IMMDevice *pDevice = __nullptr;
	IAudioClient *pAudioClient = __nullptr;
	IAudioRenderClient *pRenderClient = __nullptr;
	WAVEFORMATEX *pwfx = __nullptr;
	UINT32 bufferFrameCount;
	UINT32 numFramesPadding;
	BYTE *pData;
	DWORD flags = 0;
	HANDLE waitEvent;

public:
	/**
	 * @brief Open the endpoint
	 */
	void Open();
	/**
	 * @brief Close the endpoint
	 */
	void Close();
	/**
	 * @brief Query the number of frames that can currently be written to the frame-buffer
	 * @returns Number of frames that can be written to the frame-buffer
	 */
	size_t NumAvailable();
	/**
	 * @brief Block the calling thread until the endpoint is ready for more frames
	 */
	void Wait();
	/**
	 * @brief Lock the frame-buffer for writing
	 * @param num The requested buffer-size, in frames
	 * @return Pointer to the buffer
	 */
	unsigned char* GetBuffer(size_t num);
	/**
	 * @brief Unlock the frame-buffer after writing
	 * @param num The number of frames that were written to the buffer
	 */
	void ReleaseBuffer(size_t num);
};