/*
	Cache Simulator (Starter Code) by Justin Goins
	Oregon State University
	Spring Term 2021
*/

#ifndef _CACHECONTROLLER_H_
#define _CACHECONTROLLER_H_

#include "CacheStuff.h"
#include "Cache.h"
#include "Memory.h"
#include "RAM.h"

#include <string>
#include <fstream>
#include <vector>
#include <memory>

class CacheController {
	private:
		std::vector<Cache> caches;	// Raw cache structures
		std::vector<Memory*> layers;
		RAM ram;					// Lowest level system RAM
	
		std::string inputFile, outputFile;
	
		Memory* L1 = nullptr;
	
		unsigned int cpuReads = 0;
		unsigned int cpuWrites = 0;
	
		// Add entry to output file
		void processEntry(std::ofstream& outfile, bool isWrite, uint_fast64_t address, unsigned long int nBytes);
		

	public:
		CacheController(const std::vector<CacheInfo>&, std::string);
		void runTracefile();
		void printFooter(std::ofstream& outfile);
};

#endif //CACHECONTROLLER
