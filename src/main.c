//
//  main.c
//  Extension
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"

static int update(void* userdata);
static int swapDarkMode(void* userdata, PDMenuItem* menuItem);
const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;
PDMenuItem* darkMode = NULL;

#define TEXT_WIDTH 86
#define TEXT_HEIGHT 16

int x = (LCD_COLUMNS-TEXT_WIDTH)/2;
int y = (LCD_ROWS-TEXT_HEIGHT)/2;
int dx = 1;
int dy = 2;
int darkModeValue = 0;



#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit ) {
		const char* err;
		font = pd->graphics->loadFont(fontpath, &err);
		
		if ( font == NULL ) {
			pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);
		}
		
		PDMenuItem* darkMode = pd->system->addCheckmarkMenuItem("Dark Mode", darkModeValue, swapDarkMode(pd, darkMode), pd);

		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);
	}
	
	return 0;
}

static int update(void* userdata)
{
	PlaydateAPI* pd = userdata;
	
	pd->graphics->clear(kColorWhite);
	pd->graphics->setFont(font);
	pd->graphics->drawText("Hello World!", strlen("Hello World!"), kASCIIEncoding, x, y);

	if(darkModeValue == 1) {
		pd->graphics->drawText("On", strlen("On"), kASCIIEncoding, 20, 0);
	}
	else {
		pd->graphics->drawText("Off", strlen("Off"), kASCIIEncoding, 20, 0);
	}



	x += dx;
	y += dy;
	
	if ( x < 0 || x > LCD_COLUMNS - TEXT_WIDTH )
		dx = -dx;
	
	if ( y < 0 || y > LCD_ROWS - TEXT_HEIGHT )
		dy = -dy;
        
	pd->system->drawFPS(0,0);

	return 1;
}

static int swapDarkMode(void* userdata, PDMenuItem* menuItem)
{
	PlaydateAPI* pd = userdata;

	darkModeValue = pd->system->getMenuItemValue(menuItem);

	pd->display->setInverted(darkModeValue);

	return 1;
}

