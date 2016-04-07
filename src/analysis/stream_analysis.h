#ifndef ANALYSIS_STREAM_ANALYSIS_H
#define ANALYSIS_STREAM_ANALYSIS_H

#include <memory>
#include <string>

#include <jsoncpp/json/json.h>

#include "stream/tcp_stream.h"

class StreamAnalysisVisitor;

class StreamAnalysis
{
public:
	virtual void accept(StreamAnalysisVisitor& visitor) = 0;

	virtual std::string name() const = 0;
	virtual void run(const TcpStream& stream) = 0;
};

#endif
