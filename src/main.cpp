#include <iostream>

#include "parser/pcapng_parser.h"
#include "analysis/all_analyses.h"
#include "analysis/analysis_manager.h"
#include "presentation/all_presenters.h"
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

	REGISTER_ANALYSIS(OverallInfoAnalysis);
	REGISTER_ANALYSIS(RttAnalysis);
	REGISTER_ANALYSIS(SpeedAnalysis);
	REGISTER_ANALYSIS(WindowSizeAnalysis);

	try
	{
		auto pcapFile = PcapngFile::createFromPath(std::string{argv[1]});

		auto pcapParser = PcapngParser(std::move(pcapFile));
		auto stream = pcapParser.parse();

		AnalysisManager::instance().runAll(*stream.get());

		WebPresenter webPresenter;
		AnalysisManager::instance().visitAll(webPresenter);
		webPresenter.present("xmilko01.json");
	}
	catch (const BaseException& ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
