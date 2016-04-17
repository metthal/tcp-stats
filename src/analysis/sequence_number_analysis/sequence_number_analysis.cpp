#include "analysis/sequence_number_analysis/sequence_number_analysis.h"
#include "analysis/stream_analysis_visitor.h"

void SequenceNumberAnalysis::accept(StreamAnalysisVisitor& visitor)
{
	visitor.visit(*this);
}

void SequenceNumberAnalysis::run(const TcpStream& stream)
{
	auto output = std::make_unique<SequenceNumberOutput>();

	for (const auto& packet : stream)
	{
		std::map<std::chrono::microseconds, std::uint32_t>* seqNumBuffer = nullptr;
		if (packet->getSourceIp() == stream.getClientIp())
			seqNumBuffer = &output->clientSeqNums;
		else
			seqNumBuffer = &output->serverSeqNums;

		(*seqNumBuffer)[stream.getRelativePacketTime(packet)] = stream.getRelativeSequenceNumber(packet);
	}

	_output = std::move(output);
}
