#ifndef UTILS_CONVERSION_H
#define UTILS_CONVERSION_H

#include <chrono>
#include <ctime>
#include <sstream>
#include <string>

std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> timevalToTimestamp(const timeval& timestamp);

std::string timestampToDateTime(const std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>& timestamp);
std::string durationToTime(const std::chrono::microseconds& duration);

std::string snakeCaseString(std::string);

template <typename T> std::string convertToString(const T& value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

#endif
