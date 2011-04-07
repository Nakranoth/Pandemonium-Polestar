#ifndef _ZBRA_
#define _ZBRA_

#include <vector>

#include "Map.h"

using namespace std;

class ZBRA
{
	public:
		bool shallow; //a flag to determine whether this counts for depth traversals
		vector<ZBRA*> subArea;	//ZBRAs within this ZBRA
		Map* map; //tiles the ZBRA contains
		vector<int> fops; //the FOPs of the ZBRA
		int minSize;
		int idealSize;
		short floor, wall;
	public:
		ZBRA();
		~ZBRA();
		bool tryDim(int x, int y);
		
		ZBRA* City();
		ZBRA* House();
		ZBRA* BathRoom();
		ZBRA* Kitchen();
		ZBRA* LivingRoom();
		ZBRA* BedRoom();
		
	private:
	int getMinSize();	//Gets the sizes of this->subArea
	int getIdealSize();
};

#endif

