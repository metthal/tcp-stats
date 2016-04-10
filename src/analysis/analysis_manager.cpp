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

void AnalysisManager::runAll(const TcpStream& stream)
{
	for (auto& analysis : _analyzers)
	{
		analysis->run(stream);
	}
}

void AnalysisManager::visitAll(StreamAnalysisVisitor& visitor)
{
	for (const auto& analysis : _analyzers)
	{
		analysis->accept(visitor);
	}
}
