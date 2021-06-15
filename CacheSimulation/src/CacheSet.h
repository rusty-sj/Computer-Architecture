//
//  CacheSet.hpp
//  CacheSimulation
//
//  Created by Rashmi Jadhav on 6/1/21.
//

#ifndef _CACHESET_H_
#define _CACHESET_H_

#include "CacheBlock.h"
#include "CacheStuff.h"

#include <cstdint>
#include <random>
#include <list>

class CacheSet {
	private:
		unsigned int associativity;
		std::list<CacheBlock> blocksInSet;
	
	public:
		struct CacheResponse {
			bool eviction;
			bool dirtyEviction;
			uint_fast64_t evictedTag;
		};
	
		CacheSet(unsigned int);
	
		bool checkForTag(uint_fast64_t tag);
		
		void access(CacheSet::CacheResponse& response, bool isWrite, ReplacementPolicy rp, uint_fast64_t tag);
	
};

#endif /* CACHESETH */
