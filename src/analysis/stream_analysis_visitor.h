#ifndef ANALYSIS_STREAM_ANALYSIS_VISITOR_H
#define ANALYSIS_STREAM_ANALYSIS_VISITOR_H

#include "analysis/all_analyses.h"

class StreamAnalysisVisitor
{
public:
	virtual void visit(const PacketExchangeAnalysis& analysis) = 0;
};

#endif
