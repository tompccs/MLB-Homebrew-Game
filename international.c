/*
 *  international.c
 *  MyLittleBall
 *
 *  Created by PineCone Computing Solutions on 16/02/2009.
 *  Copyright 2009 PineCone Computing Solutions. All rights reserved.
 *
 */

#include "international.h"
#include <mxml/mxml.h>

// default language. 0 for undefined (revert to English)
u8 myLanguage = LANG_EN;

int initLang(){
	if(getWiiDefaultLang() == FAILURE || loadLang() == FAILURE) return FAILURE;
	return SUCCESS;
}

int genEnglishStrings(){
	myLanguage = LANG_EN;
	*strtbl[LANG_EN][A2_START] = *"Press 'A' to start bouncing the ball!" + 99;
	*strtbl[LANG_EN][HS_DISABLED] = *"Sorry - saving highscores disabled!" + 99;
	*strtbl[LANG_EN][HS_IS] = *"This Wii's highscore is %d" + 99;
	*strtbl[LANG_EN][HS_FAIL] = *"Highscore store failed" + 99;
	*strtbl[LANG_EN][YOU_SCORED] = *"You scored %d" + 99;
	*strtbl[LANG_EN][GAME_PAUSED] = *"Game Paused" + 99;
	*strtbl[LANG_EN][A2_CONTINUE] = *"Press 'A' to continue" + 99;
	return SUCCESS;
}

int loadLang(){
	return genEnglishStrings();
	FILE * langFile = fopen(LFPATH, "rb");				// open languages file
	if(langFile == NULL) return genEnglishStrings();	// if non-existant, just use English strings
	
	mxml_node_t *tree;
	mxml_node_t *translations;
	
	mxml_node_t *lang;
	mxml_node_t *string;
	
	tree = mxmlLoadFile(NULL, langFile, MXML_NO_CALLBACK);
	fclose(langFile);
	
	translations = mxmlFindElement(tree, tree, "translations", NULL, NULL, MXML_DESCEND);
	
	// english strings
	lang = mxmlFindElement(translations, tree, "language", "type", "en", MXML_DESCEND);
	
	string = mxmlFindElement(lang, tree, "string", "id", "0", MXML_DESCEND);
	*strtbl[LANG_EN][0] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "1", MXML_DESCEND);
	*strtbl[LANG_EN][1] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "2", MXML_DESCEND);
	*strtbl[LANG_EN][2] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "3", MXML_DESCEND);
	*strtbl[LANG_EN][3] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "4", MXML_DESCEND);
	*strtbl[LANG_EN][4] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "5", MXML_DESCEND);
	*strtbl[LANG_EN][5] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "6", MXML_DESCEND);
	*strtbl[LANG_EN][6] = *mxmlElementGetAttr(string, "text");

	// french strings
	lang = mxmlFindElement(translations, tree, "language", "type", "fr", MXML_DESCEND);
	
	string = mxmlFindElement(lang, tree, "string", "id", "0", MXML_DESCEND);
	*strtbl[LANG_FR][0] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "1", MXML_DESCEND);
	*strtbl[LANG_FR][1] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "2", MXML_DESCEND);
	*strtbl[LANG_FR][2] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "3", MXML_DESCEND);
	*strtbl[LANG_FR][3] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "4", MXML_DESCEND);
	*strtbl[LANG_FR][4] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "5", MXML_DESCEND);
	*strtbl[LANG_FR][5] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "6", MXML_DESCEND);
	*strtbl[LANG_FR][6] = *mxmlElementGetAttr(string, "text");
	
	// spanish strings
	lang = mxmlFindElement(translations, tree, "language", "type", "es", MXML_DESCEND);
	
	string = mxmlFindElement(lang, tree, "string", "id", "0", MXML_DESCEND);
	*strtbl[LANG_ES][0] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "1", MXML_DESCEND);
	*strtbl[LANG_ES][1] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "2", MXML_DESCEND);
	*strtbl[LANG_ES][2] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "3", MXML_DESCEND);
	*strtbl[LANG_ES][3] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "4", MXML_DESCEND);
	*strtbl[LANG_ES][4] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "5", MXML_DESCEND);
	*strtbl[LANG_ES][5] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "6", MXML_DESCEND);
	*strtbl[LANG_ES][6] = *mxmlElementGetAttr(string, "text");

	// german strings
	lang = mxmlFindElement(translations, tree, "language", "type", "de", MXML_DESCEND);
	
	string = mxmlFindElement(lang, tree, "string", "id", "0", MXML_DESCEND);
	*strtbl[LANG_DE][0] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "1", MXML_DESCEND);
	*strtbl[LANG_DE][1] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "2", MXML_DESCEND);
	*strtbl[LANG_DE][2] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "3", MXML_DESCEND);
	*strtbl[LANG_DE][3] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "4", MXML_DESCEND);
	*strtbl[LANG_DE][4] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "5", MXML_DESCEND);
	*strtbl[LANG_DE][5] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "6", MXML_DESCEND);
	*strtbl[LANG_DE][6] = *mxmlElementGetAttr(string, "text");

	// dutch strings
	lang = mxmlFindElement(translations, tree, "language", "type", "nl", MXML_DESCEND);
	
	string = mxmlFindElement(lang, tree, "string", "id", "0", MXML_DESCEND);
	*strtbl[LANG_NL][0] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "1", MXML_DESCEND);
	*strtbl[LANG_NL][1] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "2", MXML_DESCEND);
	*strtbl[LANG_NL][2] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "3", MXML_DESCEND);
	*strtbl[LANG_NL][3] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "4", MXML_DESCEND);
	*strtbl[LANG_NL][4] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "5", MXML_DESCEND);
	*strtbl[LANG_NL][5] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "6", MXML_DESCEND);
	*strtbl[LANG_NL][6] = *mxmlElementGetAttr(string, "text");
	
	// italian strings
	lang = mxmlFindElement(translations, tree, "language", "type", "it", MXML_DESCEND);
	
	string = mxmlFindElement(lang, tree, "string", "id", "0", MXML_DESCEND);
	*strtbl[LANG_IT][0] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "1", MXML_DESCEND);
	*strtbl[LANG_IT][1] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "2", MXML_DESCEND);
	*strtbl[LANG_IT][2] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "3", MXML_DESCEND);
	*strtbl[LANG_IT][3] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "4", MXML_DESCEND);
	*strtbl[LANG_IT][4] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "5", MXML_DESCEND);
	*strtbl[LANG_IT][5] = *mxmlElementGetAttr(string, "text");
	string = mxmlFindElement(lang, tree, "string", "id", "6", MXML_DESCEND);
	*strtbl[LANG_IT][6] = *mxmlElementGetAttr(string, "text");
	
	mxmlDelete(lang);
	mxmlDelete(string);
	mxmlDelete(tree);
	mxmlDelete(translations);
	
	return SUCCESS;
}

int getWiiDefaultLang(){
#ifdef LANG_OVERRIDE
	myLanguage = LANG_OVERRIDE;
	return SUCCESS;
#endif
	switch(CONF_GetLanguage()){
		case CONF_LANG_ENGLISH:
			myLanguage = LANG_EN;
			break;
		case CONF_LANG_FRENCH:
			myLanguage = LANG_FR;
			break;
		case CONF_LANG_GERMAN:
			myLanguage = LANG_DE;
			break;
		case CONF_LANG_DUTCH:
			myLanguage = LANG_NL;
			break;
		case CONF_LANG_SPANISH:
			myLanguage = LANG_ES;
			break;
		case CONF_LANG_ITALIAN:
			myLanguage = LANG_IT;
			break;
		default:
			myLanguage = LANG_PT;
			break;
	}
	return SUCCESS;
}
