/*
 *  main.c
 *  MyLittleBall
 *
 *  Created by PineCone Computing Solutions on 28/02/2009.
 *  Copyright 2009 PineCone Computing Solutions. All rights reserved.
 *
 */

#include "main.h"
#include "player2.h"

int currentVoice;

int selectedGameMode = 0;

int bestScore = 0;
int fatInnited = 0;

int activeController = 0;

int distanceFromCentre = 0;

int platforms[65][21];
// platforms are of size 100 by 20, so 64 by 20 can fit on screen at any one time
// array of either 1 (true: platform there) of 0 (false: no platform there).


// where is the towel? (< 1 values for absent)
int towelX = -1;
int towelY = -1;

// Var for initVideo function to ensure it does not repeat itself
int videoInit = 0;

int pColourOffset = 0;

// Function to declare all the video stuff
void initVideo(){
	if(videoInit!=1){
		
		VIDEO_Init();
		
		GRRLIB_InitVideo();
		GRRLIB_InitFreetype(komika, komika_size);
		
		// Load textures
		ball_img = GRRLIB_LoadTexture(MyLittleBall_player_001);
		introBg = GRRLIB_LoadTexture(MyLittleBall_title);
		platform1 = GRRLIB_LoadTexture(MyLittleBall_platform_0001);
		platform2 = GRRLIB_LoadTexture(MyLittleBall_platform_0002);
		platform3 = GRRLIB_LoadTexture(MyLittleBall_platform_0003);
		platform4 = GRRLIB_LoadTexture(MyLittleBall_platform_0004);
		platform5 = GRRLIB_LoadTexture(MyLittleBall_platform_0005);
		platform6 = GRRLIB_LoadTexture(MyLittleBall_platform_0006);
		towel_img = GRRLIB_LoadTexture(MyLittleBall_towel);
		igBg = GRRLIB_LoadTexture(MyLittleBall_ingame_bg);
		pmode = GRRLIB_LoadTexture(platformmode);
		bmode = GRRLIB_LoadTexture(ballmode);
		
		const unsigned char *temp;
		
		switch (myLanguage) {
			case LANG_EN:
				temp = MyLittleBall_title_message;
				break;
			case LANG_ES:
				temp = strt_es;
				break;
			case LANG_DE:
				temp = strt_de;
				break;
			case LANG_FR:
				temp = strt_fr;
			case LANG_IT:
				temp = strt_it;
				break;
			case LANG_NL:
				temp = strt_nl;
				break;
			default:
				temp = strt_pt;
		}
		
		introFg = GRRLIB_LoadTexture(temp);
		
		p2GraphicsInit();
		
		/* The Old Graphics
		 * tex_ball = GRRLIB_LoadTexture(ball);
		 * tex_intro = GRRLIB_LoadTexture(intro);
		 * tex_platform = GRRLIB_LoadTexture(platform);
		 * tex_towel = GRRLIB_LoadTexture(g2410);
		 */
		
		tex_font = GRRLIB_LoadTexture(GRRLIB_font1);
		
		// Make sure I do not repeat myself
		videoInit = 1;
	}
}

/* Generates a new set of platforms
 *  the level number indicates how
 *  dense the platforms are (with 1
 *  being easiest and 20 being hardest) */
int genNewScreen(int level){
	int c,y;
	for(c=0;c<=64;c++){
		for (y=0; y<=20; y++) {
			platforms[c][y] = 0;
		}
	}
	int density = (1280 - level*64)/32;
	for(c=0;c<=density;c++){
		platforms[rand()%65][rand()%20] = 1;
	}
	
	platforms[rand()%65][20] = 1;
	
	pColourOffset = (rand()%6) + 1;
	
	return 1;
}

// Does what it says on the tin
int printEverythingRot(int xPos, int yPos, float rotation, int theScore, int platformRotation){
	int x, y;
	u8 *myPlatform;
	int myColour = pColourOffset;
	GRRLIB_DrawImg(0, 0, 640, 480, igBg, 0, 1, 1, 0xFF);
	for (x=0; x<=64; x++) {		// print all of the platforms
		for (y=0; y<=20; y++) {
			
			switch (myColour) {
				case 1:
					myPlatform = platform1;
					break;
				case 2:
					myPlatform = platform2;
					break;
				case 3:
					myPlatform = platform3;
					break;
				case 4:
					myPlatform = platform4;
					break;
				case 5:
					myPlatform = platform5;
					break;
				case 6:
					myPlatform = platform6;
					break;
				default:
					myPlatform = platform1;
					break;
			}
			
			if(myColour >= 6) myColour = 0;
			myColour ++;
			
			if(platforms[x][y]==1)  // platform exists
				GRRLIB_DrawImg(x*10, y*20, 100, 20, myPlatform, platformRotation, 1, 1, 0xFF);
			if(platforms[x][y]==2){	// ball is in contact
				GRRLIB_DrawImg(x*10, y*20, 100, 20, myPlatform, platformRotation, 1.01, 1, 0xFF);
				platforms[x][y] = 1;
			}
		}
	}
	GRRLIB_DrawImg(xPos, yPos, 80, 80, ball_img, rotation, 0.5, 0.5, 0xFF);		// print the ball
	if(towelX >= 0) GRRLIB_DrawImg(towelX, towelY, 40, 40, towel_img, 0, 1, 1, 0xFF); // print a towel
	//u8 *tmp = GRRLIB_TextToTexture(24, 0xFFFFFF, "Score: %d", theScore);
	//GRRLIB_DrawImg(350, 50, 280, 48, tmp, 0, 1, 1, 0xFF);
	//free(tmp);
	//GRRLIB_Printf2(320, 50, 24, 0xFFFFFF, "Score: %d",theScore);
	//GRRLIB_Printf(320, 50, tex_font, 0xFF0000FF, 2, "Score: %d",theScore);	// print the score
	GRRLIB_Printf2(320, 50, 12, 0x000000FF, "Score: %d",theScore);
	return 1;
}

// Convenience function
int printEverything(int xPos, int yPos, float rotation, int theScore){
	return printEverythingRot(xPos, yPos, rotation, theScore, 0);
}

// called when game is paused. returns when un-paused.
void pauseGame(int score){
	while(1){
		WPAD_ScanPads();
		if((WPAD_ButtonsDown(activeController) & WPAD_BUTTON_A) || (WPAD_ButtonsDown(activeController) & WPAD_BUTTON_MINUS)) break;
		if(WPAD_ButtonsDown(activeController) & WPAD_BUTTON_HOME) exit(0);
		GRRLIB_DrawImg(0, 0, 640, 480, igBg, 0, 1, 1, 0xFF);
		GRRLIB_Printf(20, 40, tex_font, COLOR_red, 2, strtbl[myLanguage][GAME_PAUSED],score);
		GRRLIB_Printf(20, 100, tex_font, COLOR_red, 2, "    highscore: %d",bestScore);
		GRRLIB_Printf(20, 150, tex_font, COLOR_red, 1, strtbl[myLanguage][A2_CONTINUE]);
		GRRLIB_Render();
	}
}

int heightTrigCalculator(int xOffset, int rotation){
	return (int)((double)xOffset/tan((double)rotation));
}

// Checks whether or not ball is
// in contact with a platform
int collisionTestTrig(int ballXpos, int ballYpos, int platformRotation){ 
	int x, y;
	
	/*  as the only values we have to work with here
	 are the coordinates GRRLIB draws each object
	 from, we need upper-bound and lower-bounds
	 to ensure accurate collision checking.     */
	int xBallLowerBound = -20;			
	int xBallUpperBound = 20;			
	int yBallLowerBound = 15;			
	int yBallUpperBound = 20;			
	
	int xPlatformLowerBound = -20;		
	int xPlatFormUpperBound = 50;		
	int yPlatformLowerBound = -35;		
	int yPlatformUpperBound = -20;	
	
	// enter loop to check each platform:
	for(x=0;x<=64;x++){
		for(y=0;y<=20;y++){
			if(platforms[x][y]==1){
				// calculate point from which platform is drawn
				int platformXpos = x * 10;
				int platformYpos = y * 20;
				if(																		// if...
				   ballXpos + xBallUpperBound >= platformXpos + xPlatformLowerBound &&	
				   ballXpos + xBallLowerBound <= platformXpos + xPlatFormUpperBound	// we're horizontally aligned
				   ){	// indeed, we are in line with a platform!
					if(platformRotation > 0){ 
						int yOffset = heightTrigCalculator((((xPlatFormUpperBound - xPlatformLowerBound)/2)+platformXpos)-ballXpos, platformRotation);
						platformYpos += yOffset;
						
					}
					if(
							 ballYpos + yBallUpperBound >= platformYpos + yPlatformLowerBound &&
							 ballYpos + yBallLowerBound <= platformYpos + yPlatformUpperBound		// and we are, indeed, vertically in contact...
						){
						platforms[x][y]=2;
						return 1;
					}
				}	
			}
		}
	}
	return 0;	// otherwise return a big fat no, go and stick your head in a pig.
}

// convenience function
int collisionTest(int ballXpos, int ballYpos){ return collisionTestTrig(ballXpos, ballYpos, 0); }

// returns whether or not we have hit a towel
int getTowel(ballXpos, ballYpos){
	/*  as the only values we have to work with here
	 are the coordinates GRRLIB draws each object
	 from, we need upper-bound and lower-bounds
	 to ensure accurate collision checking.     */
	int xBallLowerBound = -20;
	int xBallUpperBound = 20;
	int yBallLowerBound = -20;
	int yBallUpperBound = 20;
	
	if(
	   ballXpos + xBallUpperBound >= towelX - 20 &&
	   ballXpos + xBallLowerBound <= towelX + 20 &&
	   ballYpos + yBallUpperBound >= towelY - 20 &&
	   ballYpos + yBallLowerBound <= towelY + 20
	   ){
		return 1;
	}
	return 0;
}

// animates the platforms falling off the screen
void animatePlatformsFalling(){
	int x, y, c;
	
	// count how many platforms we have
	int numPlatforms = 0;
	for (x=0; x<=64; x++) {
		for (y=0; y<=20; y++) {
			if(platforms[x][y] > 0) numPlatforms ++;
		}
	}
	
	// determine random veloceties for each platform:
	int veloceties[numPlatforms+1];
	int yOffsets[numPlatforms+1];
	
	for (x=0; x<numPlatforms; x++)
		yOffsets[x] = 0;
	
	for (x=0; x<=numPlatforms; x++) {
		veloceties[x] = -(rand() % 15);
		if(veloceties[x] <= 0) veloceties[x] = 8;
	}
	
	c = 0;
	int counter = 0;
	int myColour = pColourOffset;
	u8 *myPlatform;
	int m;
	
	// animate!
	for(m=0; m<40; m++){
		// iterate between each platform and move according to velocity
		counter = 0;
		myColour = pColourOffset;
		GRRLIB_DrawImg(0, 0, 640, 480, igBg, 0, 1, 1, 0xFF);
		for (x=0; x<=64; x++) {
			for (y=0; y<=20; y++) {
				switch (myColour) {
					case 1:
						myPlatform = platform1;
						break;
					case 2:
						myPlatform = platform2;
						break;
					case 3:
						myPlatform = platform3;
						break;
					case 4:
						myPlatform = platform4;
						break;
					case 5:
						myPlatform = platform5;
						break;
					case 6:
						myPlatform = platform6;
						break;
					default:
						myPlatform = platform1;
						break;
				}
				if(myColour >= 6) myColour = 0;
				myColour ++;
				if(platforms[x][y] > 0){
						yOffsets[counter] += veloceties[counter];
						veloceties[counter] ++;					
						GRRLIB_DrawImg(x*10, y*20 + yOffsets[counter], 100, 20, myPlatform, 0, 1, 1, 0xFF);					
						counter ++;
				}
			}
		}
		GRRLIB_Render();
	}
}

int playGame(){
	
	// if we are playing in platform mode, hand over to platform mode function:
	if(selectedGameMode == PLATFORMMODE){
		return playPlatformGame();
	}
	
	// seed our random-number generator with the current time
	srand ( time(NULL) );
	
	// lots of initialising...
	int level = 1;
	int points = 0;
	int height = 1;
	
	int ballBounce = -15;
	
	struct ball myBall;
	myBall.xpos = 320;
	myBall.ypos = 100;
	myBall.xvelocity = 0;
	myBall.yvelocity = 0;
	myBall.rotvelocity = rand()%10;
	myBall.rotation = 0;
	
	int rumble = 0;
	int kaboom = 0;
	
	p2Init(activeController);
	
	while(level<=20){		// while we have not reached the top level....
		// generate some platforms
		genNewScreen(level);
		
		// clear bombs
		p2Clear();
		
		// position a towel somewhere
		if((rand()%6)==1 || ballBounce < -15){	// 1 in 5 chance of towel appearing
			towelX = rand()%640;
			towelY = rand()%400;
		}else{
			towelY = -1;
			towelX = -1;
		}
		
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
			int caught = p2Iterate(myBall.xpos, myBall.ypos);
			
			// Determine if ball is in contact with a platform:
			int hittingPlatform = collisionTest(myBall.xpos, myBall.ypos);
			int addVelocity = 0;
			if(myBall.xvelocity > 0) addVelocity = -1*myBall.xvelocity;
			if(myBall.xvelocity < 0) addVelocity = myBall.xvelocity;
			
			switch (hittingPlatform) {
				case 1: // in contact!
					if(myBall.yvelocity>5 && myBall.xvelocity < 20 && myBall.xvelocity > -20){ // if we're on our way down and not cheating
						myBall.yvelocity = ballBounce + (addVelocity/4);	// Give it some bounce!
						myBall.rotvelocity = myBall.xvelocity*6; 
						myBall.xvelocity -= myBall.rotvelocity;
						if(myBall.rotvelocity < 0) myBall.rotvelocity ++;
						if(myBall.rotvelocity > 0) myBall.rotvelocity --;
						if(ballBounce < -15) ballBounce += 5;
						SND_PauseVoice(currentVoice, 0);
						SND_SetVoice(currentVoice, VOICE_MONO_8BIT,
									 PITCH + ((rand()%10000) - (rand()%10000)) + 50,	// varies the sound on each bounce
									 0, (u8 *) bounce, bounce_size, VOLUME-100, VOLUME-100, NULL);
						SND_PauseVoice(currentVoice, 0);
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
			
			// determines if we are in contact with towel, reacts accordingly
			if(getTowel(myBall.xpos,myBall.ypos) ==1 && towelX > -1){
				rumble = 10;	// rumble the controller
				SND_PauseVoice(currentVoice, 0);
				SND_SetVoice(currentVoice, VOICE_MONO_8BIT, PITCH, 0, (u8 *) yay, yay_size, VOLUME, VOLUME, NULL); // play a sound
				ballBounce = -30;	// give ball more bounce
				myBall.yvelocity = -20;	// give ball some upwards velocity
				towelX = -1; towelY = -1;  // 'destroys' the towel
			}
			
			
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
			}else{
				// ===== Accelerometer Mode: ======
				struct orient_t orient;
				WPAD_Orientation(activeController, &orient);
				myBall.xvelocity = (orient.roll)/4;
				// x-velocity of the ball now depends on the
				//'tilt' of the wiimote!
				
				// some limiting so you can't cheat...
				if(myBall.xvelocity > 20) myBall.xvelocity = 20;
				if(myBall.xvelocity < -20) myBall.xvelocity = -20;
				
				
				/* This is the old, boring way of controlling the ball: */
				/*	if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_LEFT) myBall.xvelocity --; else if(myBall.xvelocity < 0) myBall.xvelocity ++;
				 if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_RIGHT) myBall.xvelocity ++; else if(myBall.xvelocity > 0) myBall.xvelocity --;
				 
				 if(myBall.xvelocity > 7) myBall.xvelocity --;
				 if(myBall.xvelocity < -7) myBall.xvelocity ++;
				 
				 */	
			}
				
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
			
			printEverything(myBall.xpos,myBall.ypos, myBall.rotation, points);
			p2Draw();
			GRRLIB_Render();
		}
		level++;	// increase the difficulty
		if(level>19) level = 19;	// going past level 20 makes the game close to impossible. stay at level 20.
	}
	return points;
}


int startScreen(int lastScore){
	float alpha = 0;
	int lh = 480;
	// Does a nice fade in of the 'intro' graphic
	while(alpha<=255){
		GRRLIB_DrawImg(0, 0, 640, 480, igBg, 0, 1, 1, 0xFF);
		GRRLIB_DrawImg(0, 0, 640, 480, introBg, 0, 1, 1, alpha);
		if(selectedGameMode == BALLMODE){
			GRRLIB_DrawImg(270, lh, 100, 40, bmode, 0, 1, 1, 0xFF);
		}else{
			GRRLIB_DrawImg(270, lh, 100, 40, pmode, 0, 1, 1, 0xFF);
		}
		
		GRRLIB_Render();
		lh -= 8;
		alpha += 15;
	}
	
	float fadey = 0;
	float inout = 1;

	
	// Main loop. Wait for either 'A' input or 'HOME', also game mode options
	while(1){
		GRRLIB_DrawImg(0, 0, 640, 480, introBg, 0, 1, 1, 255);
		
		if(fadey >= 245) inout = 0;
		if(fadey <= 0) inout = 1;
		
		if(inout == 1) fadey += 10;
		if(inout == 0) fadey -= 10;
		
		GRRLIB_DrawImg(126, 400, 400, 60, introFg, 0, 1, 1, fadey);
		if(selectedGameMode == BALLMODE){
			GRRLIB_DrawImg(270, 350, 100, 40, bmode, 0, 1, 1, 0xFF);
		}else{
			GRRLIB_DrawImg(270, 350, 100, 40, pmode, 0, 1, 1, 0xFF);
		}
		
		WPAD_ScanPads();
		if(WPAD_ButtonsDown(0) & WPAD_BUTTON_A){
			activeController = 0;
			return 0;
		}
		if(WPAD_ButtonsDown(1) & WPAD_BUTTON_A){
			activeController = 1;
			return 0;
		}
		if(WPAD_ButtonsDown(2) & WPAD_BUTTON_A){
			activeController = 2;
			return 0;
		}
		if(WPAD_ButtonsDown(3) & WPAD_BUTTON_A){
			activeController = 3;
			return 0;
		}
		if(WPAD_ButtonsDown(activeController) & WPAD_BUTTON_HOME){
			return 1;
		}
		if(WPAD_ButtonsDown(activeController) & WPAD_BUTTON_LEFT || WPAD_ButtonsDown(activeController) & WPAD_BUTTON_RIGHT){
			int tempGM = selectedGameMode;
			if(tempGM == 1) selectedGameMode = 0;
			if(tempGM == 0) selectedGameMode = 1;
		}
		
		GRRLIB_Render();
		VIDEO_WaitVSync();
	}
	
}

void displayScore(int theScore){
	animatePlatformsFalling();
	GRRLIB_DrawImg(0, 0, 640, 480, igBg, 0, 1, 1, 255);
	GRRLIB_Render();
	int x;
	int res = 1;
	if(theScore > bestScore) res = storeHighScore(bestScore);
	
	int alpha = 0;
	
	for (x=0; x<212; x++) {
		WPAD_ScanPads();
		if(WPAD_ButtonsDown(activeController)&WPAD_BUTTON_A) return;
		GRRLIB_DrawImg(0, 0, 640, 480, igBg, 0, 1, 1, 255);
		GRRLIB_Printf(50, 50, tex_font, 0xFF000000 | alpha, 2, strtbl[myLanguage][YOU_SCORED],theScore);
		
		//GRRLIB_Printf(50, 100, tex_font, 0xFF000000 | alpha, 2, "ch: %d", SND_GetFirstUnusedVoice());
		
		if(theScore >= bestScore){
			GRRLIB_Printf(50, 80, tex_font, 0xFF000000 | alpha, 2, "HIGH SCORE!");
			bestScore = theScore;
			if(res < 1) GRRLIB_Printf(50, 100, tex_font, 0xFF000000 & alpha, 2, strtbl[myLanguage][HS_FAIL]);
		}else{
			GRRLIB_Printf(50, 80, tex_font, 0xFF000000 | alpha, 2, strtbl[myLanguage][HS_IS],bestScore);
		}
		GRRLIB_Render();
		if(alpha >= 255) alpha = 255; else alpha += 20;
	}
}

void noHighScores(){
	int x;
	for(x=0; x<150; x++){
		GRRLIB_FillScreen(COLOR_black);
		GRRLIB_Printf(50, 50, tex_font, COLOR_blue, 2, strtbl[myLanguage][HS_DISABLED], x);
		WPAD_ScanPads();
		if(WPAD_ButtonsDown(0)&&WPAD_BUTTON_A) return;
		GRRLIB_Render();
	}
}

int main(int argc, char **argv) {

	
	// set up the wiimote with accelerometer sensitivity
	WPAD_Init();
	WPAD_SetDataFormat(0, WPAD_FMT_BTNS_ACC_IR); WPAD_SetVRes(0,640,480);
	WPAD_SetDataFormat(1, WPAD_FMT_BTNS_ACC_IR); WPAD_SetVRes(1,640,480);
	WPAD_SetDataFormat(2, WPAD_FMT_BTNS_ACC_IR); WPAD_SetVRes(2,640,480);
	WPAD_SetDataFormat(3, WPAD_FMT_BTNS_ACC_IR); WPAD_SetVRes(3,640,480);

	
	// initialise sounds
	SND_Init(INIT_RATE_48000); 
	MODPlay_Init(&snd1);
	SND_Pause(0);
	
	MODPlay_AllocSFXChannels(&snd1, 0);
	
	
	// try to initialise file i/o
	if(!fatInitDefault()) noHighScores();
	bestScore = getHighScore();		// read the high score from file
	
	// initialise languages
	initLang();
	
	// initialise video
	initVideo();
	
	// initialise freetype
	GRRLIB_InitFreetype();
	
	int score = -1;	// seed score at -1
	
	currentVoice = SND_GetFirstUnusedVoice()+4;
	currentVoice += 5;
	
	// for as long as the player wants another go
	while(1){
		
		MODPlay_SetMOD(&snd1, tsMusic);
		MODPlay_Start(&snd1);
		
		if(startScreen(score)==1) break; // quit if user says so at startScreen()
		
		MODPlay_Stop(&snd1);
		
		MODPlay_SetMOD(&snd1, igMusic);
		MODPlay_Start(&snd1);
		
		score = playGame();	// play game and obtain score
		p2Stop();
		
		MODPlay_Stop(&snd1);
		
		// player has fallen. play sound
		SND_Pause(0);
		SND_SetVoice(SND_GetFirstUnusedVoice(), VOICE_MONO_8BIT, PITCH, 0, (u8 *) ohno, ohno_size, VOLUME, VOLUME, NULL);
		
		WPAD_Rumble(activeController, 0); // turn off controller rumble
		displayScore(score); // and display the score
	}
	return 0;
}

