#include "FOP.h"

#include <iostream>

FOP::FOP()
{
	bool fitted = false;
	int isSolid = 1;
	int width = 0;
	int length = 0;
	int x = 0;
	int y = 0;
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
