#ifndef STREAM_PACKET_H
#define STREAM_PACKET_H

#include <chrono>
#include <cstdint>
#include <string>

enum TcpFlags
{
	TCP_FLAG_NONE = 0x0,
	TCP_FLAG_SYN = 0x1,
	TCP_FLAG_FIN = 0x2,
	TCP_FLAG_ACK = 0x4,
	TCP_FLAG_PUSH = 0x8,
	TCP_FLAG_RESET = 0x10,
	TCP_FLAG_URGENT = 0x20
};

class Packet
{
public:
	using Timestamp = std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>;

	Packet();

	void setSize(std::uint64_t size);
	void setTimestamp(const Timestamp& timestamp);
	void setSourceIp(const std::string& sourceIp);
	void setDestIp(const std::string& destIp);
	void setSourcePort(std::uint16_t sourcePort);
	void setDestPort(std::uint16_t destPort);
	void setSequenceNumber(std::uint32_t seqNumber);
	void setAckNumber(std::uint32_t ackNumber);
	void setWindowSize(std::uint16_t windowSize);
	void setWindowScale(std::uint8_t windowScale);
	void setSyn(bool set);
	void setFin(bool set);
	void setAck(bool set);
	void setPush(bool set);
	void setReset(bool set);
	void setUrgent(bool set);

	std::uint64_t getSize() const;
	const Timestamp& getTimestamp() const;
	const std::string getSourceIp() const;
	const std::string getDestIp() const;
	std::uint16_t getSourcePort() const;
	std::uint16_t getDestPort() const;
	std::uint32_t getSequenceNumber() const;
	std::uint32_t getAckNumber() const;
	std::uint16_t getWindowSize() const;
	std::uint8_t getWindowScale() const;
	bool isSyn() const;
	bool isFin() const;
	bool isAck() const;
	bool isPush() const;
	bool isReset() const;
	bool isUrgent() const;

private:
	std::uint64_t _size;
	Timestamp _timestamp;
	std::string _sourceIp;
	std::string _destIp;
	std::uint16_t _sourcePort;
	std::uint16_t _destPort;
	std::uint32_t _seqNumber;
	std::uint32_t _ackNumber;
	std::uint16_t _windowSize;
	std::uint8_t _windowScale;
	std::uint8_t _tcpFlags;
};

#endif
