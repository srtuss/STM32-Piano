/**
 * @file serialPort.h
 * @author srtuss
 * @date 1 Nov 2018
 * @brief serial port header
 *
 */

#pragma once

#include <string>

/**
 * @brief Performs easy communication over a serial port
 */
class SerialPort
{
	void* hComm;
	int baudRate = 115200;

public:
	SerialPort();
	~SerialPort();


	int GetBaudrate();

	/**
      * @brief Open the port
	  * @param portName Name of the port
      */
	void Open(const std::string &portName);

    /**
      * @brief Read bytes from the port
	  * @param dst Destination buffer
	  * @param len number of bytes to read
	  * @return Number of bytes read
      */
	size_t Read(void *dst, size_t len);

	/**
	  * @brief Write bytes to the port
	  * @param dst Source buffer
	  * @param len number of bytes to write
	  * @return Number of bytes written
	  */
	size_t Write(void *dst, size_t len);

	/**
	  * @brief Query how many bytes can be read from the port, without blocking
	  * @return Number of bytes queued in the RX buffer
	  */
	size_t GetAvailableRead();

	/**
	  * @brief Read a byte from the port
	  * @return The byte read\n
	  *			-1 if nothing was received
	  */
	int ReadByte();
};