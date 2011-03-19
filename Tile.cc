#include "Tile.h"

Tile::Tile(int _x, int _y, Uint8 _sprite, bool _solid) :x(_x),y(_y),sprite(_sprite),solid(_solid)
{
	north = NULL;
	south = NULL;
	east = NULL;
	west = NULL;
}
