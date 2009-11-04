/*
 *  international.h
 *  MyLittleBall
 *
 *  Created by PineCone Computing Solutions on 16/02/2009.
 *  Copyright 2009 PineCone Computing Solutions. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>								/*	MyLittleBall by PineCone Computing Solutions  */
#include <string.h>								/*            (c) PineCone CS 2009                */
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <fat.h>
#include <time.h>
#include <wiiuse/wpad.h>
#include <gcmodplay.h>

// Remove trailing '_N' for debugging purpouses only!
// Always uses language as defined by following as
// opposed to detection with Wii on run

// ----------------------
#define LANG_OVERRIDE 0
// ----------------------

// language constants
#define LANG_EN 0
#define LANG_FR 1
#define LANG_ES 2
#define LANG_DE 3
#define LANG_NL 4
#define LANG_IT 5
#define LANG_PT 6

// number of existing string definitions
#define NUMBER_OF_STRINGS_DEFINED 7

// string identifier constants
#define A2_START	0
#define HS_DISABLED	1
#define HS_IS		2
#define HS_FAIL		3
#define YOU_SCORED	4
#define GAME_PAUSED 5
#define A2_CONTINUE 6

// status constants
#define SUCCESS 0
#define FAILURE 1

// language file path
#define LFPATH "sd:/apps/MyLittleBall/lang.xml"

// table of definitions
char strtbl[6][6][100];

// reads language file and stores in table of definitions
int loadLang();

// 'finds out' Wii's default language and stores in 
int getWiiDefaultLang();

// calls the above two functions
int initLang();

// for if there is no file
int genEnglishStrings();
