#include <iostream>

#include "parser/pcapng_parser.h"
#include "analysis/all_analyses.h"
#include "analysis/analysis_manager.h"
#include "utils/exceptions.h"

void printHelp()
{
	std::cout << "TCP Stats by Marek Milkovic\n"
		<< "Project for course PDS 2015/2016 at FIT BUT\n"
		<< "\n"
		<< "Program for analysis of TCP streams in pcapng files and presenting the information through webpage.\n"
		<< "\n"
		<< "    tcpstats FILE\n"
		<< "\n"
		<< "FILE                         Input file in pcapng format."
		<< std::endl;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printHelp();
		return 1;
	}

	REGISTER_ANALYSIS(PacketExchangeAnalysis);

	try
	{
		auto pcapFile = PcapngFile::createFromPath(std::string{argv[1]});
		auto pcapParser = std::make_unique<PcapngParser>(std::move(pcapFile));
		auto stream = pcapParser->parse();

		auto json = AnalysisManager::instance().runAll(*stream.get());

		Json::StyledWriter writer;
		std::cout << writer.write(json) << std::endl;
	}
	catch (const BaseException& ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
