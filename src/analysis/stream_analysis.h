#ifndef ANALYSIS_STREAM_ANALYSIS_H
#define ANALYSIS_STREAM_ANALYSIS_H

#include <memory>
#include <string>

#include <jsoncpp/json/json.h>

#include "stream/tcp_stream.h"

class StreamAnalysis
{
public:
	virtual std::string name() const = 0;
	virtual Json::Value run(const TcpStream& stream) = 0;
};

#endif
