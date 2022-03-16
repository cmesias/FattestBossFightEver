/*
 * Tile.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: Carl
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mouse.h>

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "Tiles.h"

void Tile::load(SDL_Renderer *gRenderer) {
	gTiles.loadFromFile(gRenderer, "resource/gfx/castle_2.png");
	gFont12 = TTF_OpenFont("resource/fonts/Viga-Regular.ttf", 12);
}

void Tile::free() {
	gTiles.free();
	gText.free();
	TTF_CloseFont(gFont12);
	gFont12 = NULL;
}


void Tile::initTile(Tile tile[]) {
	layer 		= 0;
	id 			= 0;
	tileCount 	= 0;
	multiW 		= 1;
	multiH 		= 1;
	hideOtherLayers 	= false;
	for (int i = 0; i < max; i++) {
		tile[i].w = tilew;
		tile[i].h = tileh;
		tile[i].id = -1;
		tile[i].alpha = 255;
		tile[i].layer = -1;
		tile[i].animTimer = 0;
		tile[i].animFrame = 0;
		tile[i].mouse = false;
		tile[i].mouseBox = false;
		tile[i].screen = false;
		tile[i].player = false;
		tile[i].side = "right";
		tile[i].collisionTile = false;
		tile[i].alive = false;
	}
}

void Tile::placeTile(Tile tile[], float x, float y, int id, int layer, bool collisionTile, SDL_Rect clip) {
	for (int i = 0; i < max; i++) {
		if (!tile[i].alive){
			tile[i].x 		= x;
			tile[i].y 		= y;
			tile[i].id 		= id;
			tile[i].alpha 	= 255;
			tile[i].clip 	= clip;
			tile[i].layer 	= layer;
			tile[i].collisionTile 	= collisionTile;
			tile[i].animTimer = 0;
			tile[i].animFrame = 0;
			tile[i].mouse 	= false;
			tile[i].mouseBox = false;
			tile[i].screen 	= false;
			tile[i].player = false;
			tile[i].side = "right";
			tile[i].alive 	= true;
			tileCount++;
			break;
		}
	}
}
void Tile::copyTile(Tile tile[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].mouse){
				if (tile[i].layer == layer) {
					id = tile[i].id;
					collisionTile = tile[i].collisionTile;
				}
			}
		}
	}
}
void Tile::changeTileCollision(Tile tile[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].mouse){
				if (tile[i].layer == layer) {
					tile[i].collisionTile = (!tile[i].collisionTile);
				}
			}
		}
	}
}
void Tile::changeTileLayer(Tile tile[], int value) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			if (tile[i].mouse){
				if (tile[i].layer == layer) {
					tile[i].layer += value;
				}
			}
		}
	}
}
void Tile::removeTile(Tile tile[], int click) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			// Left click
			if (click == 0) {
				if (tile[i].mouseBox){
					if (tile[i].layer == layer) {
						tile[i].alive = false;
						tileCount--;
					}
				}
			// Right click
			}else{
				if (tile[i].mouse){
					if (tile[i].layer == layer) {
						tile[i].alive = false;
						tileCount--;
					}
				}
			}
		}
	}
}

void Tile::removeAllTiles(Tile tile[]) {
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			tile[i].alive = false;
		}
	}
	tileCount = 0;
}

/* Place Tile */
void Tile::spawnTile(Tile tile[], int newMx, int newMy, int camx, int camy, SDL_Rect rTiles[]) {
	removeTile(tile, 0);
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			int x = int(newMx + j * tilew+camx);
			int y = int(newMy + h * tileh+camy);
			placeTile(tile, x, y, id, layer, collisionTile, rTiles[id]);
		}
	}
}

void Tile::updateTile(Tile tile[], LWindow &gWindow, int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]) {
	//std::cout << "newMx: " << newMx << std::endl;
	//std::cout << "newMy: " << newMy << std::endl;
	int tileW = tilew*multiW;
	int tileH = tileh*multiH;
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){
			//If the tile is in the screen
			if (tile[i].x + tile[i].w > camx-64 && tile[i].x < camx + gWindow.getWidth()+64
			 && tile[i].y + tile[i].h > camy-64 && tile[i].y < camy + gWindow.getHeight()+64) {
				tile[i].screen = true;
			} else {
				tile[i].screen = false;
			}
			//If the mouse+size is on the tile
			if (newMx+tileW-4 > tile[i].x && newMx+2 < tile[i].x + tile[i].w &&
					newMy+tileH-4 > tile[i].y && newMy+2 < tile[i].y + tile[i].h) {
				tile[i].mouseBox = true;
			} else {
				tile[i].mouseBox = false;
			}
			//If the mouse is on the tile
			if (mex > tile[i].x && mex < tile[i].x + tile[i].w &&
				mey > tile[i].y && mey < tile[i].y + tile[i].h) {
				tile[i].mouse = true;
			} else {
				tile[i].mouse = false;
			}
			// Reset layer if goes higher
			if (tile[i].layer > 6) {
				tile[i].layer = 0;
			}
			// Animations
			if (tile[i].id == 143) {
				tile[i].animTimer++;
				if (tile[i].animTimer > 30) {
					tile[i].animTimer = 0;
					tile[i].animFrame++;
				}
				if (tile[i].animFrame > 2) {
					tile[i].animFrame=0;
					tile[i].animTimer=0;
					tile[i].id = 142;
					tile[i].clip = rTiles[142];
				}
			}
			if (tile[i].id == 142) {
				tile[i].animTimer++;
				if (tile[i].animTimer > 30) {
					tile[i].animTimer = 0;
					tile[i].animFrame++;
				}
				if (tile[i].animFrame > 2) {
					tile[i].animFrame=0;
					tile[i].animTimer=0;
					tile[i].id = 143;
					tile[i].clip = rTiles[143];
				}
			}
			if (tile[i].id == 166) {
				tile[i].animTimer++;
				if (tile[i].animTimer > 9) {
					tile[i].animTimer = 0;
					tile[i].animFrame++;
				}
				if (tile[i].animFrame > 2) {
					tile[i].animFrame=0;
					tile[i].animTimer=0;
					tile[i].id = 167;
					tile[i].clip = rTiles[167];
				}
			}
			if (tile[i].id == 167) {
				tile[i].animTimer++;
				if (tile[i].animTimer > 9) {
					tile[i].animTimer = 0;
					tile[i].animFrame++;
				}
				if (tile[i].animFrame > 2) {
					tile[i].animFrame=0;
					tile[i].animTimer=0;
					tile[i].id = 166;
					tile[i].clip = rTiles[166];
				}
			}
		}
	}
}


bool Tile::checkCollisionRect( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA,   leftB;
    int rightA,  rightB;
    int topA, 	 topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA 	= a.x;
    rightA 	= a.x + a.w;
    topA 	= a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB 	= b.x;
    rightB 	= b.x + b.w;
    topB 	= b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA < topB )
    {
        return false;
    }

    if( topA > bottomB )
    {
        return false;
    }

    if( rightA < leftB )
    {
        return false;
    }

    if( leftA > rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

// Tile collision check, player x position
void Tile::checkCollisionXY(Tile tile[],
		float &x, float &y,
		int w, int h,
		float &vX, float &vY,
		std::string &jumpstate, int &jumps) {

	{
		// Update collision with Tiles
		//tl.checkCollision(tile, x, y, w, h, y, vY);

		// Update collision with Tiles
		// Player Velocity X Axis
		x += vX;
		SDL_Rect rectA;
		rectA.x = x+4;
		rectA.y = y+8;
		rectA.w = w-8;
		rectA.h = h/2;
		bool moveBack;
		moveBack = false;
		for (int i = 0; i < max; i++) {
			if (tile[i].alive){
				if (tile[i].collisionTile) {
					SDL_Rect rectB;
					rectB.x = tile[i].x;
					rectB.y = tile[i].y;
					rectB.w = tile[i].w;
					rectB.h = tile[i].h;
					if  ( checkCollisionRect( rectA, rectB )) {
						/*// If Player has more than 0 keys, then unlock door, otherwise continue collision check
						if (keys > 0 && useKey) {
							useKey = false;
							if (tile[i].id == 65 || tile[i].id == 69 || tile[i].id == 73 || tile[i].id == 77 ||
								tile[i].id == 67 || tile[i].id == 71 || tile[i].id == 75 || tile[i].id == 79) {
								keys--;
								Mix_PlayChannel(-1, sKeyPickup, 0);
								tile[i].alive = false;
								tl.tileCount--;
							}
						}*/
						// Continue handling collision
						moveBack = true;
					}
				}
			}
		}
		if (moveBack){
			x -= vX;
		}

		// Player Velocity Y Axis
		y += vY;;
		rectA.x = x+4;
		rectA.y = y+8;
		rectA.w = w-8;
		rectA.h = h/2;
		moveBack = false;
		for (int i = 0; i < max; i++) {
			if (tile[i].alive){
				if (tile[i].collisionTile) {
					SDL_Rect rectB;
					rectB.x = tile[i].x;
					rectB.y = tile[i].y;
					rectB.w = tile[i].w;
					rectB.h = tile[i].h;
					if  ( checkCollisionRect( rectA, rectB )) {
						// If Player has more than 0 keys, then unlock door, otherwise continue collision check
						/*if (keys > 0 && useKey) {
							useKey = false;
							if (tile[i].id == 65 || tile[i].id == 69 || tile[i].id == 73 || tile[i].id == 77 ||
								tile[i].id == 67 || tile[i].id == 71 || tile[i].id == 75 || tile[i].id == 79) {
								keys--;
								Mix_PlayChannel(-1, sKeyPickup, 0);
								tile[i].alive = false;
								tl.tileCount--;
							}
						}*/
						// Continue handling collision
						moveBack = true;
					}
				}
			}
		}
		if (moveBack){
			y -= vY;
		}
	}
}

void Tile::renderTile(SDL_Renderer *gRenderer, Tile tile[], int layer_dummy, int camx, int camy) {
	for (int i = 0; i < max; i++)
	{
		if (tile[i].alive)
		{
			// If the tile is on the screen
			if (tile[i].screen)
			{
				// Tile trasparency on Player collision
				if (tile[i].layer != 0 && tile[i].player)
				{
					//tile[i].alpha = 90;
				}else{
					//tile[i].alpha = 255;
				}
				// Render layer in hand
				if (hideOtherLayers)
				{
					if (layer == tile[i].layer)
					{
						gTiles.setAlpha(tile[i].alpha);
						gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
					}
				// Render all tiles
				}else{
					if (layer_dummy == tile[i].layer) {
						gTiles.setAlpha(tile[i].alpha);
						gTiles.render(gRenderer, tile[i].x - camx, tile[i].y - camy, tile[i].w, tile[i].h, &tile[i].clip);
					}
				}
			}
		}
	}
}
// Render Tile Debug info
void Tile::renderTileDebug(SDL_Renderer *gRenderer, Tile tile[], int newMx, int newMy, int mex, int mey, int camx, int camy, SDL_Rect rTiles[]){
	// Render Tile info
	for (int i = 0; i < max; i++) {
		if (tile[i].alive){

			// If mouse is hoveirng on Tile and we have the same layer in Hand
			if (tile[i].mouse && tile[i].layer == layer) {
				std::stringstream tempss;
				tempss << tile[i].layer;
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont12);
				gText.setAlpha(255);
				gText.render(gRenderer, tile[i].x+tile[i].w-gText.getWidth() - camx, tile[i].y - camy, gText.getWidth(), gText.getHeight());

				// If its a collision tile, render filled blue square on top left inside the Tile
				if (tile[i].collisionTile) {
					SDL_Rect tempr = {tile[i].x+6 - camx, tile[i].y+6 - camy, 10, 10};
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
					SDL_RenderFillRect(gRenderer, &tempr);
				} else {
					SDL_Rect tempr = {tile[i].x+6 - camx, tile[i].y+6 - camy, 10, 10};
					SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
					SDL_RenderDrawRect(gRenderer, &tempr);
				}

				// Render green square inside tile to let the editor know we are hovering over it
				SDL_Rect tempr = {tile[i].x+6 - camx, tile[i].y+6 - camy, tile[i].w-12, tile[i].h-12};
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
		}
	}
	// Render tile in hand
	for (int j = 0; j < multiW; j++) {
		for (int h = 0; h < multiH; h++) {
			gTiles.setAlpha(110);
			gTiles.render(gRenderer, newMx+j*tilew, newMy+h*tileh, tilew, tileh, &rTiles[id]);

			// Render mouse coordinates snapped to grid
			SDL_Rect tempr = {newMx, newMy, tilew*multiW, tileh*multiH};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);

			// Render mouse + size * multiplied
			// Render mouse coordinates unsnapped
			/*tempr = {mex, mey, tilew*multiW, tileh*multiH};
			SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(gRenderer, &tempr);*/
		}
	}
}

void Tile::LoadData(Tile tile[], std::fstream &fileToReturn)
{
	//std::cout << "Attempting to load... Tile" << std::endl;
	// Load Tile count data
	fileToReturn >> this->tileCount;
	//std::cout << this->tileCount << std::endl;

	// Load rest of Tile data
	for (int i = 0; i < this->tileCount; i++) {
		if (!tile[i].alive) {
			fileToReturn >>  tile[i].x 				>>
							  tile[i].y 			>>
							  tile[i].w 			>>
							  tile[i].h 			>>
							  tile[i].id 			>>
							  tile[i].collisionTile >>
							  tile[i].layer 		>>
							  tile[i].alpha 		>>
							  tile[i].clip.x 		>>
							  tile[i].clip.y 		>>
							  tile[i].clip.w 		>>
							  tile[i].clip.h 		>>
							  tile[i].mouse 		>>
							  tile[i].screen 		>>
							  tile[i].alive;

			//std::cout << "Tile i: " << i << ", x: " << tile[i].x << ", y: " << tile[i].y << std::endl;
		}
		//break;
	}
}

// Save Tile data
std::string Tile::SaveData(Tile tile[])
{
	/////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------//
	//----------------------------- Save Everything in One File -------------------//

	// Save all tile data in a string first
	std::stringstream tempsstb;
	for (int i = 0; i < max; i++) {
		if (tile[i].alive) {
			tempsstb << tile[i].x 				<< " "
					 << tile[i].y 				<< " "
					 << tile[i].w  				<< " "
					 << tile[i].h  				<< " "
					 << tile[i].id  			<< " "
					 << tile[i].collisionTile 	<< " "
					 << tile[i].layer 			<< " "
					 << tile[i].alpha  			<< " "
					 << tile[i].clip.x 			<< " "
					 << tile[i].clip.y 			<< " "
					 << tile[i].clip.w 			<< " "
					 << tile[i].clip.h 			<< " "
					 << tile[i].mouse 			<< " "
					 << tile[i].screen 			<< " "
			   	   	 << tile[i].alive 			<< "\n";
		}
	}
	std::string temps;
	temps = tempsstb.str().c_str();

	// Commented out because this takes out the extra line thats empty at the end
	//temps = temps.substr(0, temps.size()-1);

	// Save number of tiles in World
	std::stringstream aVeryLongString;
	aVeryLongString << tileCount << "\n";

	// Save the data of all Tiles
	aVeryLongString << temps.c_str();
	//----------------------------- Save Everything in One File -------------------//
	//-----------------------------------------------------------------------------//
	/////////////////////////////////////////////////////////////////////////////////

	// Return save data for all Tiles
	return aVeryLongString.str().c_str();
}


