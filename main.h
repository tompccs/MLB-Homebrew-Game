/*
 *  main.h
 *  MyLittleBall
 *
 *  Created by PineCone Computing Solutions on 28/02/2009.
 *  Copyright 2009 PineCone Computing Solutions. All rights reserved.
 *
 */

#ifndef _PCCS_MLB_MAIN_
#define _PCCS_MLB_MAIN_
// include libs
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>								/*	MyLittleBall by PineCone Computing Solutions  */
#include <string.h>								/*            (c) PineCone CS 2009                */
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <asndlib.h>
#include <fat.h>
#include <time.h>
#include <wiiuse/wpad.h>
#include <mxml/mxml.h>

#include <gcmodplay.h>

// include MLB files
#include "international.h"
#include "highscore.h"
#include "platformGameMode.h"

// include graphics libs
#include "GRRLIB/GRRLIB.h"

// colours
#include "colors.h"

// include images
// graphics courtesy of Chris Hildenbrand (DayDreamOz - SpriteAttack) -- thanks, Chris!
#include "resources/spriteattack/MyLittleBall_ingame_bg.h"
#include "resources/spriteattack/MyLittleBall_platform_0001.h"
#include "resources/spriteattack/MyLittleBall_platform_0002.h"
#include "resources/spriteattack/MyLittleBall_platform_0003.h"
#include "resources/spriteattack/MyLittleBall_platform_0004.h"
#include "resources/spriteattack/MyLittleBall_platform_0005.h"
#include "resources/spriteattack/MyLittleBall_platform_0006.h"
#include "resources/spriteattack/MyLittleBall_player_001.h"
#include "resources/spriteattack/MyLittleBall_title_message.h"
#include "resources/spriteattack/MyLittleBall_title.h"
#include "resources/spriteattack/MyLittleBall_towel.h"
// a couple of my own graphics
#include "resources/ballmode.h"
#include "resources/platformmode.h"
// translations of opening text
#include "resources/strt_ca.h"
#include "resources/strt_de.h"
#include "resources/strt_es.h"
#include "resources/strt_fr.h"
#include "resources/strt_it.h"
#include "resources/strt_nl.h"
#include "resources/strt_pt.h"

/* The Old Graphics:
 * #include "resources/ball.h"
 * #include "resources/intro.h"
 * #include "resources/platform.h"
 * #include "resources/towel.h"
 */

// include fonts
#include "GRRLIB/fonts/GRRLIB_font1.h"
#include "GRRLIB/fonts/komika.h"

// include sound effects
#include "resources/yay.h"
#include "resources/ohno.h"
#include "resources/bounce.h"
#include "resources/bombsound.h"
#include "resources/ouch.h"
#include "resources/clank.h"

// include music
#include "resources/tsMusic.h"
#include "resources/igMusic.h"

#define BALLMODE 0
#define PLATFORMMODE 1

// language of player
extern u8 myLanguage;

// graphics
Mtx44 GXmodelView2D;

// sound
MODPlay snd1;

// Old Graphics:
// u8 *tex_ball, *tex_intro, *tex_platform, *tex_towel, *tex_font;

// New Graphics:
u8 *ball_img, *introBg, *introFg, *platform1, *platform2, *platform3, *platform4, *platform5, *platform6, *towel_img, *tex_font, *igBg, *helvetica, *pmode, *bmode;

// define the ball struct
struct ball{
	int xpos;
	int ypos;
	int rotation;
	int xvelocity;
	int yvelocity;
	int rotvelocity;
};

// variable to store the controller which initiates each game
extern int activeController;

// variable to store the selected game mode
extern int selectedGameMode; // selected game mode. 0 for ball mode, 1 for platform mode

// shouldn't really be doing this:
extern int distanceFromCentre;	// value storing the distance the ball was from the centre of the platform in platform game mode

// define a default pitch and volume for sound effects
#define PITCH 52000
#define VOLUME 255

// only functions required by more than one file are defined here
int collisionTestTrig(int ballXpos, int ballYpos, int platformRotation);
int collisionTest(int ballXpos, int ballYpos);
int genNewScreen(int level);
void pauseGame(int score);
int printEverything(int xPos, int yPos, float rotation, int theScore);
int printEverythingRot(int xPos, int yPos, float rotation, int theScore, int platformRotation);
PCCS_MLB_P2
#endif
