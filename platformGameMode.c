/*
 *  platformGameMode.c
 *  MyLittleBall
 *
 *  Created by PineCone Computing Solutions on 18/03/2009.
 *  Copyright 2009 PineCone Computing Solutions. All rights reserved.
 *
 */

#include "platformGameMode.h"

/* The platform-style game differs from the traditional-style game in that
   the player controls the rotation of the platforms as opposed to the
   horizontal velocity of the ball. In controlling the platforms, the player
   can 'flip' the ball up by rapidly rotating the platforms
 */
int playPlatformGame(){
	
	// seed our random-number generator with the current time
	srand ( time(NULL) );
	
	// lots of initialising...
	int level = 1;
	int points = 0;
	int height = 1;
	
	int platformRotation = 0;
	int oldPlatformRotation = 0; // for calculating velocities
	
	struct ball myBall;
	myBall.xpos = 320;
	myBall.ypos = 100;
	myBall.xvelocity = 0;
	myBall.yvelocity = 0;
	myBall.rotvelocity = rand()%10;
	myBall.rotation = 0;
	
	int rumble = 0;
	//int kaboom = 0;
	
	//p2Init(activeController);
	
	while(level<=20){		// while we have not reached the top level....
		// generate some platforms
		genNewScreen(level);
		
		// clear bombs
		//p2Clear();
		
		
		// place the ball at the bottom of the screen with some upwards velocity
		myBall.ypos = 399;
		myBall.yvelocity = -20;
		while(myBall.ypos>-30){	// While the ball has not left the top of the screen
			
			
			WPAD_ScanPads();
			
			// first, check if we have fallen:
			if(myBall.ypos>=480) return points; 
			
			// handle the controller rumble
			if(rumble > 0){
				WPAD_Rumble(activeController, 1);
				rumble --;
			}else{
				WPAD_Rumble(activeController, 0);
			}
			
			// Do p2 stuff
			//int caught = p2Iterate(myBall.xpos, myBall.ypos);
			
			// Determine if ball is in contact with a platform:
			int hittingPlatform = collisionTestTrig(myBall.xpos, myBall.ypos, platformRotation);
			int addVelocity = 0;
			if(myBall.xvelocity > 0) addVelocity = -1*myBall.xvelocity;
			if(myBall.xvelocity < 0) addVelocity = myBall.xvelocity;
			
			switch (hittingPlatform) {
				case 1: // in contact!
					if(myBall.yvelocity>5 /* && myBall.xvelocity < 20 && myBall.xvelocity > -20 */){ // if we're on our way down and not cheating
						myBall.rotvelocity = myBall.xvelocity*6; 
						myBall.xvelocity -= myBall.rotvelocity;
						if(myBall.rotvelocity < 0) myBall.rotvelocity ++;
						if(myBall.rotvelocity > 0) myBall.rotvelocity --;
						if(abs(platformRotation) - abs(oldPlatformRotation) > 0)	// if the platform rotation has accelerated
							myBall.yvelocity -= (platformRotation - oldPlatformRotation);
						myBall.yvelocity -= 15;
						SND_PauseVoice(SND_GetFirstUnusedVoice()+1, 0);
						SND_SetVoice(SND_GetFirstUnusedVoice()+1, VOICE_MONO_8BIT,
									 PITCH + ((rand()%10000) - (rand()%10000)) + 50,	// varies the sound on each bounce
									 0, (u8 *) bounce, bounce_size, VOLUME-100, VOLUME-100, NULL);
						SND_PauseVoice(SND_GetFirstUnusedVoice()+1, 0);
						//rumble = 3;
					}
					if(myBall.yvelocity<=5){ // if we're goin' on up
						height -= 1;		 // then there is no bounce
						myBall.yvelocity +=	1;
					}
					break;
				case 0: // not in contact
					myBall.yvelocity += 1; // keep on fallin'
					height -= 1;
					break;
			}
			
			height ++;
			height -= myBall.yvelocity;
			
			/*
			if(caught){	// if we're caught in a blast....
				myBall.xvelocity += (rand() % 10) - (rand() % 10);
				if(myBall.xvelocity >= 0) myBall.xvelocity += 20;
				if(myBall.xvelocity < 0) myBall.xvelocity -= 20;
				myBall.yvelocity += (rand() % 20) - (rand() % 20);
				kaboom = 20;
				rumble = 10;
			}else if(kaboom > 0){
				kaboom --;
				if(myBall.xvelocity > 0) myBall.xvelocity --;
				if(myBall.xvelocity < 0) myBall.xvelocity ++;
				if(kaboom == 10) SND_SetVoice(SND_GetFirstUnusedVoice()+1, VOICE_MONO_8BIT, PITCH, 0, (u8 *) ouch, ouch_size, VOLUME, VOLUME, NULL);
			}else{*/
				// ===== Accelerometer Mode: ======
				struct orient_t orient;
				WPAD_Orientation(activeController, &orient);
				oldPlatformRotation = platformRotation;
				platformRotation = orient.pitch/4;
			GRRLIB_Printf(10, 10, tex_font, 0xFF0000FF, 1, "%d",(int)orient.pitch);
				// x-velocity of the ball now depends on the
				//'tilt' of the wiimote!
				
				/* This is the old, boring way of controlling the ball: */
				/*	if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_LEFT) myBall.xvelocity --; else if(myBall.xvelocity < 0) myBall.xvelocity ++;
				 if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_RIGHT) myBall.xvelocity ++; else if(myBall.xvelocity > 0) myBall.xvelocity --;
				 
				 if(myBall.xvelocity > 7) myBall.xvelocity --;
				 if(myBall.xvelocity < -7) myBall.xvelocity ++;
				 
				 */	
			//}
			
			if(WPAD_ButtonsHeld(activeController) & WPAD_BUTTON_HOME) exit(0);	// Quit?
			if(WPAD_ButtonsDown(activeController) & WPAD_BUTTON_MINUS) pauseGame(points); // Pause?
			
			
			// if we stray too far to one side, we re-appear on the other!
			if(myBall.xpos>640) myBall.xpos = -79;
			if(myBall.xpos<-80) myBall.xpos = 639;
			
			// plot the new location of the ball:
			myBall.ypos += myBall.yvelocity;
			myBall.xpos += myBall.xvelocity;
			
			// apply the rotation of the ball:
			myBall.rotation += myBall.rotvelocity;
			if(myBall.rotvelocity > 0) myBall.rotvelocity --;
			if(myBall.rotvelocity < 0) myBall.rotvelocity ++;
			myBall.rotation -= 2;
			
			// if the ball falls off the screen:
			if(myBall.ypos>=480) return points; 
			
			/*
			 Note about the 'height' variable:
			 This tracks the 'highest' point to which the ball
			 has gotten, the upshot of which is that the final
			 score stays at this value, and does not go down when
			 the ball falls. This is also nicer aethstetically,
			 as it means that the score does not go down.
			 */
			
			if(height > points) points = height;
			
			printEverythingRot(myBall.xpos,myBall.ypos, myBall.rotation, points, platformRotation);
			//p2Draw();
			GRRLIB_Render();
		}
		level++;	// increase the difficulty
		if(level>19) level = 19;	// going past level 20 makes the game close to impossible. stay at level 20.
	}
	return points;
}
