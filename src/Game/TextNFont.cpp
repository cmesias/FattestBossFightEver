/*
 * gTextAndFonts.cpp
 *
 *  Created on: Mar 8, 2022
 *      Author: healt
 */



#include "TextNFont.h"

void TextNFont::LoadFonts() {

	// Load fonts
	gFont13 = TTF_OpenFont("fonts/Viga-Regular.ttf", 13);
	gFont20 = TTF_OpenFont("fonts/Viga-Regular.ttf", 20);
	gFont24 = TTF_OpenFont("fonts/Viga-Regular.ttf", 24);
	gFont36 = TTF_OpenFont("fonts/Viga-Regular.ttf", 36);
}

void TextNFont::FreeFonts() {

	// Free textures
	gText.free();

	// Free fonts
	TTF_CloseFont(gFont13);
	TTF_CloseFont(gFont20);
	TTF_CloseFont(gFont24);
	TTF_CloseFont(gFont36);
	gFont13 = NULL;
	gFont20 = NULL;
	gFont24 = NULL;
	gFont36 = NULL;
}
