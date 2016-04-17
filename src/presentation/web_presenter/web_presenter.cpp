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
	graph["xAxis"]["title"]["text"] = "Relative Packet Time [ms]";
	graph["yAxis"]["title"]["text"] = "RTT [ms]";
	graph["tooltip"]["valueSuffix"] = " ms";

	Json::Value clientPlotData;
	clientPlotData["name"] = "Client";
	for (const auto& seqRtt : output->clientRtt)
	{
		Json::Value point;
		point.append(seqRtt.first.count() / 1000.0); // In milliseconds.
		point.append(seqRtt.second.count() / 1000.0); // In milliseconds.
		clientPlotData["data"].append(point);
	}

	Json::Value serverPlotData;
	serverPlotData["name"] = "Server";
	for (const auto& seqRtt : output->serverRtt)
	{
		Json::Value point;
		point.append(seqRtt.first.count() / 1000.0); // In milliseconds.
		point.append(seqRtt.second.count() / 1000.0); // In milliseconds.
		serverPlotData["data"].append(point);
	}

	graph["series"].append(clientPlotData);
	graph["series"].append(serverPlotData);

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

	Json::Value clientToServerGraph;
	clientToServerGraph["title"]["text"] = "Client to Server " + analysis.name();
	clientToServerGraph["chart"]["type"] = "line";
	clientToServerGraph["chart"]["zoomType"] = "x";
	clientToServerGraph["xAxis"]["title"]["text"] = "Time [ms]";
	clientToServerGraph["yAxis"]["title"]["text"] = "Speed [kB/s]";
	clientToServerGraph["tooltip"]["headerFormat"] = "";
	clientToServerGraph["tooltip"]["pointFormat"] = "<span>{point.x} ms: <b>{point.y}</b></span>";
	clientToServerGraph["tooltip"]["valueSuffix"] = " kB/s";
	clientToServerGraph["tooltip"]["valueDecimals"] = 2;
	Json::Value clientPlotData, clientPlotAvgData;
	clientPlotData["name"] = "Immediate";
	for (const auto& timeSpeed : output->clientToServerSpeed)
	{
		Json::Value point;
		point.append(timeSpeed.first.count() / 1000.0); // In milliseconds.
		point.append(timeSpeed.second / 1024.0); // In kilobytes.
		clientPlotData["data"].append(point);
	}
	clientPlotAvgData["name"] = "Average";
	for (const auto& timeSpeed : output->clientToServerAvgSpeed)
	{
		Json::Value point;
		point.append(timeSpeed.first.count() / 1000.0); // In milliseconds.
		point.append(timeSpeed.second / 1024.0); // In kilobytes.
		clientPlotAvgData["data"].append(point);
	}
	clientToServerGraph["series"].append(clientPlotData);
	clientToServerGraph["series"].append(clientPlotAvgData);

	Json::Value serverToClientGraph;
	serverToClientGraph["title"]["text"] = "Server to Client " + analysis.name();
	serverToClientGraph["chart"]["type"] = "line";
	serverToClientGraph["chart"]["zoomType"] = "x";
	serverToClientGraph["xAxis"]["title"]["text"] = "Time [ms]";
	serverToClientGraph["yAxis"]["title"]["text"] = "Speed [kB/s]";
	serverToClientGraph["tooltip"]["headerFormat"] = "";
	serverToClientGraph["tooltip"]["pointFormat"] = "<span>{point.x} ms: <b>{point.y}</b></span>";
	serverToClientGraph["tooltip"]["valueSuffix"] = " kB/s";
	serverToClientGraph["tooltip"]["valueDecimals"] = 2;
	Json::Value serverPlotData, serverPlotAvgData;
	serverPlotData["name"] = "Immediate";
	for (const auto& timeSpeed : output->serverToClientSpeed)
	{
		Json::Value point;
		point.append(timeSpeed.first.count() / 1000.0); // In milliseconds.
		point.append(timeSpeed.second / 1024.0); // In kilobytes.
		serverPlotData["data"].append(point);
	}
	serverPlotAvgData["name"] = "Average";
	for (const auto& timeSpeed : output->serverToClientAvgSpeed)
	{
		Json::Value point;
		point.append(timeSpeed.first.count() / 1000.0); // In milliseconds.
		point.append(timeSpeed.second / 1024.0); // In kilobytes.
		serverPlotAvgData["data"].append(point);
	}
	serverToClientGraph["series"].append(serverPlotData);
	serverToClientGraph["series"].append(serverPlotAvgData);

	Json::Value data;
	data.append(clientToServerGraph);
	data.append(serverToClientGraph);
	_root["analyses_data"][snakeCaseString(analysis.name())] = data;
}

void WebPresenter::visit(const WindowSizeAnalysis& analysis)
{
	Json::Value metadata;
	metadata["id"] = snakeCaseString(analysis.name());
	metadata["name"] = analysis.name();
	metadata["dataType"] = "graph";
	_root["analyses"].append(metadata);

	const WindowSizeOutput* output = static_cast<const WindowSizeOutput*>(analysis.output());

	Json::Value graph;
	graph["title"]["text"] = analysis.name();
	graph["chart"]["type"] = "line";
	graph["chart"]["zoomType"] = "x";
	graph["xAxis"]["title"]["text"] = "Time [ms]";
	graph["yAxis"]["title"]["text"] = "Window Size [B]";
	graph["tooltip"]["headerFormat"] = "";
	Json::Value clientPlotData;
	clientPlotData["name"] = "Client";
	for (const auto& timeWindowSize : output->clientWindowSize)
	{
		Json::Value point;
		point.append(timeWindowSize.first.count() / 1000.0); // In milliseconds.
		point.append(timeWindowSize.second);
		clientPlotData["data"].append(point);
	}
	Json::Value serverPlotData;
	serverPlotData["name"] = "Server";
	for (const auto& timeWindowSize : output->serverWindowSize)
	{
		Json::Value point;
		point.append(timeWindowSize.first.count() / 1000.0); // In milliseconds.
		point.append(timeWindowSize.second);
		serverPlotData["data"].append(point);
	}

	graph["series"].append(clientPlotData);
	graph["series"].append(serverPlotData);

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
