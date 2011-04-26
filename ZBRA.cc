#include "ZBRA.h"

#include <iostream>

void breakHere();

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
		if ((*test)->dims.x1 < dims.x1 || (*test)->dims.y1 < dims.y1 || (*test)->dims.x2 > dims.x2 || (*test)->dims.y2 > dims.y2)	cerr << "BAD BOUNDS\n";
	}
	delete manage;
}

void ZBRA::buildMap(){
	map = new Map(dims.x1,dims.y1,dims.x2-dims.x1+1,dims.y2-dims.y1+1,floor,wall);
	map->populate();
	for (vector<ZBRA*>::iterator test = subArea.begin(); test != subArea.end(); test++){
		(*test)->buildMap();//readies children maps
		
		Tile* pPoint;	//parent reference tile outside of boundary
		Tile* ipPoint;	//parent reference tile inside of boundary
		Tile* cPoint;	//child reference tile for stiching
		
		ipPoint = map->ref;
		cPoint = (*test)->map->ref;

		while (ipPoint->x < cPoint->x){
if (ipPoint->east == NULL) breakHere();
			ipPoint = ipPoint->east;
		}
		while (ipPoint->y < cPoint->y){
if (ipPoint->south == NULL) breakHere();
			ipPoint = ipPoint->south;
		}
		
		bool north, south, east, west;
		north = south = east = west = true;
		
		if (dims.x1 == (*test)->dims.x1) west = false;
		if (dims.y1 == (*test)->dims.y1) north = false;
		if (dims.x2 == (*test)->dims.x2) east = false;
		if (dims.y2 == (*test)->dims.y2) south = false;
		
		int orient = 3;	//0 = north, 1 = east, 2 = south, 3 = west/none
		if (!west && !north){
			orient = 0;
			if (!east) orient = 1;
			if (!south)	{	//bounds are synonomous, rebind parent.
				delete map;
				map = (*test)->map;
			}
		}
		else{
			if (!north) orient = 0;
			if (!east) orient = 1;
			if (!south) orient = 2;
			if (!west) orient = 3;
		}
		switch (orient){
			case 0: 
				pPoint = ipPoint;
				//if(pPoint->east == NULL) breakHere();
				while(cPoint->east != NULL){
					cPoint = cPoint->east;
					pPoint = pPoint->east;
					//if(cPoint->x != pPoint->x)breakHere();
					//if(cPoint->y != pPoint->y)breakHere();
					//if(pPoint->east == NULL)breakHere();
				}
				pPoint = pPoint->east;
				break;
			case 1:
				pPoint = ipPoint;
				
				while(cPoint->east != NULL){
					cPoint = cPoint->east;
					//if(pPoint->east == NULL)breakHere();
					pPoint = pPoint->east;
					//if(cPoint->x != pPoint->x)breakHere();
					//if(cPoint->y != pPoint->y)breakHere();
				}
				//if(pPoint->south == NULL)breakHere();
				while(cPoint->south != NULL){
					cPoint = cPoint->south;
					pPoint = pPoint->south;
					//if(cPoint->x != pPoint->x)breakHere();
					//if(cPoint->y != pPoint->y)breakHere();
					//if(pPoint->south == NULL)breakHere();
				}
				pPoint = pPoint->south;
				break;
			case 2:
				pPoint = ipPoint;
					if (cPoint->x != pPoint->x || cPoint->y != pPoint->y) breakHere();
				while(cPoint->south != NULL){
					if(cPoint->south->y - 1 != cPoint->y) breakHere(); 
					cPoint = cPoint->south;
					if(pPoint->south->y - 1 != pPoint->y) breakHere();
					pPoint = pPoint->south;
					if(cPoint->x != pPoint->x)breakHere();
					if(cPoint->y != pPoint->y)breakHere();
				}
				if(pPoint->west != NULL);
				pPoint = pPoint->west;
				break;
			case 3:
				pPoint = ipPoint->north;
				break;
		}
		//Map::unstitch(ipPoint,(*test)->dims.x2 - (*test)->dims.x1,(*test)->dims.y2 - (*test)->dims.y1);
		//Map::delChunk(ipPoint);
		if(pPoint->x > cPoint->x + 1 || pPoint->x < cPoint->x - 1 || pPoint->y > cPoint->y + 1 || pPoint->y < cPoint->y - 1 ) {breakHere(); cerr << "\nbroken\n";}
		Map::stitch(pPoint,cPoint,(*test)->wall, orient);
		Map::checkConsistency((*test)->map->ref);
	}
}

void ZBRA::AddFop(FOP* fop)
{
	fops.push_back(fop);
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
	return this;
}
		
ZBRA* ZBRA::House()
{
	shallow = false;
	floor = Tile::HOUSE;
	wall = Tile::WALL;
	//for now every house will contain 1 bathroom kitchen and living room and 2 bedrooms
	subArea.push_back((new ZBRA)->BathRoom());
	subArea.push_back((new ZBRA)->Kitchen());
	subArea.push_back((new ZBRA)->LivingRoom());
	subArea.push_back((new ZBRA)->BedRoom());
	subArea.push_back((new ZBRA)->BedRoom());
	//for now every house will contain 1 cat
	AddFop((new FOP)->Cat());
	minSize = 20 + getMinSize();
	idealSize = 30 + getIdealSize();
	return this;
}

ZBRA* ZBRA::BathRoom(){
	shallow = false;
	minSize = 20;
	idealSize = 30;
	floor = Tile::BATHROOM;
	wall = Tile::WALL;
	//every bathroom for now will contain only 1 toilet
	AddFop((new FOP)->Toilet());
	return this;
}

ZBRA* ZBRA::Kitchen(){
	shallow = false;
	floor = Tile::KITCHEN;
	wall = Tile::WALL;
	//every kitchen for now will contain only 1 fridge
	AddFop((new FOP)->Refrigerator());
	minSize = 25;
	idealSize = 40;
	return this;
}

ZBRA* ZBRA::LivingRoom(){
	shallow = false;
	floor = Tile::FLOOR;
	wall = Tile::WALL;
	//every living room for now will contain only 1 sofa
	AddFop((new FOP)->Sofa());
	minSize = 30;
	idealSize = 45;
	return this;
}

ZBRA* ZBRA::BedRoom(){
	shallow = false;
	floor = Tile::BEDROOM;
	wall = Tile::WALL;
	//every bedroom for now will contain only 1 bed
	AddFop((new FOP)->Bed());
	minSize = 20;
	idealSize = 35;
	return this;
}
