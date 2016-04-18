#include <algorithm>

#include "analysis/rtt_analysis/rtt_analysis.h"
#include "analysis/stream_analysis_visitor.h"
#include "utils/make_unique.h"

void RttAnalysis::accept(StreamAnalysisVisitor& visitor)
{
	visitor.visit(*this);
}

void RttAnalysis::run(const TcpStream& stream)
{
	auto output = std::make_unique<RttOutput>();

	SeqAckBuffer clientBuffer, serverBuffer;

	for (const auto& packet : stream)
	{
		std::map<std::chrono::microseconds, std::chrono::microseconds>* rttTable = nullptr;
		SeqAckBuffer* sendBuffer = nullptr;
		SeqAckBuffer* ackBuffer = nullptr;
		if (packet->getSourceIp() == stream.getClientIp())
		{
			sendBuffer = &clientBuffer;
			ackBuffer = &serverBuffer;
			rttTable = &output->serverRtt;
		}
		else
		{
			sendBuffer = &serverBuffer;
			ackBuffer = &clientBuffer;
			rttTable = &output->clientRtt;
		}

		sendBuffer->push_back(packet);

		if (packet->isAck())
		{
			SeqAckBuffer ackedPackets = ackPackets(packet, *ackBuffer);
			if (ackedPackets.empty())
				continue;

			for (const auto& ackedPacket : ackedPackets)
			{
				(*rttTable)[stream.getRelativePacketTime(packet)] = packet->getTimestamp() - ackedPacket->getTimestamp();
			}
		}
	}

	_output = std::move(output);
}

RttAnalysis::SeqAckBuffer RttAnalysis::ackPackets(const Packet* ack, SeqAckBuffer& ackBuffer)
{
	auto itr = std::lower_bound(ackBuffer.begin(), ackBuffer.end(), ack,
			[](const Packet* seq, const Packet* ack)
			{
				return seq->getSequenceNumber() <= ack->getAckNumber();
			});

	SeqAckBuffer ret(ackBuffer.begin(), itr);
	ackBuffer.assign(itr, ackBuffer.end());
	return ret;
}
