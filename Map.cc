#include "Map.h"

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

bool Map::populate(){
	area[0][0].sprite = floor;
	area[0][0].x = x;
	area[0][0].y = y;
	for (int i = 0;i < xsiz; i++){
		for (int j = 0; j < ysiz; j++){
			area[i][j].sprite = floor;
			area[i][j].x = i+x;
			area[i][j].y = j+y;
			
			if (i > 0) area[i][j].west = &area[i-1][j];
			if (j > 0) area[i][j].north = &area[i][j-1];
			if (i < xsiz - 1) area[i][j].east = &area[i+1][j];
			if (j < ysiz - 1) area[i][j].south = &area[i][j+1];
		}
	}
	return true;
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

