#include "stream/tcp_stream.h"

TcpStream::TcpStream() : _packets()
{
}

TcpStream::iterator TcpStream::begin()
{
	return iterator(this, 0);
}

TcpStream::iterator TcpStream::end()
{
	return iterator(this, _packets.size());
}

TcpStream::const_iterator TcpStream::begin() const
{
	return const_iterator(this, 0);
}

TcpStream::const_iterator TcpStream::end() const
{
	return const_iterator(this, _packets.size());
}

TcpStream::iterator::value_type TcpStream::operator [](int offset)
{
	return _packets[offset].get();
}

TcpStream::const_iterator::value_type TcpStream::operator [](int offset) const
{
	return _packets[offset].get();
}

void TcpStream::addPacket(std::shared_ptr<Packet> packet)
{
	_packets.push_back(packet);
}

void TcpStream::setClientWindowScale(std::uint8_t clientWindowScale)
{
	_clientWindowScale = clientWindowScale;
}

void TcpStream::setServerWindowScale(std::uint8_t serverWindowScale)
{
	_serverWindowScale = serverWindowScale;
}

bool TcpStream::isEmpty() const
{
	return getNumberOfPackets() == 0;
}

std::size_t TcpStream::getNumberOfPackets() const
{
	return _packets.size();
}

Packet::Timestamp TcpStream::getStartTime() const
{
	if (_packets.empty())
		return Packet::Timestamp();

	return _packets.front()->getTimestamp();
}

Packet::Timestamp TcpStream::getEndTime() const
{
	if (_packets.empty())
		return Packet::Timestamp();

	return _packets.back()->getTimestamp();
}

std::uint8_t TcpStream::getClientWindowScale() const
{
	return _clientWindowScale;
}

std::uint8_t TcpStream::getServerWindowScale() const
{
	return _serverWindowScale;
}

std::chrono::microseconds TcpStream::getDuration() const
{
	return getEndTime() - getStartTime();
}

std::string TcpStream::getClientIp() const
{
	if (getNumberOfPackets() < 2)
		return "";

	return (*this)[0]->getSourceIp();
}

std::string TcpStream::getServerIp() const
{
	if (getNumberOfPackets() < 2)
		return "";

	return (*this)[1]->getSourceIp();
}

std::uint16_t TcpStream::getClientPort() const
{
	if (getNumberOfPackets() < 2)
		return 0;

	return (*this)[0]->getSourcePort();
}

std::uint16_t TcpStream::getServerPort() const
{
	if (getNumberOfPackets() < 2)
		return 0;

	return (*this)[1]->getSourcePort();
}

std::uint32_t TcpStream::getWindowSize(std::uint64_t packetIndex) const
{
	return getWindowSize((*this)[packetIndex]);
}

std::uint32_t TcpStream::getWindowSize(const Packet* packet) const
{
	if (packet == nullptr)
		return 0;

	if (packet->isSyn())
		return packet->getWindowSize();

	if (packet->getSourceIp() == getClientIp())
		return packet->getWindowSize() << getClientWindowScale();
	else if (packet->getSourceIp() == getServerIp())
		return packet->getWindowSize() << getServerWindowScale();

	return 0;
}

std::uint32_t TcpStream::getRelativeSequenceNumber(std::uint64_t packetIndex) const
{
	return getRelativeSequenceNumber((*this)[packetIndex]);
}

std::uint32_t TcpStream::getRelativeSequenceNumber(const Packet* packet) const
{
	if (packet == nullptr)
		return std::numeric_limits<std::uint32_t>::max();

	if (packet->getSourceIp() == getClientIp())
		return packet->getSequenceNumber() - (*this)[0]->getSequenceNumber();
	else if (packet->getSourceIp() == getServerIp())
		return packet->getSequenceNumber() - (*this)[1]->getSequenceNumber();
	else
		return std::numeric_limits<std::uint32_t>::max();
}

std::uint32_t TcpStream::getRelativeAckNumber(std::uint64_t packetIndex) const
{
	return getRelativeAckNumber((*this)[packetIndex]);
}

std::uint32_t TcpStream::getRelativeAckNumber(const Packet* packet) const
{
	if (packet == nullptr)
		return std::numeric_limits<std::uint32_t>::max();

	if (packet->getSourceIp() == getClientIp())
		return packet->getAckNumber() - (*this)[1]->getSequenceNumber();
	else if (packet->getSourceIp() == getServerIp())
		return packet->getAckNumber() - (*this)[0]->getSequenceNumber();
	else
		return std::numeric_limits<std::uint32_t>::max();
}

std::chrono::microseconds TcpStream::getRelativePacketTime(std::uint64_t packetIndex) const
{
	return getRelativePacketTime((*this)[packetIndex]);
}

std::chrono::microseconds TcpStream::getRelativePacketTime(const Packet* packet) const
{
	return packet->getTimestamp() - getStartTime();
}
