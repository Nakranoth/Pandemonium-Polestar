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
		//set<Tile*> edges;	//List of tiles that border another.
	private:
		int x, y, xsiz, ysiz;
		short floor;
		short flags;
		
		//Queue<Tile> border; //a list of tiles that make up the border for this map's area

	public:
		Map(int xloc, int yloc, int xsiz, int ysiz, short floor = Tile::GRASS, short wall = Tile::UDEF, short flags = 0);
		~Map();
		void populate();
		void checkConsistency(Tile* ref);
		static void stitch(Tile* pRef, Tile* cRef, int wall, Map* cMap);	//stiches together a parent and child
		
	private:
		static void cloneRow(Tile* pRef, Tile* cRef, bool east);//east or west
		static void cloneTile(Tile* pRef, Tile* cRef);//east or west
};

#endif
