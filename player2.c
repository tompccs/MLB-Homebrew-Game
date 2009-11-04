/*
 *  player2.c
 *  MyLittleBall
 *
 *  Created by PineCone Computing Solutions on 28/02/2009.
 *  Copyright 2009 PineCone Computing Solutions. All rights reserved.
 *
 */

#include "player2.h"

u8 * bgraphic, * egraphic;

struct bomb_s droppedBomb;
struct bomb_s cursorBomb;

int p2Controller = -1;
int cEx;

int clankPlayed;
int explosionPlayed;

void p2Init(int controllerException){
	int x;
	cEx = controllerException;
	// Determine the controller to act as player 2
	for (x=0; x<3; x++) {
		if((WPAD_ButtonsHeld(x) & WPAD_BUTTON_B) && x != controllerException){
			p2Controller = x;
			break;
		}
	}
	p2Clear();
}

void p2Stop(){
	p2Clear();
	p2Controller = -1;
}

void p2Clear(){
	droppedBomb.fuse = -1;
	clankPlayed = 0;
	explosionPlayed = 0;
}

int p2Iterate(int p1x, int p1y){
	
	if(p2Controller == -1){
		p2Init(cEx);
		return 0;
	}
	struct ir_t ir;
	WPAD_IR(p2Controller, &ir);
	
	cursorBomb.xpos = (int)ir.x;
	cursorBomb.ypos = (int)ir.y;
	
	if(droppedBomb.fuse == -1){ // if a bomb hasn't been dropped...
		if(WPAD_ButtonsDown(p2Controller) & WPAD_BUTTON_B){
			droppedBomb.xpos = cursorBomb.xpos;
			droppedBomb.ypos = cursorBomb.ypos;
			droppedBomb.fuse = 0;
			droppedBomb.yvelocity = 0;
		}
	}else{	// a bomb has been dropped...
		
		// Make bomb move appropriately
		droppedBomb.fuse ++;
		droppedBomb.ypos += droppedBomb.yvelocity;
		
		if(collisionTest(droppedBomb.xpos, droppedBomb.ypos) == 0){
			droppedBomb.yvelocity ++;
			if(droppedBomb.ypos > 420 || droppedBomb.ypos < 0){ // if we fall off the screen...
				droppedBomb.fuse = -1;
				droppedBomb.yvelocity = 0;
				droppedBomb.ypos = 0;
				return 0;
			}
		}else{
			if(!clankPlayed){
				SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, PITCH, 0, (u8 *) clank, clank_size, VOLUME, VOLUME, NULL);
				clankPlayed = 1;
			}
			droppedBomb.yvelocity = 0;
		}
		
		// Destroy bomb if explosion has finished
		if(droppedBomb.fuse > 90){
			droppedBomb.fuse = -1;
			clankPlayed = 0;
			explosionPlayed = 0;
		}
		
		// Handle interactions with player 1
		int hit = p2HitTest(p1x, p1y);
		
		if(hit == 1){
			droppedBomb.fuse = 60;
		}
		
		return hit;
	}
	return 0;
}

void p2GraphicsInit(){
	bgraphic = GRRLIB_LoadTexture(bomb);
	egraphic = GRRLIB_LoadTexture(explosion);
}

void p2Draw(){
	// draw dropped bomb
	if(p2Controller > -1){
		if(droppedBomb.fuse > -1 && droppedBomb.fuse < 60) // if bomb exists and is unexploded....
			GRRLIB_DrawImg(droppedBomb.xpos, droppedBomb.ypos, 80, 80, bgraphic, 0, 0.5, 0.5, 0xFF);
		if(droppedBomb.fuse > 60){ // if bomb is mid-explosion...
			GRRLIB_DrawImg(droppedBomb.xpos, droppedBomb.ypos, 80, 80, egraphic, rand() % 0xFF, 1, 1, 0xFF);
			if(!explosionPlayed) SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, PITCH, 0, (u8 *) bombsound, bombsound_size, VOLUME, VOLUME, NULL);
			explosionPlayed = 1;
		}
		// draw p2's 'cursor'
		u8 transparency;
		if(droppedBomb.fuse > -1) 
			transparency = 80;
		else 
			transparency = 0xFF;
		GRRLIB_DrawImg(cursorBomb.xpos, cursorBomb.ypos, 80, 80, bgraphic, 0, 0.5, 0.5, transparency);
	}else{
		GRRLIB_Printf(50, 390, tex_font, 0xFF0000FF, 1, "press B to connect p2");
	}
}

int p2HitTest(int ballXpos, int ballYpos){
	/*  as the only values we have to work with here
	 are the coordinates GRRLIB draws each object
	 from, we need upper-bound and lower-bounds
	 to ensure accurate collision checking.     */
	int xBallLowerBound = -20;
	int xBallUpperBound = 20;
	int yBallLowerBound = -20;
	int yBallUpperBound = 20;
	
	if(
	   ballXpos + xBallUpperBound >= droppedBomb.xpos - 20 &&
	   ballXpos + xBallLowerBound <= droppedBomb.xpos + 20 &&
	   ballYpos + yBallUpperBound >= droppedBomb.ypos - 20 &&
	   ballYpos + yBallLowerBound <= droppedBomb.ypos + 20
	   ){
		return 1;
	}
	return 0;
}

struct bomb_s * p2GetBomb(){
	return &droppedBomb;
}
