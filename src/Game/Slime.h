/*
 * Slime.h
 *
 *  Created on: Jul 21, 2017
 *      Author: Carl
 */

#ifndef GAME_SLIME_H_
#define GAME_SLIME_H_

#include "Object.h"

#include <SDL2/SDL_ttf.h>
#include "../LWindow.h"
#include "../LTexture.h"

#include "../Helper.h"
#include "Monster.h"
#include "Maps.h"

class Slime: public Monster, public Helper {
public:
	int count;
	const int max = 100;
	LTexture gTexture;
	SDL_Rect rClip[5];

public: // variables
	double walkTimer;
	double walkSpeed;
	double walkFrame;

public:	// Core Functions
	void Init(Slime slime[]);
	void Load(SDL_Renderer *gRenderer);
	void Free();
	void Clear(Slime slime[]);
	void Spawn(Slime slime[], float x, float y, float w, float h, float angle, float speed, int type);
	void Update(Slime slime[], Object &obj, Object object[], Map &map, int camx, int camy);
	void Render(SDL_Renderer *gRenderer, Slime slime[], TTF_Font *gFont, LTexture gText, int camx, int camy);
	void RenderDebug(SDL_Renderer *gRenderer, Slime slime[], TTF_Font *gFont, LTexture gText, int camx, int camy);

public:	// Functions that work with other classes

	void GetDistanceOfPlayer(Slime slime[], float playerCenterX, float playerCenterY);

public:	// Animations

	// Target center
	float bmx, bmy;

	// Self center
	float bmx2, bmy2;



	/*
	 * -1: 	Cooldown state (can't attack)
	 * 0: 	Walking around
	 * 1: 	Moving towards player
	 * 2: 	Start charging-attack animation
	 */
	int animState;
	bool chargingAttack;
	float chargeTime;					// After 3 seconds, attack
	const float chargeTimeStart = 30;	// Change this to change the charge-attack animation start time

	float coolDownTimer;
	float coolDownTimeStart = 180;

public: // Saving & loading functions

	void LoadData(Slime slime[], std::fstream &fileTileDataL);		// Load data

	std::string SaveData(Slime slime[]);		// Save data

public: // Other classes: Objects


};

#endif /* GAME_SLIME_H_ */
