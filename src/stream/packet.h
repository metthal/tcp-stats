#ifndef STREAM_PACKET_H
#define STREAM_PACKET_H

#include <chrono>
#include <cstdint>
#include <string>

class Packet
{
public:
	using Timestamp = std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>;

	Packet();

	void setTimestamp(const Timestamp& timestamp);
	void setSourceIp(const std::string& sourceIp);
	void setDestIp(const std::string& destIp);
	void setSourcePort(std::uint16_t sourcePort);
	void setDestPort(std::uint16_t destPort);

	const Timestamp& getTimestamp() const;
	const std::string getSourceIp() const;
	const std::string getDestIp() const;
	std::uint16_t getSourcePort() const;
	std::uint16_t getDestPort() const;

private:
	Timestamp _timestamp;
	std::string _sourceIp;
	std::string _destIp;
	std::uint16_t _sourcePort;
	std::uint16_t _destPort;
};

#endif
