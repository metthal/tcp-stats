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
	buildMetadata(analysis.name(), "key_value");

	const OverallInfoOutput* output = static_cast<const OverallInfoOutput*>(analysis.output());

	Json::Value data;
	data.append(buildKeyValue("Client IP", output->clientIp));
	data.append(buildKeyValue("Server IP", output->serverIp));
	data.append(buildKeyValue("Client port", convertToString(output->clientPort)));
	data.append(buildKeyValue("Server port", convertToString(output->serverPort)));
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
	buildMetadata(analysis.name(), "graph");

	const RttOutput* output = static_cast<const RttOutput*>(analysis.output());

	Json::Value graph = buildGraph(analysis.name(), "Relative Packet Time [ms]", "RTT [ms]");
	graph["tooltip"]["valueSuffix"] = " ms";

	auto usecToMs = [](const std::chrono::microseconds& t) { return t.count() / 1000.0; };

	Json::Value clientPlotData = buildPlotData<std::chrono::microseconds, std::chrono::microseconds, double, double>(output->clientRtt.begin(),
			output->clientRtt.end(), usecToMs, usecToMs);
	clientPlotData["name"] = "Client";

	Json::Value serverPlotData = buildPlotData<std::chrono::microseconds, std::chrono::microseconds, double, double>(output->serverRtt.begin(),
			output->serverRtt.end(), usecToMs, usecToMs);
	serverPlotData["name"] = "Server";

	graph["series"].append(clientPlotData);
	graph["series"].append(serverPlotData);

	Json::Value data;
	data.append(graph);
	_root["analyses_data"][snakeCaseString(analysis.name())] = data;
}

void WebPresenter::visit(const SpeedAnalysis& analysis)
{
	buildMetadata(analysis.name(), "graph");

	const SpeedOutput* output = static_cast<const SpeedOutput*>(analysis.output());

	auto usecToMs = [](const std::chrono::microseconds& t) { return t.count() / 1000.0; };
	auto bToKb = [](const std::uint64_t& b) { return b / 1024.0; };

	Json::Value clientToServerGraph = buildGraph("Client to Server " + analysis.name(), "Time [ms]", "Speed [kB/s]");
	clientToServerGraph["tooltip"]["headerFormat"] = "";
	clientToServerGraph["tooltip"]["pointFormat"] = "<span>{point.x} ms: <b>{point.y}</b></span>";
	clientToServerGraph["tooltip"]["valueSuffix"] = " kB/s";
	clientToServerGraph["tooltip"]["valueDecimals"] = 2;

	Json::Value clientPlotData = buildPlotData<std::chrono::microseconds, std::uint64_t, double, double>(output->clientToServerSpeed.begin(),
			output->clientToServerSpeed.end(), usecToMs, bToKb);
	clientPlotData["name"] = "Immediate";
	clientToServerGraph["series"].append(clientPlotData);

	Json::Value clientPlotAvgData = buildPlotData<std::chrono::microseconds, std::uint64_t, double, double>(output->clientToServerAvgSpeed.begin(),
			output->clientToServerAvgSpeed.end(), usecToMs, bToKb);
	clientPlotAvgData["name"] = "Average";
	clientToServerGraph["series"].append(clientPlotAvgData);

	Json::Value serverToClientGraph = buildGraph("Server to Client " + analysis.name(), "Time [ms]", "Speed [kB/s]");
	serverToClientGraph["tooltip"]["headerFormat"] = "";
	serverToClientGraph["tooltip"]["pointFormat"] = "<span>{point.x} ms: <b>{point.y}</b></span>";
	serverToClientGraph["tooltip"]["valueSuffix"] = " kB/s";
	serverToClientGraph["tooltip"]["valueDecimals"] = 2;

	Json::Value serverPlotData = buildPlotData<std::chrono::microseconds, std::uint64_t, double, double>(output->serverToClientSpeed.begin(),
			output->serverToClientSpeed.end(), usecToMs, bToKb);
	serverPlotData["name"] = "Immediate";
	serverToClientGraph["series"].append(serverPlotData);

	Json::Value serverPlotAvgData = buildPlotData<std::chrono::microseconds, std::uint64_t, double, double>(output->serverToClientAvgSpeed.begin(),
			output->serverToClientAvgSpeed.end(), usecToMs, bToKb);
	serverPlotAvgData["name"] = "Average";
	serverToClientGraph["series"].append(serverPlotAvgData);

	Json::Value data;
	data.append(clientToServerGraph);
	data.append(serverToClientGraph);
	_root["analyses_data"][snakeCaseString(analysis.name())] = data;
}

void WebPresenter::visit(const WindowSizeAnalysis& analysis)
{
	buildMetadata(analysis.name(), "graph");

	const WindowSizeOutput* output = static_cast<const WindowSizeOutput*>(analysis.output());

	auto usecToMs = [](const std::chrono::microseconds& t) { return t.count() / 1000.0; };
	auto id = [](const std::uint32_t& b) { return b; };

	Json::Value graph = buildGraph(analysis.name(), "Time [ms]", "Window Size [B]");
	graph["tooltip"]["headerFormat"] = "";

	Json::Value clientPlotData = buildPlotData<std::chrono::microseconds, std::uint32_t, double, std::uint32_t>(output->clientWindowSize.begin(),
			output->clientWindowSize.end(), usecToMs, id);
	clientPlotData["name"] = "Client";
	graph["series"].append(clientPlotData);

	Json::Value serverPlotData = buildPlotData<std::chrono::microseconds, std::uint32_t, double, std::uint32_t>(output->serverWindowSize.begin(),
			output->serverWindowSize.end(), usecToMs, id);
	serverPlotData["name"] = "Server";
	graph["series"].append(serverPlotData);

	Json::Value data;
	data.append(graph);
	_root["analyses_data"][snakeCaseString(analysis.name())] = data;
}

void WebPresenter::visit(const SequenceNumberAnalysis& analysis)
{
	buildMetadata(analysis.name(), "graph");

	const SequenceNumberOutput* output = static_cast<const SequenceNumberOutput*>(analysis.output());

	auto usecToMs = [](const std::chrono::microseconds& t) { return t.count() / 1000.0; };
	auto id = [](const std::uint32_t& b) { return b; };

	Json::Value graph = buildGraph(analysis.name(), "Time [ms]", "Relative Sequence Number");
	graph["tooltip"]["headerFormat"] = "";

	Json::Value clientPlotData = buildPlotData<std::chrono::microseconds, std::uint32_t, double, std::uint32_t>(output->clientSeqNums.begin(),
			output->clientSeqNums.end(), usecToMs, id);
	clientPlotData["name"] = "Client";
	graph["series"].append(clientPlotData);

	Json::Value serverPlotData = buildPlotData<std::chrono::microseconds, std::uint32_t, double, std::uint32_t>(output->serverSeqNums.begin(),
			output->serverSeqNums.end(), usecToMs, id);
	serverPlotData["name"] = "Server";
	graph["series"].append(serverPlotData);

	Json::Value data;
	data.append(graph);
	_root["analyses_data"][snakeCaseString(analysis.name())] = data;
}

void WebPresenter::buildMetadata(const std::string& name, const std::string& type)
{
	Json::Value metadata;
	metadata["id"] = snakeCaseString(name);
	metadata["name"] = name;
	metadata["dataType"] = type;
	_root["analyses"].append(metadata);
}

template <typename T> Json::Value WebPresenter::buildKeyValue(const std::string& key, const T& value)
{
	Json::Value keyValue;
	keyValue["key"] = key;
	keyValue["value"] = value;
	return keyValue;
}

Json::Value WebPresenter::buildGraph(const std::string& title, const std::string& xAxisTitle, const std::string& yAxisTitle)
{
	Json::Value graph;
	graph["title"]["text"] = title;
	graph["chart"]["type"] = "line";
	graph["chart"]["zoomType"] = "x";
	graph["xAxis"]["title"]["text"] = xAxisTitle;
	graph["yAxis"]["title"]["text"] = yAxisTitle;
	graph["plotOptions"]["series"]["boostThreshold"] = "1000";
	return graph;
}

template <typename T, typename U, typename XT, typename YT> Json::Value WebPresenter::buildPlotData(
		const typename std::map<T,U>::const_iterator& begin, const typename std::map<T,U>::const_iterator& end,
		const std::function<XT(const T&)>& xTransform, const std::function<YT(const U&)>& yTransform)
{
	Json::Value plotData;
	for (auto itr = begin; itr != end; ++itr)
	{
		XT x = xTransform(itr->first);
		YT y = yTransform(itr->second);

		Json::Value point;
		point.append(x);
		point.append(y);

		plotData["data"].append(point);
	}

	return plotData;
}
