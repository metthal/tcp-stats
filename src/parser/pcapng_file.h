#ifndef PARSER_PCAPNG_FILE_H
#define PARSER_PCAPNG_FILE_H

#include <pcap/pcap.h>

#include <memory>
#include <vector>

class PcapngFile
{
public:
	PcapngFile(pcap_t* pcap);

	static std::unique_ptr<PcapngFile> createFromPath(const std::string& filePath);

	bool nextPacket(std::vector<std::uint8_t>& packet, timeval& timestamp);

private:
	pcap_t* _pcap;
};

#endif
