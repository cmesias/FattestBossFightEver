/*
 * Tilec.h
 *
 *  Created on: Jul 21, 2017
 *      Author: Carl
 */

#ifndef TILEC_H_
#define TILEC_H_

#include "Maps.h"

class Tilec {
public:
	const int max = 1000;
	int count = 0;
	int multiW = 1;
	int multiH = 1;
	int tilew = 48;
	int tileh = 48;
	int id = 0;			// 0: blue collision tile

public:
	float x,y;
	int w,h;
	int type;
	int layer;
	bool mouse;
	bool mouseBox;
	bool collision;
	bool alive;
	std::string side;		// collision with player

public:	// Core Functions
	void Init(Tilec tilec[]);
	void Update(Tilec tilec[], Map &map, int newMx, int newMy, int mex, int mey, int camx, int camy);
	void Render(SDL_Renderer *gRenderer, Tilec tilec[], int camx, int camy);

public:	// Collision functions

	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

	// Tile collision check, player x position
	void checkCollisionX(Tilec tilec[], float &x, float y, int w, int h, std::string &jumpstate, float &vX, int &jumps);

	// Tile collision check, player y position
	void checkCollisionY(Tilec tilec[], float x, float &y, int w, int h, std::string &jumpstate, float &vY, int &jumps, bool &jump);

public: // Editor functions
	void RemoveTile(Tilec tilec[], int click);
	void Copy(Tilec tilec[]);
	void Clear(Tilec tilec[]);
	void Spawn(Tilec tilec[], float x, float y, int w, int h);
	void SpawnAll(Tilec tilec[], int newMx, int newMy, int camx, int camy);

public: // tile saving functions
	void LoadData(Tilec tilec[], std::fstream &fileTileDataL);	// Load Tile data
	std::string SaveData(Tilec tilec[]);	// Save Tile data
};

#endif /* TILEC_H_ */
