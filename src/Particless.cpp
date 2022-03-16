/*
 * Particless.cpp
 *
 *  Created on: Dec 16, 2016
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
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mouse.h>

#include "Game/Players.h"
#include "Particless.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// ENEMY BULLETS //////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------//

void Particle::load(SDL_Renderer* gRenderer){
	setClips(cParticles[0], 0, 0, 8, 8);
	setClips(cParticles[1], 8, 0, 8, 8 );
	setClips(cParticles[2], 16, 0, 8, 8);
	setClips(cParticles[3], 24, 0, 8, 8);
	setClips(cParticles[4], 32, 0, 8, 8);
	setClips(cParticles[5], 48, 0, 8, 8);
	gParticles.loadFromFile(gRenderer, "img/particle/particles.png");
	gParticles.setBlendMode(SDL_BLENDMODE_ADD);
	spr_bullet.loadFromFile(gRenderer, "resource/gfx/player/basic_spell.png");
	spr_bullet_blue.loadFromFile(gRenderer, "resource/gfx/player/basic_spell.png");
	blue_block.loadFromFile(gRenderer, "resource/gfx/blue_block.png");

	// From other class
	LoadFonts();
}

void Particle::free(){
	gParticles.free();
	spr_bullet.free();
	spr_bullet_blue.free();
	blue_block.free();

	// From other class
	//textFont.Free();

	// Free textures
	gText.free();

	// From other class
	FreeFonts();
}

void Particle::init(Particle particle[]) {
	count = 0;
	for (int i = 0; i < max; i++) {
		particle[i].x 				= 0;
		particle[i].y 				= 0;
		particle[i].w 				= 4;
		particle[i].h 				= 4;
		particle[i].alpha 			= 255;
		particle[i].time 			= 0;
		particle[i].alphaspeed 		= 0;
		particle[i].deathTimer 		= 100;
		particle[i].deathTimerSpeed = 1;
		particle[i].speed 			= 0.00;
		particle[i].vX 				= 0.00;
		particle[i].vY 				= 0.00;
		particle[i].speed 			= 0.00;
		particle[i].angle 			= 0.00;
		particle[i].angleSpe 		= 0;
		particle[i].angleDir 		= 0;
		particle[i].onScreen 		= false;
		particle[i].collide 		= false;
		particle[i].alive 			= false;
		particle[i].sizeDeath 		= false;
		particle[i].splatSpeed 		= 0;
		particle[i].splatDistance 	= 0;
		particle[i].deathSpe 		= 0;
		particle[i].side 			= "";
		particle[i].type 			= 0;
		particle[i].damage 			= 0;
		particle[i].color 			= { 255, 255, 255, 255 };
	}
}

// Spawn a Particle: moves based on angle given and fixed speed
void Particle::spawnParticleAngle(Particle particle[], int type,
		float spawnX, float spawnY,
		int spawnW, int spawnH,
		double angle, double speed,
		double damage,
		SDL_Color color, int layer,
		int angleSpe, int angleDir,
		int alpha, int alphaspeed,
		int deathTimer, int deathTimerSpeed,
		bool sizeDeath, float deathSpe,
		double splatDistance, double splatSpeed) {
	for (int i = 0; i < max; i++)
	{
		if (!particle[i].alive)
		{
			particle[i].x 				= spawnX;
			particle[i].y 				= spawnY;
			particle[i].w 				= spawnW;
			particle[i].h 				= spawnH;
			particle[i].x2 				= spawnX + spawnW/2;
			particle[i].y2 				= spawnY + spawnH/2;
			particle[i].time 			= 0;
			particle[i].angle 			= angle;
			particle[i].speed 			= speed;
			particle[i].vX 				= (cos( (3.14159265/180)*(angle) ));
			particle[i].vY 				= (sin( (3.14159265/180)*(angle) ));
			particle[i].type 			= type;
			particle[i].damage 			= damage;
			//particle[i].x 				= spawnX + (rand() % 4 + 2 * (cos( (3.14159265/180)*(angle) )));
			//particle[i].y 				= spawnY + (rand() % 4 + 2 * (sin( (3.14159265/180)*(angle) )));
			//particle[i].x 				= spawnX + cos( (3.14159265/180)*(angle) );
			//particle[i].y 				= spawnY + sin( (3.14159265/180)*(angle) );


			particle[i].side 			= "";

			particle[i].onScreen 		= false;
			particle[i].collide 		= false;
			particle[i].splatDistance 	= splatDistance;
			particle[i].splatSpeed 		= splatSpeed;
			particle[i].color 			= color;
			particle[i].layer 			= layer;

			particle[i].alphaspeed 		= alphaspeed;
			particle[i].alpha 			= alpha;
			particle[i].deathTimer 		= deathTimer;
			particle[i].deathTimerSpeed = deathTimerSpeed;
			particle[i].angleSpe		= angleSpe;
			particle[i].angleDir		= angleDir;
			particle[i].sizeDeath 		= sizeDeath;
			particle[i].deathSpe 		= deathSpe;
			particle[i].alive 			= true;
			count++;
			break;
		}
	}
}

// spawn explosion
void Particle::spawnExplosion(Particle particle[], float x, float y, int size) {
	// spawn blood particle effect
	for (double j=0.0; j< 360.0; j+=rand() % 10 + 10){
		double f = (double)rand() / RAND_MAX;
		double num = 2.1 + f * (5.1 - 2.1);
		spawnParticleAngle(particle, 2,
							x-size/2,
							y-size/2,
							size, size,
						   j, num,
						   0.0,
						   {255, 0, 255, 255}, 1,
						   1, 1,
						   rand() % 100 + 150, rand() % 2 + 5,
						   rand() % 50 + 90, 0,
						   true, 0.1,
						   rand() % 15 + 2, 1);
	}
}

// Spawn a Particle: moves based on a fixed velocity given and fixed speed
/*void Particle::fireParticle(Particle particle[], int type, int damage,
									   int spawnx, int spawny, int w, int h,
									   double speed, float vX, float vY,
									   int alphaspeed, int alpha,
									   int deathTimer, int deathTimerSpeed, double angle, double angleSpe, double angleDir,
									   SDL_Color color, int layer) {
	for (int i = 0; i < 1024; i++) {
		if (!particle[i].alive) {
			particle[i].damage 			= damage;
			particle[i].type 			= type;
			particle[i].alphaspeed 		= alphaspeed;
			particle[i].alpha  			= alpha;
			particle[i].x  				= spawnx;
			particle[i].y  				= spawny;
			particle[i].w  				= w;
			particle[i].h  				= h;
			particle[i].speed  			= speed;
			particle[i].angle 			= angle;
			particle[i].angleSpe		= angleSpe;
			particle[i].angleDir		= angleDir;
			particle[i].vX  			= vX;
			particle[i].vY  			= vY;
			particle[i].deathTimer 		= deathTimer;
			particle[i].deathTimerSpeed = deathTimerSpeed;
			particle[i].sizeDeath 		= false;
			particle[i].deathSpe 		= 0;
			particle[i].time 			= 0;
			particle[i].side 			= "";
			particle[i].onScreen 		= false;
			particle[i].collide 		= false;
			particle[i].alive 			= true;
			particle[i].color 			= color;
			particle[i].layer 			= layer;
			//particle[i].angle 		= atan2 ( radianSin, radianCos) * 180 / PI;
			PARTICLES++;
			break;
		}
	}
}*/

// Generate Stars
void Particle::genStars(Particle particle[], int screnWidth, int screenHeight){
	/*this->ptimer += 1;
	if (this->ptimer > 10){
		this->ptimer = 0;
		for (int i=0; i<65; i++){
			int randAlpha = rand() % 100+150;
			int cW = screnWidth + 200;
			int cH = screenHeight + 200;
			int randW = rand() % cW;
			int randH = rand() % cH;
			SDL_Color tempColor = { rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1};
			int randl = rand() % 5 + 5;
			int randw = rand() % 2 + 2;

			fireParticle(particle, 2, 0,
									0-100 + randW, 0-100 + randH,
									10/randl, 10/randl,
									0,
									0.0,0.0, 1, randAlpha,
									100, 1, 0.0, 0.0, 0.0,
									tempColor, randl);
		}
	}*/
}

// Spawn a Particle: moves based on location of a target given
/*void Particle::fireParticle2(Particle particle[], int type, int damage, int spawnx, int spawny, int w, int h, int targetx,int targety,
										int alphaspeed, int alpha,
										int deathTimer, int deathTimerSpeed,
										double speed,
										SDL_Color color, int layer) {
	for (int i = 0; i < 1024; i++) {
		if (!particle[i].alive) {
			particle[i].damage 			= damage;
			particle[i].alpha  			= alpha;
			particle[i].deathTimer 		= deathTimer;
			particle[i].alphaspeed 		= alphaspeed;
			particle[i].deathTimerSpeed = deathTimerSpeed;
			particle[i].type 			= type;
			particle[i].x 				= spawnx;
			particle[i].y 				= spawny;
			particle[i].w 				= w;
			particle[i].h 				= h;
			particle[i].speed 			= speed;
			float bmx 					= targetx;
			float bmy 					= targety;
			float distance 				= sqrt((bmx - spawnx) * (bmx - spawnx - particle[i].w / 2) +
											   (bmy - spawny) * (bmy - spawny - particle[i].h / 2));
			particle[i].vX 				= particle[i].speed  *  (bmx - spawnx - particle[i].w / 2) / distance;
			particle[i].vY 				= particle[i].speed  *  (bmy - spawny - particle[i].h / 2) / distance;
			particle[i].angle 			= atan2(bmy - particle[i].y, bmx - particle[i].x);
			particle[i].angle 			= particle[i].angle * (180 / 3.1416);
			particle[i].angleSpe		= 0;
			particle[i].angleDir		= 0;
			particle[i].sizeDeath 		= false;
			particle[i].deathSpe 		= 0;
			particle[i].time 			= 0;
			particle[i].side 			= "";
			particle[i].collide 		= false;
			particle[i].onScreen 		= false;
			particle[i].alive 			= true;
			particle[i].color 			= color;
			particle[i].layer 			= layer;
			PARTICLES++;
			break;
		}
	}
}*/

// Update Particles
void Particle::updateBulletParticles(Particle particle[], int mapX, int mapY, int mapW, int mapH) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive)
		{
			// particle center
			particle[i].x2 = particle[i].x + particle[i].w/2;
			particle[i].y2 = particle[i].y + particle[i].h/2;

			// Player particle
			if (particle[i].type == 0)
			{
				// get particle radius
				particle[i].radius = particle[i].w;

				// Particle movement
				particle[i].x += particle[i].vX * particle[i].speed;
				particle[i].y += particle[i].vY * particle[i].speed;

				// Particle spin
				particle[i].angle += particle[i].angleSpe * particle[i].angleDir;

				// Particle death
				particle[i].time += particle[i].deathTimerSpeed;

				// Update particles angle based on its X and Y velocities
				particle[i].angle = atan2 ( particle[i].vY, particle[i].vX) * 180 / 3.14159265;

				// Particle map collision
				if (particle[i].x+particle[i].w < mapX) {
					particle[i].x = mapX+mapW-particle[i].w;
				}
				if (particle[i].x > mapX+mapW) {
					particle[i].x = mapX-particle[i].w;
				}
				if (particle[i].y+particle[i].h < mapY) {
					particle[i].y = mapY+mapH-particle[i].h;
				}
				if (particle[i].y > mapY+mapH) {
					particle[i].y = mapY-particle[i].h;
				}

				// Particle death
				if (particle[i].time > particle[i].deathTimer) {
					particle[i].alive = false;
					count--;
				}
			}

			// Enemy particle
			if (particle[i].type == 1)
			{
				// get particle radius
				particle[i].radius = particle[i].w;

				// Particle movement
				particle[i].x += particle[i].vX * particle[i].speed;
				particle[i].y += particle[i].vY * particle[i].speed;

				// Particle spin
				particle[i].angle += particle[i].angleSpe * particle[i].angleDir;

				// Particle death
				particle[i].time += particle[i].deathTimerSpeed;

				// Particle map collision
				if (particle[i].x+particle[i].w < mapX) {
					particle[i].x = mapX+mapW-particle[i].w;
				}
				if (particle[i].x > mapX+mapW) {
					particle[i].x = mapX-particle[i].w;
				}
				if (particle[i].y+particle[i].h < mapY) {
					particle[i].y = mapY+mapH-particle[i].h;
				}
				if (particle[i].y > mapY+mapH) {
					particle[i].y = mapY-particle[i].h;
				}

				// Particle death
				if (particle[i].time > particle[i].deathTimer) {
					particle[i].alive = false;
					count--;
				}
			}
		}
	}
}

// Update Particles
void Particle::updateStarParticles(Particle particle[], int mapX, int mapY, int mapW, int mapH) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive)
		{
			// Star particles
			if (particle[i].type == 2)
			{
				// Particle movement
				if (particle[i].splatDistance > 1) {
					particle[i].splatDistance -= particle[i].splatSpeed;
					particle[i].x += particle[i].vX * particle[i].speed;
					particle[i].y += particle[i].vY * particle[i].speed;
				}

				// Particle spin
				particle[i].angle += particle[i].angleSpe * particle[i].angleDir;

				// Particle death timer, Time
				particle[i].time += particle[i].deathTimerSpeed;

				// Particle death, Time
				if (particle[i].time > particle[i].deathTimer) {
					particle[i].alive = false;
					count--;
				}

				// Particle death timer, transparency
				particle[i].alpha -= particle[i].alphaspeed;
				if (particle[i].alpha < 0) {
					particle[i].alive = false;
					count--;
				}

				// Particle death upon size
				if (particle[i].sizeDeath) {
					particle[i].w -= particle[i].deathSpe;
					particle[i].h -= particle[i].deathSpe;

					if (particle[i].w <= 0 || particle[i].h <= 0){
						particle[i].alive = false;
						count--;
					}
				}
			}
			// Blue block bits particles
			if (particle[i].type == 3)
			{
				// Particle movement
				if (particle[i].splatDistance > 1) {
					particle[i].splatDistance -= particle[i].splatSpeed;
					particle[i].x += particle[i].vX * particle[i].speed;
					particle[i].y += particle[i].vY * particle[i].speed;
				}

				// Particle spin
				particle[i].angle += particle[i].angleSpe * particle[i].angleDir;

				// Particle death timer, Time
				particle[i].time += particle[i].deathTimerSpeed;

				// Particle death, Time
				if (particle[i].time > particle[i].deathTimer) {
					particle[i].alive = false;
					count--;
				}

				// Particle death timer, transparency
				particle[i].alpha -= particle[i].alphaspeed;
				if (particle[i].alpha < 0) {
					particle[i].alive = false;
					count--;
				}

				// Particle death upon size
				if (particle[i].sizeDeath) {
					particle[i].w -= particle[i].deathSpe;
					particle[i].h -= particle[i].deathSpe;

					if (particle[i].w <= 0 || particle[i].h <= 0){
						particle[i].alive = false;
						count--;
					}
				}
			}
		}
	}
}

// Render bullets
void Particle::renderBulletParticle(Particle particle[], int camX, int camY, float playerZ, SDL_Renderer* gRenderer) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive) {

			// Render Player magic cast particle
			if (particle[i].type == 0) {

				// Render texture
				spr_bullet.setBlendMode(SDL_BLENDMODE_BLEND);
				spr_bullet.setAlpha(255);
				spr_bullet.render(gRenderer, particle[i].x - camX,
										     particle[i].y - camY, 20, 20,
										     NULL, particle[i].angle);
			}

			// Render Enemy lazer particle
			if (particle[i].type == 1) {

				// Render texture
				spr_bullet_blue.setBlendMode(SDL_BLENDMODE_BLEND);
				spr_bullet_blue.setAlpha(255);
				spr_bullet_blue.render(gRenderer, particle[i].x - camX,
										     particle[i].y - camY, 20, 20,
										     NULL, particle[i].angle);
			}
		}
	}
}

// Render bullets
void Particle::renderBulletParticleDebug(Particle particle[], int camX, int camY, SDL_Renderer* gRenderer) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive) {

			// Render Player magic cast particle
			if (particle[i].type == 0) {

				// World hit-box
				SDL_Rect tempRect = {particle[i].x - camX,
								     particle[i].y - camY,
								     particle[i].w,
									 particle[i].h};
				SDL_SetRenderDrawColor(gRenderer, 0,0,255,255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
			}

			// Render Enemy lazer particle
			if (particle[i].type == 1) {

				// World hit-box
				SDL_Rect tempRect = {particle[i].x - camX,
								     particle[i].y - camY,
								     particle[i].w,
									 particle[i].h};
				SDL_SetRenderDrawColor(gRenderer, 255,0,0,255);
				SDL_RenderDrawRect(gRenderer, &tempRect);
			}

			// Render angle Text
			std::stringstream tempss;
			tempss << particle[i].type;
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont20);
			gText.render(gRenderer, particle[i].x - camX,
									particle[i].y-gText.getHeight() - camY,
									gText.getWidth(), gText.getHeight());
		}
	}
}

// Render stars
void Particle::renderStarParticle(Particle particle[], int camX, int camY, float playerZ, SDL_Renderer* gRenderer) {
	for (int i = 0; i < max; i++) {
		if (particle[i].alive) {

			// Render stars particle
			if (particle[i].type == 2) {

				/*SDL_Rect tempRect = {particle[i].x - camX/particle[i].layer,particle[i].y - camY/particle[i].layer,
						   particle[i].w,  particle[i].h};
				SDL_SetRenderDrawColor(gRenderer, particle[i].color.r, particle[i].color.g, particle[i].color.b,1);
				SDL_RenderFillRect(gRenderer, &tempRect);*/

				gParticles.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
				gParticles.setAlpha(particle[i].alpha);
				gParticles.render(gRenderer, particle[i].x - camX/particle[i].layer,particle[i].y - camY/particle[i].layer,
											 particle[i].w, particle[i].h,
											 &cParticles[4], particle[i].angle);
			} else if (particle[i].type == 3)
			{
				blue_block.setColor(particle[i].color.r, particle[i].color.g, particle[i].color.b);
				blue_block.setAlpha(particle[i].alpha);
				blue_block.render(gRenderer, particle[i].x - camX/particle[i].layer,particle[i].y - camY/particle[i].layer,
											 particle[i].w, particle[i].h,
											 NULL, particle[i].angle);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////// ENEMY BULLETS //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

