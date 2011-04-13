#include "Map.h"

Map::Map(int xloc, int yloc, int xsiz, int ysiz, short floor, short wall, short flags)
{
	//ref = new Tile(xloc,yloc);	//Must be one of the ones in area
	telements = new Tile[xsiz*ysiz];
	area = &telements;
	ref = &area[0][0];
}

Map::~Map()
{
	delete [] telements;
}

bool Map::populate()
{
	return true;
}
