#pragma once

class SStream
{
public:
	virtual int Length() = 0;
	virtual size_t Read(char *dst, size_t len) = 0;
	virtual size_t Write(const char *src, size_t len) = 0;
	virtual size_t Position() = 0;
	virtual void SetPosition(size_t position) = 0;
};

class MemoryStream : public SStream
{
	const char *ptr;
	size_t at, total;
public:
	MemoryStream(const char *ptr, size_t len)
	{
		this->ptr = ptr;
		total = len;
		at = 0;
	}
	int Length()
	{
		return total;
	}
	size_t Read(char *dst, size_t len)
	{
		if(total - at < len)
			len = total - at;
		memcpy(dst, this->ptr + at, len);
		at += len;
		return len;
	}
	size_t Write(const char *src, size_t len)
	{
		return 0;
	}
	size_t Position()
	{
		return at;
	}
};