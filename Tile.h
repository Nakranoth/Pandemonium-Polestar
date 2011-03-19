#ifndef _TILE_
#define _TILE_

#include <SDL/SDL.h>

class Tile
{
	public:
		static const int SIZE = 5;
	private:
		int x;
		int y;
		Uint8 sprite;
		bool solid;
		Tile* north;
		Tile* south;
		Tile* east;
		Tile* west;

	public: Tile(int x, int y, Uint8 sprite, bool solid = false);
};


#endif
