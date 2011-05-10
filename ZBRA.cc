#include "ZBRA.h"

#include <iostream>

void breakHere();

void ZBRA::printDimsRecursive(){
	for (vector<ZBRA*>::iterator test = subArea.begin(); test != subArea.end(); test++){
		(*test)->printDimsRecursive();
	}
	for (vector<ZBRA*>::iterator test = adjacent.begin(); test != adjacent.end(); test++){
		(*test)->printDimsRecursive();
	}
	cout << dims.x << ',' << dims.y << ':' << dims.x + dims.width << ',' << dims.y + dims.height << endl; 
}

ZBRA::ZBRA()
{
	dims.x = INT_MAX;
	dims.width = 0;
	dims.y = INT_MAX;
	dims.width = 0;
	dims.size = 0;
	flags = 0;
	effDims.x = INT_MAX;
	effDims.y = INT_MAX;
	effDims.width = 0;
	effDims.height = 0;
}

ZBRA::~ZBRA()
{
	delete map;
}

int ZBRA::getIdealSize(){
	int size = 0;
	for (vector<ZBRA*>::iterator i = subArea.begin(); i != subArea.end(); i++){
		size += (*i)->idealSize;
		for (vector<ZBRA*>::iterator j = (*i)->adjacent.begin(); j != (*i)->adjacent.end(); j++)
			size += (*j)->idealSize;
	}
	return size;
}

int ZBRA::getMinSize()
{
	int size = 0;
	for (vector<ZBRA*>::iterator i = subArea.begin(); i != subArea.end(); i++)
		size += (*i)->minSize;
	return size;
}

void ZBRA::solveRecursive(){
	//cout << seed << '\n';
	for (vector<ZBRA*>::iterator test = subArea.begin(); test != subArea.end(); test++){
		(*test)->solveRecursive();
	}
	DimSolv lambda(this);
}

void ZBRA::buildMap(Tile* superParent){
	map = new Map(dims.x,dims.y,dims.width,dims.height,floor,wall);
	map->populate();
	//if (subArea.size()) {
		for (vector<ZBRA*>::iterator child = subArea.begin(); child < subArea.end(); child++){
		(*child)->buildMap(map->ref);//readies children maps
		Tile* pPoint;	//parent reference tile for stiching
		Tile* cPoint;	//child reference tile for stiching
		
		pPoint = map->ref;
		cPoint = (*child)->map->ref;
		
		Map::stitch(pPoint,cPoint,(*child)->wall,(*child)->map);
		map->checkConsistency((*child)->map->ref);
	}
	
	for (vector<ZBRA*>::iterator adj = adjacent.begin(); adj < adjacent.end(); adj++){
		(*adj)->buildMap(map->ref);//readies children maps
		Tile* pPoint;	//parent reference tile for stiching
		Tile* cPoint;	//child reference tile for stiching
		
		pPoint = superParent;
		cPoint = (*adj)->map->ref;
		
		Map::stitch(pPoint,cPoint,(*adj)->wall,(*adj)->map);
		map->checkConsistency((*adj)->map->ref);
	}
}

void ZBRA::recursiveWiggle(int x, int y){
	dims.x += x;
	dims.y += y;
	for (vector<ZBRA*>::iterator i = adjacent.begin(); i != adjacent.end();i++){
		(*i)->recursiveWiggle(x,y);
	}
	for (vector<ZBRA*>::iterator i = subArea.begin(); i != subArea.end();i++){
		(*i)->recursiveWiggle(x,y);
	}
}

void ZBRA::AddFop(FOP* fop, Tile* tile)
{
	tile->fops.push_back(fop);
}

ZBRA* ZBRA::City()
{
	seed = time(NULL);
	srandom(seed);
	floor = Tile::GRASS;
	wall = Tile::UDEF;
	subArea.push_back((new ZBRA)->House(random())); //only one test house for now
	minSize = 25 + getMinSize();
	idealSize = 900 + getIdealSize();
	solveRecursive();
	printDimsRecursive();
	buildMap(NULL);
	AddFop((new FOP)->Character(320, 240, map->ref), map->ref);
	return this;
}
		
ZBRA* ZBRA::House(int randSeed)
{
	seed = randSeed;
	flags = ZBRA::SOFT;
	floor = Tile::UDEF;
	wall = Tile::UDEF;
	//for now every house will contain 1 bathroom kitchen and living room and 2 bedrooms
	subArea.push_back((new ZBRA)->LivingRoom(random()));
	//for now every house will contain 1 cat
	//AddFop((new FOP)->Cat(), map->ref);
	minSize = 20 + getMinSize();
	idealSize = 20 + getIdealSize();
	return this;
}

ZBRA* ZBRA::BathRoom(int randSeed){
	seed = randSeed;
	flags = 0;
	minSize = 20;
	idealSize = 30;
	floor = Tile::BATHROOM;
	wall = Tile::WALL;
	//every bathroom for now will contain only 1 toilet
	//AddFop((new FOP)->Toilet(), map->ref);
	return this;
}

ZBRA* ZBRA::Kitchen(int randSeed){
	seed = randSeed;
	flags = 0;
	floor = Tile::KITCHEN;
	wall = Tile::WALL;
	//every kitchen for now will contain only 1 fridge
	//AddFop((new FOP)->Refrigerator(), map->ref);
	minSize = 25;
	idealSize = 40;
	return this;
}

ZBRA* ZBRA::LivingRoom(int randSeed){
	seed = randSeed;
	flags = 0;
	floor = Tile::FLOOR;
	wall = Tile::WALL;
	adjacent.push_back((new ZBRA)->Kitchen(random()));
	//adjacent.push_back((new ZBRA)->HouseHallway(random()));	
	adjacent.push_back((new ZBRA)->BathRoom(random()));
	adjacent.push_back((new ZBRA)->BedRoom(random()));
	adjacent.push_back((new ZBRA)->BedRoom(random()));
	//every living room for now will contain only 1 sofa
	//AddFop((new FOP)->Sofa(), map->ref);
	minSize = 30;
	idealSize = 45;
	return this;
}

ZBRA* ZBRA::HouseHallway(int randSeed){
	seed = randSeed;
	flags = ZBRA::LONG;
	floor = Tile::ROAD;
	wall = Tile::WALL;

	minSize = 30;
	idealSize = 45;
	return this;
}

ZBRA* ZBRA::BedRoom(int randSeed){ 
	seed = randSeed;
	floor = Tile::BEDROOM;
	wall = Tile::WALL;
	//every bedroom for now will contain only 1 bed
	//AddFop((new FOP)->Bed(), map->ref);
	minSize = 20;
	idealSize = 35;
	return this;
}
