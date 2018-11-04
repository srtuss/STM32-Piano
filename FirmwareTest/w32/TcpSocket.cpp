//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib, "wsock32")

#include "TcpSocket.h"

IpEndpoint::IpEndpoint(const std::string &address, port_t port) {
	this->address = inet_addr(address.c_str());
	this->port = port;
}

TcpSocket::TcpSocket() {
	handle = INVALID_SOCKET;
}


TcpSocket::~TcpSocket() {
	Close();
}


void TcpSocket::Connect(const IpEndpoint &ipEndpoint) {
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);

	if(err != 0)
	{
		char buf[64];
		sprintf(buf, "WSAStartup failed with error: %d\n", err);
		throw PipeException(buf);
	}

	handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(handle == INVALID_SOCKET)
	{
		err = WSAGetLastError();

		char buf[64];
		sprintf(buf, "socket failed with error: %d\n", err);
		throw PipeException(buf);
	}

	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(ipEndpoint.port);
	sa.sin_addr.s_addr = ipEndpoint.address;
	if(connect(handle, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
		err = WSAGetLastError();

		char buf[64];
		sprintf(buf, "connect failed with error: %d\n", err);
		throw PipeException(buf);
	}
}

void TcpSocket::Close() {
	if(handle != INVALID_SOCKET) {
		closesocket(handle);
		handle = INVALID_SOCKET;
	}
}

int TcpSocket::Write(const char *ptr, int len) {
	return send(handle, ptr, len, 0);
}

int TcpSocket::Read(char *ptr, int len) {
	return recv(handle, ptr, len, 0);
}

bool TcpSocket::Poll(int msTimeout) {
	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(handle, &fd);
	timeval tv;
	tv.tv_sec = msTimeout / 1000;
	tv.tv_usec = (msTimeout % 1000) * 1000;
	select(0, &fd, 0, 0, &tv);
	return FD_ISSET(handle, &fd);
}

void TcpSocket::SetNonblocking(bool nonblocking)
{
	//-------------------------
	// Set the socket I/O mode: In this case FIONBIO
	// enables or disables the blocking mode for the 
	// socket based on the numerical value of iMode.
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.

	u_long value = nonblocking ? 1 : 0;
	int err = ioctlsocket(handle, FIONBIO, &value);
	if(err != NO_ERROR)
	{
		char buf[64];
		sprintf(buf, "ioctlsocket failed with error: %ld\n", err);
		throw PipeException(buf);
	}
}