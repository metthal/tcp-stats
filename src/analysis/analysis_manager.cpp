#include "analysis/analysis_manager.h"
#include "utils/conversion.h"

AnalysisManager::AnalysisManager()
{
}

AnalysisManager& AnalysisManager::instance()
{
	static AnalysisManager manager;
	return manager;
}

void AnalysisManager::add(std::unique_ptr<StreamAnalysis> analysis)
{
	_analyzers.push_back(std::move(analysis));
}

Json::Value AnalysisManager::runAll(const TcpStream& stream)
{
	Json::Value root;

	for (auto& analysis : _analyzers)
	{
		auto result = analysis->run(stream);
		root[snakeCaseString(analysis->name())] = result;
	}

	return root;
}
