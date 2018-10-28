#pragma once

#include <string>

class SerialPort
{
	void* hComm;
public:
	SerialPort();
	~SerialPort();
	void Open(const std::string &portName);
	size_t Read(void *dst, size_t len);
	size_t GetAvailableRead();
	int ReadByte();
};