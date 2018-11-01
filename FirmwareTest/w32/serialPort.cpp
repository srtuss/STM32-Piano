#include "serialPort.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

SerialPort::SerialPort()
{
	hComm = INVALID_HANDLE_VALUE;
}

SerialPort::~SerialPort()
{
	if(hComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hComm);
		hComm = INVALID_HANDLE_VALUE;
	}
}

void SerialPort::Open(const std::string &portName)
{
	hComm = CreateFileA(portName.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		/*FILE_FLAG_OVERLAPPED*/ 0,
		0);
	if(hComm == INVALID_HANDLE_VALUE)
		throw new std::exception("com open failed");

	DCB dcb = {};
	GetCommState(hComm, &dcb);
}

size_t SerialPort::GetAvailableRead()
{
	COMSTAT comstat;
	ClearCommError(hComm, 0, &comstat);
	return comstat.cbInQue;
}

size_t SerialPort::Read(void *dst, size_t len)
{
	DWORD read = 0;
	ReadFile(hComm, dst, len, &read, 0);
	return read;
}

size_t SerialPort::Write(void *dst, size_t len)
{
	DWORD written = 0;
	WriteFile(hComm, dst, len, &written, 0);
	return written;
}

int SerialPort::ReadByte()
{
	int c = 0;
	if(Read(&c, 1) == 1)
		return c;
	return -1;
}