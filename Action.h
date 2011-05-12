#ifndef _Actions_
#define _Actions_

class FOP;

using namespace std;

class Action
{
	public:
		static const short int SIT = 1, SLEEP = 2, OPEN = 3, PET = 4, TURNON = 5;

	public:
		Action();
		~Action();
		
		//The functions that will help determine what this type of FOP will be
		static void Sit(FOP* fop1, FOP* fop2);
		static void Sleep(FOP* fop1, FOP* fop2);
		static void Open(FOP* fop1, FOP* fop2);
		static void Pet(FOP* fop1, FOP* fop2);
		static void TurnOn(FOP* fop1, FOP* fop2);
};

#endif

