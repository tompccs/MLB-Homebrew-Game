/*
 *  international.h
 *  MyLittleBall
 *
 *  Created by PineCone Computing Solutions on 16/02/2009.
 *  Copyright 2009 PineCone Computing Solutions. All rights reserved.
 *
 */

#include "highscore.h"

// top secret hashing function to help secure
// the high scores from being tampered with
unsigned int hash(unsigned int a){
	a = (a+0x7ed55d16) + (a<<12);
	a = (a^0xc761c23c) ^ (a>>19);
	a = (a+0x165667b1) + (a<<5);
	a = (a+0xd3a2646c) ^ (a<<9);
	a = (a+0xfd7046c5) + (a<<3);
	a = (a^0xb55a4f09) ^ (a>>16);
	return a;
}

// retrieves current high score from file system
int getHighScore(){
	int hscore;
	int mtime;
	u32 deviceID;
	unsigned hscorehash;
	FILE * highScoreFile = fopen("sd:/apps/MyLittleBall/data.dat", "rb");
	if(highScoreFile != NULL){
		fscanf(highScoreFile, "%d",&hscore);
		fscanf(highScoreFile, "%d",&mtime);
		fscanf(highScoreFile, "%d",&deviceID);
		fscanf(highScoreFile, "%d",&hscorehash);
	}else{
		return -1;
	}
	fclose(highScoreFile);
	u32 myDID;
	ES_GetDeviceID(&myDID);
	if(hash(hscore & mtime & deviceID)==hscorehash && deviceID == myDID) return hscore;
	return -1;
}

// stores current high score to file system
int storeHighScore(int hscore){
	int mtime = time(NULL);
	u32 deviceID;
	ES_GetDeviceID(&deviceID);
	unsigned hscorehash = hash(hscore & mtime & deviceID);
	FILE * highScoreFile = fopen("sd:/apps/MyLittleBall/data.dat", "wb");
	if(highScoreFile!=NULL){
		fprintf(highScoreFile, "%d\n",hscore);
		fprintf(highScoreFile, "%d\n",mtime);
		fprintf(highScoreFile, "%d\n",deviceID);
		fprintf(highScoreFile, "%d",hscorehash);
		fclose(highScoreFile);
		return 1;
	}
	return -1;
}
