#ifndef ANALYSIS_RTT_ANALYSIS_H
#define ANALYSIS_RTT_ANALYSIS_H

#include "analysis/stream_analysis.h"

struct RttInfo
{
	std::chrono::microseconds rtt;
	const Packet* message;
	const Packet* ack;
};

struct RttOutput : public AnalysisOutput
{
	RttOutput() = default;
	RttOutput(const RttOutput& output) = default;
	virtual ~RttOutput() = default;

	std::vector<RttInfo> rtts;
};

class RttAnalysis : public StreamAnalysis
{
public:
	RttAnalysis() = default;
	virtual ~RttAnalysis() = default;

	virtual void accept(StreamAnalysisVisitor& visitor) override;

	virtual std::string name() const override { return "Round-Trip Time"; }
	virtual void run(const TcpStream& stream) override;
};

#endif
