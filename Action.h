#ifndef _Actions_
#define _Actions_

class FOP;

using namespace std;

class Action
{
	public:
		static const short int SIT = 1, SLEEP = 2, OPEN = 3, PET = 4;

	public:
		Action();
		~Action();
		
		//The functions that will help determine what this type of FOP will be
		void Sit(FOP* fop1, FOP* fop2);
		void Sleep(FOP* fop1, FOP* fop2);
		void Open(FOP* fop1, FOP* fop2);
		void Pet(FOP* fop1, FOP* fop2);
};

#endif

