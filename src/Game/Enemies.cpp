/*
 * Enemies.cpp
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mouse.h>

#include "Enemies.h"

// Load enemy resources
void Enemy::load(SDL_Renderer* gRenderer){
	gEnemyShip.loadFromFile(gRenderer, "resource/gfx/enemy.png");
}

// Free enemy resources
void Enemy::free(){
	gEnemyShip.free();
}

// Initialize
void Enemy::init(Enemy enemy[]){
	ENEMY_COUNT = 0;
	ENEMY_COUNT = 0;
	ENEMY_SPAWN = 1;
	mileScore	= 0;
	SPAWN_ENEMY	= false;
	for (int i=0; i<Max; i++){
		enemy[i].vX 			= 0.0;
		enemy[i].vY 			= 0.0;
		enemy[i].speed 			= 0.0;
		enemy[i].timer			= 0;
		enemy[i].health 		= 100;
		enemy[i].damagedTimer 	= 0;
		enemy[i].damaged 		= false;
		enemy[i].collide 		= false;
		enemy[i].alive 			= false;
	}
}

// Clear all asteroids
void Enemy::clear(Enemy enemy[]){
	for (int i = 0; i < this->Max; i++) {
		enemy[i].alive = false;
	}
	this->ENEMY_COUNT = 0;
}

// Create
void Enemy::spawn(Enemy enemy[], float x, float y, int w, int h, double vX, double vY, double speed){
	for (int i=0; i<Max; i++){
		if (!enemy[i].alive){
			enemy[i].x		 		= x;
			enemy[i].y 				= y;
			enemy[i].w 				= w;
			enemy[i].h 				= h;
			enemy[i].vX 			= vX;
			enemy[i].vY 			= vY;
			enemy[i].speed 			= speed;
			enemy[i].health 		= 100;
			enemy[i].damagedTimer 	= 0;
			enemy[i].damaged 		= false;
			enemy[i].collide		= false;
			enemy[i].alive 			= true;
			ENEMY_COUNT++;
			break;
		}
	}
}

// Update Enemy
void Enemy::update(Enemy enemy[],
			Particle particle[], Particle &p_dummy,
			bool alive,
			float targetX, float targetY, int objectRadius,
			unsigned int &score, int mapX, int mapY, int mapW, int mapH,
			Mix_Chunk *sLazer, Mix_Chunk *sAtariBoom){
	for (int i=0; i<Max; i++){
		if (enemy[i].alive){


			// Enemy movement
			enemy[i].x += enemy[i].vX * enemy[i].speed;
			enemy[i].y += enemy[i].vY * enemy[i].speed;

			// Enemy death
			if (enemy[i].health <= 0){
				//Spawn explosion after asteroid death
				for (double j=0.0; j< 360.0; j+=rand() % 10 + 10){
					int rands = rand() % 3 + 2;
					p_dummy.spawnParticleAngle(particle, 2,
							enemy[i].x+enemy[i].w/2-rands/2,
							enemy[i].y+enemy[i].h/2-rands/2,
									   rands, rands,
									   j, randDouble(0.1, 1.5),
									   0.0, 0, 0,
									   {255, 255, 255, 255}, 1,
									   1, 1,
									   rand() % 100 + 150, rand() % 2 + 2,
									   100, 0,
									   false, 0);
				}
				enemy[i].alive = false;
				ENEMY_COUNT--;

				score += 1000;
			}

			// Target alive
			if (alive){

				// Enemy death upon collision with Target
				float bmx = targetX;
				float bmy = targetY;
				float distance = sqrt((bmx - enemy[i].x - enemy[i].w / 2)* (bmx - enemy[i].x- enemy[i].w / 2) +
								      (bmy - enemy[i].y - enemy[i].h / 2)* (bmy - enemy[i].y- enemy[i].h / 2));
				if (distance < objectRadius + enemy[i].w/2){
					//Spawn explosion after asteroid death
					enemy[i].alive = false;
					ENEMY_COUNT--;

					score += 1000;
				}

				// Get some values used for shooting
				float angle = atan2(bmy - enemy[i].y-enemy[i].h/2, bmx - enemy[i].x-enemy[i].w/2);
				angle = angle * (180 / 3.1416);
				float radians = (3.1415926536/180)*(angle);
				float Cos = floor(cos(radians)*10+0.5)/10;
				float Sin = floor(sin(radians)*10+0.5)/10;

				// Enemy shooting
				enemy[i].timer += 1;
				if (enemy[i].timer > 120){
					enemy[i].timer = 0;
					Mix_PlayChannel(-1, sLazer, 0);
					p_dummy.spawnParticleAngle(particle, 1,
									   enemy[i].x+enemy[i].w/2-4/2+(Cos*16),
									   enemy[i].y+enemy[i].h/2-4/2+(Sin*16),
									   4, 4,
									   angle, 6,
									   50, 0, 100,
									   white, 1,
									   1, 1,
									   255, 1,
									   200, 2,
									   false, 0);
				}
			}

			// Asteroid damaged animation timer
			if (enemy[i].damaged){
				enemy[i].damagedTimer += 1;
				if (enemy[i].damagedTimer > 20){
					enemy[i].damaged = false;
					enemy[i].damagedTimer = 0;
				}
			}

			// Enemy death upon going off screen
			if (enemy[i].x+enemy[i].w < mapX-512){
				enemy[i].alive = false;
				ENEMY_COUNT--;
			}
			if (enemy[i].x > mapX+mapW+512){
				enemy[i].alive = false;
				ENEMY_COUNT--;
			}
		}
	}
}

// Render Enemy
void Enemy::render(Enemy enemy[], int camx, int camy, SDL_Renderer* gRenderer){
	for (int i=0; i<Max; i++){
		if (enemy[i].alive){
			gEnemyShip.render(gRenderer, enemy[i].x-camx, enemy[i].y-camy, enemy[i].w, enemy[i].h);
		}
	}
}
