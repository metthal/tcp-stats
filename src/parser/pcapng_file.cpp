#include "parser/pcapng_file.h"
#include "utils/exceptions.h"

PcapngFile::PcapngFile(pcap_t* pcap) : _pcap(pcap)
{
}

std::unique_ptr<PcapngFile> PcapngFile::createFromPath(const std::string& filePath)
{
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* pcapHandle = pcap_open_offline(filePath.c_str(), errbuf);
	if (pcapHandle == NULL)
	{
		throw InvalidInputFileException(filePath);
	}

	if (pcap_datalink(pcapHandle) != DLT_EN10MB)
	{
		throw UnsupportedDataLinkLayerException();
	}

	return std::make_unique<PcapngFile>(pcapHandle);
}

bool PcapngFile::nextPacket(std::vector<std::uint8_t>& packet, timeval& timestamp)
{
	pcap_pkthdr* hdr;
	const u_char* data;

	if (pcap_next_ex(_pcap, &hdr, &data) != 1)
		return false;

	packet.clear();
	packet.resize(hdr->len);

	timestamp = hdr->ts;
	std::copy(data, data + packet.size(), packet.begin());
	return true;
}
