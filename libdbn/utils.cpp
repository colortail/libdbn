#include "utils.h"

namespace libdbn {

uint32_t hashCode(std::string & str) {
	uint32_t seed = 31;
	uint32_t hash = 0;
	for (std::string::iterator p = str.begin();
		p != str.end();
		p++)
		hash = hash * seed + (*p);
	return (hash & 0x7fffffff);
}


}