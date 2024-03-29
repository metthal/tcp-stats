#ifndef ANALYSIS_OVERALL_INFO_ANALYSIS_H
#define ANALYSIS_OVERALL_INFO_ANALYSIS_H

#include "analysis/stream_analysis.h"

struct OverallInfoOutput : public AnalysisOutput
{
public:
	OverallInfoOutput() = default;
	OverallInfoOutput(const OverallInfoOutput& output) = default;
	virtual ~OverallInfoOutput() = default;

	std::string clientIp;
	std::string serverIp;
	std::uint16_t clientPort;
	std::uint16_t serverPort;
	Packet::Timestamp firstPacketTime;
	Packet::Timestamp lastPacketTime;
	std::chrono::microseconds duration;
	std::uint64_t numberOfPackets;
	std::uint8_t clientWindowScale;
	std::uint8_t serverWindowScale;
};

class OverallInfoAnalysis : public StreamAnalysis
{
public:
	OverallInfoAnalysis() = default;
	virtual ~OverallInfoAnalysis() = default;

	virtual void accept(StreamAnalysisVisitor& visitor) override;

	virtual std::string name() const override { return "Overall Information"; }
	virtual void run(const TcpStream& stream) override;
};

#endif
