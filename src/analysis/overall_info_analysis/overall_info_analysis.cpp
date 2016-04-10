#include "analysis/overall_info_analysis/overall_info_analysis.h"
#include "analysis/stream_analysis_visitor.h"

void OverallInfoAnalysis::accept(StreamAnalysisVisitor& visitor)
{
	visitor.visit(*this);
}

void OverallInfoAnalysis::run(const TcpStream& stream)
{
	auto output = std::make_unique<OverallInfoOutput>();
	output->clientIp = stream.isEmpty() ? "" : stream[0]->getSourceIp();
	output->serverIp = stream.isEmpty() ? "" : stream[0]->getDestIp();
	output->numberOfPackets = stream.getNumberOfPackets();
	output->firstPacketTime = stream.getStartTime();
	output->lastPacketTime = stream.getEndTime();
	output->duration = stream.getDuration();

	_output = std::move(output);
}
