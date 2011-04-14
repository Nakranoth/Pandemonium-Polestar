#include "Map.h"

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
			if (j > 0) area[i][j].south = &area[i][j-1];
			if (i < xsiz - 1) area[i][j].east = &area[i+1][j];
			if (j < ysiz - 1) area[i][j].north = &area[i][j+1];
		}
	}
	return true;
}