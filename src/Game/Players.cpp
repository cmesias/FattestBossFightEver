/*
 * Players.cpp
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
#include <vector>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_mouse.h>

//#include "Input.h"

#include "Players.h"

// Reset game
void Players::Init(float spawnX, float spawnY, std::string newName, bool respawn){
	ControlsPreference  = 1;
	x 					= spawnX;
	y 					= spawnY;
	name				= newName;
	vX 					= 0.0;
	vY 					= 0.0;
	delayT 				= 0;
	health				= 100;
	delay 				= false;
	initialshot 		= false;
	thrust 				= false;
	deathScreen 		= false;
	alive 				= true;
	returned			= false;
	indexSaved 			= -1;
	invurnerableFrame	= 1;
	invurnerableTick	= 0;
	invurnerableT		= 300;
	invurnerable		= true;

	controls 			= 0;
	A 					= false;
	B 					= false;
	X 					= false;
	Y 					= false;
	D_UP 				= false;
	D_DOWN 				= false;
	D_LEFT 				= false;
	D_RIGHT 			= false;
	LB 					= false;
	RB 					= false;
	BACK 				= false;
	START 				= false;
	L3 					= false;
	R3 					= false;
	LAngle 				= 0.0;
	RAngle 				= 0.0;
	zAxisLeft 			= 0.0;
	zAxisRight 			= 0.0;
	leftclick 			= false;
	rightclick 			= false;
	test 				= false;
	trigger 			= false;
	moving				= false;
	moveleft 			= false;
	moveright 			= false;
	moveup 				= false;
	movedown			= false;
	renderFlash 		= false;
	playSFXDownStabHittingFloor	= true;
	facing				= "right";
	flipW				= SDL_FLIP_NONE;
	jumps				= 1;
	walkTimer 			= 0;
	walkTimerVFX 		= 0;
	sprite_index 		= 0;
	stunned 			= false;
	stunTimer 			= 0;
	playSlash 			= false;
	clash 				= false;
	attack 				= false;
	spawnAttack 		= false;
	manaRegenTimer		= 0;

	if (!respawn){
		score 			= 0;
		lives 			= 1;
	}

	// Always reset these

	// Attack ability
	this->attackTimer 		= 0;
	this->attackFrame 		= 0;
	this->attack 			= false;
	this->attackType		= -1;

	// Parry ability
	this->parry 			= false;
	this->parryTimer 		= 0;
	this->parryCDTimer 		= 0;
	this->parryCDMax		= 60*2.6;

	// Dash ability
	this->dash 			= false;
	this->dashSpeed 	= 10;
	this->dashLength 	= 10;
	this->dashCounter 	= 0;
	this->dashCoolCounter = 0;

	// TODO (3-9-2022) [ ] - Save these player stats in a file. This will be the players save data.
	this->AttackSpeed 			= 6.87;
	this->maxMana				= 100;
	this->mana					= this->maxMana;
	this->manaRegenSpeed		= 8.75;
	this->manaGainOnParry		= 5.25;
	this->damage				= 75;
	this->damageMultipler		= 1;
	this->castDamage			= 25;
	this->knockBackPower		= 1.58;

}

// Check collision between 2 objects
bool Players::checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2){
	bool collide;
	if (x+w > x2 && x < x2 + w2 && y+h > y2 && y < y2 + h2){
		collide = true;
	}else{
		collide = false;
	}
	return collide;
}

// Load asteroid resources
void Players::Load(SDL_Renderer* gRenderer){

	// open joystick i
    joy = SDL_JoystickOpen(1);

    // load textures
	gPlayer.loadFromFile(gRenderer, "resource/gfx/player/player.png");
	gPlayerShadow.loadFromFile(gRenderer, "resource/gfx/player/player_shadow.png");
	gShield.loadFromFile(gRenderer, "resource/gfx/player/shield.png");
	rPlayer[0] = {0,0,48,48};			// Walking 			0
	rPlayer[1] = {48,0,48,48};			// Walking 			1
	rPlayer[2] = {96,0,48,48};			// Walking 			2
	rPlayer[3] = {144,0,48,48};			// Walking 			3

	rPlayer[4] = {0,48,48,48};			// Before Slash 	4
	rPlayer[5] = {48,48,62,48};			// Slash attack 	5
	rPlayer[6] = {0,96,48,48};			// Parry 			6
	rPlayer[7] = {144,48,48,55};		// Down-Stab attack 7
	rPlayer[8] = {48,96,48,48};			// Dash frame 0:	9
	rPlayer[9] = {96,96,48,48};			// Dash frame 1:	9

	for (int i=0; i<7; i++){setClips(rShield[i], i*48, 0, 48, 48);}

	// Load audio
	sCast 			= Mix_LoadWAV("sounds/bfxr/snd_cast.wav");
	sDownStab		= Mix_LoadWAV("sounds/bfxr/snd_downStab.wav");
	sParry			= Mix_LoadWAV("sounds/bfxr/snd_parry.wav");
	sStep			= Mix_LoadWAV("sounds/cmesias/snd_step.wav");
	sSlash 			= Mix_LoadWAV("sounds/cmesias/snd_slash.wav");
	sDash			= Mix_LoadWAV("sounds/cmesias/snd_dash.wav");

	// Other classes
	LoadFonts();
}

// Free asteroid resources
void Players::Free(){
	// Free joystick
    SDL_JoystickClose(joy);

    // Free textures
	gPlayer.free();
    gShield.free();
    gPlayerShadow.free();

    // Free audio
	Mix_FreeChunk(sCast);
	Mix_FreeChunk(sSlash);
	Mix_FreeChunk(sDownStab);
	Mix_FreeChunk(sParry);
	Mix_FreeChunk(sStep);
	Mix_FreeChunk(sDash);
	sCast 			= NULL;
	sSlash 			= NULL;
	sDownStab		= NULL;
	sParry			= NULL;
	sStep			= NULL;
	sDash			= NULL;

	// Other classes
	FreeFonts();
}

void Players::ResetHighScore(int LevelWeLoaded){

	// Set a file path for score.txt
	std::stringstream filePath;
	filePath << "data/maps/highscore";
	filePath << LevelWeLoaded;
	filePath << ".txt";

	// Overwrite "highscores.txt"
	std::ofstream fileSave;
	fileSave.open(filePath.str().c_str());
	fileSave << 0;
	fileSave.close();

	// Reset scores
	this->highscore = 0;
	this->score = 0;
}

// Player shoot
void Players::fire(Particle particle[], Particle &p_dummy, Mix_Chunk* sCastSFX, int mx, int my){

	// Determine controls
	if (controls==0){
		trigger = initialshot;
	}else if(controls==1){
		trigger = A;
	}else if(controls==2){
		trigger = initialshot;
	}

	// Get shoot trigger
	if (controls==0){
		trigger = initialshot;
	}else if(controls==1){
		trigger = A;
		A = false;
	}else if(controls==2){
		trigger = initialshot;
		initialshot = false;
	}


	// Get player angle based on mouse coordinates
	angle = atan2(my - y-h/2,mx - x-w/2);
	angle = angle * (180 / 3.1416);
	//Set player angle max limits
	if (angle < 0) {
		angle = 360 - (-angle);
	}

	// Get radiant from angle
	radians = (3.1415926536/180)*(angle);
	radianCos = floor(cos(radians)*10+0.5)/10;
	radianSin = floor(sin(radians)*10+0.5)/10;

	// get player new center position
	//x2 = (x+w/2 + (41 * (-radianCos)) - radius);
	//y2 = (y+h/2 + (41 * (-radianSin)) - radius);
	/////int newW = 41 * (-radianCos);
	//////int newH = 41 * (-radianSin);

	/* If the object is at a rotation of 0.0 (facing right),
	 * then this will be the distance the particle
	 * will spawn from the center of the object,
	 *
	 * This will provide the correct distance from the center
	 * when the object is rotated from its center.
	 *
	 * With this distance you may add this to your objects x and y center,
	 * and this will be where your particle will spawn
	 */
	barrelW  = (bulletW * cos(radians) ) - (bulletH * sin(radians) );	// add this to center of zombie (this will give us the guns barrel position)
	barrelH  = (bulletW * sin(radians) ) + (bulletH * cos(radians) );

	int wDifference = realw - w;
	int hDifference = realh - h;

	barrelX = x+realw/2-wDifference/2 - particleW/2 + barrelW;
	barrelY = y+realh/2-hDifference/2 - particleH/2 + barrelH;

	/*int newmx = mx-particleW/2;
	int newmy = my-particleH/2;
	float distance = sqrt((newmx - barrelX) * (newmx - barrelX)+
						  (newmy - barrelY) * (newmy - barrelY));
	if (distance <= 1) {
		distance = 1;
	}
	float newAngle = atan2(newmy - barrelY,newmx - barrelX);
	newAngle = newAngle * (180 / 3.1416);
	//Set player angle max limits
	if (newAngle < 0) {
		newAngle = 360 - (-newAngle);
	}*/

	// Shoot particle
	if (trigger)
	{
		// If we have enough mana
		if (this->mana >= 5) {
			if (!delay)
			{
				delay = true;

				if (powerup == 1)
				{
					// recoil of gun
					recoilX = 11 * radianCos;
					recoilY = 11 * radianSin;

					// play audio
					Mix_PlayChannel(1, sCastSFX, 0);

					// Offset to spawn the Slash Attack
					int offSetX =0;
					if (facing == "left") {
						offSetX = 32;
					}
					else if (facing == "right") {
						offSetX = 0;
					}

					// spawn particle
					p_dummy.spawnParticleAngle(particle, 0,
							x+w/2-offSetX,
							y+w/2,
							particleW, particleH,
						   angle, 21,
						   this->castDamage, this->castDamage, 0,
						   {255, 255,0}, 1,
						   1, 1,
						   255, 0,
						   100, 2,
						   false, 0);

					// Subtract mana
					this->mana -= 2;

					// muzzle flash
					renderFlash = true;
				}
			}
		}

		// We dont have enough mana, just spew out Particles from the Player instead
		else {
			// Spawn particle effect
			for (double i=0.0; i< 90.0; i+= rand() % 10 + 20){
				int rands = rand() % 11 + 3;
				float newX = x2;
				float newY = y2;
				p_dummy.spawnParticleAngle(particle, 2,
									newX-rands/2,
									newY-rands/2,
								   rands, rands,
								   angle -45 + i, randDouble(2.1, 5.1),
								   0.0, 0, 0,
								   {244, 144, 0, 255}, 1,
								   1, 1,
								   rand() % 100 + 150, rand() % 2 + 5,
								   rand() % 50 + 90, 0,
								   true, randDouble(0.1, 0.7),
								   100, 10);
			}
		}
	}

	// Shoot delay
	if (delay) {
		delayT += AttackSpeed;
		if (delayT > 60) {
			delay = false;
			delayT = 0;
		}
	}

	/*bool trigger = false;
	// Player shoot
	if (controls==0){
		trigger = initialshot;
	}else if(controls==1){
		trigger = A;
	}*/

	//if (test){
		//if (!delay) {
			//if (A) {
				//A = false;
				//trigger = false;
				//delay = true;

				// Remove shield
				/*if (shield){
					shield 	= false;
					shieldT 	= 300;
				}*/

				// Shoot particle
				/*if (powerup == "LAZER") {
					Mix_PlayChannel(-1, sLazer, 0);

					p_dummy.spawnParticleAngle(particle,
							   x+w/2-4/2+(cos(radians)*9)+(sin(radians)*12),
							   y+h/2-4/2+(sin(radians)*9)-(cos(radians)*12),
							   4, 4,
							   angle, 21,
							   255, 0,
							   100, 2,
							   100, "LAZER",
							   {255, 255,0}, 1,
							   1, 1,
							   false, 0);
					p_dummy.spawnParticleAngle(particle,
							   x+w/2-4/2+(cos(radians)*9)-(sin(radians)*12),
							   y+h/2-4/2+(sin(radians)*9)+(cos(radians)*12),
							   4, 4,
							   angle, 21,
							   255, 0,
							   100, 2,
							   100, "LAZER",
							   {255, 255,0}, 1,
							   1, 1,
							   false, 0);
				}*/
			//}
		/*}
		//Start delay timer after our first shot!
		if (delay) {
			delayT += AttackSpeed;
			if (delayT > 60) {
				delay = false;
				delayT = 0;
			}
		}*/
	//}

	// Handle gun recoil
	if (recoilX < 0) {
		recoilX += 2.5;
	}
	if (recoilX > 0) {
		recoilX -= 2.5;
	}
	if (recoilY < 0) {
		recoilY += 2.5;
	}
	if (recoilY > 0) {
		recoilY -= 2.5;
	}

}

// Update Player
void Players::Update(Map &map,
					Enemy enemy[], Enemy &e_dummy,
					Particle particle[], Particle &p_dummy,
					Tile &tl, Tile tile[],
					Tilec &tlc, Tilec tilec[],
					Object &obj, Object object[],
					int mex, int mey, int camx, int camy,
					float spawnX, float spawnY,
					LWindow gWindow, SDL_Renderer* gRenderer,
					LTexture gText, TTF_Font *gFont, SDL_Color color,
					Mix_Chunk *sAtariBoom, bool &RestartLevel,
					int LevelWeLoaded)
{
	// Player center
	x2 = x+w/2;
	y2 = y+h/2;

	// Reset upon leaving pause menu
	if (returned){
		returned 		= false;
		leftclick 	= false;
		initialshot 	= false;
		thrust		= false;
		A				= false;
		RB			= false;
	}

	// Player alive
	if (alive)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//----------------------------------- Player Move -------------------------------------------------//
		//-------------------------------------------------------------------------------------------------//
		bool trigger = false;
		// Get Angle
		if (this->controls==0){
			trigger = this->thrust;
		}else if(this->controls==1){
			this->angle = this->LAngle;
			trigger = this->RB;
		}else if(this->controls==2){
			trigger = this->thrust;
		}

		// Player angle by rotation
		if (this->rotateLeft){
			//angle -= 5;
		}else if (this->rotateRight){
			//angle += 5;
		}

		////////////////////////////////////////////////////////////////////////////////////
		//--------------------------------------------------------------------------------//
		//----------------------------------- Movement -----------------------------------//
		{

			// X Axis movement
			{
				// Move left
				if (this->moveleft && !this->attack && !this->parry && !this->dash) {
					if (this->vX > -this->velMax) {
						this->vX -= this->velSpeed;
					}
					this->moving = true;
			        if (!this->shift) {
			        	this->facing = "left";
			        }
				}
				// Move right
				else if (this->moveright && !this->attack && !this->parry && !this->dash) {
					if (this->vX < this->velMax) {
						this->vX += this->velSpeed;
					}
					this->moving = true;
			        if (!this->shift) {
			        	this->facing = "right";
			        }
				}
			}

			// Direction facing
			if (this->ControlsPreference == 1) {
				if (mex+camx > this->x) {
		        	this->facing = "right";
				}else {
		        	this->facing = "left";
				}
			}


			// Y Axis movement
			{
				// Move up
				if ((this->moveup && !this->attack && !this->parry && !this->dash)) {
					if (this->vY > -this->velMax) {
						this->vY -= this->velSpeed;
					}
					this->moving = true;
			        if (!this->shift) {
			        	//this->facing = "left";
			        }
				}
				// Move down
				else if (this->movedown && !this->attack && !this->parry && !this->dash) {
					if (this->vY < this->velMax) {
						this->vY += this->velSpeed;
					}
					this->moving = true;
			        if (!this->shift) {
			        	//this->facing = "right";
			        }
				}
			}

			// If not dashing
			if (!this->dash) {
				// Max X speed
				if (this->vX < -this->velMax) {
			        vX = vX - vX * 0.2;
				}
				if (this->vX > this->velMax) {
			        vX = vX - vX * 0.2;
				}
				// Max Y speed
				if (this->vY < -this->velMax) {
			        vY = vY - vY * 0.2;
				}
				if (this->vY > this->velMax) {
			        vY = vY - vY * 0.2;
				}
			}

			// Facing direction for flip
		    if (facing == "left") {
		        flipW = SDL_FLIP_HORIZONTAL;
		    }
		    if (facing == "right") {
		        flipW = SDL_FLIP_NONE;
		    }
		}
		//----------------------------------- Movement -----------------------------------//
		//--------------------------------------------------------------------------------//
		////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////
		//--------------------------------------------------------------------------------//
		//--------------------------- Movement & Collision w/ Tiles ----------------------//
		std::string tempHere = "";
		// Handle movement of player, and handle collision with Tiles
		tl.checkCollisionXY(tile,
				this->x, this->y,
				this->w, this->h,
				this->vX, this->vY,
				tempHere, this->jumps);

		//--------------------------- Movement & Collision w/ Tiles ----------------------//
		//--------------------------------------------------------------------------------//
		////////////////////////////////////////////////////////////////////////////////////


	    // TODO [ ] - Do !Parrying and !Attack and !Parry
	    ////////////////////////////////////////////////////////////////////////////
	    //------------------------------------------------------------------------//
	    //--------------------------------- Do !Parrying -------------------------//

	    // IDKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
	    //{
		// Idle animation
		if (!moving) {

			// If not attacking
			if (!this->attack && !this->dash && !this->parry) {
				sprite_index = 0;
			}

		// Moving animation
		} else {

			// If not attacking
			//if (!this->attack) {

				///////////////////////////////////////////////////////////////////////////
				//-----------------------------------------------------------------------//
				//----------------------------- Do walkTimer ----------------------------//
				{
					// Increment animation timer
					walkTimer += 10;

					// If walkTimer is at 30 frames
					if (walkTimer == 30)
					{
						//
					}

					// Increment current animation frame
					if (walkTimer > 60)
					{
						// Reset timer
						walkTimer = 0;
						// Go to next animation frame
						sprite_index++;
					}

					// Reset sprite
					if (sprite_index > 3) {
						sprite_index = 0;
					}

					// Adjust player shadow based on what frame we're currently rendering
					if (sprite_index == 0) {
						shadowW = 44;
					}else if (sprite_index == 1) {
						shadowW = 32;
					}else if (sprite_index == 2) {
						shadowW = 38;
					}else if (sprite_index == 3) {
						shadowW = 42;
					}
				}
				//----------------------------- Do walkTimer ----------------------------//
				//-----------------------------------------------------------------------//
				///////////////////////////////////////////////////////////////////////////

				///////////////////////////////////////////////////////////////////////////
				//-----------------------------------------------------------------------//
				//--------------------------- Do walkTimerVFX ---------------------------//
				{
					//	Increase walkTimerVFX
					walkTimerVFX += 1;

					// If walkTimer is at 30 frames
					if (walkTimerVFX == 15 || walkTimerVFX == 30 || walkTimerVFX == 45 || walkTimerVFX == 60)
					{
						// Visual and audio effects
						{
							// Spawn particle
							int tempAngel = 0;
							float adjustX = 0;
							if (facing == "left" ) {
								tempAngel = 0;
								adjustX = 24;
							} else if (facing == "right" ) {
								tempAngel = 180;
								adjustX = -24;
							}

							// Spawn size and pos
							int randSize = rand() % 5 + 5;

							float spawnX = this->x+this->w/2 + adjustX;
							float spawnY = this->y+this->h;

							// Spawn particle effect at feet
							p_dummy.spawnParticleAngle(particle, 2,
												spawnX-randSize/2,
												spawnY-randSize/2,
												randSize, randSize,
											   tempAngel, randDouble(0.1, 0.4),
											   0.0, 0, 0,
											   {255, 255, 255, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, randDouble(0.1, 0.7),
											   100, 10);
							// Play sound effect
							Mix_PlayChannel(-1, sStep, 0);
						}
					}

					// Reset walkTimerVFX
					if (walkTimerVFX > 60)
					{
						walkTimerVFX = 0;
					}
				}
				//--------------------------- Do walkTimerVFX ---------------------------//
				//-----------------------------------------------------------------------//
				///////////////////////////////////////////////////////////////////////////
			//}
		}

		// Animations
		{
			///////////////////////////////////////////////////////////////////////////
			//-----------------------------------------------------------------------//
			//------------------------------- Do Dash -------------------------------//
			if (this->dash) {

				if (dashCounter >= 0 && dashCounter < 2) {
					sprite_index = 9;
				}
				else if (dashCounter >= 2 && dashCounter < 4) {
					sprite_index = 8;
				}
				else if (dashCounter >= 4 && dashCounter < 6) {
					sprite_index = 9;
				}
				else if (dashCounter >= 6 && dashCounter < 8) {
					sprite_index = 8;
				}
				else if (dashCounter >= 8 && dashCounter < 10) {
					sprite_index = 9;
				}

				int rands = rand() % 9 + 2;
				float newX = x+w/2;
				float newY = y+h;
				p_dummy.spawnParticleAngle(particle, 2,
									newX-rands/2,
									newY-rands/2,
								   rands, rands,
								   0, randDouble(0.1, 0.3),
								   0.0, 0, 0,
								   {255, 255, 255, 255}, 1,
								   1, 1,
								   rand() % 100 + 150, rand() % 2 + 5,
								   rand() % 50 + 90, 0,
								   true, 0.11,
								   rand() % 9 + 2, 1);

				// If dash counter is greater than 0
				if (dashCounter > 0) {

					// Subtract dash counter by 1 every frame
					dashCounter -= 1;
				}
				// If dash counter goes lower than 0
				else {

					// Stop player movement
					StopMovement();

					// Dash on cool down
					dash = false;

					// Start dash cool down timer
					dashCoolCounter = dashCooldown;
				}
			}

			//------------------------------- Do Dash -------------------------------//
			//-----------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////
			//-----------------------------------------------------------------------//
			//----------------------------- Do Attacking ----------------------------//
			// If attacking
			else if (this->attack)
			{
				// If attack-type: Slash Attack
				if (this->attackType == 0)
				{
					// Stop player movement
					StopMovement();

					// Increase attack timer/frames
					this->attackTimer += this->attackTimerSpe;

					// If attack timer below 15 frames
					if (this->attackTimer < 15)
					{
						// Set pre-slashing sprite
						this->sprite_index = 4;
					}

					// At frame 16, spawn attack collision
					else{

						// Set slashing sprite
						sprite_index = 5;

						// If are spawning an attack-object
						if (spawnAttack) {

							// Immediatly stop attacking
							spawnAttack = false;

							// If facing right
							int width;
							if (facing == "right") {

								// Set attack object's x pos to the right of player's body
								width = 38;
							}else{

								// Set attack object's x pos to the left of player's body
								width = -21;
							}
							// Attack-object's width and height
							int tempWidth = 38;
							int tempHeight = 64;

							// Spawn attack object (it will appear in the world for 1 frame then remove itself)
							obj.spawn(object, x+w/2-38/2+width,
											  y-16,
											  tempWidth, tempHeight,
											  0);
						}
						// Play slash sound effect
						if (playSlash) {
							playSlash = false;
							Mix_PlayChannel(-1, sSlash, 0);
						}
					}
					// Attack over
					if (attackTimer > 20) {
						// Reset attack-type
						attackType = -1;
						attackTimer = 0;
						attack = false;

						// Stop shooting slashes
						this->initialshot = false;
					}
				}

				// Handle Down-Stab attack
				/*else if (attackType == 1)
				{
					// Set animation until Player hits the ground
					sprite_index = 7;

					// If are spawning an attack-object
					if (spawnAttack) {
						spawnAttack = false;

						// Spawn down-stab object
						obj.spawnFollow(object, x, y, 28, 20, 1, &swordX, &swordY);
					}

					// If player is on the ground
					if (jumpstate=="ground") {
						//attack = false;

						// If we can play the sound effect
						if (playSFXDownStabHittingFloor) {
							// Stop playing sound effect
							playSFXDownStabHittingFloor = false;

							// Player sound effect
							//Mix_PlayChannel(-1, sDownStab, 0);
						}

						// Attack timer
						attackTimer++;

						// After 0.25 second or 15 frames
						// Attack over
						if (attackTimer > 15)
						{
							// Reset attack-type
							attackType = -1;

							// Enable play sound effect next time we down stab
							playSFXDownStabHittingFloor = true;

							// Stop attack
							attackTimer = 0;
							attack = false;
						}
					}
				}*/
			}
			//----------------------------- Do Attacking ----------------------------//
			//-----------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////////////////
			//-----------------------------------------------------------------------//
			//------------------------------ Do Parrying ----------------------------//
			// Parrying animation
			if (this->parry)
			{
				// Stop movement
				StopMovement();

				// Determine direction
				if (this->facing == "left") {
					this->sprite_index = 6;
				}
				if (this->facing == "right") {
					this->sprite_index = 6;
				}

				// Start Parrying timer
				this->parryTimer++;

				// Parry for 15 frames
				if (this->parryTimer > 15){
					this->parryTimer = 0;
					this->parry = false;
					//this->StopParrying();
				}
			// Parry cool-down
			} else if (!this->parry) {
				if (this->parryCDTimer > 0) {
					this->parryCDTimer -= 1;
				}
			}
			//------------------------------ Do Parrying ----------------------------//
			//-----------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////////////////
			//-----------------------------------------------------------------------//
			//----------------------------- Do NOT Dash -----------------------------//

			// If dash on cooldown
			if (!this->dash) {

				// Start cooldown countdown
				if (this->dashCoolCounter > 0) {
					this->dashCoolCounter -= 1;
				}
			}

			//----------------------------- Do NOT Dash -----------------------------//
			//-----------------------------------------------------------------------//
			///////////////////////////////////////////////////////////////////////////


		} // end Animations



		//------------------------------ Do Parrying ----------------------------//
		//-----------------------------------------------------------------------//
		///////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////
		//-----------------------------------------------------------------------//
		//------------------------------- Do Stunned ----------------------------//
	    // Stunned
	    /*if (this->stunned) {
	        if (this->facing == "left") {
	        	this->sprite_index = 3;
	        }
	        if (facing == "right") {
	        	this->sprite_index = 3;
	        }
	        this->stunTimer += 1;
	        if (this->stunTimer > 90) {
	        	this->stunTimer = 0;
	            this->stunned = false;
	        }
	    }*/
		//------------------------------- Do Stunned ----------------------------//
		//-----------------------------------------------------------------------//
		///////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////
		//--------------------------------------------------------------------------------//
		//-------------------------------- Stop Movement ---------------------------------//

	    // Location of sword when down-stabbing
		this->swordX = this->x;
		this->swordY = this->y+35;

		// Rustle timer
		// player left and right swaying
		if (moving) {
			rustleW += 1 * rustleDirX;
			rustleH += 1 * rustleDirY;
		}
		if (rustleW > 4) {
			rustleDirX = rustleDirX * -1;
		}
		if (rustleW < -4) {
			rustleDirX = rustleDirX * -1;
		}
		if (rustleH > 4) {
			rustleDirY = rustleDirY * -1;
		}
		if (rustleH < -4) {
			rustleDirY = rustleDirY * -1;
		}

		// Player not moving X
		if (!moveleft && !moveright && !dash) {
	        vX = vX - vX * 0.2;
		}

		// Player not moving Y
		if (!moveup && !movedown && !dash) {
	        vY = vY - vY * 0.2;
		}

		// Player not moving
		if (!moveup && !movedown && !moveleft && !moveright && !dash) {
			moving = false;
		}

		//-------------------------------- Stop Movement ---------------------------------//
		//--------------------------------------------------------------------------------//
		////////////////////////////////////////////////////////////////////////////////////

		// Player shoot
		fire(particle, p_dummy, sCast, mex+camx, mey+camy);

		// Player shield
		if (invurnerable){
			invurnerableT-=1;
			invurnerableTick += 20;
			if (invurnerableTick>60){
				invurnerableTick = 0;
				invurnerableFrame += 1;
			}
			if (invurnerableFrame>7){
				invurnerableFrame = 1;
				invurnerableTick = 0;
			}
			if (invurnerableT<=0){
				invurnerableT= 300;
				invurnerable = false;
			}
		}

		// Check high score MAX
		if (score > 999999999){
			score = 999999999;
		}

		// Player death
		if (health <=0)
		{
			SaveHighScore(LevelWeLoaded);

			//Spawn explosion after asteroid death
			// spawn blood particle effect
			for (double i=0.0; i< 360.0; i+=rand() % 10 + 10){
				int rands = rand() % 9 + 2;
				float newX = x+w/2;
				float newY = y+h/2;
				p_dummy.spawnParticleAngle(particle, 2,
									newX-rands/2,
									newY-rands/2,
								   rands, rands,
								   i, randDouble(2.1, 5.1),
								   0.0, 0, 0,
								   {255, 0, 0, 255}, 1,
								   1, 1,
								   rand() % 100 + 150, rand() % 2 + 5,
								   rand() % 50 + 90, 0,
								   true, 0.11,
								   rand() % 9 + 2, 1);
			}

			// Take away lives
			lives-= 1;

			// Reset Player
			Init(spawnX, spawnY, name, true);

			// Player ran out of lives, que Death Screen
			if (lives<=0){

				// Reset some accumulated stuff
				e_dummy.ENEMY_SPAWN = 1;
				e_dummy.mileScore	= 0;
				e_dummy.SPAWN_ENEMY	= false;

				// Set variables
				invurnerableT 	= 300;
				invurnerable 	= true;
				alive 			= false;
				deathScreen 	= true;

				// SAVE HIGH SCORE
			}
		}

		// Update Player score and Wave
		if (this->score > this->highscore){
			this->highscore = this->score;
		}

		// Player level boundaries
		if( this->x < map.x ){
			this->x = map.x;
		}
		if( this->y < map.y ){
			this->vY = 0.0;
			this->y = map.y;
		}
		if( this->x+this->realw > map.w ){
			this->x = map.w-this->realw;
		}
		if( this->y+realh > map.h ){
			this->y = map.h-this->realh;
		}

		// Regain mana if not attacking
		if (!this->attack) {
			// Every frame player regenerates mana
			if (this->mana < this->maxMana)
			{
				// Start timer
				this->manaRegenTimer += 1;

				// After 1 second or 60 frames
				if (this->manaRegenTimer > 60) {

					// Reset timer
					this->manaRegenTimer = 0;

					// Increase mana based on mana regen speed
					this->mana += this->manaRegenSpeed;
				}
			}
		}

		// Check if mana spilling over max
		if (this->mana > this->maxMana)
		{
			this->mana = this->maxMana;
		}

		// Every frame reduce damage multiplier by 2%
		if (this->damageMultipler > 1.0025) {
			this->damageMultipler -= 0.0025;
		}


	}else{
		// High-Score moving
		this->travel += 0.05 * this->dir;
		if (this->travel > 10){
			this->dir = -1;
		}
		if (this->travel < -10){
			this->dir = 1;
		}

		// Continue YES or NO Screen
		if (this->deathScreen)
		{
			// Set button position
			continueButton[0] = {0 + screenWidth/2 -96/2-100, screenHeight/2-gText.getHeight()/2, 96, 33};
			continueButton[1] = {0 + screenWidth/2 -96/2+100, screenHeight/2-gText.getHeight()/2, 96, 33};
			continueButton[2] = {0 + screenWidth/2 -256/2 , screenHeight-gText.getHeight()-72, 256, 39};

			// High Score display position
			position  = 0+travel;
			position2 = continueButton[1].x+continueButton[1].w-gText.getWidth()+travel;

			// Left click
			if (leftclick)
			{
				// Option: Yes, reset everything
				if (checkCollision(mex, mey, 1, 1, continueButton[0].x, continueButton[0].y, continueButton[0].w, continueButton[0].h))
				{
					// Restores players base stats and health
					leftclick			= false;
					RestorePlayer(spawnX, spawnY);
					RestartLevel = true;

					// Clear Asteroids & Enemies
					e_dummy.clear(enemy);
				}

				// Option: No, go to Main Menu
				if (checkCollision(mex, mey, 1, 1, continueButton[1].x, continueButton[1].y, continueButton[1].w, continueButton[1].h))
				{
					score = 0;
					leftclick  = false;
					deathScreen = false;
				}

				// Option: Reset high scores
				if (checkCollision(mex, mey, 1, 1, continueButton[2].x, continueButton[2].y, continueButton[2].w, continueButton[2].h))
				{
					// Reset high scores
					ResetHighScore(LevelWeLoaded);
				}
			}
		// Menu Screen
		}else{
			SDL_ShowCursor(true);
			// Set button position
			continueButton[0] = {screenWidth/2-96/2, screenHeight/2-gText.getHeight()/2, 96, 33};
			continueButton[2] = {screenWidth/2-256/2 , screenHeight-gText.getHeight()-72, 256, 39};

			// High Score display position
			position 	= continueButton[0].w/2-gText.getWidth()/2-100+travel;
			position2 = continueButton[0].x+continueButton[0].w/2-gText.getWidth()/2+100+travel;

			// Left click
			if (leftclick)
			{
				// Option: Play
				if (checkCollision(mex, mey, 1, 1, continueButton[0].x, continueButton[0].y, continueButton[0].w, continueButton[0].h))
				{
					// Restores players base stats and health
					leftclick			= false;
					RestorePlayer(spawnX, spawnY);
					RestartLevel = true;

					// Clear Asteroids & Enemies
					e_dummy.clear(enemy);
				}

				// Option: Reset high scores
				if (checkCollision(mex, mey, 1, 1, continueButton[2].x, continueButton[2].y, continueButton[2].w, continueButton[2].h))
				{
					// Reset high scores
					ResetHighScore(LevelWeLoaded);
				}
			}
		}
	}
}

// Render Player
void Players::Render(int mx, int my, int camx, int camy, LWindow gWindow, SDL_Renderer* gRenderer,
					 SDL_Color color, int &PARTICLES, LTexture gText) {
	gText.setAlpha(255);
	// If alive
	if (alive){

		/* Get difference of real size and collision size,
		 * with this you can center the player by subtracting
		 * this with the player's position
		 */
		int wDifference = realw - w;				// get difference between player's collision size and actual size in pixels
		int hDifference = realh - h;
		int newX = x-wDifference/2;					// player starting position
		int newY = y-hDifference/2;
		int newRustleW = rustleW * radianCos;		// make swaying motions while moving
		int newRustleH = rustleH * radianSin;

		// render player
		{
			// Render shadow
			int shadowH = 48;
			gPlayerShadow.setAlpha(110);
			gPlayerShadow.render(gRenderer, x+w/2-shadowW/2-camx,
											y+shadowH/4-2-camy,
											shadowW, shadowH, NULL, 0.0, NULL);

			//gPlayer.render(gRenderer, newX-recoilX+newRustleW-camx, newY-recoilY+newRustleH-camy, realw, realh, &rPlayer[sprite_index], 0.0);
			// Adjust player sprite if on attack animation
			// The '-9", in the y coordinate is adjusting the sprite to the appropriiate position
			if (sprite_index == 5) {
				if (facing == "right") {
					gPlayer.render(gRenderer, x-camx, y-camy, 62, 48, &rPlayer[sprite_index], 0.0, NULL, flipW);
				}else{
					gPlayer.render(gRenderer, x-18-camx, y-camy, 62, 48, &rPlayer[sprite_index], 0.0, NULL, flipW);
				}
			}else if (sprite_index == 7) {
				if (facing == "right") {
					gPlayer.render(gRenderer, x-camx, y-camy, 48, 55, &rPlayer[sprite_index], 0.0, NULL, flipW);
				}else{
					gPlayer.render(gRenderer, x-camx, y-camy, 48, 55, &rPlayer[sprite_index], 0.0, NULL, flipW);
				}
			}else{
				gPlayer.render(gRenderer, x-12-camx, y-camy, realw, realh, &rPlayer[sprite_index], 0.0, NULL, flipW);
			}
		}

		// muzzle flash
		if (renderFlash) {
			renderFlash = false;
			//double barrelW  = (((bulletW+45) * cos(radians) ) - (bulletH * sin(radians) )) - 120/2;
			//double barrelH  = (((bulletW+45) * sin(radians) ) + (bulletH * cos(radians) )) - 120/2;
			// render muzzle flash

		}


		/*SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255);
		SDL_RenderDrawLine(gRenderer, x+w/2 + barrelW-camx,
									  y+h/2 + barrelH-camy,
									  mx, my);*/

		// Player sight to mouse
		//SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		//SDL_RenderDrawLine(gRenderer, x+w/2-camx, y+h/2-camy, mx, my);

		// Render Shield
		if (invurnerable){

			gShield.render(gRenderer, x+w/2 - 96/2 - camx, y+h/2 - 96/2 + 2 - camy, 96, 96, &rShield[invurnerableFrame], 180);

			/*SDL_Rect tempRect = {X-camx, Y-16-camy, (w*shieldT)/300, 3};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);*/
		}

		// Render number of lives left
		/*for (int i=0; i<lives; i++){
			//gPlayer.render(gRenderer, screenWidth-32-i*16, 90, 16, 16);
			spr_player_head.render(gRenderer, screenWidth/2+i*16, 72, 16, 16);
		}*/


		// reticle
		//double wedth = 21 * radianSin;
		//double hedth = 19 * radianCos;
		//SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
		//SDL_RenderDrawLine(gRenderer, mx-16-wedth, my+hedth, mx+16-wedth, my+hedth);
		//SDL_RenderDrawLine(gRenderer, mx-wedth, my-16+hedth, mx-wedth, my+16+hedth);

		/*SDL_Rect tempRect = {mx-24-wedth, my-24+hedth, 48, 48};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);*/
	}
}

void Players::RenderUI(SDL_Renderer *gRenderer, int camX, int camY)
{
	// Player UI
	{
		// Show death screen if not alive
		if (!this->alive){
			// Continue YES or NO Screen
			if (deathScreen)
			{
				// Render Text
				gText.loadFromRenderedText(gRenderer, "You have died. Continue?", {255,255,255}, gFont24);
				gText.render(gRenderer, screenWidth/2-gText.getWidth()/2, screenHeight/2-gText.getHeight()/2-50, gText.getWidth(), gText.getHeight());

				// Render buttons: Yes
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &continueButton[0]);

				// Render buttons: No
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
				SDL_RenderDrawRect(gRenderer, &continueButton[1]);

				// Render buttons: ResetHighScore
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &continueButton[2]);

				// Render button texts: Yes or No
				gText.loadFromRenderedText(gRenderer, "Yes", {255,255,255}, gFont24);
				gText.render(gRenderer,  continueButton[0].x+continueButton[0].w/2-gText.getWidth()/2,
										 continueButton[0].y+continueButton[0].h/2-gText.getHeight()/2,
										 gText.getWidth(), gText.getHeight());

				gText.loadFromRenderedText(gRenderer, "No", {255,255,255}, gFont24);
				gText.render(gRenderer,  continueButton[1].x+continueButton[1].w/2-gText.getWidth()/2,
										 continueButton[1].y+continueButton[1].h/2-gText.getHeight()/2,
										 gText.getWidth(), gText.getHeight());

				// Render Text
				gText.loadFromRenderedText(gRenderer, "Reset High Scores", {255,255,255}, gFont24);
				gText.render(gRenderer, continueButton[2].x+continueButton[2].w/2-gText.getWidth()/2,
										 continueButton[2].y+continueButton[2].h/2-gText.getHeight()/2,
										 gText.getWidth(), gText.getHeight());
			// Player Menu screen
			}else{

				// Render buttons: Play
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &continueButton[0]);

				// Render buttons: ResetHighScore
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &continueButton[2]);

				// Render Text
				gText.loadFromRenderedText(gRenderer, "PLAY", {255,255,255}, gFont24);
				gText.render(gRenderer, continueButton[0].x+continueButton[0].w/2-gText.getWidth()/2,
										 continueButton[0].y+continueButton[0].h/2-gText.getHeight()/2,
										 gText.getWidth(), gText.getHeight());

				// Render Text
				//gText.loadFromRenderedText(gRenderer, "Reset High Scores", color, gFont24);
				//gText.render(gRenderer, continueButton[2].x+continueButton[2].w/2-gText.getWidth()/2,
				//						 continueButton[2].y+continueButton[2].h/2-gText.getHeight()/2,
				//						 gText.getWidth(), gText.getHeight());
			}

			// Render High Score text
			/*for (int i=0; i<10; i++){
				std::stringstream tempString(highList[i].c_str());
				std::string line;
				while (getline(tempString, line)) {
					std::stringstream iss(line);
					std::string temps[2];
					iss >> temps[0] >> temps[1];

					// Show Player where they are ranked
					if (indexSaved==i){
						gText.loadFromRenderedText(gRenderer, temps[0].c_str(), {244,144,20}, gFont20);
						gText.setAlpha(255-i*10);
						gText.render(gRenderer, continueButton[0].x+position,
								continueButton[0].y+continueButton[0].h+20+i*14,
								gText.getWidth(), gText.getHeight());
					}else{
						gText.loadFromRenderedText(gRenderer, temps[0].c_str(), color, gFont20);
						gText.setAlpha(255-i*10);
						gText.render(gRenderer, continueButton[0].x+position,
								continueButton[0].y+continueButton[0].h+20+i*14,
								gText.getWidth(), gText.getHeight());
					}

					gText.loadFromRenderedText(gRenderer, temps[1].c_str(), color, gFont20);
					gText.setAlpha(255-i*10);
					gText.render(gRenderer, position2,
							continueButton[1].y+continueButton[1].h+20+i*14,
							gText.getWidth(), gText.getHeight());
				}
			}*/
		}


		// Health
		{
			// Health bar
			int uiX = screenWidth * 0.95 - 100 - 10;
			int uiY = screenHeight * 0.96 - 20 - 20 - 48 - 6;

			gText.loadFromRenderedText(gRenderer, "Health", {255,255,255}, gFont13);
			gText.render(gRenderer,  uiX-gText.getWidth()-2, uiY, gText.getWidth(), gText.getHeight());

			// Render health, bg
			int barWidth = 150;
			SDL_Rect tempRect = {uiX, uiY, (barWidth*this->healthMax)/this->healthMax, 24};
			SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render health
			tempRect = {uiX + 1, uiY + 1, ((barWidth*this->health)/this->healthMax) - 2, 24-2};
			SDL_SetRenderDrawColor(gRenderer, 255, 30, 30, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render health, border
			tempRect = {uiX, uiY, (barWidth*this->healthMax)/this->healthMax, 24};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);
		}

		// Mana
		{
			// Mana bar
			int uiX = screenWidth * 0.95 - 100 - 10;
			int uiY = screenHeight * 0.96 - 20 - 20 - 24 - 6;

			gText.loadFromRenderedText(gRenderer, "Mana", {255,255,255}, gFont13);
			gText.render(gRenderer,  uiX-gText.getWidth()-2, uiY, gText.getWidth(), gText.getHeight());

			// Render mana, bg
			int barWidth = 150;
			SDL_Rect tempRect = {uiX, uiY, (barWidth*this->maxMana)/this->maxMana, 24};
			SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render mana
			tempRect = {uiX + 1, uiY + 1, ((barWidth*this->mana)/this->maxMana) - 2, 24-2};
			SDL_SetRenderDrawColor(gRenderer, 0, 90, 200, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render mana, border
			tempRect = {uiX, uiY, (barWidth*this->maxMana)/this->maxMana, 24};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);
		}

		// Parry CD
		{
			// Parry bar
			int uiX = screenWidth * 0.95 - 100 - 10;
			int uiY = screenHeight * 0.96 - 20 - 20;

			gText.loadFromRenderedText(gRenderer, "Parry CD ", {255,255,255}, gFont13);
			gText.render(gRenderer,  uiX-gText.getWidth()-2, uiY, gText.getWidth(), gText.getHeight());

			// Render Parry, bg
			int barWidth = 150;
			SDL_Rect tempRect = {uiX, uiY, (barWidth*this->parryCDMax)/this->parryCDMax, 24};
			SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render Parry
			int tempN = this->parryCDTimer - this->parryCDMax;
			tempRect = {uiX + 1, uiY + 1, ((barWidth*(-tempN))/this->parryCDMax) - 2, 24-2};
			SDL_SetRenderDrawColor(gRenderer, 0, 220, 220, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render Parry, border
			tempRect = {uiX, uiY, (barWidth*this->parryCDMax)/this->parryCDMax, 24};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);
		}

		// Dash counter
		{
			// Dash bar
			int uiX = screenWidth * 0.95 - 100 - 10;
			int uiY = screenHeight * 0.96 - 20 - 20 + 24 + 6;

			gText.loadFromRenderedText(gRenderer, "Dash CD ", {255,255,255}, gFont13);
			gText.render(gRenderer,  uiX-gText.getWidth()-2, uiY, gText.getWidth(), gText.getHeight());

			// Render dash, bg
			int barWidth = 150;
			SDL_Rect tempRect = {uiX, uiY, (barWidth*this->maxMana)/this->maxMana, 24};
			SDL_SetRenderDrawColor(gRenderer, 60, 60, 60, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render dash
			int tempN = this->dashCooldown - this->dashCoolCounter;
			tempRect = {uiX + 1, uiY + 1, ((barWidth*tempN)/this->dashCooldown) - 2, 24-2};
			SDL_SetRenderDrawColor(gRenderer, 200, 200, 20, 255);
			SDL_RenderFillRect(gRenderer, &tempRect);

			// Render dash, border
			tempRect = {uiX, uiY, (barWidth*this->maxMana)/this->maxMana, 24};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);
		}
	}

	// Highscore text
	std::stringstream tempsi;
	tempsi << "Highscore: " << this->highscore;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {244, 144, 20}, gFont24);
	gText.render(gRenderer, screenWidth-gText.getWidth()-15, 75, gText.getWidth(), gText.getHeight());

	tempsi.str( std::string() );
	tempsi << "Score: " << this->score;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255, 255, 255}, gFont24);
	gText.render(gRenderer, screenWidth-gText.getWidth()-15, 75+28*1, gText.getWidth(), gText.getHeight());

	//tempsi.str( std::string() );
	//tempsi << "dmg x " << this->damageMultipler;
	//gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255,255,255}, gFont13);
	//gText.render(gRenderer, this->x+this->w/2-gText.getWidth()/2-camX, this->y-gText.getHeight()-camY, gText.getWidth(), gText.getHeight());

}

void Players::RenderDebug(SDL_Renderer *gRenderer, int camX, int camY)
{
	////////////////////////////////////////////////////////////
	//--------------------- Render boxes ---------------------//
	//--------------------------------------------------------//

	// Original box, untouched
	SDL_Rect tempRect = {x-camX, y-camY, w, h};
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(gRenderer, &tempRect);

	////////////////////////////////////////////////////////////
	//---------------------- Render text ---------------------//
	//--------------------------------------------------------//

	std::stringstream tempsi;
	tempsi << "attack: " << attack;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255,255,255}, gFont24);
	gText.render(gRenderer, 0, 31, gText.getWidth(), gText.getHeight());

	tempsi.str( std::string() );
	tempsi << "attackType: " << attackType;
	gText.loadFromRenderedText(gRenderer, tempsi.str().c_str(), {255,255,255}, gFont24);
	gText.render(gRenderer, 0, 57, gText.getWidth(), gText.getHeight());

	/*tempsi.str( std::string() );
	tempsi << "Health: " << health;
	gText.loadFromRenderedText(tempsi.str().c_str(), {0,255,0}, gFont24, gRenderer);
	gText.render(gRenderer, screenWidth-gText.getWidth()-15, 58, gText.getWidth(), gText.getHeight());*/

	// Check what keys are pressed
	/*for (int i=0; i<25; i++){
		std::std::stringstream tempss;
		tempss << "i: " << i << " -        " << SDL_JoystickGetButton(joy, i);
		gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
		gText.render(gRenderer, 5, 5+i*15, gText.getWidth(), gText.getHeight());
	}

	std::std::stringstream tempss;
	tempss.str(std::string());
	tempss << "Axis: " << 0 << " -        " << SDL_JoystickGetAxis(joy, 0);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 5, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 1 << " -        " << SDL_JoystickGetAxis(joy, 1);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 20, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 2 << " -        " << SDL_JoystickGetAxis(joy, 2);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 35, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 3 << " -        " << SDL_JoystickGetAxis(joy, 3);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 50, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 4 << " -        " << SDL_JoystickGetAxis(joy, 4);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 65, gText.getWidth(), gText.getHeight());

	tempss.str(std::string());
	tempss << "Axis: " << 5 << " -        " << SDL_JoystickGetAxis(joy, 5);
	gText.loadFromRenderedText(tempss.str().c_str(), {255, 255, 255, 255}, gFont, gRenderer);
	gText.render(gRenderer, 60, 80, gText.getWidth(), gText.getHeight());*/
}



// Key Pressed
void Players::OnKeyDown(SDL_Keycode sym )
{
	switch (sym)
	{
	case SDLK_a:
		this->controls = 0;
		this->moveleft = true;
		break;
	case SDLK_d:
		this->controls = 0;
		this->moveright = true;
		break;
	case SDLK_w:
		this->controls = 0;
		this->moveup = true;
		break;
	case SDLK_s:
		this->controls = 0;
		this->movedown = true;
		break;
	case SDLK_h:
		//debug = ( !debug );
		break;
	case SDLK_y:
		this->camlocked = ( !this->camlocked );
		break;
	case SDLK_m:						// Dodge
		// Future code to dodge
	case SDLK_j:						// Attack (spell attack? This is not finalized.)

		break;
	case SDLK_LSHIFT:
		this->shift = true;
		break;
	case SDLK_n:						// Attack

		// Attack
		SlashAttack();
		break;
	case SDLK_c:						// Parry

		// Activate Parry
		ActivateParry();
		break;
	case SDLK_SPACE: 					// Dash

		// Activate Dash
		ActivateDash();
		break;
	}
}

// Key Released
void Players::OnKeyUp(SDL_Keycode sym )
{
	switch (sym){
	case SDLK_a:
		this->moveleft = false;
		break;
	case SDLK_d:
		this->moveright = false;
		break;
	case SDLK_w:
		this->moveup = false;
		break;
	case SDLK_s:
		this->movedown = false;
		break;
	case SDLK_j:

		break;
	case SDLK_LSHIFT:
		this->shift 		= false;
		break;
	case SDLK_SPACE:
    	//
		break;
	}
}

// Player Mouse Pressed
void Players::mouseClickState(SDL_Event &e){
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			this->controls = 0;
			this->leftclick = true;

			// Attack
			SlashAttack();
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			this->controls = 0;
			this->rightclick = true;
			this->thrust = true;
		}
	}else if (e.type == SDL_MOUSEBUTTONUP) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			this->leftclick = false;

			// Shoot bullet
			this->initialshot = false;
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			this->rightclick = false;
			this->thrust = false;
		}
	}
}

// Update XBOX 360 controls
void Players::updateJoystick( SDL_Event &e){

	/* Xbox 360 Controls */

	// Get Left Analog Angle
	if (((SDL_JoystickGetAxis(this->joy, 0) < -JOYSTICK_DEAD_ZONE) || (SDL_JoystickGetAxis(this->joy, 0) > JOYSTICK_DEAD_ZONE)) ||
		((SDL_JoystickGetAxis(this->joy, 1) < -JOYSTICK_DEAD_ZONE) || (SDL_JoystickGetAxis(this->joy, 1) > JOYSTICK_DEAD_ZONE))){
		this->LAngle = atan2(SDL_JoystickGetAxis(this->joy, 1), SDL_JoystickGetAxis(this->joy, 0)) * ( 180.0 / M_PI );
	}

	// Xbox 360 Controls
	if (e.type == SDL_JOYBUTTONDOWN && e.jbutton.state == SDL_PRESSED && e.jbutton.which == 0){
		switch(e.jbutton.button){
		case 0:
			this->D_UP = true;
			break;
		case 1:
			this->D_DOWN = true;
			break;
		case 2:
			this->D_LEFT = true;
			break;
		case 3:
			this->D_RIGHT = true;
			break;
		case 4:
			this->START = true;
			break;
		case 5:
			this->BACK = true;
			break;
		case 6:
			this->L3 = true;
			break;
		case 7:
			this->R3 = true;
			break;
		case 8:
			this->LB = true;
			break;
		case 9:
			this->RB = true;
			break;
		case 10:
			this->A = true;
			break;
		case 11:
			this->B = true;
			break;
		case 12:
			this->X = true;
			break;
		case 13:
			this->Y = true;
			break;
		}
	}else if (e.type == SDL_JOYBUTTONUP && e.jbutton.state == SDL_RELEASED && e.jbutton.which == 0){
		switch(e.jbutton.button){
		case 0:
			this->D_UP = false;
			break;
		case 1:
			this->D_DOWN = false;
			break;
		case 2:
			this->D_LEFT = false;
			break;
		case 3:
			this->D_RIGHT = false;
			break;
		case 4:
			this->START = false;
			break;
		case 5:
			this->BACK = false;
			break;
		case 6:
			this->L3 = false;
			break;
		case 7:
			this->R3 = false;
			break;
		case 8:
			this->LB = false;
			break;
		case 9:
			this->RB = false;
			break;
		case 10:
			this->A = false;
			break;
		case 11:
			this->B = false;
			break;
		case 12:
			this->X = false;
			break;
		case 13:
			this->Y = false;
			break;
		}
	}
}

void Players::SlashAttack() {
	if (!this->attack && !this->stunned && !this->parry) {

		// If currently dashing
		if (this->dash)
		{
			// Stop dashing
			StopDashing();
		}

		// Set attack parameters
		this->playSlash = true;
		this->clash = false;
		this->attack = true;
		this->spawnAttack = true;
		this->attackType = 0;

		// Shoot bullet
		this->initialshot = true;

		// Do normal Slash Attack
		/*if (!this->movedown) {
			this->attackType = 0;
		}
		// Do Stab-Down attack
		else {
			this->attackType = 1;
		}*/
	}
}

void Players::ActivateParry() {
    if (!this->parry && this->parryCDTimer <= 0 && !this->stunned)
    {
		// Stop attacking
		if (this->attack) {
			StopSlashAttack();
		}

		// Stop dashing
		if (this->dash) {
			StopDashing();
		}

    	// enable parrying
    	this->parry = true;

    	// Have parry on CD
    	this->parryCDTimer = this->parryCDMax;

    	// Play sound effect
    	Mix_PlayChannel(-1, sParry, 0);
    }
}

void Players::ActivateDash() {
	// If Dash is not on cool down
	if (this->dashCoolCounter <= 0 && this->dashCounter <=0) {

		// Make sure we are not parrying before dashing
		// Because we dont want to stop the animation of parrying
		// if we dash.
		if (!this->parry) {

			// Stop attacking
			if (this->attack) {
				StopSlashAttack();
			}

			// Depending on which way the player is moving,
			if (this->moveleft) {
				this->vX -= this->dashSpeed;
			}else if (moveright) {
				this->vX += this->dashSpeed;
			}
			if (this->moveup) {
				this->vY -= this->dashSpeed;
			}else if (movedown) {
				this->vY += this->dashSpeed;
			}
			this->dash = true;
			this->dashCounter = this->dashLength;

			// Play dash sound effect
			Mix_PlayChannel(-1, sDash, 0);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// MUTATOR FUNCTIONS ///////////////////////////////

void Players::RestorePlayer(float spawnX, float spawnY) {
	// Reset Player
	std::string newName;
	newName="AAA";
	Init(spawnX, spawnY, newName, false);
	///////////input.getInput(gameLoop, quit, newName, gWindow, gRenderer);

}

float Players::moveX(float value) {
	return this->x += value;
}

float Players::moveY(float value) {
	return this->y += value;
}

// Applies a shield to Player
void Players::ActivateInvurnerability(){
	invurnerableFrame			= 1;
	invurnerableTick			= 0;
	invurnerableT				= 300;
	invurnerable				= true;
}

void Players::StopParrying()
{
	// Reset parry CD
	this->parryTimer = 0;
	this->parry = false;
	this->parryCDTimer = this->parryCDMax;
}

void Players::ResetParry()
{
	// Reset parry CD
	this->parryTimer = 0;
	this->parry = false;
	this->parryCDTimer = 0;

	// Increase damage multiplier
	//this->damageMultipler += 0.1;
}

void Players::ExtendParryDuration()
{
	//  Extend parry
	this->parryTimer = 0;

	// Gain mana back
	this->mana += this->manaGainOnParry;

	// Increase damage multiplier
	//this->damageMultipler += 0.1;
}

void Players::IncreaseHealth(float value) {
	this->health += value;
	if (this->health > this->healthMax) {
		this->health = this->healthMax;
	}
}

void Players::IncreaseScore(float value) {
	this->score += value;
}

void Players::ShortenParryCD(float value) {
	this->parryCDTimer -= value;

	if (this->parryCDTimer <= 0) {
		this->parryCDTimer = 0;
	}
}

void Players::ApplyHighScore(float previousHighScore) {
	this->highscore = previousHighScore;
}



void Players::StopMovement()
{
	// Stop player movement
    this->vX = 0.0;
    this->vY = 0.0;
    this->moving = false;
}

void Players::StopSlashAttack()
{
	// Reset slash attack timers
	attackType = -1;
    attackTimer = 0;
    attack = false;
}

void Players::StopDashing()
{
	// Reset dash timers
	this->dash = false;
	this->dashCounter 	= 0;
	this->dashCoolCounter = this->dashCooldown;
}

void Players::ResetDashing()
{
	// Reset dash timers
	this->dash = false;
	this->dashCounter 	= 0;
	this->dashCoolCounter = 0;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// GETTER FUNCTIONS ////////////////////////////////

float Players::getX() {
	return this->x;
}

float Players::getY() {
	return this->y;
}

float Players::getW() {
	return this->w;
}

float Players::getH() {
	return this->h;
}

float Players::getLeftSide() {
	return this->x;
}

float Players::getRightSide() {
	return this->x+this->w;
}

float Players::getTopSide() {
	return this->y;
}

float Players::getBottomSide() {
	return this->y+this->h;
}

float Players::getDamage() {
	return this->damage * this->damageMultipler;
}

float Players::getCastDamage() {
	return this->castDamage;
}


// Get x center of player
float Players::getCenterX() {
	return this->x + this->w/2;
}

// Get y center of player
float Players::getCenterY() {
	return this->y + this->h/2;
}

// Get value depending on direction facing
float Players::getXDir() {
	// Knockback player
	float xDir = 1;
	if (this->facing=="left") {
		xDir = -1;
	}
	if (this->facing=="right") {
		xDir = 1;
	}
	return xDir;
}

// Get knockback power
float Players::getKnockBackPower() {
	return this->knockBackPower;
}


float Players::getInvurnerableStatus() {
	return this->invurnerable;
}

float Players::getParryStatus() {
	return this->parry;
}

float Players::getDashStatus() {
	return this->dash;
}

float Players::getScore() {
	return this->score;
}

void Players::SaveHighScore(int LevelWeLoaded) {

	// Open highscore first to check value
	unsigned int tempScore = -1;

	// Open file, and get previous High Score
	{
		std::stringstream filePath;
		filePath << "data/maps/highscore";
		filePath << LevelWeLoaded;
		filePath << ".txt";

		std::fstream fileOpen(filePath.str().c_str());

		// If a file exist
		if (fileOpen.is_open()) {
			// Store previous High score in this variable
			fileOpen >> tempScore;


			// Check if current Score is higher than previous High score
			if (this->score > tempScore) {

				// Save current Score as new High Score
				std::stringstream filePath;
				filePath << "data/maps/highscore";
				filePath << LevelWeLoaded;
				filePath << ".txt";

				std::ofstream fileSave;
				fileSave.open(filePath.str().c_str());
				fileSave << this->score;
				fileSave.close();
			}
		}

		// If no file exists, create new highscore
		else {

			std::cout<< "File does not exist on Saving Highscore, creating new file.\n";
			{
				std::stringstream filePath;
				filePath << "data/maps/highscore";
				filePath << LevelWeLoaded;
				filePath << ".txt";

				std::ofstream fileSave;
				fileSave.open(filePath.str().c_str());
				fileSave << this->score;
				fileSave.close();
			}
		}


		fileOpen.close();

	}
}


















