#ifndef _DIMSOLV_
#define _DIMSOLV_

#include <vector>
#include <math.h>


using namespace std;

class ZBRA;
struct bounds {
		int x, y, width, height, size;
	};

class DimSolv{
	private:
		static const short int PREFER_CCW = 1, PREFER_CW = 2;

	public:
		vector<bounds*> avail;
	
	private:
		//int sqsize;
	
	public:
		DimSolv(ZBRA* solveme, bool asAdjacent = false, ZBRA* parent = NULL);
		
	private:
		void standardSolver(ZBRA* solveme, bool asAdjacent = false, ZBRA* parent = NULL);
		void longSolver(ZBRA* solveme, bool asAdjacent = false, ZBRA* parent = NULL);
		void positionChildren(ZBRA* solveme, vector<ZBRA*> children);
		void amalgamateAdjacent(ZBRA* solveme, vector<ZBRA*> adjacent, bool asAdjacent);
		void putTop(vector<ZBRA*> toAdd, ZBRA* solveme, int availOrient);
		void putBottom(vector<ZBRA*> toAdd, ZBRA* solveme, int availOrient);
		void putLeft(vector<ZBRA*> toAdd, ZBRA* solveme, int availOrient);
		void putRight(vector<ZBRA*> toAdd, ZBRA* solveme, int availOrient);
		void toBounds(ZBRA* solveme, vector<ZBRA*> children);
		void getEffDims(ZBRA* solveme);
		void wiggle(vector<ZBRA*> toMove, int xOff, int yOff);
		void setSpot(ZBRA* setSpot, int x, int y);
		void setEffSpot(ZBRA* setSpot, int x, int y);
};

#endif