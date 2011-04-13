#include "ZBRA.h"

#include <iostream>

ZBRA::ZBRA()
{
}

ZBRA::~ZBRA()
{
	delete map;
}

bool ZBRA::tryDim(int x, int y)
{
	if (x*y < minSize) return false;
	map = new Map(0,0,x,y,floor,wall);
	if (map->populate()) return true;
	else delete map;
	return false;
}

int ZBRA::getIdealSize()
{
	int size = 0;
	for (vector<ZBRA*>::iterator i = subArea.begin(); i != subArea.end(); i++)
		size += (*i)->idealSize;
	return size;
}

int ZBRA::getMinSize()
{
	int size = 0;
	for (vector<ZBRA*>::iterator i = subArea.begin(); i != subArea.end(); i++)
		size += (*i)->minSize;
	return size;
}

Tile* ZBRA::getRefTile()
{
	return map->ref;
}

ZBRA* ZBRA::BathRoom()
{
	shallow = false;
	minSize = 20;
	idealSize = 30;
	floor = Tile::FLOOR;
	wall = Tile::WALL;
	return this;
}

ZBRA* ZBRA::City()
{
	shallow = false;
	floor = Tile::GRASS;
	wall = Tile::UDEF;
	subArea.push_back((new ZBRA)->House()); //only one test house for now
	minSize = 25 + getMinSize();
	idealSize = 35 + getIdealSize();
	cout << minSize << "->" << idealSize << endl;
	return this;
}
		
ZBRA* ZBRA::House()
{
	shallow = false;
	floor = Tile::FLOOR;
	wall = Tile::WALL;
	//for now every house will contain 1 bathroom kitchen and living room and 2 bedrooms
	subArea.push_back((new ZBRA)->BathRoom());
	subArea.push_back((new ZBRA)->Kitchen());
	subArea.push_back((new ZBRA)->LivingRoom());
	subArea.push_back((new ZBRA)->BedRoom());
	subArea.push_back((new ZBRA)->BedRoom());
	minSize = 20 + getMinSize();
	idealSize = 30 + getIdealSize();
	return this;
}

ZBRA* ZBRA:: Kitchen()
{
	shallow = false;
	floor = Tile::FLOOR;
	wall = Tile::WALL;
	minSize = 25;
	idealSize = 40;
}

ZBRA* ZBRA:: LivingRoom()
{
	shallow = false;
	floor = Tile::FLOOR;
	wall = Tile::WALL;
	minSize = 30;
	idealSize = 45;
}

ZBRA* ZBRA:: BedRoom()
{
	shallow = false;
	floor = Tile::FLOOR;
	wall = Tile::WALL;
	minSize = 20;
	idealSize = 35;
}
