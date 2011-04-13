#ifndef _MAP_
#define _MAP_

#include "Tile.h"

class Map
{
	public:
		Tile* ref; //this is the reference tile for this area
	private:
		Tile** area;		//Reference pointer to telements.
		Tile* telements;	//Mem space for tiles to be generated into. Should make life easier.
		//Queue<Tile> border; //a list of tiles that make up the border for this map's area

	public:
		Map(int xloc, int yloc, int xsiz, int ysiz, short floor = Tile::GRASS, short wall = Tile::UDEF, short flags = 0);
		~Map();
		bool populate();
};

#endif
