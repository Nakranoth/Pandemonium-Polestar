#ifndef _ZBRA_
#define _ZBRA_

#include <vector>
#include <math.h>
#include <limits.h>

#include "Map.h"
#include "DimSolv.h"

using namespace std;

class ZBRA
{
	public:
		bool shallow;	//flag: counts for depth traversals.
		bool fitted;	//flag: has a set position in the map.
		vector<ZBRA*> subArea;	//ZBRAs within this ZBRA
		Map* map;	//tiles the ZBRA contains
		vector<int> fops;	//the FOPs of the ZBRA, presently a placeholder.
		int minSize;
		int idealSize;
		short floor, wall;
		bounds dims;
	public:
		ZBRA();
		~ZBRA();
		
		//The functions that will help determine what this type of ZBRA will contain
		ZBRA* City();
		ZBRA* House();
		ZBRA* BathRoom();
		ZBRA* Kitchen();
		ZBRA* LivingRoom();
		ZBRA* BedRoom();
		
	private:
		int getMinSize();	//Gets the sizes of this->subArea
		int getIdealSize();
		void solveRecursive(ZBRA* area);
		void buildMap();
};

#endif

