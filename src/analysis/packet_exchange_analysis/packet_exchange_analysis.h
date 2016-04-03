#ifndef ANALYSIS_PACKET_EXCHANGE_ANALYSIS_H
#define ANALYSIS_PACKET_EXCHANGE_ANALYSIS_H

#include "analysis/stream_analysis.h"

class PacketExchangeAnalysis : public StreamAnalysis
{
public:
	virtual std::string name() const override { return "Packet Exchange"; }
	virtual Json::Value run(const TcpStream& stream) override;
};

#endif
