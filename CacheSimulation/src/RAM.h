//
//  RAM.hpp
//  CacheSimulation
//
//  Created by Rashmi Jadhav on 6/1/21.
//

#ifndef _RAM_H_
#define _RAM_H_

#include "CacheStuff.h"
#include "Memory.h"
#include "CacheSet.h"

#include <cstdint>
#include <limits>

class RAM : public Memory {
	private:
		unsigned int accessCycles;						// # cycles for first bus access
		unsigned int busWidth;							// # cycles per bus access
		unsigned int prefetchCycles;					// # cycles required for each contiguous bus transfer
		uint_fast64_t prefetchAddr = UINT_FAST64_MAX;	// Track currently prefetched location
		
	public:
		RAM();
		void resetStats() override;
		void opFinished() override;
		void access(bool isWrite, uint_fast64_t address, unsigned long nBytes) override;
		void setAccessCycles(unsigned int accessCycles, unsigned int busWidth = 8, unsigned int prefetechCycles = 1);
};

#endif /* RAMH */
