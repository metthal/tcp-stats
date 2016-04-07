#include <fstream>

#include "presentation/web_presenter/web_presenter.h"
#include "utils/conversion.h"

WebPresenter::WebPresenter(const std::string& outputFilePath) : _outputFilePath(outputFilePath)
{
}

void WebPresenter::visit(const PacketExchangeAnalysis& analysis)
{
	/*Json::Value root;

	// Fill in all analyses present in the output.
	for (const auto& output : _outputs)
	{
		Json::Value analysis;
		analysis["name"] = output.analysisName;
		analysis["id"] = snakeCaseString(output.analysisName);
		analysis["outputType"] = output.outputTypeAsString();
		analysis["data"] = output.data;
		root.append(analysis);
	}

	Json::StyledWriter writer;
	std::ofstream outputFile("xmilko01.json", std::ios::trunc);
	outputFile << writer.write(root);
	outputFile.close();*/
}
