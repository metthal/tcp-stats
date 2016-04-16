#ifndef ANALYSIS_STREAM_ANALYSIS_VISITOR_H
#define ANALYSIS_STREAM_ANALYSIS_VISITOR_H

#include "analysis/all_analyses.h"

class StreamAnalysisVisitor
{
public:
	virtual void visit(const OverallInfoAnalysis& analysis) = 0;
	virtual void visit(const RttAnalysis& analysis) = 0;
	virtual void visit(const SpeedAnalysis& analysis) = 0;
	virtual void visit(const WindowSizeAnalysis& analysis) = 0;
};

#endif
