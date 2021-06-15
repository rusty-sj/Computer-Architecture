//
//  Memory.hpp
//  CacheSimulation
//
//  Created by Rashmi Jadhav on 6/1/21.
//

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <string>

class Memory {
	private:
	
	protected:
		Memory* nextLayer = nullptr; 	// Points to lower layer in hierarchy
		
		// Modified by all memories in hierarchy
		unsigned int globalCycles = 0;
		unsigned int currentCycles;
		std::string layerName;
	
	public:
		void setNext(Memory*);				// Specify next layer in the memory hierarchy
		virtual void access(bool isWrite, uint_fast64_t address, unsigned long numBytes) = 0;
		virtual void resetStats() = 0;		// Reset counter per operation
		virtual void opFinished() = 0;		// Time to update global counters
	
		void setName(std::string);
		unsigned int getGlobalCycles() const;
		unsigned int getCurrentCycles() const;
};

#endif /* MEMORY */
