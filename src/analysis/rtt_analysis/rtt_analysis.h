#ifndef ANALYSIS_RTT_ANALYSIS_H
#define ANALYSIS_RTT_ANALYSIS_H

#include "analysis/stream_analysis.h"

#include <map>

struct RttOutput : public AnalysisOutput
{
	RttOutput() = default;
	RttOutput(const RttOutput& output) = default;
	virtual ~RttOutput() = default;

	std::map<std::chrono::microseconds, std::chrono::microseconds> clientRtt;
	std::map<std::chrono::microseconds, std::chrono::microseconds> serverRtt;
};

class RttAnalysis : public StreamAnalysis
{
	using SeqAckBuffer = std::vector<const Packet*>;

public:
	RttAnalysis() = default;
	virtual ~RttAnalysis() = default;

	virtual void accept(StreamAnalysisVisitor& visitor) override;

	virtual std::string name() const override { return "Round-Trip Time"; }
	virtual void run(const TcpStream& stream) override;

protected:
	SeqAckBuffer ackPackets(const Packet* ack, SeqAckBuffer& ackBuffer);
};

#endif
