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
	character = city.map->ref->fops[0];

	//setting running to true
	running = true;

	//setting up testing x and y velocities for holding down keys use
	charXvel = 0;
	charYvel = 0;

	//Telling SDL to initialize everything it has...
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
        	return false;
    	}

	//Setting the Icon of the window
	Uint32 colorkey;
	icon = SDL_LoadBMP("icon.bmp"); //loading bmp
	colorkey = SDL_MapRGB(icon->format, 255, 255, 255); //setting the color for our transparency in a colorkey
	SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey); //making the image set to the colorkey
	SDL_WM_SetIcon(icon,NULL);

	//Setting the title of the window
	SDL_WM_SetCaption("Pandemonium Polestar","Pandemonium Polestar");

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

	CHARACTER = NULL;
	char file2[] = "./FOPS/char.bmp";
	if((CHARACTER = SurfaceLoader::LoadImage(file2)) == NULL) {
		return false;
	}
	colorkey = SDL_MapRGB(CHARACTER->format, 255, 255, 255);
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
	checkCollision();

	//Move the character based on its velocity (can be 0)
	character->x += charXvel;
	character->y += charYvel;
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
	SurfaceLoader::DrawImage(screen, CHARACTER, 320 - (character->width / 2), 240 - (character->length / 2), 0, 0, Tile::SIZE, Tile::SIZE);

	//Draw the location tile of the character (should be under character)
	SurfaceLoader::DrawImage(screen, TILES, (character->location->x)*Tile::SIZE + (320 - character->x), (character->location->y)*Tile::SIZE + (240 - character->y), 120, 30, Tile::SIZE, Tile::SIZE);
	

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
	for(unsigned int i = 0; i < ref->fops.size(); i++)
	{
		RenderFOP(ref->fops[i]);
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

void GUI::RenderFOP(FOP* fop)
{
	SurfaceLoader::DrawImage(screen, fop->image, (fop->x) + (320 - character->x), (fop->y) + (320 - character->y));
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
		charYvel -= 2;
	}
	//DOWN
	if(sym == 274)
	{
		charYvel += 2;
	}
	//LEFT
	if(sym == 276)
	{
		charXvel -= 2;
	}
	//RIGHT
	if(sym == 275)
	{
		charXvel += 2;
	}
	//ESCAPE
	if(sym == 27)
	{
		onExit();
	}
	//character presses button, we want to do a check for objects
	//Right CTRL
	if(sym == 30)
	{
		
	}
}

void GUI::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	//UP
	if(sym == 273)
	{
		charYvel = 0;
	}
	//DOWN
	if(sym == 274)
	{
		charYvel = 0;
	}
	//LEFT
	if(sym == 276)
	{
		charXvel = 0;
	}
	//RIGHT
	if(sym == 275)
	{
		charXvel = 0;
	}
	//ESCAPE
	if(sym == 27)
	{
		onExit();
	}
}

bool GUI::checkCollision()
{
	//North
	if(charYvel < 0 && character->location->type == Tile::WALL && (character->y - character->length/2) < character->location->y)
	{
		character->y -= charYvel;
		charYvel = 0;
		return true;
	}
	//South
	if(charYvel > 0 && character->location->type == Tile::WALL && (character->y + character->length/2) > character->location->y)
	{
		character->y -= charYvel;
		charYvel = 0;
		return true;
	}
	//East
	if(charXvel > 0 && character->location->type == Tile::WALL && (character->x + character->width/2) > character->location->x)
	{
		character->x -= charXvel;
		charXvel = 0;
		return true;
	}
	//West
	if(charXvel < 0 && character->location->type == Tile::WALL && (character->x - character->width/2) < character->location->x)
	{
		character->x -= charXvel;
		charXvel = 0;
		return true;
	}

	return false;
}
