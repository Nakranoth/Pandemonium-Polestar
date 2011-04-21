#ifndef _MAP_
#define _MAP_

#include <set>

#include "Tile.h"

class Map
{
	public:
		Tile* ref; //this is the reference tile for this area
		short wall;
	private:
		Tile** area;	//Mem space for tiles to be generated into. MUST BE DELETED.
		int x, y, xsiz, ysiz;
		short floor;
		short flags;
		
		//Queue<Tile> border; //a list of tiles that make up the border for this map's area

	public:
		Map(int xloc, int yloc, int xsiz, int ysiz, short floor = Tile::GRASS, short wall = Tile::UDEF, short flags = 0);
		~Map();
		bool populate();
		void delChunk(Tile* ref);
		void unstitch(Tile* ref, int width, int height);
};

#endif
