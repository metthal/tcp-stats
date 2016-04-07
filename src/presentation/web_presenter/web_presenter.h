#ifndef PRESENTATION_WEB_PRESENTER_H
#define PRESENTATION_WEB_PRESENTER_H

#include "analysis/stream_analysis_visitor.h"

class WebPresenter : public StreamAnalysisVisitor
{
public:
	WebPresenter(const std::string& outputFilePath);

	virtual void visit(const PacketExchangeAnalysis& analysis) override;

private:
	std::string _outputFilePath;
};

#endif
