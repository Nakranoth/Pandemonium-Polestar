#ifndef _FOP_
#define _FOP_

#include <vector>

#include "Tile.h"
#include "Action.h"

using namespace std;

class FOP
{
	public:
		bool fitted;	//flag: has a set position in the map.
		int isSolid; //flag: if this FOP can be passed over or not
		vector<int> actions;	//holds the actions that can be performed on this FOP
		int width; //holds the pixel width of the FOP
		int length; //holds the pixel length of the FOP
		int x;
		int y;
	public:
		FOP();
		~FOP();
		
		//The functions that will help determine what this type of FOP will be
		FOP* Sofa();
		FOP* Refrigerator();
		FOP* Toilet();
		FOP* Bed();
		FOP* Cat();
		
	private:
		int getSize();	//Gets the length and width of the FOP
};

#endif

