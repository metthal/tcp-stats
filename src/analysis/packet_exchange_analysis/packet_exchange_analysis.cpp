#include "analysis/packet_exchange_analysis/packet_exchange_analysis.h"
#include "utils/conversion.h"

Json::Value PacketExchangeAnalysis::run(const TcpStream& stream)
{
	Json::Value root;

	for (const auto& packet : stream)
	{
		Json::Value packetValue;

		packetValue["src_ip"] = packet->getSourceIp();
		packetValue["dst_ip"] = packet->getDestIp();
		packetValue["src_port"] = packet->getSourcePort();
		packetValue["dst_port"] = packet->getDestPort();

		root.append(packetValue);
	}

	return root;
}
