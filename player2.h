/*
 *  player2.h
 *  MyLittleBall
 *
 *  Created by PineCone Computing Solutions on 28/02/2009.
 *  Copyright 2009 PineCone Computing Solutions. All rights reserved.
 *
 */

#ifndef PCCS_MLB_P2
#define PCCS_MLB_P2

#include "main.h"

#include "resources/bomb.h"
#include "resources/explosion.h"

#define SAFE 0
#define HIT_EXPLOSION 1

struct bomb_s {
	int xpos;
	int ypos;
	int yvelocity;
	int fuse;
};

void p2GraphicsInit();

void p2Init(int controllerException);
int p2Iterate(int p1x, int p1y);
struct bomb_s * p2GetBomb();
int p2HitTest();
void p2Draw();
void p2Clear();
void p2Stop();

#endif
