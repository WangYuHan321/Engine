#include <string>
#include <list>
#include <map>
#include <vector>
#include <iostream>

namespace Engine
{
	typedef std::string string;
	typedef unsigned int uint32;
	typedef unsigned long ulong;
	typedef unsigned long long ullong;

	typedef unsigned char uchar;

	uint32 Hash(const char* str);
}