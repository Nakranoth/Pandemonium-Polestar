#ifndef _SURFACELOADER_H_
    #define _SURFACELOADER_H_

#include <SDL/SDL.h>

class SurfaceLoader
{
	public:
        	static SDL_Surface* LoadImage(char* File);
        	static bool DrawImage(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);
};

#endif
