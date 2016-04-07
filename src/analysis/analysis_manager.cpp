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

bool AnalysisManager::runAll(const TcpStream& stream)
{
	/*std::vector outputs;
	for (auto& analysis : _analyzers)
	{
		outputs.push_back(analysis->run(stream));
	}

	return outputs;*/
	return false;
}

void AnalysisManager::visitAll(StreamAnalysisVisitor& visitor)
{
	for (const auto& analysis : _analyzers)
	{
		analysis->accept(visitor);
	}
}
