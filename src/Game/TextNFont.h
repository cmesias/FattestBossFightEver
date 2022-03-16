/*
 * TextNFont.h
 *
 *  Created on: Mar 8, 2022
 *      Author: healt
 */

#ifndef TEXTNFONT_H_
#define TEXTNFONT_H_

#include <SDL2/SDL_ttf.h>

#include "../LTexture.h"

class TextNFont {

public:
	// Textures
	LTexture gText;

	// Fonts
	TTF_Font *gFont13 = NULL;
	TTF_Font *gFont20 = NULL;
	TTF_Font *gFont24 = NULL;
	TTF_Font *gFont36 = NULL;

	// Load fonts
	void LoadFonts();

	// Free resources
	void FreeFonts();
};



#endif /* TEXTNFONT_H_ */
