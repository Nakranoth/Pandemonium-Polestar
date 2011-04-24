#include "ZBRA.h"

#include <iostream>

ZBRA::ZBRA()
{
	dims.x1 = INT_MAX;
	dims.x2 = INT_MAX;
	dims.y1 = INT_MAX;
	dims.y2 = INT_MAX;
	dims.size = 0;
	fitted = false;
}

ZBRA::~ZBRA()
{
	delete map;
}

int ZBRA::getIdealSize(){
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

void ZBRA::solveRecursive(ZBRA* area){
	DimSolv* manage = new DimSolv(area);
	for (vector<ZBRA*>::iterator test = area->subArea.begin(); test != area->subArea.end(); test++){
		solveRecursive(*test);
	}
	delete manage;
}

void ZBRA::buildMap(){
	//cout << dims.x1 << ',' << dims.y1 << " -> " << dims.x2 << ',' << dims.y2 << " " << fitted << endl;
	map = new Map(dims.x1,dims.y2,dims.x2-dims.x1,dims.y2-dims.y1,floor,wall);
	map->populate();
	for (vector<ZBRA*>::iterator test = subArea.begin(); test != subArea.end(); test++){
		(*test)->buildMap();
	}
}


ZBRA* ZBRA::BathRoom(){
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
	//DimSolv(this);
	//DimSolv(*subArea.begin());
	solveRecursive(this);
	buildMap();
	// cout << minSize << "->" << idealSize << endl;
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
	idealSize = 300 + getIdealSize();
	return this;
}

ZBRA* ZBRA::Kitchen(){
	shallow = false;
	floor = Tile::FLOOR;
	wall = Tile::WALL;
	minSize = 25;
	idealSize = 40;
}

ZBRA* ZBRA::LivingRoom(){
	shallow = false;
	floor = Tile::FLOOR;
	wall = Tile::WALL;
	minSize = 30;
	idealSize = 45;
}

ZBRA* ZBRA::BedRoom(){
	shallow = false;
	floor = Tile::FLOOR;
	wall = Tile::WALL;
	minSize = 20;
	idealSize = 35;
}
