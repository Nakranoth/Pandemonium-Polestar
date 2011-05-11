#ifndef _FOP_
#define _FOP_

#include <vector>
#include <SDL/SDL.h>

class Tile;
#include "Action.h"
#include "SurfaceLoader.h"
#include "config.cfg"

using namespace std;

class FOP
{
	public:
		bool fitted;	//flag: has a set position in the map.
		int isSolid; //flag: if this FOP can be passed over or not
		vector<int> actions;	//holds the actions that can be performed on this FOP
		int width; //holds the pixel width of the FOP
		int length; //holds the pixel length of the FOP
		int x; //x position of this FOP
		int y; //y position of this FOP
		int Xvel; //x velocity of this fop (only non zero on moving AI)
		int Yvel; //y velocity of this fop (only non zero on moving AI)
		Tile* location; //holds the tile that the FOP is located on

		SDL_Surface* image; //holds the pointer to the image of this file
		Uint8 colorkey;

		//animation type variables
		int maxFrames;
		bool oscillate;

	private:
		//animation type variables
		int currentFrame;
		int frameOffset;
		int frameIncrement;
		int frameRate; //miliseconds
		long oldTime;
	public:
		FOP();
		~FOP();
		
		//The functions that will help determine what this type of FOP will be
		FOP* Sofa();
		FOP* Refrigerator();
		FOP* Toilet();
		FOP* Bed();
		FOP* Cat();
		FOP* Character(int x, int y, Tile* ref);

		void setFrameRate(int rate);
		void setCurrentFrame(int frame);
		int getCurrentFrame();
		int getCurrentFrameOffset(); //This returns the appropriate number dependent on the direction
		void startAnimation();
		void stopAnimation();
		
	private:
		int getSize();	//Gets the length and width of the FOP
};

#endif

