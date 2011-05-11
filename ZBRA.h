#ifndef _ZBRA_
#define _ZBRA_

#include <vector>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include <algorithm>

#include "Map.h"
#include "DimSolv.h"
#include "FOP.h"

using namespace std;

class ZBRA
{
	public:
		static const int SHALLOW = 1, FITTED = 2, SOFT = 4, LONG = 8;
		int flags;
		//bool shallow;	//flag: counts for depth traversals.
		//bool fitted;	//flag: has a set position in the map.
		//bool soft;	//flag: has no area of its own. Is made up completely of children.
		//bool long		//flag: is long and narrow, rather than squarish.
		vector<ZBRA*> subArea;	//ZBRAs within this ZBRA
		vector<ZBRA*> adjacent;	//ZBRAs near myself.
		ZBRA** subAreaArray;	//For Debugging purposes only.
		Map* map;	//tiles the ZBRA contains
		int minSize;
		int idealSize;
		short floor, wall;
		bounds dims;
		bounds effDims;	//effective dimensions: this + adjacents.
		int seed;
		set<Tile*> walls; // must only be populated after city is finalized.
		
	public:
		ZBRA();
		~ZBRA();
		
		void recursiveWiggle(int x, int y);//shift own, subarea, and adjacent.
		
		//The functions that will help determine what this type of ZBRA will contain
		ZBRA* City();
		ZBRA* House(int randSeed);
		ZBRA* BathRoom(int randSeed);
		ZBRA* Kitchen(int randSeed);
		ZBRA* LivingRoom(int randSeed);
		ZBRA* BedRoom(int randSeed);
		ZBRA* HouseHallway(int randSeed);
		
	private:
		int getMinSize();	//Gets the sizes of this->subArea
		int getIdealSize();
		void solveRecursive();
		void buildMap(Tile* superParent);//my parent's reference tile.
		void AddFop(FOP* fop, Tile* tile); //builds list of fops contained in a tile
		void AddFop(FOP* fop); //builds list of fops contained in this ZBRA
		void printDimsRecursive();
		void findWalls(Tile* cityRef, set<Tile*> pWalls);//cityRef we can go to the proper tile set
		void addDoors();
};

#endif

