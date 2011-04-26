#ifndef _MAP_
#define _MAP_

#include <set>
#include <assert.h>

#include "Tile.h"


struct bounds;

class Map
{
	public:
		Tile* ref; //this is the reference tile for this area
		short wall;
		Tile** area;	//Mem space for tiles to be generated into. MUST BE DELETED.
	private:
		int x, y, xsiz, ysiz;
		short floor;
		short flags;
		
		//Queue<Tile> border; //a list of tiles that make up the border for this map's area

	public:
		Map(int xloc, int yloc, int xsiz, int ysiz, short floor = Tile::GRASS, short wall = Tile::UDEF, short flags = 0);
		~Map();
		void populate();
		static void checkConsistency(Tile* ref);
		static void delChunk(Tile* ref);
		static void unstitch(Tile* ref, int width, int height);
		static void stitch(Tile* pRef, Tile* cRef, int wall, int orient);	//stiches together a parent and child, while
		
	private:
		static void runEast(Tile* inRunner, Tile* outRunner, int wall, int orient, bool first);
		static void runSouth(Tile* inRunner, Tile* outRunner, int wall, int orient, bool first);
		static void runWest(Tile* inRunner, Tile* outRunner, int wall, int orient, bool first);
		static void runNorth(Tile* inRunner, Tile* outRunner, int wall, int orient, bool first);
};

#endif
