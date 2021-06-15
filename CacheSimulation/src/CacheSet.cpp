//
//  CacheSet.cpp
//  CacheSimulation
//
//  Created by Rashmi Jadhav on 6/1/21.
//

#include "CacheSet.h"
#include "CacheStuff.h"

#include <iostream>

CacheSet::CacheSet(unsigned int associativity) {
	this->associativity = associativity;
	CacheBlock block;

	for(size_t i = 0; i < associativity; ++i) {
		blocksInSet.push_back(block);
	}
}

bool CacheSet::checkForTag(uint_fast64_t tag) {
	for (std::list<CacheBlock>::iterator it = blocksInSet.begin(); it != blocksInSet.end(); it++){
		if (it->isValid && it->tag == tag) {		// Tag already exists, return true
			return true;
		}
	}
	return false;									// Tag not found
}

void CacheSet::access(CacheSet::CacheResponse& response, bool isWrite, ReplacementPolicy rp, uint_fast64_t tag) {
	std::list<CacheBlock>::iterator it;
	// CacheSet's responses
	response.eviction = false;
	response.dirtyEviction = false;
	response.evictedTag = 0;
	
	// Create block to be written into the set
	CacheBlock block;
	block.isValid = true;
	block.isDirty = false;
	block.tag = tag;
	
	// Keeps track of whether all blocks in the set are valid or not so that eviction can be handled
	bool allValid = true;
	
	// Keep the location of invalid block in the set
	std::list<CacheBlock>::iterator invalidBlockPtr;
	
	// Check if block was already present, if yes, bring to recently accessed position and return because it was a hit
	for (it = blocksInSet.begin(); it != blocksInSet.end(); it++) {
		if (it->isValid) {
			if (it->tag == tag) {
				blocksInSet.erase(it);
				blocksInSet.push_back(block);
				return;
			}
		} else {
			// Found a block that is garbage and can be replaced by the new block
			allValid = false;
			invalidBlockPtr = it;
		}
	}
	
	// For write-through cache, the block is marked dirty as it is not written to next memory yet
	//if (isWrite) {
	block.isDirty = true;
	//}
	
	if (rp == ReplacementPolicy::LRU) {
		// All blocks are in set are valid, have to evict
		if (allValid) {
			response.eviction = true;
			response.evictedTag = blocksInSet.front().tag;
			if (blocksInSet.front().isDirty)					// Set dirty eviction if popped block is Dirty
				response.dirtyEviction = true;
		}
		// Remove last accessed from front, add most recent at the back
		blocksInSet.pop_front();
		blocksInSet.push_back(block);
		
	} else  if (rp == ReplacementPolicy::Random) {
		it = blocksInSet.begin();
		
		// All blocks are in set are valid, have to evict from a random index
		if (allValid) {
			unsigned int evictIndex = rand() % this->associativity;
			std::advance(it, evictIndex);						// Go to the random index
			response.eviction = true;
			response.evictedTag = it->tag;
			if (it->isDirty)									// Set dirty eviction if popped block is Dirty
				response.dirtyEviction = true;
			(*it) = block;										// Replace the block at location
		} else {
			// use the already found invalid block and replace its contents
			(*invalidBlockPtr) = block;							// Replace the block at location
		}
	}
}
