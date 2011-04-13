#include "GUI.h"

int main()
{
	SDL_Surface *screen;
	Uint8 *keys;

	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(640,480,16,SDL_SWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Pandemonium Polestar", NULL); //sets title bar
	keys = SDL_GetKeyState(NULL);

	ZBRA test;
	test.City();

	while(keys[SDLK_ESCAPE] == false)
	{
		outputMap(test.getRefTile());

		//This allows for listening of key and mouse events and such
		SDL_PumpEvents();
	}

	return 0;
}

void outputMap(Tile* refTile)
{
	cout<<"WE WILL BE OUTPUTING THE MAP STARTING AT X = " << refTile->x << " and Y = " << refTile->y << endl;
	
}

//grasb a ref. tile and they should be stitched together so that i can travers them all and display them

//grab red tile
//output this tile (top left of the room, eventually will be a tile near the player)
