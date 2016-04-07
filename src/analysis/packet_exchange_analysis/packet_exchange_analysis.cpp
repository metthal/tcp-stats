#include "analysis/packet_exchange_analysis/packet_exchange_analysis.h"
#include "analysis/stream_analysis_visitor.h"

void PacketExchangeAnalysis::accept(StreamAnalysisVisitor& visitor)
{
	visitor.visit(*this);
}

void PacketExchangeAnalysis::run(const TcpStream& stream)
{
/*	Json::Value root;

	for (const auto& packet : stream)
	{
		Json::Value packetValue;

		packetValue["src_ip"] = packet->getSourceIp();
		packetValue["dst_ip"] = packet->getDestIp();
		packetValue["src_port"] = packet->getSourcePort();
		packetValue["dst_port"] = packet->getDestPort();

		root.append(packetValue);
	}

	return emitOutput(OutputType::Table, root);*/
}
