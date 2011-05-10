#include "Map.h"
#include "DimSolv.h"


#include <iostream>
void breakHere(){cerr << "Broken\n";}

using namespace std;

Map::Map(int xloc, int yloc, int xsiz, int ysiz, short floor, short wall, short flags){
	//ref = new Tile(xloc,yloc);	//Must be one of the ones in area
	//telements = new Tile[xsiz*ysiz];
	area = new Tile*[xsiz];
	for(int i = 0; i < xsiz; i++) area[i] = new Tile[ysiz];
	ref = &area[0][0];
	this->floor = floor;
	this->wall = wall;
	this->flags = flags;
	this->x = xloc;
	this->y = yloc;
	this->xsiz = xsiz;
	this->ysiz = ysiz;
}

Map::~Map(){
	for(int i = 0; i < xsiz; i++) delete [] area[i];
	delete [] area;
}

void Map::populate(){
	if (xsiz <= 0 || ysiz <=0){
		cerr << "\nSizeless map\n\n";
		return;
	}
	area[0][0].type = floor;
	area[0][0].x = x;
	area[0][0].y = y;
	for (int i = xsiz - 1;i >= 0; i--){
		for (int j = ysiz - 1; j >= 0; j--){
			area[i][j].type = floor;
			area[i][j].x = i+x;
			area[i][j].y = j+y;
			
			if (i > 0) area[i][j].west = &area[i-1][j];
			if (j > 0) area[i][j].north = &area[i][j-1];
			if (i < xsiz - 1) area[i][j].east = &area[i+1][j];
			if (j < ysiz - 1) area[i][j].south = &area[i][j+1];
		}
	}
	
	checkConsistency(ref);
}

void Map::checkConsistency(Tile* ref){
	set<Tile*> toCheck;
	toCheck.insert(ref);

	//create a list of things that have already been rendered
	set<Tile*> isChecked;

	//While there is still stuff to render...
	set<Tile*>::iterator it;//declared before loop to avoid mem leak.
	while(!toCheck.empty())
	{
		it = toCheck.begin();
		if (*it == NULL) return;
		Tile* curr = *it;

		if (curr->north && !isChecked.count(curr->north)){
			toCheck.insert(curr->north);
		}
		if (curr->west && !isChecked.count(curr->west)){
			toCheck.insert(curr->west);
		}
		if (curr->south && !isChecked.count(curr->south)){
			toCheck.insert(curr->south);
		}
		if (curr->east && !isChecked.count(curr->east)){
			toCheck.insert(curr->east);
		}
		
		if(curr->north){
			if(curr->x != curr->north->x)	cerr << "InconsNorthX " << curr->x << curr->north->x << "\n" ;
			if(curr->y != curr->north->y + 1)	cerr << "InconsNorthY\n";
		}
		
		if(curr->west){
			if(curr->x != curr->west->x + 1)	cerr << "InconsWestX\n";
			if(curr->y != curr->west->y)	cerr << "InconsWestY\n";
		}
		
		if(curr->south){
			if(curr->x != curr->south->x)	cerr << "InconsSouthX\n";
			if(curr->y != curr->south->y - 1)	cerr << "InconsSouthY\n";
		}
			
		if(curr->east){
			if(curr->x != curr->east->x - 1)	cerr << "InconsEastX\n";
			if(curr->y != curr->east->y)	cerr << "InconsEastY\n";
		}
		
		
		
		isChecked.insert(curr);
		toCheck.erase(it);
	}
}

void Map::stitch(Tile* parent, Tile* child, int wall, Map* cMap){	
	//lets just clone upwards for the grade. Worry about optimization later
	//navigate to aligned points
	Tile* prun, *crun;
	prun = parent;
	crun = child;
	while (prun->x < crun->x){
		if (prun->east != NULL)	prun = prun->east;
		else{
			cerr << "Bad x run. " << prun->x << '<' << crun->x << endl;
			return;
		}
	}
	while (prun->y < crun->y){
		if (prun->south != NULL)	prun = prun->south;
		else{
			cerr << "Bad y run. " << prun->y << '<' << crun->y << endl;
			return;
		}
	}
	//walk edge, make wall.
	while (prun->east != NULL && crun->east != NULL){
		if(prun->north != NULL){
			if(wall != Tile::UDEF) prun->north->type = wall;
		}
		else{
			if(wall != Tile::UDEF) crun->type = wall;
		}
		prun = prun->east;
		crun = crun->east;
	}
	if(prun->north != NULL){
		if (wall != Tile::UDEF){
			prun->north->type = wall;
			if (prun->north->east != NULL) prun->north->east->type = wall;
		}
	}
	else{
		if(wall != Tile::UDEF) crun->type = wall;
	}
	//walk edge, make wall.
	while (prun->south != NULL && crun->south != NULL){
		if(prun->east != NULL ){
			if(wall != Tile::UDEF) prun->east->type = wall;
		}
		else{
			if(wall != Tile::UDEF){
				crun->type = wall;
			}
		}
		prun = prun->south;
		crun = crun->south;
	}
	if(prun->east != NULL){
		if (wall != Tile::UDEF){
			prun->east->type = wall;
			if (prun->east->south != NULL) prun->east->south->type = wall;
		}
	}
	else{
		if(wall != Tile::UDEF) crun->type = wall;
	}
	//more wall walking
	while (prun->west != NULL && crun->west != NULL){
		if(prun->south != NULL ){
			if(wall != Tile::UDEF) prun->south->type = wall;
		}
		else{
			if(wall != Tile::UDEF) crun->type = wall;
		}
		prun = prun->west;
		crun = crun->west;
	}
	if(prun->south != NULL){
		if (wall != Tile::UDEF){
			prun->south->type = wall;
			if (prun->south->west != NULL) prun->south->west->type = wall;
		}
	}
	else{
		if(wall != Tile::UDEF) crun->type = wall;
	}
	//One last time
	while (prun->north != NULL && crun->north != NULL){
		if(prun->west != NULL){
			if(wall != Tile::UDEF) prun->west->type = wall;
		}
		else{
			if(wall != Tile::UDEF) crun->type = wall;
		}
		prun = prun->north;
		crun = crun->north;
	}
	if(prun->west != NULL){
		if (wall != Tile::UDEF){
			prun->west->type = wall;
			if (prun->west->north != NULL) prun->west->north->type = wall;
		}
	}
	else{
		if(wall != Tile::UDEF) crun->type = wall;
	}
	if (prun->x != crun->x || prun->y != crun->y){
		cerr << "CRAP!\n";
		return;
	}
	cloneRow(prun,crun,true);
	//delete cMap;
	//walk the parent pointr to where it belongs
	//*pAdder = *cAdder;
	//while ((*pAdder)->north != NULL) *pAdder = (*pAdder)->north;
	//while ((*pAdder)->west != NULL) *pAdder = (*pAdder)->west;
}

void Map::cloneRow(Tile* prun, Tile* crun,bool east){
	if (east){
		while (prun->east != NULL && crun->east != NULL){
			if(crun->type != Tile::UDEF){
				cloneTile(prun,crun);
			}
			prun = prun->east;
			crun = crun->east;
		}
		if(crun->type != Tile::UDEF){
			cloneTile(prun,crun);
		}
	}
	else{
		while (prun->west != NULL && crun->west != NULL){
			if(crun->type != Tile::UDEF){
				cloneTile(prun,crun);
			}
			prun = prun->west;
			crun = crun->west;
		}
		if(crun->type != Tile::UDEF){
			cloneTile(prun,crun);
		}
	}
	
	if (prun->south != NULL && crun->south != NULL){
		cloneRow(prun->south,crun->south,!east);
		//delete crun;
	} 
}

void Map::cloneTile(Tile* prun, Tile* crun){
	prun->type = crun->type;
	for (vector<FOP*>::iterator i = crun->fops.begin(); i < crun->fops.end();i++){
		prun->fops.push_back(*i);
	}
}
