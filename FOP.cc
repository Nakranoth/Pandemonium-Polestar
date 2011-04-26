#include "FOP.h"

#include <iostream>

FOP::FOP()
{
	bool fitted = false;
	int isSolid = 1;
	//vector<int> actions;	//holds the actions that can be performed on this FOP
	int width = 0;
	int length = 0;
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
	//actions.push_back((new Action)->Sit());
	return this;
}

FOP* FOP::Refrigerator()
{
	width = Tile::SIZE;
	length = Tile::SIZE;
	//actions.push_back((new Action)->Open());
	return this;
}

FOP* FOP::Toilet()
{
	width = Tile::SIZE;
	length = Tile::SIZE;
	//actions.push_back((new Action)->Sit());
	return this;
}

FOP* FOP::Bed()
{
	width = Tile::SIZE*2;
	length = Tile::SIZE;
	//actions.push_back((new Action)->Sit());
	//actions.push_back((new Action)->Sit());
	return this;
}
