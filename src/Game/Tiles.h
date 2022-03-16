/*
 * Tile.h
 *
 *  Created on: Jan 16, 2017
 *      Author: Carl
 */

#ifndef LOCAL_TILES_H_
#define LOCAL_TILES_H_

#include "../LWindow.h"
#include "../LTexture.h"

class Tile {
public:
	/* Tile Variables */
	const int max = 5000;
	int tileCount = 0;
	int multiW = 1;
	int multiH = 1;
	int tilew = 48;
	int tileh = 48;
	bool hideOtherLayers = false;
	LTexture gTiles;
	LTexture gText;
	TTF_Font *gFont12 = NULL;
public:
	float x;
	float y;
	int w;
	int h;
	int id;
	int alpha;
	SDL_Rect clip;
	int layer;
	int animTimer;
	int animFrame;
	bool mouse;				// Mouse point
	bool mouseBox;			// Mouse box
	bool screen;
	bool player;
	bool alive;
	std::string side;		// collision with player
	std::string sideSlime;	// collision with Slime
	bool collisionTile;		// If true, this Tile is considered a collision tile

public:	// Initial
	void initTile(Tile tile[]);

	void load(SDL_Renderer *gRenderer);

	void free();

public:	// editor functions
	void placeTile(Tile tile[], float x, float y, int id, int layer, bool collisionTile, SDL_Rect clip);

	void copyTile(Tile tile[]);

	void changeTileCollision(Tile tile[]);

	void changeTileLayer(Tile tile[], int value);

	void removeTile(Tile tile[], int click);

	void removeAllTiles(Tile tile[]);

	/* Place Tile */
	void spawnTile(Tile tile[], int newMx, int newMy, int camx, int camy, SDL_Rect rTiles[]);

public:	// core game functions
	void updateTile(Tile tile[], LWindow &gWindow, int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]);

	bool checkCollisionRect( SDL_Rect a, SDL_Rect b );

	// Tile collision check, player x position
	void checkCollisionXY(Tile tile[], float &x, float &y, int w, int h, float &vX, float &vY, std::string &jumpstate, int &jumps);

	void renderTile(SDL_Renderer *gRenderer, Tile tile[], int layer_dummy, int camx, int camy);

	// Render Tile Debug info
	void renderTileDebug(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]);

public: // tile saving functions

	// Load Tile data
	void LoadData(Tile tile[], std::fstream &fileToReturn);

	// Save Tile data
	std::string SaveData(Tile tile[]);
};

#endif /* LOCAL_TILES_H_ */
