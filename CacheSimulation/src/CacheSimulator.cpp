/*
	Cache Simulator (Starter Code) by Justin Goins
	Oregon State University
	Spring Term 2021
*/

#include "CacheSimulator.h"
#include "CacheStuff.h"
#include "CacheController.h"

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

/*
	This function creates the cache and starts the simulator.
	Accepts core ID number, configuration info, and the name of the tracefile to read.
*/
void initializeCache(unsigned int id, std::vector<CacheInfo> config, string tracefile) {
	CacheController singlecore = CacheController(config, tracefile);
	singlecore.runTracefile();
}

/*
	This function accepts a configuration file and a trace file on the command line.
	The code then initializes a cache simulator and reads the requested trace file(s).
*/
int main(int argc, char* argv[]) {
	if (argc < 3) {
		cerr << "You need two command line arguments. You should provide a configuration file and a trace file." << endl;
		return 1;
	}
	
	srand(static_cast<unsigned int>(time(NULL)));	// Set a random seed used for ReplacementPolicy::Random

	CacheInfo cacheInfo;
	
	// determine how many cache levels the system is using
	unsigned int numCacheLevels;

	// read the configuration file
	cout << "Reading config file: " << argv[1] << endl;
	ifstream infile(argv[1]);
	unsigned int tmp;
	unsigned int memoryAccessCycles;
	infile >> numCacheLevels;
	infile >> memoryAccessCycles;
	
	std::vector<CacheInfo> config;
	config.reserve(numCacheLevels);
	
	
	for (unsigned int i = 0; i < numCacheLevels; i++) {
		cacheInfo.memoryAccessCycles = memoryAccessCycles;
		infile >> cacheInfo.numberSets;
		infile >> cacheInfo.blockSize;
		infile >> cacheInfo.associativity;
		infile >> tmp;
		cacheInfo.rp = static_cast<ReplacementPolicy>(tmp);
		infile >> tmp;
		cacheInfo.wp = static_cast<WritePolicy>(tmp);
		infile >> cacheInfo.cacheAccessCycles;
		
		config.push_back(cacheInfo);
	}
	
	infile.close();
	
	// Examples of how you can access the configuration file information
	cout << "System has " << numCacheLevels << " cache(s)." << endl;
	
	for (unsigned int i = 0; i < numCacheLevels; i++) {
		cout << config[i].numberSets << " sets with " << config[i].blockSize << " bytes in each block. N = " << config[i].associativity << endl;

		if (config[i].rp == ReplacementPolicy::Random)
			cout << "Using random replacement protocol" << endl;
		else
			cout << "Using LRU protocol" << endl;
		
		if (config[i].wp == WritePolicy::WriteThrough)
			cout << "Using write-through policy" << endl;
		else
			cout << "Using write-back policy" << endl;
	}

	// start the cache operation...
	string tracefile(argv[2]);
	initializeCache(0, config, tracefile);

	return 0;
}
