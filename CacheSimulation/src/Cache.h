//
//  Cache.h
//  CacheSimulation
//
//  Created by Rashmi Jadhav on 6/1/21.
//

#ifndef _CACHE_H_
#define _CACHE_H_

#include "Memory.h"
#include "CacheStuff.h"
#include "CacheSet.h"
#include <cstdint>
#include <vector>
#include <limits>

class Cache : public Memory {
	private:
		CacheInfo ci;
		unsigned int numByteOffsetBits;
		unsigned int numSetIndexBits;
			
		// Counters for Cache
		unsigned int globalHits = 0;
		unsigned int globalMisses = 0;
		unsigned int globalEvictions = 0;
		
		// Counters for current operation
		unsigned int currentHits;
		unsigned int currentMisses;
		unsigned int currentEvictions;
	
		std::vector<CacheSet> cacheSets;
	
		void accessBlock(bool, uint_fast64_t);
		void getBlockFromNextMemory(uint_fast64_t);
	
		uint_fast64_t computeTag(uint_fast64_t byteAddress) const;
		unsigned int computeSet(uint_fast64_t byteAddress) const;
	
	public:
		// this structure can filled with information about each memory operation
		struct CacheResponse {
			unsigned int globalHits;
			unsigned int globalMisses;
			unsigned int globalEvictions;
			unsigned int currentHits;
			unsigned int currentMisses;
			unsigned int currentEvictions;
		};
		
		Cache(const CacheInfo&);
	
		void resetStats() override;
		void opFinished() override;
	
		std::string getName() const;
		void getStats(Cache::CacheResponse& cr) const;
		void access(bool isWrite, uint_fast64_t address, unsigned long numBytes) override;
};

#endif /* CACHEH */
