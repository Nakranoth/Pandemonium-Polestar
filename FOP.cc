#include "FOP.h"

#include <iostream>

FOP::FOP()
{
	fitted = false;
	isSolid = 1;
	width = 0;
	length = 0;
	x = 0;
	y = 0;
}

FOP::~FOP()
{
}

int FOP::getSize()
{
	return width*length;
}


FOP* FOP::Sofa()
{
	width = Tile::SIZE*2 + 10;
	length = Tile::SIZE + 20;
	actions.push_back(Action::SIT);
	return this;
}

FOP* FOP::Refrigerator()
{
	width = Tile::SIZE;
	length = Tile::SIZE;
	actions.push_back(Action::OPEN);
	return this;
}

FOP* FOP::Toilet()
{
	width = Tile::SIZE;
	length = Tile::SIZE;
	actions.push_back(Action::SIT);
	return this;
}

FOP* FOP::Bed()
{
	width = Tile::SIZE*2;
	length = Tile::SIZE;
	actions.push_back(Action::SIT);
	actions.push_back(Action::SLEEP);
	return this;
}

FOP* FOP::Cat()
{
	width = Tile::SIZE;
	length = Tile::SIZE;
	actions.push_back(Action::PET);
	return this;
}

FOP* FOP::Character(int x, int y, Tile* ref)
{
	this->x = x;
	this->y = y;
	width = Tile::SIZE;
	length = Tile::SIZE;
	location = ref;

	while(location->x < x/Tile::SIZE)
	{
		location = location->east;
	}
	while(location->y < y/Tile::SIZE)
	{
		location = location->south;
	}

	return this;
}
