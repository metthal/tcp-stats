#include "analysis/speed_analysis/speed_analysis.h"
#include "analysis/stream_analysis_visitor.h"

void SpeedAnalysis::accept(StreamAnalysisVisitor& visitor)
{
	visitor.visit(*this);
}

void SpeedAnalysis::run(const TcpStream& stream)
{
	auto output = std::make_unique<SpeedOutput>();

	std::uint64_t seconds = std::chrono::duration_cast<std::chrono::seconds>(stream.getDuration()).count() + 1;
	output->speedInTime.resize(seconds, 0);

	auto startTime = stream[0]->getTimestamp();
	for (const auto& packet : stream)
	{
		auto relativeTime = packet->getTimestamp() - startTime;
		std::uint64_t relativeSeconds = std::chrono::duration_cast<std::chrono::seconds>(relativeTime).count();

		output->speedInTime[relativeSeconds] += packet->getSize();
	}

	_output = std::move(output);
}
