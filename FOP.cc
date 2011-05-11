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
	if((image = SurfaceLoader::LoadImage(file)) == NULL)
	{
		cout<<"Error loading " << file << "...exiting\n";
		exit(1);
	}
	SurfaceLoader::setTransparency(image, RED, GREEN, BLUE);
	
	return this;
}

FOP* FOP::Refrigerator()
{
	width = Tile::SIZE;
	length = Tile::SIZE;
	actions.push_back(Action::OPEN);

	char file[] = "./FOPS/refrigerator.bmp";
	if((image =  SurfaceLoader::LoadImage(file)) == NULL)
	{
		cout<<"Error loading " << file << "...exiting\n";
		exit(1);
	}
	SurfaceLoader::setTransparency(image, RED, GREEN, BLUE);

	return this;
}

FOP* FOP::Toilet()
{
	width = Tile::SIZE;
	length = Tile::SIZE;
	actions.push_back(Action::SIT);
	
	char file[] = "./FOPS/toilet.bmp";
	if((image =  SurfaceLoader::LoadImage(file)) == NULL)
	{
		cout<<"Error loading " << file << "...exiting\n";
		exit(1);
	}
	SurfaceLoader::setTransparency(image, RED, GREEN, BLUE);

	return this;
}

FOP* FOP::Bed()
{
	width = Tile::SIZE*2;
	length = Tile::SIZE;
	actions.push_back(Action::SIT);
	actions.push_back(Action::SLEEP);
	
	char file[] = "./FOPS/bed.bmp";
	if((image =  SurfaceLoader::LoadImage(file)) == NULL)
	{
		cout<<"Error loading " << file << "...exiting\n";
		exit(1);
	}
	SurfaceLoader::setTransparency(image, RED, GREEN, BLUE);

	return this;
}

FOP* FOP::Cat()
{
	width = Tile::SIZE;
	length = Tile::SIZE;
	actions.push_back(Action::PET);
	
	char file[] = "./FOPS/catsleep.bmp";
	if((image =  SurfaceLoader::LoadImage(file)) == NULL)
	{
		cout<<"Error loading " << file << "...exiting\n";
		exit(1);
	}
	SurfaceLoader::setTransparency(image, RED, GREEN, BLUE);

	return this;
}

FOP* FOP::Character(int x, int y, Tile* ref)
{
	//Uint8 colorkey;
	this->x = x;
	this->y = y;
	width = Tile::SIZE;
	length = Tile::SIZE;
	
	/*char file[] = "./FOPS/char.bmp";
	if((image = SDL_LoadBMP(file)) == NULL)
	{
		cout<<"Error loading " << file << "...exiting\n";
		exit(1);
	}
	colorkey = SDL_MapRGB(image->format, RED, GREEN, BLUE);
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
	if((unsigned)oldTime + (unsigned)frameRate > SDL_GetTicks())
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

int FOP::getCurrentFrameOffset()
{
	if(Yvel > 0) //if moving down, we want the first row in char.bmp
		frameOffset = 0;
	else if(Yvel < 0) //if moving up, we want the second row
		frameOffset = 1;
	else if(Xvel < 0) //if moving left, we want the third row
		frameOffset = 2;
	else if(Xvel > 0) //if moving right, we want the fourth row
		frameOffset = 3;

	return frameOffset; //face user by default
}
