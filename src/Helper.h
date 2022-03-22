/*
 * Helper.h
 *
 *  Created on: May 18, 2017
 *      Author: Carl
 */

#ifndef LOCAL_HELPER_H_
#define LOCAL_HELPER_H_

#include "Game/Timers.h"

class Helper {

public:
    Timer fps;

public:
    // fps
    bool cap 					= true;
	int frame 					= 0;
    const int FRAMES_PER_SECOND = 60;
    int screenWidth = 1920;
    int screenHeight = 1080;
    //const int screenWidth = 1280;
    //const int screenHeight = 720;

    static const int UniqueTileBarTiles = 288;
    //const int screenWidth = 1008;
    // const int screenHeight = 624;
	//const int screenWidth = 2304;
	//const int screenHeight = 1296;
	//const int screenWidth = 2880;
    //const int screenHeight = 1620;
    //const int screenWidth = 1280;
    //const int screenHeight = 720;
     //const int screenWidth = 640;
    //const int screenHeight = 360;
    //const int screenWidth = 480;
    //const int screenHeight = 270;

public:

	// Generate a random double number
	double randDouble(double fMin, double fMax);

	// Set Texture clips
	void setClips(SDL_Rect &tempRect, int x, int y, int w, int h);

	// Check sollision
	bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);

	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

	// Doesnt work properly (3/17/2022)
	void ChangeRenderResolution(SDL_Renderer *gRenderer, int changeTo);

	void RenderRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color);

	void RenderFillRect(SDL_Renderer *gRenderer, float x, float y, float w, float h, SDL_Color color);

};

#endif /* LOCAL_HELPER_H_ */
