#ifndef _MAP_
#define _MAP_

#include "Tile.h"

class Map
{
	public:
		Tile* ref; //this is the reference tile for this area
	private:
		//Queue<Tile> border; //a list of tiles that make up the border for this map's area

	public: Map();
	//public !Map();
};

#endif
