#ifndef STREAM_TCP_STREAM_H
#define STREAM_TCP_STREAM_H

#include <memory>
#include <vector>

#include "stream/packet.h"

class TcpStream
{
public:
	template <typename VT, typename BT> class iterator_impl : public std::iterator<std::random_access_iterator_tag, std::int64_t>
	{
		using base_type = BT;

	public:
		using value_type = VT;

		// Is default-constructible, copy-constructible, copy-assignable and destructible
		iterator_impl() : _stream(nullptr), _pos(0) {}
		iterator_impl(base_type stream, std::int64_t pos) : _stream(stream), _pos(pos) {}
		iterator_impl(const iterator_impl<value_type, base_type>& itr) : _stream(itr._stream), _pos(itr._pos) {}
		iterator_impl<value_type, base_type>& operator =(iterator_impl<value_type, base_type> rhs)
		{
			std::swap(_stream, rhs._stream);
			std::swap(_pos, rhs._pos);
			return *this;
		}
		~iterator_impl() {}

		// Can be compared for equivalence using the equality/inequality operators
		bool operator ==(const iterator_impl<value_type, base_type>& itr) const { return (_stream == itr._stream) && (_pos == itr._pos); }
		bool operator !=(const iterator_impl<value_type, base_type>& itr) const { return !(*this == itr); }

		// Can be dereferenced as an rvalue
		// Can be dereferenced as an lvalue
		value_type operator *() const { return _pos < static_cast<std::int64_t>(_stream->_packets.size()) ? _stream->_packets[_pos].get() : nullptr; }
		value_type operator ->() const { return _pos < static_cast<std::int64_t>(_stream->_packets.size()) ? _stream->_packets[_pos].get() : nullptr; }

		// Can be incremented
		const iterator_impl<value_type, base_type>& operator ++() const
		{
			_pos += _pos >= static_cast<std::int64_t>(_stream->_packets.size()) ? 0 : 1;
			return *this;
		}
		const iterator_impl<value_type, base_type> operator ++(int) const
		{
			iterator_impl<value_type, base_type> tmp(*this);
			_pos = _pos >= static_cast<std::int64_t>(_stream->_packets.size()) ? 0 : _pos + 1;
			return tmp;
		}

		// Can be decremented
		const iterator_impl<value_type, base_type>& operator --() const
		{
			_pos = _pos == 0 ? 0 : _pos - 1;
			return *this;
		}
		const iterator_impl<value_type, base_type> operator --(int) const
		{
			iterator_impl<value_type, base_type> tmp(*this);
			_pos = _pos == 0 ? 0 : _pos - 1;
			return tmp;
		}

		// Supports the arithmetic operators + and - between an iterator and an integer value, or subtracting an iterator from another.
		const iterator_impl<value_type, base_type>& operator +(int value) const
		{
			_pos = std::max(static_cast<std::int64_t>(0), std::min(static_cast<std::int64_t>(_stream->_packets.size()), _pos + value));
			return *this;
		}
		const iterator_impl<value_type, base_type>& operator -(int value) const { return *this + (-value); }
		std::int64_t operator -(const iterator_impl<value_type, base_type>& itr) const
		{
			if (_stream != itr._stream)
				return std::numeric_limits<std::int64_t>::max();

			return _pos - itr._pos;
		}

		// Can be compared with inequality relational operators (<, >, <= and >=)
		bool operator <(const iterator_impl<value_type, base_type>& itr) const { return _pos < itr._pos; }
		bool operator >(const iterator_impl<value_type, base_type>& itr) const { return _pos > itr._pos; }
		bool operator <=(const iterator_impl<value_type, base_type>& itr) const { return (*this == itr) || (*this < itr); }
		bool operator >=(const iterator_impl<value_type, base_type>& itr) const { return (*this == itr) || (*this > itr); }

		// Supports compound assignment operations += and -=
		const iterator_impl<value_type, base_type>& operator +=(int value) const { return *this + value; }
		const iterator_impl<value_type, base_type>& operator -=(int value) const { return *this - value; }

		// Supports the offset dereference operator ([])
		value_type operator [](int offset) const
		{
			return _pos + offset < static_cast<std::int64_t>(_stream->_packets.size()) ? _stream->_packets[_pos + offset].get() : nullptr;
		}

	private:
		base_type _stream;
		mutable std::int64_t _pos;
	};

	using iterator = iterator_impl<Packet*, TcpStream*>;
	using const_iterator = iterator_impl<const Packet*, const TcpStream*>;

	TcpStream();

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

	iterator::value_type operator [](int offset);
	const_iterator::value_type operator [](int offset) const;

	void addPacket(std::shared_ptr<Packet> packet);

	void setClientWindowScale(std::uint8_t clientWindowScale);
	void setServerWindowScale(std::uint8_t serverWindowScale);

	bool isEmpty() const;
	std::size_t getNumberOfPackets() const;

	Packet::Timestamp getStartTime() const;
	Packet::Timestamp getEndTime() const;
	std::uint8_t getClientWindowScale() const;
	std::uint8_t getServerWindowScale() const;
	std::chrono::microseconds getDuration() const;

	std::string getClientIp() const;
	std::string getServerIp() const;
	std::uint32_t getWindowSize(std::uint64_t packetIndex) const;
	std::uint32_t getWindowSize(const Packet* packet) const;
	std::uint32_t getRelativeSequenceNumber(std::uint64_t packetIndex) const;
	std::uint32_t getRelativeSequenceNumber(const Packet* packet) const;
	std::uint32_t getRelativeAckNumber(std::uint64_t packetIndex) const;
	std::uint32_t getRelativeAckNumber(const Packet* packet) const;
	std::chrono::microseconds getRelativePacketTime(std::uint64_t packetIndex) const;
	std::chrono::microseconds getRelativePacketTime(const Packet* packet) const;

private:
	std::vector<std::shared_ptr<Packet>> _packets;
	std::uint8_t _clientWindowScale;
	std::uint8_t _serverWindowScale;
};

#endif
