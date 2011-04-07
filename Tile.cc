#include "Tile.h"

Tile::Tile(int _x, int _y, Uint8 _sprite, bool _solid) :x(_x),y(_y),sprite(_sprite),solid(_solid){
	north = NULL;
	south = NULL;
	east = NULL;
	west = NULL;
}

Tile::Tile(){
	north = NULL;
	south = NULL;
	east = NULL;
	west = NULL;
	x = y = sprite = solid = 0;
}

Tile::~Tile(){
	if (north) north->south = NULL;
	if (south) south->north = NULL;
	if (east) east->west = NULL;
	if (west) west->east = NULL;
}
