//
//  CacheBlock.h
//  CacheSimulation
//
//  Created by Rashmi Jadhav on 6/1/21.
//

#ifndef _CACHEBLOCK_H_
#define _CACHEBLOCK_H_

#include <cstdint>

struct CacheBlock {
	bool isValid = false;
	bool isDirty = false;
	uint_fast64_t tag = 0;
};

#endif /* CACHEBLOCK */
