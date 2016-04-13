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

	if (stream->getNumberOfPackets() < 2)
		throw NotEnoughPacketsException();

	auto synPkt = stream->operator[](0);
	auto synAckPkt = stream->operator[](1);

	if (!synPkt->isSyn() || synPkt->isAck())
		throw InvalidHandshakeException();

	if (!synAckPkt->isSyn() || !synAckPkt->isAck())
		throw InvalidHandshakeException();

	stream->setClientWindowScale(synPkt->getWindowScale());
	stream->setServerWindowScale(synAckPkt->getWindowScale());

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

	packet->setSize(data.size());
	packet->setTimestamp(timevalToTimestamp(timestamp));
	packet->setSourceIp(parseIp(ipHeader->saddr));
	packet->setDestIp(parseIp(ipHeader->daddr));
	packet->setSourcePort(ntohs(tcpHeader->source));
	packet->setDestPort(ntohs(tcpHeader->dest));
	packet->setSequenceNumber(ntohl(tcpHeader->seq));
	packet->setAckNumber(ntohl(tcpHeader->ack_seq));
	packet->setWindowSize(ntohs(tcpHeader->window));
	packet->setSyn(tcpHeader->syn);
	packet->setFin(tcpHeader->fin);
	packet->setAck(tcpHeader->ack);
	packet->setPush(tcpHeader->psh);
	packet->setReset(tcpHeader->rst);
	packet->setUrgent(tcpHeader->urg);

	std::vector<std::uint8_t> opts;
	std::copy(data.data() + sizeof(ether_header) + ipHeader->ihl * 4 + sizeof(tcphdr),
			data.data() + sizeof(ether_header) + ipHeader->ihl * 4 + tcpHeader->doff * 4,
			std::back_inserter(opts));

	parseTcpOptions(opts, packet);

	return packet;
}

void PcapngParser::parseTcpOptions(const std::vector<std::uint8_t>& data, std::shared_ptr<Packet>& packet)
{
	if (data.size() < 3)
		return;

	std::uint8_t option = data[0];
	std::uint8_t size = data[1];
	if ((option == TCPOPT_EOL) || (data.size() < size))
		return;

	std::vector<std::uint8_t> newData;
	switch (option)
	{
		case TCPOPT_NOP:
			newData.assign(data.begin() + 1, data.end());
			break;
		case TCPOPT_WINDOW:
			packet->setWindowScale(data[2]);
			newData.assign(data.begin() + size, data.end());
			break;
		default:
			newData.assign(data.begin() + size, data.end());
			break;
	}

	parseTcpOptions(newData, packet);
}

std::string PcapngParser::parseIp(std::uint32_t addr)
{
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr, str, INET_ADDRSTRLEN);
	return std::string(str);
}
