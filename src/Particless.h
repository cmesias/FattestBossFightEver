/*
 * Particless.h
 *
 *  Created on: Dec 16, 2016
 *      Author: Carl
 */



#ifndef LOCAL_PARTICLESS_H_
#define LOCAL_PARTICLESS_H_

#include "LTexture.h"
#include "Helper.h"
#include "Game/TextNFont.h"

class Particle: public Helper, public TextNFont {

public:	// other variables
	int count;
	const int max = 512;
	LTexture spr_bullet;					// Bullet Textures
	LTexture spr_bullet_blue;				// Bullet Texture blue
	LTexture blue_block;					// Blue block, this will be used to create "bits" that come off of a block when hit
	LTexture gParticles;					// Particle Textures
	SDL_Rect cParticles[ 6 ];				// [0: Blue], [1: Green], [2: Orange], [3: Red], [4: White], [5: Yellow] Particle

public:	// variables
	float x2, y2;							// particle center
	int radius;								// particle radius
	float x, y;
	float w, h;
	int alpha;
	int alphaspeed;
	int time;
	int deathTimer;
	int deathTimerSpeed;
	float angle;
	double vX, vY;
	double speed;
	double angleSpe, angleDir;
	bool onScreen;
	bool alive;
	bool collide;
	std::string side;
	/* 0: damages enemies
	 * 1: damages player
	 * 2: stars, no damage
	 * 3: blue block bits, no damage
	 */
	int type;
	int damage;
	SDL_Color color;
	int layer;

	bool sizeDeath;
	float deathSpe;

	int ptimer = 0;

	double splatSpeed;			// splat speed
	double splatDistance;		// how many pixels the particle will travel before stopping, or -1 for forever

public:	// basic functions
	void init(Particle particle[]);
	void load(SDL_Renderer* gRenderer);
	void free();

public:	// functions
	void spawnParticleAngle(Particle particle[], int type,
			float spawnX, float spawnY,
			int spawnW, int spawnH,
			double angle, double speed,
			double damage,
			SDL_Color color, int layer,
			int angleSpe, int angleDir,
			int alpha, int alphaspeed,
			int deathTimer, int deathTimerSpeed,
			bool sizeDeath, float deathSpe,
			double splatDistance = 1, double splatSpeed = 0.0);

	void spawnExplosion(Particle particle[], float x, float y, int size);

	/*void fireParticle(Particle particle[], int type, int damage,
										   int spawnx, int spawny, int w, int h,
										   double speed, float vX, float vY,
										   int alphaspeed, int alpha,
										   int deathTimer, int deathTimerSpeed, double angle, double angleSpe, double angleDir,
										   SDL_Color color, int layer);
	void fireParticle2(Particle particle[], int type, int damage, int spawnx, int spawny, int w, int h, int targetx,int targety,
											int alphaspeed, int alpha,
											int deathTimer, int deathTimerSpeed,
											double speed,
											SDL_Color color, int layer);*/
	void updateBulletParticles(Particle particle[], int mapX, int mapY, int mapW, int mapH);

	void renderBulletParticle(Particle particle[], int camX, int camY, float playerZ, SDL_Renderer* gRenderer);

	void renderBulletParticleDebug(Particle particle[], int camX, int camY, SDL_Renderer* gRenderer);

public:	// Star
	void genStars(Particle particle[], int screnWidth, int screenHeight);
	void updateStarParticles(Particle particle[], int mapX, int mapY, int mapW, int mapH);
	void renderStarParticle(Particle particle[], int camX, int camY, float playerZ, SDL_Renderer* gRenderer);
};

#endif /* LOCAL_PARTICLESS_H_ */
