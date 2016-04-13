#ifndef PRESENTATION_WEB_PRESENTER_H
#define PRESENTATION_WEB_PRESENTER_H

#include "analysis/stream_analysis_visitor.h"

class WebPresenter : public StreamAnalysisVisitor
{
public:
	WebPresenter();

	void present(const std::string& fileName);

	virtual void visit(const OverallInfoAnalysis& analysis) override;
	virtual void visit(const RttAnalysis& analysis) override;
	virtual void visit(const SpeedAnalysis& analysis) override;

protected:
	template <typename T> Json::Value buildKeyValue(const std::string& key, const T& value);

private:
	Json::Value _root;
};

#endif
