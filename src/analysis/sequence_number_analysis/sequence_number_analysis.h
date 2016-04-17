#ifndef ANALYSIS_SEQUENCE_NUMBER_ANALYSIS
#define ANALYSIS_SEQUENCE_NUMBER_ANALYSIS

#include "analysis/stream_analysis.h"

struct SequenceNumberOutput : public AnalysisOutput
{
	SequenceNumberOutput() = default;
	SequenceNumberOutput(const SequenceNumberOutput& output) = default;
	virtual ~SequenceNumberOutput() = default;

	std::map<std::chrono::microseconds, std::uint32_t> clientSeqNums;
	std::map<std::chrono::microseconds, std::uint32_t> serverSeqNums;
};

class SequenceNumberAnalysis : public StreamAnalysis
{
public:
	SequenceNumberAnalysis() = default;
	virtual ~SequenceNumberAnalysis() = default;

	virtual void accept(StreamAnalysisVisitor& visitor) override;

	virtual std::string name() const override { return "Sequence Numbers"; }
	virtual void run(const TcpStream& stream) override;
};

#endif
