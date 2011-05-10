#include "FOP.h"
#include "Tile.h"

#include <iostream>

FOP::FOP()
{
	fitted = false;
	isSolid = 1;
	width = 0;
	length = 0;
	x = 0;
	y = 0;
	Xvel = 0;
	Yvel = 0;

	image = NULL;

	currentFrame = 0;
	maxFrames = 0;
	frameIncrement = 1;
	frameRate = 100;
	oldTime = 0;
	oscillate = false;
}

FOP::~FOP()
{
	SDL_FreeSurface(image);
}

int FOP::getSize()
{
	return width*length;
}


FOP* FOP::Sofa()
{
	width = Tile::SIZE*2 + 10;
	length = Tile::SIZE + 20;
	actions.push_back(Action::SIT);
	
	char file[] = "./FOPS/sofa.bmp";
	image = SurfaceLoader::LoadImage(file);
	colorkey = SDL_MapRGB(image->format, 255, 42, 113);
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorkey); //making the image transparency set to the colorkey
	
	return this;
}

FOP* FOP::Refrigerator()
{
	width = Tile::SIZE;
	length = Tile::SIZE;
	actions.push_back(Action::OPEN);

	char file[] = "./FOPS/refrigerator.bmp";
	image = SurfaceLoader::LoadImage(file);
	colorkey = SDL_MapRGB(image->format, 255, 42, 113);
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorkey); //making the image transparency set to the colorkey

	return this;
}

FOP* FOP::Toilet()
{
	width = Tile::SIZE;
	length = Tile::SIZE;
	actions.push_back(Action::SIT);
	
	char file[] = "./FOPS/toilet.bmp";
	image = SurfaceLoader::LoadImage(file);
	colorkey = SDL_MapRGB(image->format, 255, 42, 113);
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorkey); //making the image transparency set to the colorkey

	return this;
}

FOP* FOP::Bed()
{
	width = Tile::SIZE*2;
	length = Tile::SIZE;
	actions.push_back(Action::SIT);
	actions.push_back(Action::SLEEP);
	
	char file[] = "./FOPS/bed.bmp";
	image = SurfaceLoader::LoadImage(file);
	colorkey = SDL_MapRGB(image->format, 255, 42, 113);
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorkey); //making the image transparency set to the colorkey

	return this;
}

FOP* FOP::Cat()
{
	width = Tile::SIZE;
	length = Tile::SIZE;
	actions.push_back(Action::PET);
	
	char file[] = "./FOPS/cat.bmp";
	image = SurfaceLoader::LoadImage(file);
	colorkey = SDL_MapRGB(image->format, 255, 42, 113);
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorkey); //making the image transparency set to the colorkey

	return this;
}

FOP* FOP::Character(int x, int y, Tile* ref)
{
	this->x = x;
	this->y = y;
	width = Tile::SIZE;
	length = Tile::SIZE;
	
	/*char file[] = "./FOPS/char.bmp";
	image = SurfaceLoader::LoadImage(file);
	colorkey = SDL_MapRGB(image->format, 255, 42, 113);
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorkey); //making the image transparency set to the colorkey
*/
	location = ref;

	while(location->x < x/Tile::SIZE)
	{
		location = location->east;
	}
	while(location->y < y/Tile::SIZE)
	{
		location = location->south;
	}

	return this;
}

void FOP::startAnimation()
{
	if(oldTime + frameRate > SDL_GetTicks())
	{
		return;
	}

	oldTime = SDL_GetTicks();

	currentFrame += frameIncrement;

	if(oscillate)
	{
		if(frameIncrement > 0)
		{
			if(currentFrame >= maxFrames -1)
				frameIncrement = -frameIncrement;
		}
		else
		{
			if(currentFrame <= 0)
				frameIncrement = -frameIncrement;
		}
	}
	else
	{
		if(currentFrame >= maxFrames -1)
			currentFrame = 0;
	}
}

void FOP::stopAnimation()
{
	currentFrame = 0;
}

void FOP::setFrameRate(int rate)
{
	frameRate = rate;
}

void FOP::setCurrentFrame(int frame)
{
	if(frame >= 0 && frame < maxFrames)
		currentFrame = frame;
}

int FOP::getCurrentFrame()
{
	return currentFrame;
}
