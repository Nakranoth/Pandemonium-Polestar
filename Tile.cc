#include "Tile.h"

Tile::Tile(int _x, int _y, Uint8 _type, bool _solid) :x(_x),y(_y),type(_type),solid(_solid)
{
	north = NULL;
	south = NULL;
	east = NULL;
	west = NULL;
}

Tile::Tile()
{
	north = NULL;
	south = NULL;
	east = NULL;
	west = NULL;
	x = y = type = solid = 0;
}

Tile::~Tile()
{
	if (north) north->south = NULL;
	if (south) south->north = NULL;
	if (east) east->west = NULL;
	if (west) west->east = NULL;
	while (!fops.empty()){
		if ((*fops.begin())->location == this) {
			((*fops.begin())->location == NULL);
		}
		fops.erase(fops.begin());
	}
}
