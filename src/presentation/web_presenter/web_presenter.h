#ifndef PRESENTATION_WEB_PRESENTER_H
#define PRESENTATION_WEB_PRESENTER_H

#include <map>
#include "analysis/stream_analysis_visitor.h"

class WebPresenter : public StreamAnalysisVisitor
{
public:
	WebPresenter();

	void present(const std::string& fileName);

	virtual void visit(const OverallInfoAnalysis& analysis) override;
	virtual void visit(const RttAnalysis& analysis) override;
	virtual void visit(const SpeedAnalysis& analysis) override;
	virtual void visit(const WindowSizeAnalysis& analysis) override;
	virtual void visit(const SequenceNumberAnalysis& analysis) override;

protected:
	void buildMetadata(const std::string& name, const std::string& metadata);

	template <typename T> Json::Value buildKeyValue(const std::string& key, const T& value);

	Json::Value buildGraph(const std::string& title, const std::string& xAxisTitle, const std::string& yAxisTitle);
	template <typename T, typename U, typename XT, typename YT> Json::Value buildPlotData(
			const typename std::map<T,U>::const_iterator& begin, const typename std::map<T,U>::const_iterator& end,
			const std::function<XT(const T&)>& xTransform, const std::function<YT(const U&)>& yTransform);

private:
	Json::Value _root;
};

#endif
