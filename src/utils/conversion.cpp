#include "utils/conversion.h"

std::string snakeCaseString(std::string str)
{
	std::size_t pos;
	while ((pos =  str.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ ")) != std::string::npos)
	{
		str[pos] = (str[pos] == ' ') ? '_' : tolower(str[pos]);
	}

	return str;
}
