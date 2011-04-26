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

void Map::unstitch(Tile* ref, int width, int height){
	//Give NW corner
	for (int i = 0; i < width; i++){
		if (ref->north){
			ref->north->south = NULL;
			ref->north = NULL;
		}
		if (i != width - 1) ref = ref->east;		
	}
	for (int i = 0; i < height; i++){
		if (ref->east){
			ref->east->west = NULL;
			ref->east = NULL;
		}
		if (i != height - 1) ref = ref->south;		
	}
	for (int i = 0; i < width; i++){
		if (ref->south){
			ref->south->north = NULL;
			ref->south = NULL;
		}
		if (i != width - 1) ref = ref->west;		
	}
	for (int i = 0; i < height; i++){
		if (ref->west){
			ref->west->east = NULL;
			ref->west = NULL;
		}
		if (i != height - 1) ref = ref->north;		
	}
}

void Map::delChunk(Tile* ref){
	set<Tile*> toKill;
	toKill.insert(ref);
	while(!toKill.empty()){
		set<Tile*>::iterator it;
		it = toKill.begin();
		Tile* curr = *it;
		if (curr->north){
			toKill.insert(curr->north);
			curr->north->south = NULL;
		}
		if (curr->west){
			toKill.insert(curr->west);
			curr->west->east = NULL;
		}
		if (curr->south){
			toKill.insert(curr->south);
			curr->south->north = NULL;
		}
		if (curr->north){
			toKill.insert(curr->east);
			curr->east->west = NULL;
		}
		delete curr;
		curr = NULL;
		toKill.erase(it);
	}
}

void Map::stitch(Tile* pRef, Tile* cRef, int wall, int orient){
	Tile* inRunner = cRef;
	Tile* outRunner = pRef;
	
	switch(orient){
		case 0:
			runSouth(inRunner,outRunner,wall,orient,true);
			break;
		case 1:
			runWest(inRunner,outRunner,wall,orient,true);
			break;
		case 2:
			runNorth(inRunner,outRunner,wall,orient,true);
			break;
		case 3:
			runEast(inRunner,outRunner,wall,orient,true);
			break;
	}
}

void Map::runEast(Tile* inRunner, Tile* outRunner, int wall,int orient, bool first){
//cover the north wall
	if (orient == 3 && !first) return;
	first = false;
	if (wall != Tile::UDEF) outRunner->type = wall;
	if (outRunner->x == inRunner->x - 1){
		outRunner = outRunner->east;
		if (wall != Tile::UDEF) outRunner->type = wall;
	}
	while(inRunner->east != NULL){
		assert(outRunner->x == inRunner->x);
		assert(outRunner->y == inRunner->y - 1);
		outRunner->south = inRunner;
		inRunner->north = outRunner;
		outRunner = outRunner->east;
		inRunner = inRunner->east;
		if (wall != Tile::UDEF) outRunner->type = wall;
	}
	if (outRunner->east != NULL){
		assert(outRunner->x == inRunner->x);
		assert(outRunner->y == inRunner->y - 1);
		outRunner = outRunner->east;
		if (wall != Tile::UDEF) outRunner->type = wall;
		if (outRunner->south == NULL) breakHere();
		outRunner = outRunner->south;
		if (wall != Tile::UDEF) outRunner->type = wall;
		assert(outRunner->x == inRunner->x + 1);
		assert(outRunner->y == inRunner->y);
		runSouth(inRunner,outRunner,wall,orient,first);
	}
}

void Map::runSouth(Tile* inRunner, Tile* outRunner, int wall, int orient, bool first){
//cover the east wall
	if (orient == 0 && !first) return;
	first = false;
	if (wall != Tile::UDEF) outRunner->type = wall;
	if (outRunner->y == inRunner->y + 1){
		assert(outRunner->south != NULL);
		outRunner = outRunner->south;
		if (wall != Tile::UDEF) outRunner->type = wall;
	}
	while(inRunner->south != NULL){
		assert(outRunner->south != NULL);
		assert(outRunner->x == inRunner->x + 1);
		assert(outRunner->y == inRunner->y);
		outRunner->west = inRunner;
		inRunner->east = outRunner;
		outRunner = outRunner->south;
		inRunner = inRunner->south;
		if (wall != Tile::UDEF) outRunner->type = wall;
	}
	if (outRunner->south != NULL){
		assert(outRunner->x == inRunner->x + 1);
		assert(outRunner->y == inRunner->y);
		outRunner = outRunner->south;
		if (wall != Tile::UDEF) outRunner->type = wall;
		assert(outRunner->west != NULL);
		outRunner = outRunner->west;
		if (wall != Tile::UDEF) outRunner->type = wall;
		assert(outRunner->x == inRunner->x);
		assert(outRunner->y == inRunner->y + 1);
		runWest(inRunner,outRunner,wall,orient,first);
	}
}

void Map::runWest(Tile* inRunner, Tile* outRunner, int wall,int orient, bool first){
//cover the north wall
	if (orient == 1 && !first) return;
	first = false;
	if (wall != Tile::UDEF) outRunner->type = wall;
	if (outRunner->x == inRunner->x + 1){
		assert(outRunner->west != NULL);
		outRunner = outRunner->west;
		if (wall != Tile::UDEF) outRunner->type = wall;
	}
	while(inRunner->west != NULL){
		assert(outRunner->x == inRunner->x);
		assert(outRunner->y == inRunner->y + 1);
		outRunner->north = inRunner;
		inRunner->south = outRunner;
		outRunner = outRunner->west;
		inRunner = inRunner->west;
		if (wall != Tile::UDEF) outRunner->type = wall;
	}
	if (outRunner->west != NULL){
		assert(outRunner->x == inRunner->x);
		assert(outRunner->y == inRunner->y + 1);
		outRunner = outRunner->west;
		if (wall != Tile::UDEF) outRunner->type = wall;
		assert(outRunner->north != NULL);
		outRunner = outRunner->north;
		if (wall != Tile::UDEF) outRunner->type = wall;
		assert(outRunner->x == inRunner->x - 1);
		assert(outRunner->y == inRunner->y);
		runNorth(inRunner,outRunner,wall,orient,first);
	}
}

void Map::runNorth(Tile* inRunner, Tile* outRunner, int wall, int orient, bool first){
//cover the east wall
	if (orient == 2 && !first) return;
	first = false;
	if (wall != Tile::UDEF) outRunner->type = wall;
	if (outRunner->y == inRunner->y - 1){
		assert(outRunner->north != NULL);
		outRunner = outRunner->north;
		if (wall != Tile::UDEF) outRunner->type = wall;
	}
	while(inRunner->north != NULL){
		assert(outRunner->x == inRunner->x - 1);
		assert(outRunner->y == inRunner->y);
		outRunner->east = inRunner;
		inRunner->west = outRunner;
		outRunner = outRunner->north;
		inRunner = inRunner->north;
		if (wall != Tile::UDEF) outRunner->type = wall;
	}
	if (outRunner->north != NULL){
		assert(outRunner->x == inRunner->x - 1);
		assert(outRunner->y == inRunner->y);
		outRunner = outRunner->north;
		if (wall != Tile::UDEF) outRunner->type = wall;
		assert(outRunner->east != NULL);
		outRunner = outRunner->east;
		if (wall != Tile::UDEF) outRunner->type = wall;
		assert(outRunner->x == inRunner->x);
		assert(outRunner->y == inRunner->y - 1);
		runEast(inRunner,outRunner,wall,orient,first);
	}
}