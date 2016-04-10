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

bool TcpStream::isEmpty() const
{
	return getNumberOfPackets() == 0;
}

std::size_t TcpStream::getNumberOfPackets() const
{
	return _packets.size();
}

void TcpStream::addPacket(std::shared_ptr<Packet> packet)
{
	_packets.push_back(packet);
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

std::chrono::microseconds TcpStream::getDuration() const
{
	return getEndTime() - getStartTime();
}
