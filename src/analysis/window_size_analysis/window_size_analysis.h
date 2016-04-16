#ifndef ANALYSIS_WINDOW_SIZE_ANALYSIS_H
#define ANALYSIS_WINDOW_SIZE_ANALYSIS_H

#include "analysis/stream_analysis.h"

struct WindowSizeOutput : public AnalysisOutput
{
	WindowSizeOutput() = default;
	WindowSizeOutput(const WindowSizeOutput& output) = default;
	virtual ~WindowSizeOutput() = default;

	std::map<std::chrono::microseconds, std::uint32_t> clientWindowSize;
	std::map<std::chrono::microseconds, std::uint32_t> serverWindowSize;
};

class WindowSizeAnalysis : public StreamAnalysis
{
public:
	WindowSizeAnalysis() = default;
	virtual ~WindowSizeAnalysis() = default;

	virtual void accept(StreamAnalysisVisitor& visitor) override;

	virtual std::string name() const override { return "Window Size"; }
	virtual void run(const TcpStream& stream) override;
};

#endif
