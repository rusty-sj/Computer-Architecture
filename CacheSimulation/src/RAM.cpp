//
//  RAM.cpp
//  CacheSimulation
//
//  Created by Rashmi Jadhav on 6/1/21.
//

#include "RAM.h"
#include "Memory.h"

#include <iostream>

// Constructor
RAM::RAM() {
	this->layerName = "RAM";
}

void RAM::resetStats(){
	this->currentCycles = 0;
}

void RAM::opFinished(){
	this->globalCycles += this->currentCycles;
}

void RAM::setAccessCycles(unsigned int accessCycles, unsigned int busWidth, unsigned int prefetechCycles) {
	this->accessCycles = accessCycles;
	this->busWidth = busWidth;
	this->prefetchCycles = prefetechCycles;
}

// info for a read or write operation for RAM specifically
void RAM::access(bool isWrite, uint_fast64_t byteAddress, unsigned long numBytes){
	uint_fast64_t seekPosition = byteAddress;
	uint_fast64_t finalAddress = byteAddress + numBytes - 1; // Get the last address involved in the current operation
	
	if (seekPosition == this->prefetchAddr) { // If reached max
		this->currentCycles += this->prefetchCycles;
	} else {
		this->currentCycles += this->accessCycles;
	}
	unsigned int numBusAccesses = 0;
	do {
		seekPosition += this->busWidth;
		numBusAccesses++;
	} while(seekPosition <= finalAddress);
	std::cout << "\tRAM handled " << (isWrite ? "write" : "read") << " request for " << numBytes << " bytes at " << byteAddress <<" using "<< numBusAccesses << " bus access(es)!" << std::endl;
	this->currentCycles += ((numBusAccesses - 1) * this->prefetchCycles);
	this->prefetchAddr = seekPosition;
}
