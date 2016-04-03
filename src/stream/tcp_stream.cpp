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

void TcpStream::add(std::shared_ptr<Packet> packet)
{
	_packets.push_back(packet);
}
