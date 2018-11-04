#pragma once

#include <memory>
#include <string>
#include <stdio.h>
#include "stream.h"

#define MAX_PATH _MAX_PATH

enum FileOpenMode
{
	Read,
	Write
};

class FStream : public SStream
{
	FILE *m_f;
public:
	FStream(const std::string &filename, FileOpenMode openMode)
	{
		m_f = fopen(filename.c_str(), openMode == FileOpenMode::Write ? "wb" : "rb");
	}
	~FStream()
	{
		fclose(m_f);
	}
	int Length()
	{
		struct stat buf;
		fstat(fileno(m_f), &buf);
		return buf.st_size;
	}
	size_t Read(char *dst, size_t len)
	{
		return fread(dst, 1, len, m_f);
	}
	size_t Write(const char *src, size_t len)
	{
		return fwrite(src, 1, len, m_f);
	}
	size_t Position()
	{
		return ftell(m_f);
	}
	void SetPosition(size_t position)
	{
		if(fseek(m_f, position, SEEK_SET) != 0)
			throw std::exception("fseek failed");
	}
};

static std::shared_ptr<SStream> FileOpenRead(const std::string &filename)
{
	std::shared_ptr<SStream> ptr(new FStream(filename, FileOpenMode::Read));
	return ptr;
}

static std::shared_ptr<SStream> FileOpenWrite(const std::string &filename)
{
	std::shared_ptr<SStream> ptr(new FStream(filename, FileOpenMode::Write));
	return ptr;
}

static bool FileExists(std::string filename)
{
	FILE *f = fopen(filename.c_str(), "rb");
	if(f)
	{
		fclose(f);
		return true;
	}
	return false;
}