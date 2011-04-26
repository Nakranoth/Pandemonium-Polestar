#ifndef _DIMSOLV_
#define _DIMSOLV_

#include <vector>
#include <math.h>
#include <time.h>

using namespace std;

class ZBRA;
struct bounds {
		int x1, y1, x2, y2, size;
	};

class DimSolv{
	public:
		vector<bounds*> avail;
	
	private:
		ZBRA* parent;
		bounds* pBounds;
		vector<ZBRA*> fChild;	//fitted children
		time_t initTime;
		int sqsize;
	
	public:
		DimSolv(ZBRA* solveme);
		
	private:
		void fidget(ZBRA* fitMe, bounds into, int sqsize);
		void calcBounds();
		void boundsPairReduction(bounds* coord, bounds* pBounds, vector<ZBRA*>::iterator match);
};

#endif