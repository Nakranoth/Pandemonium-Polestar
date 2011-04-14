#include "ZBRA.h"

#include <iostream>

ZBRA::ZBRA()
{
}

ZBRA::~ZBRA(){
	delete map;
}

bool ZBRA::tryDim(int x, int y){
	if (x*y < minSize) return false;
	for (vector<ZBRA*>::iterator i = subArea.begin(); i != subArea.end(); i++){
		int siz = ceil(sqrt((*i)->idealSize));
		(*i)->tryDim(siz,siz);
	}
	map = new Map(0,0,x,y,floor,wall);
	if (map->populate()) return true;
	else {
		delete map;
		map = NULL;
		return false;
	}
}

int ZBRA::getIdealSize(){
	int size = 0;
	for (vector<ZBRA*>::iterator i = subArea.begin(); i != subArea.end(); i++)
		size += (*i)->idealSize;
	return size;
}

int ZBRA::getMinSize(){
	int size = 0;
	for (vector<ZBRA*>::iterator i = subArea.begin(); i != subArea.end(); i++)
		size += (*i)->minSize;
	return size;
}

ZBRA* ZBRA::BathRoom(){
	shallow = false;
	minSize = 20;
	idealSize = 30;
	floor = Tile::FLOOR;
	wall = Tile::WALL;
	return this;
}

ZBRA* ZBRA::City(){
	shallow = false;
	floor = Tile::GRASS;
	wall = Tile::UDEF;
	subArea.push_back((new ZBRA)->House());
	minSize = 25 + getMinSize();
	idealSize = 35 + getIdealSize();
	cout << minSize << "->" << idealSize << endl;
	return this;
}
		
ZBRA* ZBRA::House(){
	shallow = false;
	floor = Tile::FLOOR;
	wall = Tile::WALL;
	subArea.push_back((new ZBRA)->BathRoom());
	subArea.push_back((new ZBRA)->Kitchen());
	subArea.push_back((new ZBRA)->LivingRoom());
	subArea.push_back((new ZBRA)->BedRoom());
	subArea.push_back((new ZBRA)->BedRoom());
	minSize = 20 + getMinSize();
	idealSize = 30 + getIdealSize();
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