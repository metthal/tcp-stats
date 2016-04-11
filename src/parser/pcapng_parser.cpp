#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#include <iostream>

#include "parser/pcapng_parser.h"
#include "utils/conversion.h"
#include "utils/exceptions.h"

PcapngParser::PcapngParser(std::unique_ptr<PcapngFile> file) : _file(std::move(file))
{
}

std::shared_ptr<TcpStream> PcapngParser::parse()
{
	std::vector<std::uint8_t> data;
	timeval timestamp;

	auto stream = std::make_shared<TcpStream>();

	while (_file->nextPacket(data, timestamp))
	{
		auto packet = parsePacket(data, timestamp);
		stream->addPacket(packet);
	}

	return stream;
}

std::shared_ptr<Packet> PcapngParser::parsePacket(const std::vector<std::uint8_t>& data, const timeval& timestamp)
{
	const ether_header* ethernetHeader = reinterpret_cast<const ether_header*>(data.data());
	if (ntohs(ethernetHeader->ether_type) != ETHERTYPE_IP)
	{
		throw UnsupportedIpProtocolException();
	}

	const iphdr* ipHeader = reinterpret_cast<const iphdr*>(data.data() + sizeof(ether_header));
	if (ipHeader->version != 4)
	{
		throw UnsupportedIpProtocolException();
	}

	if (ipHeader->protocol != IPPROTO_TCP)
	{
		throw UnsupportedTransportProtocolException();
	}

	const tcphdr* tcpHeader = reinterpret_cast<const tcphdr*>(data.data() + sizeof(ether_header) + ipHeader->ihl * 4);

	auto packet = std::make_shared<Packet>();

	packet->setTimestamp(timevalToTimestamp(timestamp));
	packet->setSourceIp(parseIp(ipHeader->saddr));
	packet->setDestIp(parseIp(ipHeader->daddr));
	packet->setSourcePort(ntohs(tcpHeader->source));
	packet->setDestPort(ntohs(tcpHeader->dest));
	packet->setSequenceNumber(ntohl(tcpHeader->seq));
	packet->setAckNumber(ntohl(tcpHeader->ack_seq));
	packet->setSyn(tcpHeader->syn);
	packet->setFin(tcpHeader->fin);
	packet->setAck(tcpHeader->ack);
	packet->setPush(tcpHeader->psh);
	packet->setReset(tcpHeader->rst);
	packet->setUrgent(tcpHeader->urg);

	return packet;
}

std::string PcapngParser::parseIp(std::uint32_t addr)
{
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr, str, INET_ADDRSTRLEN);
	return std::string(str);
}
