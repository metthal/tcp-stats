#include <algorithm>

#include "analysis/speed_analysis/speed_analysis.h"
#include "analysis/stream_analysis_visitor.h"
#include "utils/make_unique.h"

void SpeedAnalysis::accept(StreamAnalysisVisitor& visitor)
{
	visitor.visit(*this);
}

void SpeedAnalysis::run(const TcpStream& stream)
{
	auto output = std::make_unique<SpeedOutput>();

	std::deque<const Packet*> clientToServerLastSecond, serverToClientLastSecond;
	for (const auto& packet : stream)
	{
		std::deque<const Packet*>* packetQueue = nullptr;
		std::map<std::chrono::microseconds, std::uint64_t>* speedMap = nullptr;
		std::map<std::chrono::microseconds, std::uint64_t>* avgSpeedMap = nullptr;
		if (packet->getSourceIp() == stream.getClientIp())
		{
			speedMap = &output->clientToServerSpeed;
			avgSpeedMap = &output->clientToServerAvgSpeed;
			packetQueue = &clientToServerLastSecond;
		}
		else
		{
			speedMap = &output->serverToClientSpeed;
			avgSpeedMap = &output->serverToClientAvgSpeed;
			packetQueue = &serverToClientLastSecond;
		}

		auto speed = recalculatePackets(*packetQueue, packet);
		(*speedMap)[stream.getRelativePacketTime(packet)] = speed;

		std::uint64_t avgSpeed = 0;
		for (const auto& timeSpeed : *speedMap)
			avgSpeed += timeSpeed.second;

		(*avgSpeedMap)[stream.getRelativePacketTime(packet)] = avgSpeed / speedMap->size();
	}

	_output = std::move(output);
}

std::uint64_t SpeedAnalysis::recalculatePackets(std::deque<const Packet*>& packetQueue, const Packet* newPacket)
{
	const auto second = std::chrono::seconds(1);
	while (!packetQueue.empty() && (newPacket->getTimestamp() - packetQueue.front()->getTimestamp() >= second))
	{
		packetQueue.pop_front();
	}

	packetQueue.push_back(newPacket);

	std::uint64_t speed = 0;
	std::for_each(packetQueue.begin(), packetQueue.end(), [&speed](const Packet* packet) { speed += packet->getSize(); });
	return speed;
}
