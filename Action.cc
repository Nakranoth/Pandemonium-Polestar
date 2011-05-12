//This is the file that contains any of the
//actions that can be performed by a FOP in the game

#include "Action.h"
#include "FOP.h"

#include <iostream>

Action::Action()
{
}

Action::~Action()
{
}

void Action::Sit(FOP* fop1, FOP* fop2)
{
	printf("You sit.\n");
}

void Action::Sleep(FOP* fop1, FOP* fop2)
{
	printf("You decide to sleep...\n");
}

void Action::Open(FOP* fop1, FOP* fop2)
{
	printf("You open the object.\n");
}

void Action::Pet(FOP* fop1, FOP* fop2)
{
	printf("You pet the cat.\n");
}

void Action::TurnOn(FOP* fop1, FOP* fop2)
{
	printf("You turn on the object.\n");
}
