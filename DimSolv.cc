#include "DimSolv.h"
#include "ZBRA.h"

#include <iostream>

using namespace std;


DimSolv::DimSolv(ZBRA* solveme){
	if (solveme->subArea.empty()) return;
	parent = solveme;
	pBounds = &(parent->dims);
	
	initTime = time(NULL);
	srandom(initTime);
	
	if (!parent->fitted){
		pBounds->x1 = 0;
		pBounds->y1 = 0;
		//City's a square. All else are what they are.
		int siz = ceil(sqrt(solveme->idealSize));
		pBounds->x2 = pBounds->x1 + siz;
		pBounds->y2 = pBounds->y1 + siz;
	}
	
	pBounds->size = (pBounds->x2 - pBounds->x1) * (pBounds->y2 - pBounds->y1);
	avail.push_back(new bounds(*pBounds));
	
	//pBounds now represents the total bounds available for the area.
	//We will try to put the largest child into the "largest" available bounds.
	//If all children fit, we assume everyone can be populated.
	//Then we make the maps.
	//Otherwise, we cry.
	bool done;
	do{
		done = true;
		ZBRA* i = (*solveme->subArea.begin());
		for (vector<ZBRA*>::iterator test = solveme->subArea.begin(); test != solveme->subArea.end(); test++){
			if (i->fitted){
				i = *test;
			}
			else{
				done = false;
				if (!(*test)->fitted && (*test)->idealSize > i->idealSize){
					i = *test;
				}
			}
		}
		int size = i->idealSize;
		vector<bounds*>::iterator largest = avail.begin();
		for(vector<bounds*>::iterator j = avail.begin(); j != avail.end() && !i->fitted; j++){
			if (size < (*j)->size && (*j)->size < (size + (size / 10))){
				//Close enough, give it the space. 
				i->dims.x1 = (*j)->x1;
				i->dims.x2 = (*j)->x2;
				i->dims.y1 = (*j)->y1;
				i->dims.y2 = (*j)->y2;
				i->fitted = true;
				fChild.push_back(i);
				avail.erase(j);
			}
			if ((*largest)->size < (*j)->size && !i->fitted) largest = j;
		}
		if (!i->fitted){
			if ((*largest)->size < i->minSize){
				cerr << (*largest)->size << " < " << i->minSize;
				break;
			}
			//try to find a good fit, prefer the largest available space.
			//compare your square dimensions against the available space.
			sqsize = ceil(sqrt(i->idealSize));
			//x dimension is a good fit.
			if (((*largest)->x2 - (*largest)->x1 > sqsize && (*largest)->x2 - (*largest)->x1 < sqsize + (sqsize / 10)) || ((*largest)->x2 - (*largest)->x1 < sqsize && (*largest)->x2 - (*largest)->x1 + (((*largest)->x2 - (*largest)->x1) / 10) > sqsize)){
				i->dims.x1 = (*largest)->x1;
				i->dims.x2 = (*largest)->x2;
				int ysiz = ceil(i->idealSize / ((*largest)->x2 - (*largest)->x1));
				if (random() / (RAND_MAX / 2) == 1){
					i->dims.y1 = (*largest)->y1;
					i->dims.y2 = i->dims.y1 + ysiz; 
				}
				else{
					i->dims.y2 = (*largest)->y2;
					i->dims.y1 = i->dims.y2 - ysiz;
				}
				i->fitted = true;
			}
			//y dimension is a good fit.
			else if (((*largest)->y2 - (*largest)->y1 > sqsize && (*largest)->y2 - (*largest)->y1 < sqsize + (sqsize / 10)) || ((*largest)->y2 - (*largest)->y1 < sqsize && (*largest)->y2 - (*largest)->y1 + (((*largest)->y2 - (*largest)->y1) / 10) > sqsize)){
				i->dims.y1 = (*largest)->y1;
				i->dims.y2 = (*largest)->y2;
				int xsiz = ceil(i->idealSize / ((*largest)->y2 - (*largest)->y1));
				if (random() / (RAND_MAX / 2) == 1){
					i->dims.x1 = (*largest)->x1;
					i->dims.x2 = i->dims.x1 + xsiz;
				}
				else{
					i->dims.x2 = (*largest)->x2;
					i->dims.x1 = i->dims.x2 - xsiz;
					//(*largest)->x2 = i->dims.x1 - (i->wall == Tile::UDEF?1:2);
				}
				i->fitted = true;
			}
			else{	//pick a corner
				switch((random() - 1) / (RAND_MAX / 4)){	//-1 so if it's randmax, it's 3 not 4
					case 0:	//top left
						i->dims.x1 = (*largest)->x1;
						i->dims.y1 = (*largest)->y1;
						//Make it fit
						fidget(i,**largest, sqsize);
						break;
					case 1:	//bottom left
						i->dims.x1 = (*largest)->x1;
						i->dims.y2 = (*largest)->y2;
						fidget(i,**largest, sqsize);
						break;
					case 2:	//bottom right
						i->dims.x2 = (*largest)->x2;
						i->dims.y2 = (*largest)->y2;
						fidget(i,**largest, sqsize);
						break;
					case 3:	//top right
						i->dims.x2 = (*largest)->x2;
						i->dims.y1 = (*largest)->y1;
						fidget(i,**largest, sqsize);
						break;
					//i->fitted=true;
				}
			}
//cout << i->dims.x1 << ',' << i->dims.y1 << " -> " << i->dims.x2 << ',' << i->dims.y2 << " " << i->fitted << "\n\n";
			fChild.push_back(i);
			calcBounds();
		}
	}while (!done);
}

void DimSolv::fidget(ZBRA* fitMe, bounds into, int sqsize){
	int orient = 0;	//0 = 11/22 1 = 21/12, 2 = 12/21, 3 = 22/11;
	int width = sqsize, height = sqsize;
	orient += (fitMe->dims.x1 == into.x1)?0:1;
	orient += (fitMe->dims.y1 == into.y1)?0:2;
	
	
	switch(orient){
		case 0:	//x1,y1
			if (into.x1 + width <= into.x2 && into.y1 + height <= into.y2){
				fitMe->dims.x2 = into.x1 + width;
				fitMe->dims.y2 = into.y1 + height;
				fitMe->fitted = true;
			}
			else if (into.x1 + width > into.x2 && into.y1 + height > into.y2){
				fitMe->dims.x2 = into.x2;
				fitMe->dims.y2 = into.y2;
				fitMe->fitted = true;
			}
			else if (into.x1 + width > into.x2){
				fitMe->dims.x2 = into.x2;
				width = into.x2 - into.x1;
				while (width * height < fitMe->idealSize && into.y1 + height < into.y2) height++;
				fitMe->dims.y2 = into.y1 + height;
				fitMe->fitted = true;
			}
			else{
				fitMe->dims.y2 = into.y2;
				height = into.y2 - into.y1;
				while (width * height < fitMe->idealSize && into.x1 + width < into.x2) width++;
				fitMe->dims.x2 = into.x1 + width;
				fitMe->fitted = true;
			}
			break;
		case 1:	//x2,y1
			if (into.x2 - width >= into.x1 && into.y1 + height <= into.y2){
				fitMe->dims.x1 = into.x2 - width;
				fitMe->dims.y2 = into.y1 + height;
				fitMe->fitted = true;
			}
			else if (into.x2 - width < into.x1 && into.y1 + height > into.y2){
				fitMe->dims.x1 = into.x1;
				fitMe->dims.y2 = into.y2;
				fitMe->fitted = true;
			}
			else if (into.x2 - width < into.x1){
				fitMe->dims.x1 = into.x1;
				width = into.x2 - into.x1;
				while (width * height < fitMe->idealSize && into.y1 + height < into.y2) height++;
				fitMe->dims.y2 = into.y1 + height;
				fitMe->fitted = true;
			}
			else{
				fitMe->dims.y2 = into.y2;
				height = into.y2 - into.y1;
				while (width * height < fitMe->idealSize && into.x2 - width > into.x1) width++;
				fitMe->dims.x1 = into.x2 - width;
				fitMe->fitted = true;
			}
			break;
		case 2:	//x1, y2
			if (into.x1 + width <= into.x2 && into.y2 - height >= into.y1){
				fitMe->dims.x2 = into.x1 + width;
				fitMe->dims.y1 = into.y2 - height;
				fitMe->fitted = true;
			}
			else if (into.x1 + width > into.x2 && into.y2 - height < into.y1){
				fitMe->dims.x2 = into.x2;
				fitMe->dims.y1 = into.y1;
				fitMe->fitted = true;
			}
			else if (into.x1 + width > into.x2){
				fitMe->dims.x2 = into.x2;
				width = into.x2 - into.x1;
				while (width * height < fitMe->idealSize && into.y2 - height > into.y1) height++;
				fitMe->dims.y1 = into.y2 - height;
				fitMe->fitted = true;
			}
			else{
				fitMe->dims.y1 = into.y1;
				height = into.y2 - into.y1;
				while (width * height < fitMe->idealSize && into.x1 + width < into.x2) width++;
				fitMe->dims.x2 = into.x1 + width;
				fitMe->fitted = true;
			}
			fitMe->dims.size = (fitMe->dims.x2 - fitMe->dims.x1) * (fitMe->dims.y2 - fitMe->dims.y1);
			break;
		case 3:	//x2,y2
			if (into.x2 - width >= into.x1 && into.y2 - height >= into.y1){
				fitMe->dims.x1 = into.x2 - width;
				fitMe->dims.y1 = into.y2 - height;
				fitMe->fitted = true;
			}
			else if (into.x2 - width < into.x1 && into.y2 - height < into.y1){
				fitMe->dims.x1 = into.x1;
				fitMe->dims.y1 = into.y1;
				fitMe->fitted = true;
			}
			else if (into.x2 - width < into.x1){
				fitMe->dims.x1 = into.x1;
				width = into.x2 - into.x1;
				while (width * height < fitMe->idealSize && into.y2 - height > into.y1) height++;
				fitMe->dims.y1 = into.y2 - height;
				fitMe->fitted = true;
			}
			else{
				fitMe->dims.y1 = into.y1;
				height = into.y2 - into.y1;
				while (width * height < fitMe->idealSize && into.x2 - width > into.x1) width++;
				fitMe->dims.x1 = into.x2 - width;
				fitMe->fitted = true;
			}
			break;
		
	}
//cout << fitMe->dims.x1 << ',' << fitMe->dims.y1 << " -> " << fitMe->dims.x2 << ',' << fitMe->dims.y2 << " " << fitMe->fitted << endl;
}

void DimSolv::calcBounds(){
	avail.clear();	//clear to make sure no conflits.
	bounds* into = new bounds();
	into->x1 = pBounds->x1;
	into->y1 = pBounds->y1;
	into->x2 = pBounds->x2;
	into->y2 = pBounds->y2;
	
	boundsPairReduction(into, pBounds, (vector<ZBRA*>::iterator)NULL);
	
	//handle every other one.
	for (vector<ZBRA*>::iterator j = fChild.begin(); j != fChild.end(); j++){	
		into->x1 = (*j)->dims.x2;
		into->y1 = (*j)->dims.y1;
		boundsPairReduction(into, pBounds, j);
		into->x1 = (*j)->dims.x1;
		into->y1 = (*j)->dims.y2;
		boundsPairReduction(into, pBounds, j);
	}
	delete into;
}

void DimSolv::boundsPairReduction(bounds* coord, bounds* pBounds, vector<ZBRA*>::iterator j){
	bounds* into = new bounds(*coord);

	for (vector<ZBRA*>::iterator i = fChild.begin(); i != fChild.end(); i++){
		if (i != j){
			int wallCorr = ((*i)->wall != Tile::UDEF)?1:0;
			if (into->y1 >= (*i)->dims.y1 - wallCorr && into->y1 >= (*i)->dims.y2 + wallCorr && into->x2 > (*i)->dims.x1 - wallCorr && into->x1 < (*i)->dims.x2 + wallCorr){
				into->x2 = (*i)->dims.x1 - wallCorr - 1;
			}  
			else if (into->y1 <= (*i)->dims.y1 - wallCorr && into->y2 >= (*i)->dims.y1 - wallCorr && into->x2 >= (*i)->dims.x1 - wallCorr && into->x1 <= (*i)->dims.x2 - wallCorr){	//chop all the way from bottom
				into->y2 = (*i)->dims.y1 - wallCorr - 1;
			}
		}
	}
	if (into->x1 >= into->x2 || into->y1 >= into->y2) delete into;
	else{
		into->size = (into->x2 - into->x1) * (into->y2 - into->y1);
		avail.push_back(into);
	}	
	
	into = new bounds(*coord);	
		
	for (vector<ZBRA*>::iterator i = fChild.begin(); i != fChild.end(); i++){
		if (i != j){
			int wallCorr = ((*i)->wall != Tile::UDEF)?1:0;
			if (into->x1 >= (*i)->dims.x1 - wallCorr && into->x1 >= (*i)->dims.x2 + wallCorr && into->y2 > (*i)->dims.y1 - wallCorr && into->y1 < (*i)->dims.y2 + wallCorr){
				into->y2 = (*i)->dims.y1 - wallCorr - 1;
			}  
			else if (into->x1 <= (*i)->dims.x1 - wallCorr && into->x2 >= (*i)->dims.x1 - wallCorr && into->y2 >= (*i)->dims.y1 - wallCorr && into->y1 <= (*i)->dims.y2 - wallCorr){	//chop all the way from bottom
				into->x2 = (*i)->dims.x1 - wallCorr - 1;
			}
		}
	}
	if (into->x1 >= into->x2 || into->y1 >= into->y2) delete into;
	else{ //if(!avail.empty()){
		//if (into->x2 != (*avail.end())->x2 && into->y2 != (*avail.end())->y2){
			into->size = (into->x2 - into->x1) * (into->y2 - into->y1);
			avail.push_back(into);
		
	}
	//else delete into;
}