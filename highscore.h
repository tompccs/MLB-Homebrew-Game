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

unsigned int hash(unsigned int a);
int getHighScore();
int storeHighScore(int hscore);
