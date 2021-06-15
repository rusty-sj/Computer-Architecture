/*
	Cache Simulator (Starter Code) by Justin Goins
	Oregon State University
	Spring Term 2021
*/

#include "CacheController.h"
#include "Cache.h"
#include "Memory.h"
#include "RAM.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <cmath>

using namespace std;

CacheController::CacheController(const std::vector<CacheInfo>& cis, string tracefile) {
	unsigned long int numCacheLevels = cis.size();
	
	// store the configuration info
	this->inputFile = tracefile;
	this->outputFile = this->inputFile + ".out";
	
	// Create RAM type of Memory object
	ram = RAM();
	ram.setAccessCycles(cis[0].memoryAccessCycles);
	
	// create your cache structure
	caches.reserve(numCacheLevels);
	layers.reserve(numCacheLevels + 1);	    // One extra for caches followed by RAM
	
	for(unsigned int i = 0; i < numCacheLevels; i++) {
		caches.emplace_back(cis[i]);	    // Create cache object and place in caches
		caches.back().setName("L" + std::to_string(i + 1));
		layers.push_back(&caches.back());	// Put address to created cache in memory pointers vector
	}
	layers.emplace_back(&ram);				// Add RAM object to memory vector
	
	auto iterator = layers.cbegin();
	Memory* memory = *iterator;
	iterator++;
	
	L1 = memory;	// Store first cache in L1 for a fast access later
	
	// Manage pointers in memory layers; very important!!
	while (iterator != layers.cend()) {
		memory->setNext(*iterator);
		memory = (*iterator);
		iterator++;
	}
}

void CacheController::processEntry(std::ofstream &outfile, bool isWrite, uint_fast64_t address, unsigned long nBytes) {
	// Reset stats to 0 for each memory level
	for (auto iterator = layers.cbegin(); iterator != layers.cend(); iterator++) {
		(*iterator)->resetStats();
	}
	
	// Number of reads and writes by the CPU
	if (isWrite) {
		cpuWrites++;
	} else {
		cpuReads++;
	}
	
	// Process the operation
	L1->access(isWrite, address, nBytes);
	
	// Inform each layer that the task is finished and report the utilized clock cycles
	unsigned int nCycles = 0;
	for (auto iterator = layers.cbegin(); iterator != layers.cend(); iterator++) {
		(*iterator)->opFinished();
		nCycles += (*iterator)->getCurrentCycles();
	}
	outfile << " " << nCycles;
	
	for (auto iterator = caches.cbegin(); iterator != caches.cend(); iterator++) {
		Cache::CacheResponse cr;
		iterator->getStats(cr);
		if (cr.currentHits || cr.currentMisses || cr.currentEvictions) {
			outfile << " " << iterator->getName();
			if (cr.currentMisses) {
				outfile << " miss";
			}
			if (cr.currentHits) {
				outfile << " hit";
			}
			if (cr.currentEvictions) {
				outfile << " eviction";
			}
		}
		
		if (cr.currentHits > 0) {
			cout << "\t" << iterator->getName() << " Operation at address 0x" << std::hex << address << std::dec << " caused " << cr.currentHits << " hit(s)." << endl;
		}
		if (cr.currentMisses > 0) {
			cout << "\t" << iterator->getName() << " Operation at address 0x" << std::hex << address << std::dec << " caused " << cr.currentMisses << " miss(es)." << endl;
		}
		if (cr.currentEvictions > 0) {
			cout << "\t" << iterator->getName() << " Operation at address 0x" << std::hex << address << std::dec << " caused " << cr.currentEvictions << " eviction(s)." << endl;
		}
	}
	
}

/*
	Starts reading the tracefile and processing memory operations.
*/
void CacheController::runTracefile() {
	cout << "Input tracefile: " << inputFile << endl;
	cout << "Output file name: " << outputFile << endl;
	
	// process each input line
	string line;
	// define regular expressions that are used to locate commands
	regex commentPattern("==.*");
	regex instructionPattern("I .*");
	regex loadPattern(" (L )(.*)(,)([[:digit:]]+)$");
	regex storePattern(" (S )(.*)(,)([[:digit:]]+)$");
	regex modifyPattern(" (M )(.*)(,)([[:digit:]]+)$");

	// open the output file
	ofstream outfile(outputFile);
	// open the output file
	ifstream infile(inputFile);
	// parse each line of the file and look for commands
	while (getline(infile, line)) {
		// these strings will be used in the file output
		string opString, activityString;
		smatch match; // will eventually hold the hexadecimal address string
		uint_fast64_t address;

		// ignore comments
		if (std::regex_match(line, commentPattern) || std::regex_match(line, instructionPattern)) {
			// skip over comments and CPU instructions
			continue;
		} else if (std::regex_match(line, match, loadPattern)) {
			cout << "Found a load op!" << endl;
			istringstream hexStream(match.str(2));
			hexStream >> std::hex >> address;
			outfile << match.str(1) << match.str(2) << match.str(3) << match.str(4);
			processEntry(outfile, false, address, stoul(match.str(4)));
		} else if (std::regex_match(line, match, storePattern)) {
			cout << "Found a store op!" << endl;
			istringstream hexStream(match.str(2));
			hexStream >> std::hex >> address;
			outfile << match.str(1) << match.str(2) << match.str(3) << match.str(4);
			processEntry(outfile, true, address, stoul(match.str(4)));
		} else if (std::regex_match(line, match, modifyPattern)) {
			cout << "Found a modify op!" << endl;
			istringstream hexStream(match.str(2));
			// first process the read operation
			hexStream >> std::hex >> address;
			outfile << match.str(1) << match.str(2) << match.str(3) << match.str(4);
			processEntry(outfile, false, address, stoul(match.str(4)));
			outfile << endl;
			// now process the write operation
			hexStream >> std::hex >> address;
			outfile << match.str(1) << match.str(2) << match.str(3) << match.str(4);
			processEntry(outfile, true, address, stoul(match.str(4)));
		} else {
			throw runtime_error("Encountered unknown line format in tracefile.");
		}
		outfile << endl;
	}
	printFooter(outfile);
	
	infile.close();
	outfile.close();
}

void CacheController::printFooter(std::ofstream &outfile) {
	for (auto iterator = caches.cbegin(); iterator != caches.cend(); iterator++) {
		Cache::CacheResponse cr;
		iterator->getStats(cr);
		outfile << iterator->getName() << " Cache: ";
		outfile << " Hits:" << cr.globalHits << " Misses:" << cr.globalMisses << " Evictions:" << cr.globalEvictions << endl;
	}
	
	unsigned int totalCycles = 0;
	for (auto iterator = layers.cbegin(); iterator != layers.cend(); iterator++) {
		totalCycles += (*iterator)->getGlobalCycles();
	}
	outfile << "Cycles:" << totalCycles << " Reads:" << cpuReads << " Writes:" << cpuWrites << endl;
}

