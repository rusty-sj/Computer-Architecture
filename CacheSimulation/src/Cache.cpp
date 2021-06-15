//
//  Cache.cpp
//  CacheSimulation
//
//  Created by Rashmi Jadhav on 6/1/21.
//

#include "Cache.h"
#include "CacheStuff.h"
#include "Memory.h"
#include "CacheSet.h"

#include <iostream>
#include <cmath>

// Constructor
Cache::Cache(const CacheInfo& ci): ci(ci) {
	this->numByteOffsetBits = log2(ci.blockSize);
	this->numSetIndexBits = log2(ci.numberSets);
	
	//create the cache structure
	this->cacheSets.reserve(ci.numberSets);
	for(size_t i = 0; i < ci.numberSets; ++i) {
		this->cacheSets.emplace_back(ci.associativity);
	}
}

void Cache::accessBlock(bool isWrite, uint_fast64_t byteAddress) {
	unsigned int setIndex = computeSet(byteAddress);
	uint_fast64_t tag = computeTag(byteAddress);
	
	// Default access time
	currentCycles += ci.cacheAccessCycles;
	
	CacheSet::CacheResponse response;
	if(!isWrite) {
		if (cacheSets[setIndex].checkForTag(tag)) {
			// Read hit
			currentHits++;
			cacheSets[setIndex].access(response, false, ci.rp, tag);
		} else {
			// Read Miss
			currentMisses++;
			getBlockFromNextMemory(byteAddress);
		}
	} else {
		if (cacheSets[setIndex].checkForTag(tag)) {
			// Write Hit
			currentHits++;
		} else {
			// Write Miss
			currentMisses++;
			getBlockFromNextMemory(byteAddress);
			// Extra penalty for miss
			currentCycles += ci.cacheAccessCycles;
		}
		// Regardless of a hit or miss, update the block to maintain LRU
		cacheSets[setIndex].access(response, true, ci.rp, computeTag(byteAddress));
		if (ci.wp == WritePolicy::WriteThrough){
			nextLayer->access(true, byteAddress, ci.blockSize);
		}
	}
}

void Cache::access(bool isWrite, uint_fast64_t byteAddress, unsigned long numBytes) {
	uint_fast64_t startBlock = byteAddress / ci.blockSize;					// First affected block address
	uint_fast64_t endBlock = (byteAddress + numBytes - 1) / ci.blockSize;	// Last affected block address
	unsigned int nBlocks = (int) (endBlock - startBlock + 1);						// number of affected blocks
	
	// Access each affected block from start to end
	uint_fast64_t requestedBlock = startBlock;
	for (unsigned int i = 0; i < nBlocks; i++) {
		accessBlock(isWrite, requestedBlock * ci.blockSize);
		requestedBlock++;
	}
	
	std::cout<< "\t" << layerName << " " << (isWrite ? "wrote" : "read") << " " << numBytes << " bytes at " << byteAddress << std::endl;
}

void Cache::getBlockFromNextMemory(uint_fast64_t byteAddress) {
	
	// Ask a lower layer
	unsigned int setIndex = computeSet(byteAddress);
	nextLayer->access(false, byteAddress, ci.blockSize);
	
	CacheSet::CacheResponse response;
	cacheSets[setIndex].access(response, false, ci.rp, computeTag(byteAddress));
	
	// was there an eviction?
	if (response.eviction) {
		// no penalty for evictions in write-through cache
		currentEvictions++;
		if (ci.wp == WritePolicy::WriteBack && response.dirtyEviction) {
			// Determine byte address of the block that was evicted
			uint_fast64_t evictedAddress = ((response.evictedTag << numSetIndexBits) | setIndex) << numByteOffsetBits;
			std::cout << "\tWB cache is evicting dirty block with tag: " << response.evictedTag << " and byte address: " << evictedAddress << std::endl;
			nextLayer->access(true, evictedAddress, ci.blockSize);
		}
	}
}


void Cache::resetStats() {
	this->currentCycles = 0;
	this->currentHits = 0;
	this->currentMisses = 0;
	this->currentEvictions = 0;
}

void Cache::getStats(Cache::CacheResponse& cr) const {
	cr.currentHits = currentHits;
	cr.currentMisses = currentMisses;
	cr.currentEvictions = currentEvictions;
	cr.globalHits = globalHits;
	cr.globalMisses = globalMisses;
	cr.globalEvictions = globalEvictions;
}

std::string Cache::getName() const {
	return this->layerName;
}

void Cache::opFinished() {
	globalCycles += currentCycles;
	globalHits += currentHits;
	globalMisses += currentMisses;
	globalEvictions += currentEvictions;
}

inline uint_fast64_t Cache::computeTag(uint_fast64_t byteAddress) const {
	return byteAddress >> (numSetIndexBits + numByteOffsetBits);
}

inline unsigned int Cache::computeSet(uint_fast64_t byteAddress) const {
	return (byteAddress / ci.blockSize) % ci.numberSets;
}

