#include "SurfaceLoader.h"

SDL_Surface* SurfaceLoader::LoadImage(char* File)
{
	//Declaring a temporary surface to load the image into and the image
	//surface holding the proper format of the temp surface.
	SDL_Surface* temp_surf = NULL;
	SDL_Surface* image = NULL;

	//Attempt to load the bmp file
	if((temp_surf = SDL_LoadBMP(File)) == NULL) {
		return NULL;
	}

	image = SDL_DisplayFormat(temp_surf);
	SDL_FreeSurface(temp_surf);

	return image;
}

bool SurfaceLoader::DrawImage(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y)
{
	//Exit if there is nothing to draw
	if(Surf_Dest == NULL || Surf_Src == NULL)
	{
		return false;
	}

	//Create a rectangle so that we can use this
	//rectangle to know where to draw on our screen
	SDL_Rect DestR;

	DestR.x = X;
	DestR.y = Y;

	SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);

	return true;
}

bool SurfaceLoader::DrawImage(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H)
{
	//Exit if there is nothing to draw
	if(Surf_Dest == NULL || Surf_Src == NULL)
	{
		return false;
	}

	//Create a rectangle so that we can use this
	//rectangle to know where to draw on the screen
	SDL_Rect DestR;

	DestR.x = X;
	DestR.y = Y;

	//Create a second rectangle from the image loaded to
	//determine what part of the image to draw...(for tilesets)
	SDL_Rect SrcR;

	SrcR.x = X2;
	SrcR.y = Y2;
	SrcR.w = W;
	SrcR.h = H;

	SDL_BlitSurface(Surf_Src, &SrcR, Surf_Dest, &DestR);

    return true;
}
