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
			RenderMap(character->location); //city.map->ref taken out for rendering around character...
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

	background = NULL;
	if((background = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 32, NULL, NULL, NULL, NULL)) == NULL)
	{
        	return false;
    	}

	foreground = NULL;
	if((foreground = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 640, 480, 32, NULL, NULL, NULL, NULL)) == NULL)
	{
        	return false;
    	}

	SurfaceLoader::setTransparency(foreground,RED,GREEN,BLUE);

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

	SurfaceLoader::setTransparency(CHARACTER, RED, GREEN, BLUE);	

	//sets up the ZBRA 'city' to be a city
	city.City();
	character = city.map->ref->fops[0];

	//setting up testing x and y velocities for holding down keys use
	character->Xvel = 0;
	character->Yvel = 0;

	character->maxFrames = 5;
	character->oscillate = false;

	//setting up the initial render distance...could change for certain unlit areas 
	renderdistance = 11;

	//setting running to true
	running = true;

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
	moveFOP(character);

	//from this point you can go through a list of moveable FOPs and call moveFOP on them
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
		case Tile::WALL:
			SurfaceLoader::DrawImage(background, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 90, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::FLOOR:
			SurfaceLoader::DrawImage(background, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 120, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::ROAD:
			SurfaceLoader::DrawImage(background, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 60, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::GRASS:
			SurfaceLoader::DrawImage(background, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 30, 0, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::DOOR:
			SurfaceLoader::DrawImage(background, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 0, 30, Tile::SIZE, Tile::SIZE);
			break;
		
		case Tile::KITCHEN:
			SurfaceLoader::DrawImage(background, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 30, 30, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::BATHROOM:
			SurfaceLoader::DrawImage(background, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 60, 30, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::BEDROOM:
			SurfaceLoader::DrawImage(background, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 90, 30, Tile::SIZE, Tile::SIZE);
			break;

		case Tile::HOUSE:
			SurfaceLoader::DrawImage(background, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 0, 60, Tile::SIZE, Tile::SIZE);
			break;

		default:
		case Tile::UDEF:
			SurfaceLoader::DrawImage(background, TILES, (ref->x)*Tile::SIZE + (320 - character->x),
						(ref->y)*Tile::SIZE + (240 - character->y), 0, 0, Tile::SIZE, Tile::SIZE);
			break;
	}

	//Draw the character everytime
	SurfaceLoader::DrawImage(foreground, CHARACTER, 320 - (character->width / 2), 240 - (character->length / 2), character->getCurrentFrameOffset()*character->length, character->getCurrentFrame()*character->length, character->width, character->length);

	//Draw the location tile of the character (should be under character)
	//SurfaceLoader::DrawImage(background, TILES, (character->location->x)*Tile::SIZE + (320 - character->x), (character->location->y)*Tile::SIZE + (240 - character->y), 120, 30, Tile::SIZE, Tile::SIZE);
	
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
	SDL_FreeSurface(background);
	SDL_FreeSurface(foreground);
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
	SDL_FillRect(background,NULL,0);
	SDL_FillRect(foreground,NULL,SDL_MapRGBA(foreground->format,RED,GREEN,BLUE,255));

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

		//check to see if it is too far from the player to render
		if(abs(character->location->x - curr->x) > renderdistance || abs(character->location->y - curr->y) > renderdistance)
		{
			isRendered.insert(curr);
			toRender.erase(it);
			continue;
		}
		else
		{
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
		}

		Render(curr);
		isRendered.insert(curr);
		toRender.erase(it);
	}

	draw.x = 0;
	draw.y = 0;

	SDL_BlitSurface(background, NULL, screen, NULL);
	SDL_BlitSurface(foreground, NULL, screen, NULL);

	//SurfaceLoader::DrawImage(screen, background, 0, 0);
	//SurfaceLoader::DrawImage(screen, foreground, 0, 0);


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
	unsigned int i;
	for(i=0; i<(tile->fops.size()); i++)
	{
		SurfaceLoader::DrawImage(foreground, tile->fops[i]->image, ((tile->fops[i]->x) - (tile->fops[i]->width/2))+ (320 - character->x), ((tile->fops[i]->y) - (tile->fops[i]->length/2))+ (240 - character->y));
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
		character->Yvel -= SPEED;
	}
	//DOWN
	if(sym == SDLK_DOWN)
	{
		character->Yvel += SPEED;
	}
	//LEFT
	if(sym == SDLK_LEFT)
	{
		character->Xvel -= SPEED;
	}
	//RIGHT
	if(sym == SDLK_RIGHT)
	{
		character->Xvel += SPEED;
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
		//cout<<"We want to interact!\n";	
		breakhere();

		//Check the tile in front of you (based on your orientation aka your frame 0=down, 1=up, 2=left, 3=right)
		//If there is an object, do its action
		vector<FOP*>::iterator fop;
		switch(character->getCurrentFrameOffset())
		{
			case 0:
				//For every fop within that tile
				for (fop = character->location->south->fops.begin(); fop < character->location->south->fops.end(); fop++)
				{
					//do an action
					(*fop)->doAction(character, *fop);
				}
				break;
			case 1:
				//For every fop within that tile
				for (fop = character->location->north->fops.begin(); fop < character->location->north->fops.end(); fop++)
				{
					//do an action
					(*fop)->doAction(character, *fop);
				}
				break;
			case 2:
				//For every fop within that tile
				for (fop = character->location->west->fops.begin(); fop < character->location->west->fops.end(); fop++)
				{
					//do an action
					(*fop)->doAction(character, *fop);
				}
				break;
			case 3:
				//For every fop within that tile
				for (fop = character->location->east->fops.begin(); fop < character->location->east->fops.end(); fop++)
				{
					//do an action
					(*fop)->doAction(character, *fop);
				}
				break;
			default:
				//For every fop within that tile (shouldn't get here, but just incase assume 0)
				for (fop = character->location->south->fops.begin(); fop < character->location->south->fops.end(); fop++)
				{
					//do an action
					(*fop)->doAction(character, *fop);
				}
				break;
		}
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
 *moveFOP()
 *
 *This function is called whenever we
 *want to do all the actions necessary
 *for a FOP to move, such as collision
 *checking, movement of its location
 *tile, starting or stopping its
 *animation squence, etc.
 *************************************/
void GUI::moveFOP(FOP* fop)
{
	//Moving the x/y tile location of the character to keep track of him
	//in terms of which tile of the map he's on
	if(fop->x / Tile::SIZE < fop->location->x)
	{
		fop->location = fop->location->west;
	}
	if(fop->x / Tile::SIZE > fop->location->x)
	{
		fop->location = fop->location->east;
	}
	if(fop->y / Tile::SIZE < fop->location->y)
	{
		fop->location = fop->location->north;
	}
	if(fop->y / Tile::SIZE > fop->location->y)
	{
		fop->location = fop->location->south;
	}

	//check for any collisions that would result in a stopping
	//of the velocity before moving the player
	checkCollision(fop);

	//Move the character based on its velocity (can be 0)
	fop->x += fop->Xvel;
	fop->y += fop->Yvel;

	if(fop->Xvel != 0 || fop->Yvel != 0)
	{
		//Move the animation for the character whenever he moves
		fop->startAnimation();
	}
	else
	{
		fop->stopAnimation();
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
	//check North tile
	if(fop->Yvel < 0)
	{
		if(fop->location->north->type == Tile::WALL && (fop->y - fop->length/2 + fop->Yvel) < fop->location->y*Tile::SIZE)
		{
			fop->Yvel = 0;
		}
		//Do an extra check for horizontals (so your not moving partially into a side wall as you move up)
		//check the fop against the fop's northwest using the fop's left boundary and top boundary
		else if(checkCollisionCorners(fop, fop->location->north->west, (fop->x - fop->width/2) + fop->Xvel, (fop->y - fop->width/2) + fop->Yvel))
		{
			//adjust the character into the proper spot to go past the corner
			fop->x = (fop->location->x)*Tile::SIZE + (fop->width/2);
		}
		//check the fop against the fop's northeast using the fop's right boundary and top boundary
		else if(checkCollisionCorners(fop, fop->location->north->east, (fop->x + fop->width/2) + fop->Xvel, (fop->y - fop->width/2) + fop->Yvel))
		{
			//adjust the character into the proper spot to go past the corner
			fop->x = (fop->location->x)*Tile::SIZE + (fop->width/2);
		}
		else if(checkFopCollision(fop->location->north, character))
		{
			fop->Yvel = 0;
		}
	}



	//South
	if(fop->Yvel > 0)
	{
		if(fop->location->south->type == Tile::WALL && (fop->y + fop->length/2 + fop->Yvel) > fop->location->south->y*Tile::SIZE)
		{
			fop->Yvel = 0;
		}
		//check the fop against the fop's southwest using the fop's left boundary and bottom boundary
		else if(checkCollisionCorners(fop, fop->location->south->west, (fop->x - fop->width/2) + fop->Xvel, (fop->y + fop->width/2) + fop->Yvel))
		{
			//adjust the character into the proper spot to go past the corner
			fop->x = (fop->location->x)*Tile::SIZE + (fop->width/2);
		}
		//check the fop against the fop's southeast using the fop's right boundary and bottom boundary
		else if(checkCollisionCorners(fop, fop->location->south->east, (fop->x + fop->width/2) + fop->Xvel, (fop->y + fop->width/2) + fop->Yvel))
		{
			//adjust the character into the proper spot to go past the corner
			fop->x = (fop->location->x)*Tile::SIZE + (fop->width/2);
		}
		else if(checkFopCollision(fop->location->south, character))
		{
			fop->Yvel = 0;
		}
	}



	//East
	if(fop->Xvel > 0)
	{
		if(fop->location->east->type == Tile::WALL && (fop->x + fop->width/2 + fop->Xvel) > fop->location->east->x*Tile::SIZE)
		{
			fop->Xvel = 0;
		}
		//check the fop against the fop's eastnorth using the fop's right boundary and top boundary
		else if(checkCollisionCorners(fop, fop->location->east->north, (fop->x + fop->width/2) + fop->Xvel, (fop->y - fop->width/2) + fop->Yvel))
		{
			//adjust the character into the proper spot to go past the corner
			fop->y = (fop->location->y)*Tile::SIZE + (fop->length/2);
		}
		//check the fop against the fop's eastsouth using the fop's right boundary and bottom boundary
		else if(checkCollisionCorners(fop, fop->location->east->south, (fop->x + fop->width/2) + fop->Xvel, (fop->y + fop->width/2) + fop->Yvel))
		{
			//adjust the character into the proper spot to go past the corner
			fop->y = (fop->location->y)*Tile::SIZE + (fop->length/2);
		}
		else if(checkFopCollision(fop->location->east, character))
		{
			fop->Xvel = 0;
		}
	}



	//West
	if(fop->Xvel < 0)
	{
		if(fop->location->west->type == Tile::WALL && (fop->x - fop->width/2 + fop->Xvel) < fop->location->x*Tile::SIZE)
		{
			fop->Xvel = 0;
		}
		//check the fop against the fop's westnorth using the fop's left boundary and top boundary
		else if(checkCollisionCorners(fop, fop->location->west->north, (fop->x - fop->width/2) + fop->Xvel, (fop->y - fop->width/2) + fop->Yvel))
		{
			//adjust the character into the proper spot to go past the corner
			fop->y = (fop->location->y)*Tile::SIZE + (fop->length/2);
		}
		//check the fop against the fop's westsouth using the fop's left boundary and bottom boundary
		else if(checkCollisionCorners(fop, fop->location->west->south, (fop->x - fop->width/2) + fop->Xvel, (fop->y + fop->width/2) + fop->Yvel))
		{
			//adjust the character into the proper spot to go past the corner
			fop->y = (fop->location->y)*Tile::SIZE + (fop->length/2);
		}
		else if(checkFopCollision(fop->location->west, character))
		{
			fop->Xvel = 0;
		}
	}

	return false;
}

bool GUI::checkCollisionCorners(FOP* fop, Tile* corner, int charXBoundary, int charYBoundary)
{
	if(corner->type == Tile::WALL && charXBoundary < corner->x*Tile::SIZE + Tile::SIZE && charXBoundary > corner->x*Tile::SIZE && charYBoundary < corner->y*Tile::SIZE + Tile::SIZE && charYBoundary > corner->y*Tile::SIZE)
	{
		//cout<<"HALFWAY DETECT\n";
		return true;
	}
	return false;
}

/*******************************************
 *checkFopCollision()
 *
 *This function takes in a tile that is being
 *checked for collision and grabs its list of
 *fops in order to see if any of them collide
 *with the character/fop moving into that tile.
 *If it returns true, it will stop the moving
 *FOP's movement.
 *******************************************/
bool GUI::checkFopCollision(Tile* foptile, FOP* movingfop)
{
	vector<FOP*>::iterator fop;
	//declaring and initializing the bounds of the moving FOP we are checking with
	int leftbound = (movingfop->x - movingfop->width/2) + movingfop->Xvel;
	int rightbound = (movingfop->x + movingfop->width/2) + movingfop->Xvel;
	int topbound = (movingfop->y - movingfop->length/2) + movingfop->Yvel;
	int bottombound = (movingfop->y + movingfop->length/2) + movingfop->Yvel;

	//declaring the bounds of the FOPs in the tile
	int fopleft, fopright, foptop, fopbottom = 0;

	//declaring all of the tiles we will check (within a radius of the character)
	int arraysize = 9;
	Tile* tiles[arraysize];
	tiles[0] = movingfop->location;
	tiles[1] = movingfop->location->north;
	tiles[2] = movingfop->location->north->east;
	tiles[3] = movingfop->location->east;
	tiles[4] = movingfop->location->east->south;
	tiles[5] = movingfop->location->south;
	tiles[6] = movingfop->location->south->west;
	tiles[7] = movingfop->location->west;
	tiles[8] = movingfop->location->west->north;
	tiles[9] = movingfop->location->north->north;

	//If we ever needed a radius of 2 around the character to check for fops then uncomment this
	/*tiles[10] = movingfop->location->north->north->east;
	tiles[11] = movingfop->location->north->north->east->east;
	tiles[12] = movingfop->location->north->east->east;
	tiles[13] = movingfop->location->east->east;
	tiles[14] = movingfop->location->east->east->south;
	tiles[15] = movingfop->location->east->east->south->south;
	tiles[16] = movingfop->location->east->south->south;
	tiles[17] = movingfop->location->south->south;
	tiles[18] = movingfop->location->south->south->west;
	tiles[19] = movingfop->location->south->south->west->west;
	tiles[20] = movingfop->location->south->west->west;
	tiles[21] = movingfop->location->west->west;
	tiles[22] = movingfop->location->west->west->north;
	tiles[23] = movingfop->location->west->west->north->north;
	tiles[24] = movingfop->location->west->north->north;*/

	//For every tile around the character
	for(int i = 0; i < arraysize; i++)
	{
		//For every fop within that tile
		for (fop = foptile->fops.begin(); fop < foptile->fops.end(); fop++)
		{
			fopleft = ((*fop)->x - (*fop)->width/2);
			fopright = ((*fop)->x + (*fop)->width/2);
			foptop = ((*fop)->y - (*fop)->length/2);
			fopbottom = ((*fop)->y + (*fop)->length/2);

			if( (leftbound <= fopright && rightbound >= fopleft) && (topbound <= fopbottom && bottombound >= foptop) )
			{
				return true;
			}
		}
	}

	return false;
}
