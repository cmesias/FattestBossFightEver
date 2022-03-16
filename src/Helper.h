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
    //const int screenWidth = 1920;
    //const int screenHeight = 1080;
    //const int screenWidth = 1008;
    // const int screenHeight = 624;
	//const int screenWidth = 2304;
	//const int screenHeight = 1296;
	//const int screenWidth = 2880;
	//const int screenHeight = 1620;
    const int screenWidth = 1280;
    const int screenHeight = 720;
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

};

#endif /* LOCAL_HELPER_H_ */
