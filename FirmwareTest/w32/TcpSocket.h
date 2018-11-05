#pragma once

#include <string>

typedef unsigned long ipAddress_t;
typedef unsigned short port_t;

class IpEndpoint
{
public:
	ipAddress_t address;
	port_t port;
	IpEndpoint(const std::string &address, port_t port);
};

typedef SOCKET UINT_PTR;

class TcpSocket
{
	SOCKET handle;
public:
	TcpSocket();
	~TcpSocket();
	void Connect(const IpEndpoint &ipEndpoint);
	void Close();
	int Write(const char *ptr, int len);
	int Read(char *ptr, int len);
	bool Poll(int msTimeout);
	void SetNonblocking(bool nonblocking);
};

