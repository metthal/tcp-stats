#include <iomanip>

#include "utils/conversion.h"

std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> timevalToTimestamp(const timeval& timestamp)
{
	auto duration = std::chrono::seconds{timestamp.tv_sec} + std::chrono::microseconds{timestamp.tv_usec};
	return std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>(duration);
}

std::string timestampToDateTime(const std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>& timestamp)
{
	char buf[32];
	std::time_t timet = std::chrono::system_clock::to_time_t(timestamp);
	tm* localTime = localtime(&timet);
	strftime(buf, sizeof(buf), "%H:%M:%S", localTime);
	return std::string(buf);
}

std::string durationToTime(const std::chrono::microseconds& duration)
{
	std::uint64_t usecs = duration.count();
	std::uint64_t hours = (usecs / 3600000000) % 24;
	usecs %= 3600000000;
	std::uint64_t mins = (usecs / 60000000) % 60;
	usecs %= 60000000;
	std::uint64_t secs = (usecs / 1000000) % 60;
	usecs %= 1000000;

	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << hours;
	ss << ":";
	ss << std::setw(2) << std::setfill('0') << mins;
	ss << ":";
	ss << std::setw(2) << std::setfill('0') << secs;
	ss << ".";
	ss << std::setw(6) << std::setfill('0') << usecs;
	return ss.str();
}

std::string snakeCaseString(std::string str)
{
	std::size_t pos;
	while ((pos =  str.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ -")) != std::string::npos)
	{
		switch (str[pos])
		{
			case ' ':
			case '-':
				str[pos] = '_';
				break;
			default:
				str[pos] = tolower(str[pos]);
				break;
		}
	}

	return str;
}
