#include "GUI.h"

#include "Map.h"

void breakHere();

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
			RenderMap(city.map->ref);
			//breakHere();
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

	//setting up testing charx and chary positions....(observer persay)
	charX = 320;
	charY = 240;

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

	TILES = NULL;
	char file[] = "./tiles.bmp";
	if((TILES = SurfaceLoader::LoadImage(file)) == NULL) {
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
	Events::OnEvent(Event);
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
	//Load the correct image for the tile depending on the type it is
	switch(ref->type)
	{
		case Tile::UDEF:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE - charX + 320,
						(ref->y)*Tile::SIZE - charY + 240, 0, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::WALL:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE - charX + 320,
						(ref->y)*Tile::SIZE - charY + 240, 90, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::FLOOR:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE - charX + 320,
						(ref->y)*Tile::SIZE - charY + 240, 120, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::ROAD:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE - charX + 320,
						(ref->y)*Tile::SIZE - charY + 240, 60, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::GRASS:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE - charX + 320,
						(ref->y)*Tile::SIZE - charY + 240, 30, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::DOOR:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE - charX + 320,
						(ref->y)*Tile::SIZE - charY + 240, 0, 30, Tile::SIZE, Tile::SIZE);
			break;
	}
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
	SDL_FreeSurface(TILES);
	SDL_Quit();
}

/*************************************
 *outputMap()
 *
 *This function uses a reference tile
 *to render the rest of the map.
 *************************************/
void GUI::RenderMap(Tile* refTile)
{
	//Clear the screen
	SDL_FillRect(screen,NULL,0);

	//create a list of things yet to be rendered
	set<Tile*> toRender;
	toRender.insert(refTile);

	//create a list of things that have already been rendered
	set<Tile*> isRendered;

	//While there is still stuff to render...
	set<Tile*>::iterator it;//declared before loop to avoid mem leak.
	while(!toRender.empty())
	{
		it = toRender.begin();
		Tile* curr = *it;

		if (curr->north && !isRendered.count(curr->north)){
			toRender.insert(curr->north);
		}
		if (curr->west && !isRendered.count(curr->west)){
			toRender.insert(curr->west);
		}
		if (curr->south && !isRendered.count(curr->south)){
			toRender.insert(curr->south);
		}
		if (curr->east && !isRendered.count(curr->east)){
			toRender.insert(curr->east);
		}
			
		Render(curr);
		isRendered.insert(curr);
		toRender.erase(it);
	}



	//Will go after the loop finishing the output of
	//the map so that all tiles are rendered THEN output
	SDL_Flip(screen);
	
}

/*************************************
 *onExit()
 *
 *This function handles what happens
 *during the onExit event.
 *************************************/
void GUI::onExit()
{
	running = false;
}

void GUI::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	//UP
	if(sym == 273)
	{
		charY -= 10;
	}
	//DOWN
	if(sym == 274)
	{
		charY += 10;
	}
	//LEFT
	if(sym == 276)
	{
		charX -= 10;
	}
	//RIGHT
	if(sym == 275)
	{
		charX += 10;
	}
	//ESCAPE
	if(sym == 27)
	{
		onExit();
	}
}
