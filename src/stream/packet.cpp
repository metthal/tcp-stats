#include "stream/packet.h"

Packet::Packet() : _size(0), _sourceIp(), _destIp(), _sourcePort(0), _destPort(0), _seqNumber(0), _ackNumber(0),
	_windowSize(0), _windowScale(0), _tcpFlags(TCP_FLAG_NONE)
{
}

void Packet::setSize(std::uint64_t size)
{
	_size = size;
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

void Packet::setSequenceNumber(std::uint32_t seqNumber)
{
	_seqNumber = seqNumber;
}

void Packet::setAckNumber(std::uint32_t ackNumber)
{
	_ackNumber = ackNumber;
}

void Packet::setWindowSize(std::uint16_t windowSize)
{
	_windowSize = windowSize;
}

void Packet::setWindowScale(std::uint8_t windowScale)
{
	_windowScale = windowScale;
}

void Packet::setSyn(bool set)
{
	_tcpFlags = set ? (_tcpFlags | TCP_FLAG_SYN) : (_tcpFlags & ~TCP_FLAG_SYN);
}

void Packet::setFin(bool set)
{
	_tcpFlags = set ? (_tcpFlags | TCP_FLAG_FIN) : (_tcpFlags & ~TCP_FLAG_FIN);
}

void Packet::setAck(bool set)
{
	_tcpFlags = set ? (_tcpFlags | TCP_FLAG_ACK) : (_tcpFlags & ~TCP_FLAG_ACK);
}

void Packet::setPush(bool set)
{
	_tcpFlags = set ? (_tcpFlags | TCP_FLAG_PUSH) : (_tcpFlags & ~TCP_FLAG_PUSH);
}

void Packet::setReset(bool set)
{
	_tcpFlags = set ? (_tcpFlags | TCP_FLAG_RESET) : (_tcpFlags & ~TCP_FLAG_RESET);
}

void Packet::setUrgent(bool set)
{
	_tcpFlags = set ? (_tcpFlags | TCP_FLAG_URGENT) : (_tcpFlags & ~TCP_FLAG_URGENT);
}

std::uint64_t Packet::getSize() const
{
	return _size;
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

std::uint16_t Packet::getWindowSize() const
{
	return _windowSize;
}

std::uint8_t Packet::getWindowScale() const
{
	return _windowScale;
}

bool Packet::isSyn() const
{
	return _tcpFlags & TCP_FLAG_SYN;
}

bool Packet::isFin() const
{
	return _tcpFlags & TCP_FLAG_FIN;
}

bool Packet::isAck() const
{
	return _tcpFlags & TCP_FLAG_ACK;
}

bool Packet::isPush() const
{
	return _tcpFlags & TCP_FLAG_PUSH;
}

bool Packet::isReset() const
{
	return _tcpFlags & TCP_FLAG_RESET;
}

bool Packet::isUrgent() const
{
	return _tcpFlags & TCP_FLAG_URGENT;
}

std::uint32_t Packet::getSequenceNumber() const
{
	return _seqNumber;
}

std::uint32_t Packet::getAckNumber() const
{
	return _ackNumber;
}
