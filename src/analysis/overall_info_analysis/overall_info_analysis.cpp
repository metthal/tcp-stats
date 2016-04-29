#include "analysis/overall_info_analysis/overall_info_analysis.h"
#include "analysis/stream_analysis_visitor.h"
#include "utils/make_unique.h"

void OverallInfoAnalysis::accept(StreamAnalysisVisitor& visitor)
{
	visitor.visit(*this);
}

void OverallInfoAnalysis::run(const TcpStream& stream)
{
	auto output = std::make_unique<OverallInfoOutput>();
	output->clientIp = stream.getClientIp();
	output->serverIp = stream.getServerIp();
	output->clientPort = stream.getClientPort();
	output->serverPort = stream.getServerPort();
	output->numberOfPackets = stream.getNumberOfPackets();
	output->firstPacketTime = stream.getStartTime();
	output->lastPacketTime = stream.getEndTime();
	output->duration = stream.getDuration();
	output->clientWindowScale = stream.getClientWindowScale();
	output->serverWindowScale = stream.getServerWindowScale();

	_output = std::move(output);
}
