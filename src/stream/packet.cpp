#include "stream/packet.h"

Packet::Packet() : _sourceIp(), _destIp(), _sourcePort(0), _destPort(0)
{
}

void Packet::setTimestamp(const Packet::Timestamp& timestamp)
{
	_timestamp = timestamp;
}

void Packet::setSourceIp(const std::string& sourceIp)
{
	_sourceIp = sourceIp;
}

void Packet::setDestIp(const std::string& destIp)
{
	_destIp = destIp;
}

void Packet::setSourcePort(std::uint16_t sourcePort)
{
	_sourcePort = sourcePort;
}

void Packet::setDestPort(std::uint16_t destPort)
{
	_destPort = destPort;
}

const Packet::Timestamp& Packet::getTimestamp() const
{
	return _timestamp;
}

const std::string Packet::getSourceIp() const
{
	return _sourceIp;
}

const std::string Packet::getDestIp() const
{
	return _destIp;
}

std::uint16_t Packet::getSourcePort() const
{
	return _sourcePort;
}

std::uint16_t Packet::getDestPort() const
{
	return _destPort;
}
