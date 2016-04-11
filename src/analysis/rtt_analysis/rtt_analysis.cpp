#include <cassert>
#include <unordered_map>
#include <utility>

#include "analysis/rtt_analysis/rtt_analysis.h"
#include "analysis/stream_analysis_visitor.h"

void RttAnalysis::accept(StreamAnalysisVisitor& visitor)
{
	visitor.visit(*this);
}

void RttAnalysis::run(const TcpStream& stream)
{
	using SeqAckTable = std::unordered_map<std::uint32_t, std::pair<const Packet*, bool>>;

	auto output = std::make_unique<RttOutput>();

	SeqAckTable seqAckTable;
	seqAckTable[stream[0]->getSequenceNumber() + 1] = std::make_pair(stream[0], false);
	seqAckTable[stream[1]->getSequenceNumber() + 1] = std::make_pair(stream[1], false);

	RttInfo irttInfo;
	irttInfo.message = stream[0];
	irttInfo.ack = stream[1];
	irttInfo.rtt = irttInfo.ack->getTimestamp() - irttInfo.message->getTimestamp();
	output->rtts.push_back(irttInfo);

	for (auto itr = stream.begin() + 2; itr != stream.end(); ++itr)
	{
		const Packet* packet = *itr;

		if (packet->isAck())
		{
			SeqAckTable::const_iterator seqItr = seqAckTable.find(packet->getAckNumber());
			if (seqItr != seqAckTable.end())
			{
				// If not acknowledged already.
				if (!seqItr->second.second)
				{
					RttInfo rttInfo;
					rttInfo.message = seqItr->second.first;
					rttInfo.ack = packet;
					rttInfo.rtt = rttInfo.ack->getTimestamp() - rttInfo.message->getTimestamp();
					output->rtts.push_back(rttInfo);

					// Acknowledge.
					seqAckTable[seqItr->first] = std::make_pair(seqItr->second.first, true);
				}
			}
		}

		seqAckTable[packet->getSequenceNumber()] = std::make_pair(packet, false);
	}

	_output = std::move(output);
}
