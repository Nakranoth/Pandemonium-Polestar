#ifndef _TILE_
#define _TILE_

#include <SDL/SDL.h>
#include "FOP.h"

class Tile
{
	public:
		static const int SIZE = 30;
		static const short int UDEF = 0, WALL = 1, FLOOR = 2, ROAD = 3, GRASS = 4, DOOR = 5, KITCHEN = 6, BATHROOM = 7, BEDROOM = 8, HOUSE = 9;

		int x;
		int y;
		Uint8 type;
		bool solid;
		Tile* north;
		Tile* south;
		Tile* east;
		Tile* west;
		vector<FOP*> fops;	//the FOPs of the tile
	
	private:

	public: 
		Tile(int x, int y, Uint8 type, bool solid = false);
		Tile();
		~Tile();
};


#endif
