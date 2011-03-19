#ifndef _ZBRA_
#define _ZBRA_

#include <vector>

#include "Map.h"

using namespace std;

class ZBRA
{
	public:
		bool shallow; //a flag to determine whether this counts for depth traversals
		vector<Tile> tiles; //tiles the ZBRA contains
		vector<int> fops; //the FOPs of the ZBRA
	public:
		ZBRA();
};

#endif

