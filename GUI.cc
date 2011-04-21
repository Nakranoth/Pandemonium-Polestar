#include "GUI.h"

/*************************************
 *onExecute()
 *
 *This function is called from main
 *to start the game.
 *************************************/
int GUI::onExecute()
{
	//Initialize stuff and make sure it is done correctly
	if(Initialize() == 0)
	{
		return -1;
	}

	//Enter the main game loop
	while(running)
	{
		//Poll for events from the user
		while(SDL_PollEvent(&Event))
		{
            		EventHandler(&Event);
        	}
		
		Logic();
		Render(city.map->ref);
	}

	//Before exiting take care of any cleaning up
	Cleanup();

	return 0;
}

/*************************************
 *Initialize()
 *
 *This function initializes variables
 *to be used in the game.
 *************************************/
bool GUI::Initialize()
{
	//sets up the ZBRA 'city' to be a city
	city.City();

	//setting running to true
	running = true;

	//Telling SDL to initialize everything it has...
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
        	return false;
    	}

	//attempting to set the screen's properties, exit with error otherwise
	//initializing the screen to null
	screen = NULL;
	if((screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
	{
        	return false;
    	}

	GRASS = NULL;
	if((GRASS = SurfaceLoader::LoadImage("./grass.bmp")) == NULL) {
		return false;
	}

	return true;
}

/*************************************
 *EventHandler()
 *
 *This function is called to listen
 *for any events input from the user
 *and handle them accordingly.
 *************************************/
void GUI::EventHandler(SDL_Event* Event)
{
	if(Event->type == SDL_QUIT)
	{
        	running = false;
    	}
}

/*************************************
 *Logic()
 *
 *This function handles the logic and
 *data manipulation of gameplay.
 *************************************/
void GUI::Logic()
{

}

/*************************************
 *Render()
 *
 *This function is called to render
 *stuff to the screen.
 *************************************/
void GUI::Render(Tile* ref)
{
	while(ref != NULL)
	{
		SurfaceLoader::DrawImage(screen, GRASS, (ref->x)*Tile::SIZE, (ref->y)*Tile::SIZE, 0, 0, Tile::SIZE, Tile::SIZE);
		ref = ref->east;
	}

	SDL_Flip(screen);

}

/*************************************
 *Cleanup()
 *
 *This function takes care of anything
 *that needs to be done before cleanly
 *exiting the game.
 *************************************/
void GUI::Cleanup()
{
	SDL_FreeSurface(screen);
	SDL_FreeSurface(GRASS);
	SDL_Quit();
}

/*************************************
 *outputMap()
 *
 *This function uses a reference tile
 *to render the rest of the map.
 *************************************/
void GUI::outputMap(Tile* refTile)
{
	//cout<<"WE WILL BE OUTPUTING THE MAP STARTING AT X = " << refTile->x << " and Y = " << refTile->y << endl;
	
}


