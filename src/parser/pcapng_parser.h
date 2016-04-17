#ifndef PARSER_PCAPNG_PARSER_H
#define PARSER_PCAPNG_PARSER_H

#include <memory>

#include "parser/pcapng_file.h"
#include "stream/packet.h"
#include "stream/tcp_stream.h"

class PcapngParser
{
public:
	PcapngParser(std::unique_ptr<PcapngFile> file);

	std::shared_ptr<TcpStream> parse();

private:
	std::shared_ptr<Packet> parsePacket(const std::vector<std::uint8_t>& data, const timeval& timestamp, std::int32_t llProto);
	void parseTcpOptions(const std::vector<std::uint8_t>& data, std::shared_ptr<Packet>& packet);
	std::string parseIp(std::uint32_t addr);

	std::unique_ptr<PcapngFile> _file;
};

#endif
