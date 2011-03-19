#include "GUI.h"

int main()
{
	SDL_Surface *screen;
	Uint8 *keys;

	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(640,480,16,SDL_SWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Pandemonium Polestar", NULL); //sets title bar
	keys = SDL_GetKeyState(NULL);

	while(keys[SDLK_ESCAPE] == false)
	{
		ZBRA* test = new ZBRA();
		SDL_PumpEvents();
	}

	return 0;
}
