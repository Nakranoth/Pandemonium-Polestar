#include "DimSolv.h"
#include "ZBRA.h"

#include <iostream>

using namespace std;


DimSolv::DimSolv(ZBRA* solveme, bool asAdjacent, ZBRA* parent){
	if (solveme->flags & ZBRA::LONG){
		longSolver(solveme, asAdjacent, parent);
	}
	else{
		standardSolver(solveme, asAdjacent, parent);
	}
}

void DimSolv::standardSolver(ZBRA* solveme, bool asAdjacent, ZBRA* parent){	

	srandom(solveme->seed);
	
	//pick your own shape/size/whatnot.
	
	solveme->dims.x = 0;
	solveme->dims.y = 0;
	int sqsize = (solveme->flags & ZBRA::SOFT) ? max(solveme->idealSize/10,(int)ceil(sqrt(solveme->idealSize))) : ceil(sqrt(solveme->idealSize));
	int rand = random();
	//cerr << "sqsiz:" << sqsize << '\n';
	//cerr << rand << ">" << RAND_MAX / 2 << '=' << (rand > RAND_MAX / 2) <<'\n' << (long int)solveme << '\n';
	if (rand > RAND_MAX / 2){
		//square
		solveme->dims.width = sqsize;
		solveme->dims.height = sqsize;
	}
	else if (rand > RAND_MAX / 3){
		//1 unit off
		if (random() > RAND_MAX / 2){
			solveme->dims.width = sqsize + ceil(sqsize/10);
			solveme->dims.height = sqsize - ceil(sqsize/10);
			if(solveme->dims.width <= 1){
				solveme->dims.width = 2;
			}
			if(solveme->dims.height <= 1){
				solveme->dims.height = 2;
			}
			while(solveme->dims.width * solveme->dims.height < solveme->minSize){
				solveme->dims.height++;
			}
		}
		else{
			solveme->dims.width = sqsize - ceil(sqsize/10);
			solveme->dims.height = sqsize + ceil(sqsize/10);
			if(solveme->dims.width <= 1){
				solveme->dims.width = 2;
			}
			if(solveme->dims.height <= 1){
				solveme->dims.height = 2;
			}
			while(solveme->dims.width * solveme->dims.height < solveme->minSize){
				solveme->dims.width++;
			}
		}
	}
	else if (rand > RAND_MAX / 4){
		//2 units off
		if (random() > RAND_MAX / 2){
			solveme->dims.width = sqsize + ceil(2*sqsize/10);
			solveme->dims.height = sqsize - ceil(2*sqsize/10);
			if(solveme->dims.width <= 1){
				solveme->dims.width = 2;
			}
			if(solveme->dims.height <= 1){
				solveme->dims.height = 2;
			}
			while(solveme->dims.width * solveme->dims.height < solveme->minSize){
				solveme->dims.height++;
			}
		}
		else{
			solveme->dims.width = sqsize - ceil(2*sqsize/10);
			solveme->dims.height = sqsize + ceil(2*sqsize/10);
			if(solveme->dims.width <= 1){
				solveme->dims.width = 2;
			}
			if(solveme->dims.height <= 1){
				solveme->dims.height = 2;
			}
			while(solveme->dims.width * solveme->dims.height < solveme->minSize){
				solveme->dims.width++;
			}
		}
	}
	else if (rand > RAND_MAX / 5){
		//3 units off
		if (random() > RAND_MAX / 2){
			solveme->dims.width = sqsize + ceil(3*sqsize/10);
			solveme->dims.height = sqsize - ceil(3*sqsize/10);
			if(solveme->dims.width <= 1){
				solveme->dims.width = 2;
			}
			if(solveme->dims.height <= 1){
				solveme->dims.height = 2;
			}
			while(solveme->dims.width * solveme->dims.height < solveme->minSize){
				solveme->dims.height++;
			}				
		}
		else{
			solveme->dims.width = sqsize - ceil(3*sqsize/10);
			solveme->dims.height = sqsize + ceil(3*sqsize/10);
			if(solveme->dims.width <= 1){
				solveme->dims.width = 2;
			}
			if(solveme->dims.height <= 1){
				solveme->dims.height = 2;
			}
			while(solveme->dims.width * solveme->dims.height < solveme->minSize){
				solveme->dims.width++;
			}
		}
	}
	else{
		//4 units off
		if (random() > RAND_MAX / 2){
			solveme->dims.width = sqsize + ceil(4*sqsize/10);
			solveme->dims.height = sqsize - ceil(4*sqsize/10);
			if(solveme->dims.width <= 1){
				solveme->dims.width = 2;
			}
			if(solveme->dims.height <= 1){
				solveme->dims.height = 2;
			}
			while(solveme->dims.width * solveme->dims.height < solveme->minSize){
				solveme->dims.height++;
			}				
		}
		else{
			solveme->dims.width = sqsize - ceil(4*sqsize/10);
			solveme->dims.height = sqsize + ceil(4*sqsize/10);
			if(solveme->dims.width <= 1){
				solveme->dims.width = 2;
			}
			if(solveme->dims.height <= 1){
				solveme->dims.height = 2;
			}
			while(solveme->dims.width * solveme->dims.height < solveme->minSize){
				solveme->dims.width++;
			}			
		}
	}
	//cerr << solveme->dims.width << ":" << solveme->dims.height << '\n';
	solveme->dims.size = solveme->dims.width * solveme->dims.height;
	if (!solveme->adjacent.empty() || !solveme->subArea.empty()){
		//handle sticking together our children and adjacent.
		positionChildren(solveme, solveme->subArea);	//just gives them positions. Should be sortof based on my size compared to theirs.
		amalgamateAdjacent(solveme,solveme->adjacent,asAdjacent);//does what is says it will.
	}
	getEffDims(solveme);
}

void DimSolv::positionChildren(ZBRA* solveme, vector<ZBRA*> children){
	//figure out a smart way to put children into yourself
	//I need to care how many, and how large each is compared to me.
	
	//cycle through and calculate their adjacent's total quasi bounds
	//Should already be calculated
	/*for (vector<ZBRA*>::iterator i = children.begin(); i < children.end(); i++){
		setEffDims(*i);
	}*/
	
	//Trivial case
	if (children.size() < 1) return;
	
	//Easy case: soft, 1 child. Smallest xy to largest xy is mine... If i only have 1 child.
	if (children.size() == 1){
		if (solveme->flags & ZBRA::SOFT){
			toBounds(solveme, children);//I am my children's effective bounds
			//getEffDims(solveme);
			setSpot(solveme,0,0);
			return;
		}//else slightly harder case: !soft, 1 child
		if (children[0]->effDims.width > solveme->dims.width) solveme->dims.width = children[0]->effDims.width;
		if (children[0]->effDims.height > solveme->dims.height) solveme->dims.height = children[0]->effDims.height;
		//pick a corner.
		int rand = random();
		if(rand < RAND_MAX / 4){//top left
			setEffSpot(children[0],solveme->dims.x,solveme->dims.y);
		}
		else if (rand < RAND_MAX / 2){//top right
			setEffSpot(children[0],solveme->dims.x + solveme->dims.width - children[0]->dims.width ,solveme->dims.y);
		}
		else if (rand < RAND_MAX / 4 * 3){//carefully avoiding overflows
			setEffSpot(children[0],solveme->dims.x + solveme->dims.width - children[0]->dims.width ,solveme->dims.y + solveme->dims.height - children[0]->dims.height);
		}
		else{
			setEffSpot(children[0],solveme->dims.x,solveme->dims.y + solveme->dims.height - children[0]->dims.height);
		}
	}
	else{cerr << "Too many children to handle now";}
	
}

void DimSolv::amalgamateAdjacent(ZBRA* solveme, vector<ZBRA*> adjacent, bool asAdjacent){
	//asAdjacent means that we need to leave a side of myself available.
	for (vector<ZBRA*>::iterator i = adjacent.begin(); i != adjacent.end(); i++){
		DimSolv(*i,true, solveme);
	}
	int rand = random();
	int leftHeight = 0, rightHeight = 0, topWidth = 0, bottomWidth = 0;
	vector<ZBRA*> top, bottom, left, right;
	if (asAdjacent){
		//we need to "reserve" a spot for the adjacent piece.
	}
	for (vector<ZBRA*>::iterator i = adjacent.begin(); i != adjacent.end(); i++){
		if(rand < RAND_MAX / 4){	//Try top, bottom, left, right, expand.
			if (topWidth < solveme->dims.width && (topWidth + (*i)->dims.width <= solveme->dims.width || leftHeight < solveme->dims.height || rightHeight < solveme->dims.height)){
				topWidth += (*i)->dims.width;
				top.push_back(*i);
			}
			else if(bottomWidth < solveme->dims.width && (bottomWidth + (*i)->dims.width <= solveme->dims.width || leftHeight < solveme->dims.height || rightHeight < solveme->dims.height)){
				bottomWidth += (*i)->dims.width;
				bottom.push_back(*i);
			}
			else if(leftHeight < solveme->dims.height && (leftHeight + (*i)->dims.height <= solveme->dims.height || topWidth < solveme->dims.width || bottomWidth < solveme->dims.width)){
				leftHeight += (*i)->dims.width;
				left.push_back(*i);
			}
			else if(rightHeight < solveme->dims.height && (rightHeight + (*i)->dims.height <= solveme->dims.height || topWidth < solveme->dims.width || bottomWidth < solveme->dims.width)){
				rightHeight += (*i)->dims.width;
				right.push_back(*i);
			}
			else{
				solveme->dims.width = topWidth + (*i)->dims.width;
				topWidth += (*i)->dims.width;
				top.push_back(*i);
			}
			rand = random();
		}
		else if (rand < RAND_MAX / 2){//Try bottom, top, right, left, expand.
			if (bottomWidth < solveme->dims.width && (bottomWidth + (*i)->dims.width <= solveme->dims.width || leftHeight < solveme->dims.height || rightHeight < solveme->dims.height)){	
				bottomWidth += (*i)->dims.width;
				bottom.push_back(*i);
			}
			else if(topWidth < solveme->dims.width && (topWidth + (*i)->dims.width <= solveme->dims.width || leftHeight < solveme->dims.height || rightHeight < solveme->dims.height)){
				topWidth += (*i)->dims.width;
				top.push_back(*i);
			}
			else if(rightHeight < solveme->dims.height && (rightHeight + (*i)->dims.height <= solveme->dims.height || topWidth < solveme->dims.width || bottomWidth < solveme->dims.width)){
				rightHeight += (*i)->dims.width;
				right.push_back(*i);
			}
			else if(leftHeight < solveme->dims.height && (leftHeight + (*i)->dims.height <= solveme->dims.height || topWidth < solveme->dims.width || bottomWidth < solveme->dims.width)){
				leftHeight += (*i)->dims.width;
				left.push_back(*i);
			}
			else{
				solveme->dims.width = bottomWidth + (*i)->dims.width;
				bottomWidth += (*i)->dims.width;
				bottom.push_back(*i);
			}
			rand = random();
		}
		else if (rand < RAND_MAX / 4 * 3){ //Try left, right, top, bottom, expand.
			if(leftHeight < solveme->dims.height && (leftHeight + (*i)->dims.height <= solveme->dims.height || topWidth < solveme->dims.width || bottomWidth < solveme->dims.width)){
				leftHeight += (*i)->dims.width;
				left.push_back(*i);
			}
			else if(rightHeight < solveme->dims.height && (rightHeight + (*i)->dims.height <= solveme->dims.height || topWidth < solveme->dims.width || bottomWidth < solveme->dims.width)){
				rightHeight += (*i)->dims.width;
				right.push_back(*i);
			}
			else if (topWidth < solveme->dims.width && (topWidth + (*i)->dims.width <= solveme->dims.width || leftHeight < solveme->dims.height || rightHeight < solveme->dims.height)){
				topWidth += (*i)->dims.width;
				top.push_back(*i);
			}
			else if(bottomWidth < solveme->dims.width && (bottomWidth + (*i)->dims.width <= solveme->dims.width || leftHeight < solveme->dims.height || rightHeight < solveme->dims.height)){
				bottomWidth += (*i)->dims.width;
				bottom.push_back(*i);
			}
			else{
				solveme->dims.height = leftHeight + (*i)->dims.height;
				leftHeight += (*i)->dims.width;
				left.push_back(*i);
			}
			rand = random();
		}
		
		else {	//right, left, bottom, top, expand.
			if(rightHeight < solveme->dims.height && (rightHeight + (*i)->dims.height <= solveme->dims.height || topWidth < solveme->dims.width || bottomWidth < solveme->dims.width)){
				rightHeight += (*i)->dims.width;
				right.push_back(*i);
			}
			else if(leftHeight < solveme->dims.height && (leftHeight + (*i)->dims.height <= solveme->dims.height || topWidth < solveme->dims.width || bottomWidth < solveme->dims.width)){
				leftHeight += (*i)->dims.width;
				left.push_back(*i);
			}
			else if(bottomWidth < solveme->dims.width && (bottomWidth + (*i)->dims.width <= solveme->dims.width || leftHeight < solveme->dims.height || rightHeight < solveme->dims.height)){
				bottomWidth += (*i)->dims.width;
				bottom.push_back(*i);
			}
			else if (solveme->dims.width < topWidth && (topWidth + (*i)->dims.width <= solveme->dims.width || leftHeight < solveme->dims.height || rightHeight < solveme->dims.height)){
				topWidth += (*i)->dims.width;
				top.push_back(*i);
			}
			else{
				solveme->dims.height = rightHeight + (*i)->dims.height;
				rightHeight += (*i)->dims.width;
				right.push_back(*i);
			}
			rand = random();
		}
	}
	bool clockwise = (rand > RAND_MAX / 2 );
	if (top.size()){
		if (leftHeight > solveme->dims.height && clockwise){
			putTop(top,solveme,PREFER_CCW);
		}
		else if (rightHeight > solveme->dims.height && !clockwise){
			putTop(top,solveme,PREFER_CW);
		}
		else{
			putTop(top,solveme,0);
		}
	}
	if (bottom.size()){
		if (leftHeight > solveme->dims.height && !clockwise){
			putBottom(bottom,solveme,PREFER_CW);
		}
		else if (rightHeight > solveme->dims.height && clockwise){
			putBottom(bottom,solveme,PREFER_CCW);
		}
		else{
			putBottom(bottom,solveme,0);
		}
	}
	if (left.size()){
		if (bottomWidth > solveme->dims.width && clockwise){
			putLeft(left,solveme,PREFER_CCW);
		}
		else if (topWidth > solveme->dims.width && !clockwise){
			putLeft(left,solveme,PREFER_CW);
		}
		else{
			putLeft(left,solveme,0);
		}
	}
	if (right.size()){
		if (bottomWidth > solveme->dims.width && !clockwise){
			putRight(right,solveme,PREFER_CW);
		}
		else if (topWidth > solveme->dims.width && clockwise){
			putRight(right,solveme,PREFER_CCW);
		}
		else{
			putRight(right,solveme,0);
		}
	}
}
void DimSolv::longSolver(ZBRA* solveme, bool asAdjacent, ZBRA* parent){ standardSolver(solveme,asAdjacent,parent);}

void DimSolv::putTop(vector<ZBRA*> toAdd, ZBRA* solveme, int availOrient){
	//connect the set left/right wise, joined by bottom corners.
	int rand = random();
	int left = (*toAdd.begin())->dims.x;
	int right = (*toAdd.begin())->dims.x + (*toAdd.begin())->dims.width;
	ZBRA* leftMost;
	ZBRA* rightMost;
	leftMost = rightMost = *toAdd.begin();  
	setEffSpot(*toAdd.begin(),solveme->dims.x,solveme->dims.y + (*toAdd.begin())->dims.height);
	for (vector<ZBRA*>::iterator i = toAdd.begin() + 1; i != toAdd.end(); i++){
		if (rand < RAND_MAX / 2){
			setEffSpot(*i, left - (*i)->dims.width, solveme->dims.y + (*i)->dims.height);
			leftMost = *i;
			left -= (*i)->dims.width;
		}
		else{
			setEffSpot(*i,right,solveme->dims.y + (*i)->dims.height);
			rightMost = *i;
			right += (*i)->dims.width;
		}
		rand = random();
	}
	//shift left-right according to total sizes and orientation.
	if (right - left > solveme->dims.width && availOrient != 0){
		if (availOrient & PREFER_CW){	//shift to right corner of solveme.
			int shift = right - solveme->dims.x + solveme->dims.width;
			wiggle(toAdd,shift,0);
		}
		else if (availOrient & PREFER_CCW){
			int shift = left - solveme->dims.x;
			wiggle(toAdd,shift,0);
		}
	}
	else{	//just put us so that we make contact.
		int shift = min(left - solveme->dims.x,right - solveme->dims.x + solveme->dims.width);
		wiggle(toAdd,shift,0); 
	}
}

void DimSolv::putBottom(vector<ZBRA*> toAdd, ZBRA* solveme, int availOrient){
	//connect the set left/right wise, joined by top corners.
	int rand = random();
	int left = (*toAdd.begin())->dims.x;
	int right = (*toAdd.begin())->dims.x + (*toAdd.begin())->dims.width;
	ZBRA* leftMost;
	ZBRA* rightMost;
	leftMost = rightMost = *toAdd.begin();
	setEffSpot(*toAdd.begin(),solveme->dims.x,solveme->dims.y + solveme->dims.height);
	for (vector<ZBRA*>::iterator i = toAdd.begin() + 1; i != toAdd.end(); i++){
		if (rand < RAND_MAX / 2){
			setEffSpot(*i, left - (*i)->dims.width, solveme->dims.y + (*i)->dims.height);
			leftMost = *i;
			left -= (*i)->dims.width;
		}
		else{
			setEffSpot(*i,right,solveme->dims.y + (*i)->dims.height);
			rightMost = *i;
			right += (*i)->dims.width;
		}
		rand = random();
	}
	//shift left-right according to total sizes and orientation.
	if (right - left > solveme->dims.width && availOrient != 0){
		if (availOrient & PREFER_CCW){	//shift to left corner of solveme.
			int shift = right - solveme->dims.x + solveme->dims.width;
			wiggle(toAdd,shift,0);
		}
		else if (availOrient & PREFER_CW){
			int shift = left - solveme->dims.x;
			wiggle(toAdd,shift,0);
		}
	}
	else{	//just put us so that we make contact.
		int shift = min(left - solveme->dims.x,right - solveme->dims.x + solveme->dims.width);
		wiggle(toAdd,shift,0); 
	}
}

void DimSolv::putLeft(vector<ZBRA*> toAdd, ZBRA* solveme, int availOrient){
//connect the set up/down wise, joined by right edge.
	int rand = random();
	int top = (*toAdd.begin())->dims.y;
	int bottom = (*toAdd.begin())->dims.y + (*toAdd.begin())->dims.height;
	ZBRA* topMost;
	ZBRA* bottomMost;
	topMost = bottomMost = *toAdd.begin();
	setEffSpot(*toAdd.begin(),solveme->dims.x - (*toAdd.begin())->dims.width,solveme->dims.y);
	for (vector<ZBRA*>::iterator i = toAdd.begin() + 1; i != toAdd.end(); i++){
		if (rand < RAND_MAX / 2){
			setEffSpot(*i, solveme->dims.x - (*i)->dims.width, top - (*i)->dims.height);
			topMost = *i;
			top -= (*i)->dims.height;
		}
		else{
			setEffSpot(*i,solveme->dims.x - (*i)->dims.width, bottom);
			bottomMost = *i;
			bottom += (*i)->dims.height;
		}
		rand = random();
	}
	//shift left-right according to total sizes and orientation.
	if (bottom - top > solveme->dims.height && availOrient != 0){
		if (availOrient & PREFER_CW){	//shift to left corner of solveme.
			int shift = bottom - solveme->dims.y + solveme->dims.height;
			wiggle(toAdd,0,shift);
		}
		else if (availOrient & PREFER_CCW){
			int shift = top - solveme->dims.y;
			wiggle(toAdd,0,shift);
		}
	}
	else{	//just put us so that we make contact.
		int shift = min(top - solveme->dims.y,bottom - solveme->dims.y + solveme->dims.height);
		wiggle(toAdd,0,shift); 
	}
}

void DimSolv::putRight(vector<ZBRA*> toAdd, ZBRA* solveme, int availOrient){
//connect the set up/down wise, joined by left edge.
	int rand = random();
	int top = (*toAdd.begin())->dims.y;
	int bottom = (*toAdd.begin())->dims.y + (*toAdd.begin())->dims.height;
	ZBRA* topMost;
	ZBRA* bottomMost;
	topMost = bottomMost = *toAdd.begin();
	setEffSpot(*toAdd.begin(), solveme->dims.x - (*toAdd.begin())->dims.width,solveme->dims.y);
	for (vector<ZBRA*>::iterator i = toAdd.begin() + 1; i != toAdd.end(); i++){
		if (rand < RAND_MAX / 2){
			setEffSpot(*i, solveme->dims.x - (*i)->dims.width, top - (*i)->dims.height);
			topMost = *i;
			top -= (*i)->dims.height;
		}
		else{
			setEffSpot(*i,solveme->dims.x - (*i)->dims.width, bottom);
			bottomMost = *i;
			bottom += (*i)->dims.height;
		}
		rand = random();
	}
	//shift left-right according to total sizes and orientation.
	if (bottom - top > solveme->dims.height && availOrient != 0){
		if (availOrient & PREFER_CCW){	//shift to left corner of solveme.
			int shift = bottom - solveme->dims.y + solveme->dims.height;
			wiggle(toAdd,0,shift);
		}
		else if (availOrient & PREFER_CW){
			int shift = top - solveme->dims.y;
			wiggle(toAdd,0,shift);
		}
	}
	else{	//just put us so that we make contact.
		int shift = min(top - solveme->dims.y,bottom - solveme->dims.y + solveme->dims.height);
		wiggle(toAdd,0,shift); 
	}
}

//We only need to look through our children and their adjacents. Their children are internal.
void DimSolv::toBounds(ZBRA* solveme, vector<ZBRA*> children){
	int top = INT_MAX; 
	int left = INT_MAX;
	int bottom = INT_MIN;
	int right = INT_MIN;
	
	int x2, y2;
	for (vector<ZBRA*>::iterator i = children.begin(); i < children.end(); i++){
		if((*i)->effDims.x > (*i)->effDims.x){
			(*i)->effDims.x = (*i)->effDims.x;
			(*i)->effDims.y = (*i)->effDims.y;
			(*i)->effDims.width = (*i)->effDims.width;
			(*i)->effDims.height = (*i)->effDims.height;
		}
		
		if (top > (*i)->effDims.y) top = (*i)->effDims.y;
		if (left > (*i)->effDims.x) left = (*i)->effDims.x;
		if (bottom < (y2 = (*i)->effDims.y + (*i)->effDims.height)) bottom = y2;
		if (right < (x2 = (*i)->effDims.x + (*i)->effDims.width)) right = x2;
		/*for (vector<ZBRA*>::iterator j = (*i)->adjacent.begin(); j < (*i)->adjacent.end(); j++){
			if (top > (*j)->effDims.y) top = (*j)->effDims.y;
			if (left > (*j)->effDims.x) left = (*j)->effDims.x;
			if (bottom < (y2 = (*j)->effDims.y + (*j)->effDims.height)) bottom = y2;
			if (right < (x2 = (*j)->effDims.x + (*j)->effDims.width)) right = x2;
		}*/
	}
	cerr << left << ':' << right << ';' << top << ':' << bottom << endl;
	solveme->dims.x = left;
	solveme->dims.y = top;
	solveme->dims.width = right - left;
	solveme->dims.height = bottom - top;
}

void DimSolv::getEffDims(ZBRA* solveme){
	solveme->effDims.x = solveme->dims.x;
	solveme->effDims.y = solveme->dims.y;
	solveme->effDims.width = solveme->dims.width;
	solveme->effDims.height = solveme->dims.height;
	for (vector<ZBRA*>::iterator j = solveme->adjacent.begin(); j < solveme->adjacent.end(); j++){
		if ( (*j)->effDims.x > (*j)->dims.x){	//effDims not set. Set to dims.
			(*j)->effDims.x = (*j)->dims.x;
			(*j)->effDims.y = (*j)->dims.y;
			(*j)->effDims.width = (*j)->dims.width;
			(*j)->effDims.height = (*j)->dims.height;
		}
		if ((*j)->effDims.x < solveme->effDims.x){
			solveme->effDims.width += solveme->effDims.x - (*j)->effDims.x;
			solveme->effDims.x = (*j)->effDims.x;
		}
		if ((*j)->effDims.y < solveme->effDims.y){
			solveme->effDims.height += solveme->effDims.y - (*j)->effDims.y;
			solveme->effDims.y = (*j)->effDims.y;
		}
		if ((*j)->effDims.x + (*j)->effDims.width > solveme->effDims.x + solveme->effDims.width){
			solveme->effDims.width += (*j)->effDims.x + (*j)->effDims.width - solveme->effDims.x + solveme->effDims.width;
		}
		if ((*j)->effDims.y + (*j)->effDims.height > solveme->effDims.y + solveme->effDims.height){
			solveme->effDims.height += (*j)->effDims.y + (*j)->effDims.height - solveme->effDims.y + solveme->effDims.height;
		}
	}
	//cerr << solveme->effDims.x << ',' << solveme->effDims.y << ':' << solveme->effDims.width << ',' << solveme->effDims.height << "vs" << solveme->dims.x << ',' << solveme->dims.y << ':' << solveme->dims.width << ',' << solveme->dims.height << endl;
}

//Shifts a group and their subArea/adjacent. Used for moving around subsets of areas.
void DimSolv::wiggle(vector<ZBRA*> toMove, int xOff, int yOff){
	for (vector<ZBRA*>::iterator i = toMove.begin(); i != toMove.end(); i++){
		(*i)->recursiveWiggle(xOff,yOff);
	}
}

//Moves all subArea/adjacents to a spot, abstracts away (some) math from call.
void DimSolv::setSpot(ZBRA* setSpot, int x, int y){
	int xOff = x - setSpot->dims.x;
	int yOff = y - setSpot->dims.y;
	setSpot->recursiveWiggle(xOff,yOff);
	//cerr << "is:" << setSpot->dims.x << ',' << setSpot->dims.y << " Should:" << x << ',' << y << endl;
}

//As setSpot, but adjusted for effective bound differences
void DimSolv::setEffSpot(ZBRA* spot, int x, int y){
	setSpot(spot,x - spot->effDims.x, y - spot->effDims.y);
	spot->effDims.x = x;
	spot->effDims.y = y;
}