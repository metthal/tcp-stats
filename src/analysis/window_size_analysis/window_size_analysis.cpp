#include "analysis/window_size_analysis/window_size_analysis.h"
#include "analysis/stream_analysis_visitor.h"

void WindowSizeAnalysis::accept(StreamAnalysisVisitor& visitor)
{
	visitor.visit(*this);
}

void WindowSizeAnalysis::run(const TcpStream& stream)
{
	auto output = std::make_unique<WindowSizeOutput>();

	for (const auto& packet : stream)
	{
		std::map<std::chrono::microseconds, std::uint32_t>* windowSizeBuffer = nullptr;
		if (packet->getSourceIp() == stream.getClientIp())
			windowSizeBuffer = &output->clientWindowSize;
		else
			windowSizeBuffer = &output->serverWindowSize;

		(*windowSizeBuffer)[stream.getRelativePacketTime(packet)] = stream.getWindowSize(packet);
	}

	_output = std::move(output);
}
