#ifndef ANALYSIS_STREAM_ANALYSIS_H
#define ANALYSIS_STREAM_ANALYSIS_H

#include <memory>
#include <string>

#include <jsoncpp/json/json.h>

#include "stream/tcp_stream.h"

class StreamAnalysisVisitor;

struct AnalysisOutput
{
	~AnalysisOutput() = default;

protected:
	AnalysisOutput() = default;
	AnalysisOutput(const AnalysisOutput& output) = default;
};

class StreamAnalysis
{
public:
	StreamAnalysis() = default;
	~StreamAnalysis() = default;

	virtual void accept(StreamAnalysisVisitor& visitor) = 0;

	virtual std::string name() const = 0;
	virtual void run(const TcpStream& stream) = 0;

	const AnalysisOutput* output() const { return _output.get(); }

protected:
	std::unique_ptr<AnalysisOutput> _output;
};

#endif
