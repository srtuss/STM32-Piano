#pragma once

#include <string>

class Protocol
{
	std::string rx;
	char lastRecv = 0;
	int state = 0;
public:
	const std::string& Get()
	{
		return rx;
	}

	bool Next(char c)
	{
		switch(state)
		{
		case 0:
			if(c == '~')
			{
				state = 1;
				rx.clear();
			}
			break;
		case 1:
			if(c == '\n' && lastRecv == '\r')
			{
				rx.pop_back();

				state = 0;
				return true;
			}
			else
				rx += c;
			break;

		}

		lastRecv = c;
		return false;
	}
};