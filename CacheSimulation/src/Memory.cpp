//
//  Memory.cpp
//  CacheSimulation
//
//  Created by Rashmi Jadhav on 6/1/21.
//

#include "Memory.h"


void Memory::setNext(Memory* nextMemory) {
	this->nextLayer = nextMemory;
}

void Memory::setName(std::string layerName) {
	this->layerName = layerName;
}

unsigned int Memory::getGlobalCycles() const {
	return this->globalCycles;
}

unsigned int Memory::getCurrentCycles() const {
	return this->currentCycles;
}
