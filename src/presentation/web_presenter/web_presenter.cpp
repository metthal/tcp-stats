#include <fstream>

#include "presentation/web_presenter/web_presenter.h"
#include "utils/conversion.h"

WebPresenter::WebPresenter() : _root()
{
}

void WebPresenter::present(const std::string& fileName)
{
	std::ofstream file(fileName, std::ios::trunc);
	Json::StyledWriter writer;
	file << "tcpstats_data = " << writer.write(_root);
	file.close();
}

void WebPresenter::visit(const OverallInfoAnalysis& analysis)
{
	Json::Value metadata;
	metadata["id"] = snakeCaseString(analysis.name());
	metadata["name"] = analysis.name();
	metadata["dataType"] = "key_value";
	_root["analyses"].append(metadata);

	const OverallInfoOutput* output = static_cast<const OverallInfoOutput*>(analysis.output());

	Json::Value data;
	data.append(buildKeyValue("Client IP", output->clientIp));
	data.append(buildKeyValue("Server IP", output->serverIp));
	data.append(buildKeyValue("Number of packets", convertToString(output->numberOfPackets)));
	data.append(buildKeyValue("Connection established on", timestampToDateTime(output->firstPacketTime)));
	data.append(buildKeyValue("Connection finished on", timestampToDateTime(output->lastPacketTime)));
	data.append(buildKeyValue("Connection lasted for", durationToTime(output->duration)));
	data.append(buildKeyValue("Client to server window multiplier", convertToString(1 << output->clientWindowScale)));
	data.append(buildKeyValue("Server to client window multiplier", convertToString(1 << output->serverWindowScale)));
	_root["analyses_data"][snakeCaseString(analysis.name())] = data;
}

void WebPresenter::visit(const RttAnalysis& analysis)
{
	Json::Value metadata;
	metadata["id"] = snakeCaseString(analysis.name());
	metadata["name"] = analysis.name();
	metadata["dataType"] = "graph";
	_root["analyses"].append(metadata);

	const RttOutput* output = static_cast<const RttOutput*>(analysis.output());

	Json::Value graph;
	graph["title"]["text"] = analysis.name();
	graph["chart"]["type"] = "line";
	graph["chart"]["zoomType"] = "x";
	graph["xAxis"]["title"]["text"] = "Packet Number";
	graph["yAxis"]["title"]["text"] = "RTT [ms]";
	graph["tooltip"]["valueSuffix"] = " ms";

	Json::Value plotData;
	for (const auto& rttInfo : output->rtts)
	{
		// In milliseconds.
		plotData["data"].append(rttInfo.rtt.count() / 1000.0);
	}
	graph["series"].append(plotData);

	Json::Value data;
	data.append(graph);
	_root["analyses_data"][snakeCaseString(analysis.name())] = data;
}

void WebPresenter::visit(const SpeedAnalysis& analysis)
{
	Json::Value metadata;
	metadata["id"] = snakeCaseString(analysis.name());
	metadata["name"] = analysis.name();
	metadata["dataType"] = "graph";
	_root["analyses"].append(metadata);

	const SpeedOutput* output = static_cast<const SpeedOutput*>(analysis.output());

	Json::Value graph;
	graph["title"]["text"] = analysis.name();
	graph["chart"]["type"] = "line";
	graph["xAxis"]["title"]["text"] = "Time [s]";
	graph["yAxis"]["title"]["text"] = "Speed [kB/s]";
	graph["tooltip"]["headerFormat"] = "";
	graph["tooltip"]["pointFormat"] = "<span>{point.x}s: <b>{point.y}</b></span>";
	graph["tooltip"]["valueSuffix"] = " kB/s";
	graph["tooltip"]["valueDecimals"] = 2;
	graph["legend"]["enabled"] = false;

	Json::Value plotData;
	for (const auto& speed : output->speedInTime)
	{
		// In kilobytes.
		plotData["data"].append(speed / 1024.0);
	}
	graph["series"].append(plotData);

	Json::Value data;
	data.append(graph);
	_root["analyses_data"][snakeCaseString(analysis.name())] = data;
}

template <typename T> Json::Value WebPresenter::buildKeyValue(const std::string& key, const T& value)
{
	Json::Value keyValue;
	keyValue["key"] = key;
	keyValue["value"] = value;
	return keyValue;
}
