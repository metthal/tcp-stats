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

std::uint32_t TcpStream::getWindowSize(std::uint64_t packetIndex) const
{
	return getWindowSize((*this)[packetIndex]);
}

std::uint32_t TcpStream::getWindowSize(const Packet* packet) const
{
	if (packet == nullptr)
		return 0;

	if (packet->getSourceIp() == getClientIp())
		return packet->getWindowSize() << getClientWindowScale();
	else if (packet->getSourceIp() == getServerIp())
		return packet->getWindowSize() << getServerWindowScale();

	return 0;
}
