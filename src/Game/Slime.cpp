/*
 * Slime.cpp
 *
 *  Created on: Jul 21, 2017
 *      Author: Carl
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "Slime.h"

void Slime::Init(Slime slime[]) {
	count = 0;
	for (int i = 0; i < max; i++) {
		slime[i].x 					= 0;
		slime[i].y 					= 0;
		slime[i].w 					= 0;
		slime[i].h 					= 0;
		slime[i].vX 				= 0.0;
		slime[i].vY 				= 0.0;
		slime[i].speed 				= 0.0;
		slime[i].angle 				= 0.0;
		slime[i].health 			= 50;
		slime[i].damage 			= 5;
		slime[i].distance 			= 1;
		slime[i].alive 				= false;
		slime[i].collision 			= false;
		slime[i].onScreen 			= false;
		slime[i].alert 				= false;
		slime[i].renderInFront 		= false;
		slime[i].type 				= 0;
		slime[i].jumpstate			= "falling";
		slime[i].jumps				= 1;
		slime[i].jump 				= false;
		slime[i].animSpeed			= 15;
		slime[i].animTimer			= 0;
		slime[i].animFrame			= 0;
		slime[i].walkTimer			= 0;
		slime[i].walkSpeed			= 1;
		slime[i].walkFrame			= 0;
		slime[i].animState			= -1;
		slime[i].coolDownTimer		= this->coolDownTimeStart;
		slime[i].coolDownTimeStart	= this->coolDownTimeStart;

		slime[i].chargingAttack 	= false;
		slime[i].chargeTime 		= this->chargeTimeStart;
	}

	// Other classes:
}

void Slime::Load(SDL_Renderer *gRenderer) {
	gTexture.loadFromFile(gRenderer, "resource/gfx/slime.png");
	gSlimeShadow.loadFromFile(gRenderer, "resource/gfx/slime_shadow.png");
	rClip[0] = {0, 0, 32, 24};
	rClip[1] = {32, 0, 32, 24};
	rClip[2] = {64, 0, 32, 24};
	rClip[3] = {96, 0, 32, 24};
	rClip[4] = {128, 0, 32, 24};
}

void Slime::Free() {
	gTexture.free();
	gSlimeShadow.free();
}

void Slime::Clear(Slime slime[]) {
	count = 0;
	for (int i = 0; i < max; i++) {
		slime[i].alive = false;
	}
}

void Slime::Spawn(Slime slime[], float x, float y, float w, float h, float angle, float speed, int type) {
	for (int i = 0; i < max; i++) {
		if (!slime[i].alive) {

			///////////////////////////////////////////////////////////////////////////////////////
			//----------- When slime is spawned, spawn going a random direction -----------------//
			int randDir = rand() % 2;
			int dir;
			if (randDir == 0)
				dir 				= 1;
			else
				dir 				= -1;
			//----------- When slime is spawned, spawn going a random direction -----------------//
			///////////////////////////////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////////////////////////////
			//------------------------------ Save/load this data --------------------------------//
			slime[i].x 				= x;
			slime[i].y 				= y;
			slime[i].w 				= w;
			slime[i].h 				= h;
			slime[i].radius 		= w/2;
			slime[i].speed 			= speed;
			slime[i].type 			= type;
			slime[i].damage			= 5;
			slime[i].alive 			= true;

			// Spawning normal slime, set default parameters
			if (type == 0) {
				slime[i].health = 50;
			} else {
				slime[i].health = 100;
			}
			slime[i].vX 			= dir;
			slime[i].vY 			= sin( (3.14159265/180)*(angle) );
			slime[i].angle 			= 0.0;

			//------------------------------ Save/load this data --------------------------------//
			///////////////////////////////////////////////////////////////////////////////////////

			//------------------------------------------------------------------------//
			/* initial centering of slime's circle collision shape */
			float radians 			= (3.1415926536/180)*(0.0);
			float Cos 				= floor(cos(radians)*10+0.5)/10;
			float Sin 				= floor(sin(radians)*10+0.5)/10;
			int newW 				= 11 * (-Cos);
			int newH 				= 11 * (-Sin);
			slime[i].x2 			= x+w/2 + newW - slime[i].radius;
			slime[i].y2 			= y+h/2 + newH - slime[i].radius;
			//------------------------------------------------------------------------//
			//slime[i].vX 				= cos( (3.14159265/180)*(angle) );
			slime[i].x2 				= x + w/2;
			slime[i].y2 				= y + h/2;
			slime[i].animState			= -1;
			slime[i].coolDownTimer		= this->coolDownTimeStart;
			slime[i].coolDownTimeStart	= this->coolDownTimeStart;
			slime[i].chargingAttack 	= false;
			slime[i].chargingAttack 	= false;
			slime[i].chargeTime 		= this->chargeTimeStart;
			slime[i].distance 			= 1;
			slime[i].collision 			= false;
			slime[i].onScreen 			= false;
			count++;
			break;
		}
	}
}

void Slime::Update(Slime slime[], Object &obj, Object object[], Map &map, int camx, int camy) {
	for (int i = 0; i < this->max; i++) {
		if (slime[i].alive) {

			// slime screen check
			if (slime[i].x + slime[i].w >= camx && slime[i].x <= camx + screenWidth &&
				slime[i].y + slime[i].h >= camy && slime[i].y <= camy + screenHeight) {
				slime[i].onScreen = true;
			}else{
				slime[i].onScreen = false;
			}

			// slime circle collision check with other slimes
			/*for (int j = 0; j < this->max; j++) {
				if (i != j) {
					if (slime[j].alive) {
						float bmx = slime[j].x+slime[j].w/2;
						float bmy = slime[j].y+slime[j].h/2;
						float bmx2 = slime[i].x+slime[i].w/2;
						float bmy2 = slime[i].y+slime[i].h/2;
						float angle = atan2(bmy - bmy2,bmx - bmx2);
						angle = angle * (180 / 3.1416);
						if (angle < 0) {
							angle = 360 - (-angle);
						}
						float radians = (3.1415926536/180)*(angle);
						float Cos = floor(cos(radians)*10+0.5)/10;
						float Sin = floor(sin(radians)*10+0.5)/10;
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));
						if (distance <= 0.01) {
							distance = 0.01;
						}
						if (distance < slime[i].w/2 + slime[j].w/2) {
							//slime[i].x -= 1 * Cos;
							//slime[i].vX = slime[i].vX * -1;

							// This will move both slimes away from each other so they don't overlap
							slime[j].x += 1 * Cos;

							// This still only bounces the slime thats being launched
							//slime[j].vX = slime[j].vX * -1.2;

							//slime[i].y -= 1 * Sin;
						}
					}
				}
			}*/

			// Animation
			slime[i].animTimer += slime[i].animSpeed;
			if (slime[i].animTimer > 60) {
				slime[i].animTimer = 0;
				slime[i].animFrame++;
			}

			if (slime[i].animFrame > 4) {
				slime[i].animFrame = 0;
				slime[i].animTimer = 0;
			}

			// If slime health goes lower than 0, remove slime
			if (slime[i].health <= 0) {
				slime[i].alive = false;
				count--;
			}

			// Decelerate slime velocity every frame
			slime[i].vX = slime[i].vX - slime[i].vX * 0.2;
			slime[i].vY = slime[i].vY - slime[i].vY * 0.2;

			////////////////////////////////////////////////////////////////////////////
			/////////////////////////// Handle Slime Animations ////////////////////////

			// Cooldown animation
			if (slime[i].animState == -1)
			{
				// If cool down timer greater than 0
				if (slime[i].coolDownTimer > 0) {

					// Start countdown
					slime[i].coolDownTimer--;
				}

				// Countdown finished
				else {
					// Reset cool down timer
					slime[i].coolDownTimer = this->coolDownTimeStart;

					// Reset animation state
					slime[i].animState = 0;
				}
			}

			// Walking around
			else if (slime[i].animState == 0)
			{
				// If player is within 300 distance from the slime, slime will follow player
				if (slime[i].distance < 300)
				{
					// Slime is on alert !
					slime[i].alert = true;
				} else
				{
					// Slime is NOT on alert
					slime[i].alert = false;
				}

				// Slime is on alert
				if (slime[i].alert)
				{
					// Change x velocity to go towards player
					slime[i].vX = 0.5 * (bmx - bmx2) / slime[i].distance;
				}

				// If player is within 80 pixels, start charging animation
				if (slime[i].distance < 80 && !slime[i].chargingAttack)
				{
					// Start charge-attack animation
					slime[i].chargingAttack = true;

					// Change animation state
					slime[i].animState = 2;

					// Stop moving slime
					slime[i].vX = 0.0;
				}
			}

			// Moving towards player animation
			else if (slime[i].animState == 1) {

			}

			// Charging-attack animation
			else if (slime[i].animState == 2) {

				// If Charge attack animation
				if (slime[i].chargingAttack)
				{
					// If count down has not reached 0 seconds
					if (slime[i].chargeTime > 0) {

						// Start counting down charge-attack animation
						slime[i].chargeTime--;
					}

					// Countdown reached 0 seconds.
					else {

						// Stop charge attack animation
						slime[i].chargingAttack = false;

						// Reset charge-attack count down
						slime[i].chargeTime = this->chargeTimeStart;

						// Set animation state to cooldown (-1)
						slime[i].animState = -1;

						///////////////////////////////////////////////////////////////////////////////////////////////////////
						//----------------------------- Spawn attack at the end of charge attack ----------------------------//
		                int width;

		                // Player on right side of Slime
		                if (slime[i].bmx > slime[i].x+slime[i].w/2) {

		                	// Set attack object's x pos to the right of player's body
			                width = 64;
		                }

		                // Player on left side of Slime
		                else{

		                	// Set attack object's x pos to the left of player's body
			                width = -64;
		                }
		                // Attack-object's width and height
		                int tempWidth = 64;
		                int tempHeight = 34;

		               /* // Spawn attack object (it will appear in the world for 1 frame then remove itself)
		                obj.spawn(object, slime[i].x+slime[i].w/2 + width,
		                				  slime[i].y-20,
										  tempWidth, tempHeight,
										  2,
										  i);		// Save Slimes index for reference later (can be used to knockback this slime for example)*/
		                // Spawn attack object (it will appear in the world for 1 frame then remove itself)
		                obj.spawn(object, slime[i].x+slime[i].w/2 - tempWidth/2,
		                				  slime[i].y-20,
										  tempWidth, tempHeight,
										  2,
										  i);		// Save Slimes index for reference later (can be used to knockback this slime for example)

		                // Play slime hit sound effect

						//----------------------------- Spawn attack at the end of charge attack ----------------------------//
						///////////////////////////////////////////////////////////////////////////////////////////////////////
					}
				}
			}
			/////////////////////////// Handle Slime Animations ////////////////////////
			////////////////////////////////////////////////////////////////////////////

			// Camera level bounds
			if( slime[i].x < 0 ){
				slime[i].x = 0;
			}
			if( slime[i].y < 0 ){
				slime[i].y = 0;
			}
			if( slime[i].x+slime[i].w > map.w ){
				slime[i].x = map.w-slime[i].w;
			}
			if( slime[i].y+slime[i].h > map.h ){
				slime[i].y = map.h-slime[i].h ;
			}
		}
	}

	// Other classes:
}

void Slime::RenderBack(SDL_Renderer *gRenderer, Slime slime[], TTF_Font *gFont, LTexture gText, int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (slime[i].alive) {
			if (!slime[i].renderInFront) {

				// Walking around or idle
				if (slime[i].animState == -1)
				{
					gTexture.setColor(255, 255, 255);
				}

				// Walking around or idle
				else if (slime[i].animState == 0) {

					// If slime is on alert (sees player)
					if (slime[i].alert)
					{
						// Set color red
						gTexture.setColor(200, 0, 0);
					} else {
						// Set color normal
						gTexture.setColor(255, 255, 255);
					}
				}

				// Moving towards player animation
				else if (slime[i].animState == 1) {

				}

				// Charging-attack animation
				else if (slime[i].animState == 2) {
					gTexture.setColor(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
				}

				// Render slime shadow
				gSlimeShadow.render(gRenderer, slime[i].x-3 - camx, slime[i].y-8 - camy,
											slime[i].w+6, slime[i].h+12);

				// Render slime
				gTexture.render(gRenderer, slime[i].x - camx, slime[i].y - camy,
											slime[i].w, slime[i].h,
											&rClip[slime[i].animFrame],
											slime[i].angle);
			}
		}
	}
}

void Slime::RenderFront(SDL_Renderer *gRenderer, Slime slime[], TTF_Font *gFont, LTexture gText, int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (slime[i].alive) {
			if (slime[i].renderInFront) {

				// Walking around or idle
				if (slime[i].animState == -1)
				{
					gTexture.setColor(255, 255, 255);
				}

				// Walking around or idle
				else if (slime[i].animState == 0) {

					// If slime is on alert (sees player)
					if (slime[i].alert)
					{
						// Set color red
						gTexture.setColor(200, 0, 0);
					} else {
						// Set color normal
						gTexture.setColor(255, 255, 255);
					}
				}

				// Moving towards player animation
				else if (slime[i].animState == 1) {

				}

				// Charging-attack animation
				else if (slime[i].animState == 2) {
					gTexture.setColor(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
				}

				// Render slime shadow
				gSlimeShadow.render(gRenderer, slime[i].x-3 - camx, slime[i].y-8 - camy,
											slime[i].w+6, slime[i].h+12);

				// Render slime
				gTexture.render(gRenderer, slime[i].x - camx, slime[i].y - camy,
											slime[i].w, slime[i].h,
											&rClip[slime[i].animFrame],
											slime[i].angle);
			}
		}
	}
}

void Slime::RenderDebug(SDL_Renderer *gRenderer, Slime slime[], TTF_Font *gFont, LTexture gText, int camx, int camy) {
	for (int i = 0; i < max; i++) {
		if (slime[i].alive) {

			// DEBUG SQAURES UNDER HERE

			// Render box around slime
			SDL_Rect tempRect = {slime[i].x-camx, slime[i].y-camy, slime[i].w, slime[i].h};
			SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);

			// Slime imaginary line that checks if there is a block in the next step
			// if not, then the slime turns around
			// Where we check this is inside 'checkSlimeTilecCollision()' in PlayGame class
			SDL_Rect rectMiddle = {slime[i].x+slime[i].w/2-1-camx, slime[i].y+slime[i].h-camy, 2, slime[i].h+2};
			SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(gRenderer, &rectMiddle);


			/// TEXT UNDER HERE

			// Render Text
			std::stringstream tempss;
			tempss << "AS: " << slime[i].animState  << ", chargeAttack: " << slime[i].chargingAttack << ", d: " << slime[i].distance;
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255}, gFont);
			//gText.render(gRenderer, slime[i].x-camx, slime[i].y-gText.getHeight()-camy, gText.getWidth(), gText.getHeight());
		}
	}
}

// Functions that work with other classes

void Slime::GetDistanceOfPlayer(Slime slime[], float targetX, float targetY, float targetW, float targetH) {
	for (int i = 0; i < max; i++) {
		if (slime[i].alive) {

			////////////////////////////////////////////////////////////////////////////
			/////////////////////////// GET DISTANCE OF PLAYER /////////////////////////
			// Get center of attack-object (spawned by the player attacking)
			slime[i].bmx = targetX+targetW/2;
			slime[i].bmy = targetY+targetH/2;

			// Get center of slime
			slime[i].bmx2 = slime[i].x+slime[i].w/2;
			slime[i].bmy2 = slime[i].y+slime[i].h/2;

			// Get angle of slime relative to attack-object
			float angle = atan2(slime[i].bmy - slime[i].bmy2, slime[i].bmx - slime[i].bmx2);
			angle = angle * (180 / 3.1416);
			if (angle < 0) {
				angle = 360 - (-angle);
			}

			// Handle radians, cos, sin
			float radians = (3.1415926536/180)*(angle);
			float Cos = floor(cos(radians)*10+0.5)/10;
			float Sin = floor(sin(radians)*10+0.5)/10;

			// Distance of slime relative to attack-object
			slime[i].distance = sqrt((slime[i].bmx - slime[i].bmx2) * (slime[i].bmx - slime[i].bmx2)+
								  (slime[i].bmy - slime[i].bmy2) * (slime[i].bmy - slime[i].bmy2));

			// If it goes less than 0, the game crashes
			if (slime[i].distance <= 0.01) {
				slime[i].distance = 0.01;
			}

			/////////////////////////// GET DISTANCE OF PLAYER /////////////////////////
			////////////////////////////////////////////////////////////////////////////

			// Check if player is in front of slime or not
			if (slime[i].y+slime[i].h > targetY+targetH){
				slime[i].renderInFront = true;
			} else {
				slime[i].renderInFront = false;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// LOAD-SAVE ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------ Load Slime data --------------------------------------//
// Load Slime data
void Slime::LoadData(Slime slime[], std::fstream &fileTileDataL)
{
	// Reset all slimes
	Init(slime);

	//std::cout << "Attempting to load... Slime" << std::endl;
	// Load Slime count data
	fileTileDataL >> this->count;
	//std::cout << this->count << std::endl;

	// Load rest of Slime data
	for (int h = 0; h < this->count; h++) {
		if (!slime[h].alive) {

			fileTileDataL >>
				slime[h].x 		>>
				slime[h].y	 	>>
				slime[h].w 		>>
				slime[h].h 		>>
				slime[h].radius >>
				slime[h].speed 	>>
				slime[h].type 	>>
				slime[h].damage >>
				slime[h].health >>
				slime[h].vX 	>>
				slime[h].vY 	>>
				slime[h].angle 	>>
				slime[h].alive;

			//std::cout << "Slime h: " << h << ", x: " << slime[h].x << ", y: " << slime[h].y << std::endl;

			// When we load slimes, spawn them 1 pixel from the ground so that we dont have glitches or problems
			slime[h].y-=2;
		}
		//break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------ Save Slime data --------------------------------------//
// Save Tile data
std::string Slime::SaveData(Slime slime[])
{
	/////////////////////////////////////////////////////////////////////////////////
	//-----------------------------------------------------------------------------//
	//----------------------------- Save Everything in One File -------------------//

	// Save all tile data in a string first
	std::stringstream tempsstb;
	for (int i = 0; i < max; i++) {
		if (slime[i].alive) {
			tempsstb
				<< slime[i].x 			<< " "
				<< slime[i].y 			<< " "
				<< slime[i].w  			<< " "
				<< slime[i].h  			<< " "
				<< slime[i].radius  	<< " "
				<< slime[i].speed  		<< " "
				<< slime[i].type  		<< " "
				<< slime[i].damage  	<< " "
				<< slime[i].health  	<< " "
				<< slime[i].vX  		<< " "
				<< slime[i].vY  		<< " "
				<< slime[i].angle  		<< " "
			   	<< slime[i].alive 		<< "\n";
		}
	}
	std::string temps;
	temps = tempsstb.str().c_str();
	//temps = temps.substr(0, temps.size()-1);

	// Save number of tiles in World
	std::stringstream aVeryLongString;
	aVeryLongString << count << "\n";

	// Save the data of all Tiles
	aVeryLongString << temps.c_str();
	//----------------------------- Save Everything in One File -------------------//
	//-----------------------------------------------------------------------------//
	/////////////////////////////////////////////////////////////////////////////////

	return aVeryLongString.str().c_str();
}
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// LOAD-SAVE ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////











