#ifndef ANALYSIS_ANALYSIS_MANAGER_H
#define ANALYSIS_ANALYSIS_MANAGER_H

#include <memory>
#include <vector>

#include <jsoncpp/json/json.h>

#include "analysis/stream_analysis.h"
#include "stream/tcp_stream.h"

#define REGISTER_ANALYSIS(a) AnalysisManager::instance().add(std::make_unique<a>())

class AnalysisManager
{
public:
	static AnalysisManager& instance();

	void add(std::unique_ptr<StreamAnalysis> analysis);
	Json::Value runAll(const TcpStream& stream);

private:
	AnalysisManager();

	std::vector<std::unique_ptr<StreamAnalysis>> _analyzers;
};

#endif
