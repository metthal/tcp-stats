#ifndef SPEED_ANALYSIS_H
#define SPEED_ANALYSIS_H

#include <map>
#include <utility>

#include "analysis/stream_analysis.h"

struct SpeedOutput : public AnalysisOutput
{
	SpeedOutput() = default;
	SpeedOutput(const SpeedOutput& output) = default;
	virtual ~SpeedOutput() = default;

	std::map<std::chrono::microseconds, std::uint64_t> clientToServerSpeed;
	std::map<std::chrono::microseconds, std::uint64_t> clientToServerAvgSpeed;
	std::map<std::chrono::microseconds, std::uint64_t> serverToClientSpeed;
	std::map<std::chrono::microseconds, std::uint64_t> serverToClientAvgSpeed;
};

class SpeedAnalysis : public StreamAnalysis
{
public:
	SpeedAnalysis() = default;
	virtual ~SpeedAnalysis() = default;

	virtual void accept(StreamAnalysisVisitor& visitor) override;

	virtual std::string name() const override { return "Speed"; }
	virtual void run(const TcpStream& stream) override;

protected:
	std::uint64_t recalculatePackets(std::deque<const Packet*>& packetQueue, const Packet* newPacket);
};

#endif
