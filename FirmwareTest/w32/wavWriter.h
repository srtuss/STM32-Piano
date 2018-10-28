#pragma once

#include "filesystem.h"

class WavWriter
{
	std::shared_ptr<SStream> stream;

	static void WriteShort(const std::shared_ptr<SStream> &stream, int v)
	{
		stream->Write((const char*)&v, 2);
	}
	static void WriteLong(const std::shared_ptr<SStream> &stream, int v)
	{
		stream->Write((const char*)&v, 4);
	}
	int sampleRate;

public:
	WavWriter(const std::string &path, int sampleRate = 44100)
	{
		this->sampleRate = sampleRate;
		stream = FileOpenWrite(path);
		stream->SetPosition(44); // reserve some space for the header
	}
	~WavWriter()
	{
		UpdateHeader();
	}
	void Write(float left, float right)
	{
		stream->Write((char*)&left, 4);
		stream->Write((char*)&right, 4);
	}

	struct riffHeader
	{
		char riff[4] = {'R', 'I', 'F', 'F'};
		long filesize;
		char wave[4] = {'W', 'A', 'V', 'E'};
	};

	struct chunkHeader
	{
		char sig[4];
		long length;
	};

	struct formatHeader
	{
		short formatTag, numChannels;
		long sampleRate;
		long bytesPerSecond;
		short blockAlign;
		short bitsPerSample;
	};

	void UpdateHeader()
	{
		int at = stream->Position();
		stream->SetPosition(0);
		stream->Write("RIFF", 4);
		WriteLong(stream, at);
		stream->Write("WAVE", 4);

		stream->Write("fmt ", 4);
		WriteLong(stream, 16);
		WriteShort(stream, 3);
		WriteShort(stream, 2);
		WriteLong(stream, sampleRate);
		WriteLong(stream, sampleRate * 8);
		WriteShort(stream, 8);
		WriteShort(stream, 32);

		stream->Write("data", 4);
		WriteLong(stream, at - 44);

		stream->SetPosition(at);
	}
};