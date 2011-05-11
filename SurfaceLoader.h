#ifndef _SURFACELOADER_H_
    #define _SURFACELOADER_H_

#include <SDL/SDL.h>
#include "config.cfg"

class SurfaceLoader
{
	public:
        	static SDL_Surface* LoadImage(char* File);
        	static bool DrawImage(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);
		static bool DrawImage(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H);
		static bool setTransparency(SDL_Surface* surf, int R, int G, int B);
};

#endif
