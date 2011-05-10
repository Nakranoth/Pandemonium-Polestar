#include "GUI.h"

#include "Map.h"

void breakhere();

/*************************************
 *breakhere()
 *
 *This function is solely for the
 *purpose of debugging and setting a
 *break point.
 *************************************/
void breakhere()
{
}

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
	character = city.map->ref->fops[0];

	//setting running to true
	running = true;

	//setting up testing x and y velocities for holding down keys use
	character->Xvel = 0;
	character->Yvel = 0;

	character->maxFrames = 5;
	character->oscillate = false;

	//Telling SDL to initialize everything it has...
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
        	return false;
    	}

	Uint32 colorkey; //holds the colorkey for transparencies when loading images

	//Setting the Icon of the window	
	char iconfile[] = "icon.bmp";
	if((ICON = SDL_LoadBMP(iconfile)) == NULL) {
		cerr << "Error in loading " << iconfile << "..program exit.\n";
		return false;
	}
	colorkey = SDL_MapRGB(ICON->format, 255, 255, 255); //setting the color for our transparency in a colorkey
	SDL_SetColorKey(ICON, SDL_SRCCOLORKEY, colorkey); //making the image set to the colorkey
	SDL_WM_SetIcon(ICON,NULL);

	//Setting the title of the window
	SDL_WM_SetCaption("Pandemonium Polestar","Pandemonium Polestar");

	//attempting to set the screen's properties, exit with error otherwise
	//initializing the screen to null
	screen = NULL;
	if((screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
	{
        	return false;
    	}

	//loading the tiles surface from the tiles.bmp to get the tile pictures from
	TILES = NULL;
	char file[] = "TILES/tiles.bmp";
	if((TILES = SurfaceLoader::LoadImage(file)) == NULL) {
		cerr << "Error in loading " << file << "...program exit.\n";
		return false;
	}

	//loading the character surface from the char.bmp to get the character's pictures from
	CHARACTER = NULL;
	char file2[] = "FOPS/char.bmp";
	if((CHARACTER = SurfaceLoader::LoadImage(file2)) == NULL) {
		cerr << "Error in loading " << file2 << "...program exit.\n";
		return false;
	}
	colorkey = SDL_MapRGB(CHARACTER->format, 255, 42, 113);
	SDL_SetColorKey(CHARACTER, SDL_SRCCOLORKEY, colorkey); //making the image transparency set to the colorkey

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

	//Moving the x/y tile location of the character to keep track of him
	//in terms of which tile of the map he's on
	if(character->x / Tile::SIZE < character->location->x)
	{
		character->location = character->location->west;
	}
	if(character->x / Tile::SIZE > character->location->x)
	{
		character->location = character->location->east;
	}
	if(character->y / Tile::SIZE < character->location->y)
	{
		character->location = character->location->north;
	}
	if(character->y / Tile::SIZE > character->location->y)
	{
		character->location = character->location->south;
	}

	//check for any collisions that would result in a stopping
	//of the velocity before moving the player
	checkCollision(character);

	//Move the character based on its velocity (can be 0)
	character->x += character->Xvel;
	character->y += character->Yvel;

	if(character->Xvel != 0 || character->Yvel != 0)
	{
		//Move the animation for the character whenever he moves
		character->startAnimation();
	}
	else
	{
		character->stopAnimation();
	}
}

/*************************************
 *Render()
 *
 *This function is called to render
 *stuff to the screen.
 *************************************/
void GUI::Render(Tile* ref)
{
	//Draw the character everytime
	SurfaceLoader::DrawImage(screen, CHARACTER, 320 - (character->width / 2), 240 - (character->length / 2), character->getCurrentFrameOffset()*character->length, character->getCurrentFrame()*character->length, character->width, character->length);

	//Draw the location tile of the character (should be under character)
	//SurfaceLoader::DrawImage(screen, TILES, (character->location->x)*Tile::SIZE + (320 - character->x), (character->location->y)*Tile::SIZE + (240 - character->y), 120, 30, Tile::SIZE, Tile::SIZE);
	

	//Load the correct image for the tile depending on the type it is
	switch(ref->type)
	{
		case Tile::WALL:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 90, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::FLOOR:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 120, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::ROAD:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 60, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::GRASS:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 30, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::DOOR:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 0, 30, Tile::SIZE, Tile::SIZE);
			break;
		
		case Tile::KITCHEN:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 30, 30, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::BATHROOM:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 60, 30, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::BEDROOM:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 90, 30, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::HOUSE:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 0, 60, Tile::SIZE, Tile::SIZE);
			break;

		default:
		case Tile::UDEF:
			SurfaceLoader::DrawImage(screen, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 0, 0, Tile::SIZE, Tile::SIZE);
			break;
	}

	//Draw all the FOPs of the tile being drawn after drawing the tile
	RenderFOPs(ref);
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
	SDL_FreeSurface(ICON);
	SDL_FreeSurface(CHARACTER);
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

/**************************************
 *RenderFOPs()
 *
 *This will be called whenever we must
 *render all FOPs for a certain tile. This
 *function will go through every fop of
 *a passed in tile and render it before
 *exiting to render more tiles/fops.
 **************************************/
void GUI::RenderFOPs(Tile* tile)
{
	for(unsigned int i=0; i<(tile->fops.size()); i++)
	{
		SurfaceLoader::DrawImage(screen, tile->fops[i]->image, (tile->fops[i]->x) + (320 - character->x), (tile->fops[i]->y) + (320 - character->y));
	}
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

/*************************************
 *OnKeyDown()
 *
 *This function handles what happens
 *when a key is pressed down.
 *************************************/
void GUI::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	//UP
	if(sym == SDLK_UP)
	{
		character->Yvel -= 2;
	}
	//DOWN
	if(sym == SDLK_DOWN)
	{
		character->Yvel += 2;
	}
	//LEFT
	if(sym == SDLK_LEFT)
	{
		character->Xvel -= 2;
	}
	//RIGHT
	if(sym == SDLK_RIGHT)
	{
		character->Xvel += 2;
	}
	//ESCAPE
	if(sym == SDLK_ESCAPE)
	{
		onExit();
	}
	//character presses button, we want to do a check for objects
	//Right CTRL
	if(sym == SDLK_f)
	{
		cout<<"We want to interact!\n";	
		breakhere();	
	}
}

/*************************************
 *OnKeyUp()
 *
 *This function handles what happens
 *when a key is released.
 *************************************/
void GUI::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	//UP
	if(sym == SDLK_UP)
	{
		character->Yvel = 0;
	}
	//DOWN
	if(sym == SDLK_DOWN)
	{
		character->Yvel = 0;
	}
	//LEFT
	if(sym == SDLK_LEFT)
	{
		character->Xvel = 0;
	}
	//RIGHT
	if(sym == SDLK_RIGHT)
	{
		character->Xvel = 0;
	}
	//ESCAPE
	if(sym == SDLK_ESCAPE)
	{
		onExit();
	}
}

/*************************************
 *checkCollision()
 *
 *This function is called whenever we
 *want to determine if there is a
 *collision with the character against
 *an object or a wall.
 *
 *NOTE: This could be changed to take
 *	in a FOP and be called for any
 *	collision regarding a FOP (as
 *	in other AI colliding with stuff)
 *************************************/
bool GUI::checkCollision(FOP* fop)
{
	//North
	if(fop->Yvel < 0 && fop->location->north->type == Tile::WALL && (fop->y - fop->length/2 + fop->Yvel) < fop->location->y*Tile::SIZE)
	{
		fop->Yvel = 0;
		return true;
	}
	//South
	if(fop->Yvel > 0 && fop->location->south->type == Tile::WALL && (fop->y + fop->length/2 + fop->Yvel) > fop->location->south->y*Tile::SIZE)
	{
		fop->Yvel = 0;
		return true;
	}
	//East
	if(fop->Xvel > 0 && fop->location->east->type == Tile::WALL && (fop->x + fop->width/2 + fop->Xvel) > fop->location->east->x*Tile::SIZE)
	{
		fop->Xvel = 0;
		return true;
	}
	//West
	if(fop->Xvel < 0 && fop->location->west->type == Tile::WALL && (fop->x - fop->width/2 + fop->Xvel) < fop->location->x*Tile::SIZE)
	{
		fop->Xvel = 0;
		return true;
	}

	return false;
}
