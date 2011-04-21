#ifndef _GUI_
#define _GUI_

#include <SDL/SDL.h>
#include <iostream>

#include "ZBRA.h"
#include "Tile.h"
#include "SurfaceLoader.h"

class GUI
{

	private:
	bool running;
	SDL_Surface* screen;
	SDL_Surface* GRASS;

	public:
	SDL_Event Event;
	ZBRA city;

	public:
	int onExecute();
	bool Initialize();
	void EventHandler(SDL_Event* Event);
	void Logic();
	void Render();
	void Cleanup();
	void outputMap(Tile* refTile);
};

#endif
