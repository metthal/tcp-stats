#ifndef SPEED_ANALYSIS_H
#define SPEED_ANALYSIS_H

#include "analysis/stream_analysis.h"

struct SpeedOutput : public AnalysisOutput
{
	SpeedOutput() = default;
	SpeedOutput(const SpeedOutput& output) = default;
	virtual ~SpeedOutput() = default;

	std::vector<std::uint64_t> speedInTime;
};

class SpeedAnalysis : public StreamAnalysis
{
public:
	SpeedAnalysis() = default;
	virtual ~SpeedAnalysis() = default;

	virtual void accept(StreamAnalysisVisitor& visitor) override;

	virtual std::string name() const override { return "Speed"; }
	virtual void run(const TcpStream& stream) override;
};

#endif
