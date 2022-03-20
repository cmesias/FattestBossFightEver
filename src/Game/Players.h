/*
 * Players.h
 *
 *  Created on: Dec 18, 2016
 *      Author: Carl
 */

#ifndef LOCAL_PLAYERS_H_
#define LOCAL_PLAYERS_H_

#include <SDL2/SDL_ttf.h>

#include "../LWindow.h"
#include "../LTexture.h"
#include "../Particless.h"
#include "Enemies.h"
#include "../Helper.h"
#include "Maps.h"
#include "Tiles.h"
#include "Tilec.h"
#include "Object.h"
#include "TextNFont.h"

//Player
class Players: public Helper, public TextNFont {
public:
	enum Result { QUIT, LOADMENU, PLAYGAME, QUITPLAYGAME };
public:
	// Joystick Sensitivity
	const int JOYSTICK_DEAD_ZONE = 8000;
	void OnKeyDown(SDL_Keycode sym );
	void OnKeyUp(SDL_Keycode sym );
	void mouseClickState(SDL_Event &e);
	void updateJoystick(SDL_Event &e);

public:	// resources
	// Local resources
	LTexture gPlayer;
	LTexture gShield;

	// Audio
	Mix_Chunk *sCast 			= NULL;
	Mix_Chunk *sSlash 			= NULL;
	Mix_Chunk *sDownStab		= NULL;
	Mix_Chunk *sParry			= NULL;
	Mix_Chunk *sStep			= NULL;
	Mix_Chunk *sDash			= NULL;

	/*
	 * 0-3: Walk
	 * 4:   Before slash
	 * 5:   Slash
	 * 6:   Parry
	 * 7:   Down Stab
	 * 8:   Dash frame 0
	 * 9:   Dash frame 1
	 */
	SDL_Rect rPlayer[10];
	SDL_Rect rShield[7];

	/*
	 * 0-3: Walk
	 * 4:   Before slash
	 * 5:   Slash
	 * 6:   Parry
	 * 7:   Down Stab
	 * 8:   Dash frame 0
	 * 9:   Dash frame 1
	 */
	int sprite_index;

	/*
	 * 0: Keyboard
	 * 1: Keyboard + Mouse
	 */
	int ControlsPreference = 1;	// default: 0

public:	// variables
	std::string name;
	int timer = 0;
	int realw = 48, realh = 48;			// object size in actual pixel size
	int w = 24, h = 48;					// object size in collision pixels
	float x = 0,
		  y = 0;
	float x2, y2;						// player circle center
	float swordX;						// x pos Location of sword when down-stab attack
	float swordY;						// y pos Location of sword when down-stab attack
	int radius = w/2;					// player radius
	int panTimer 		= 0;
	int shoottimer 		= 0;
	float angle 		= 0.0,
		   radians 		= 0.0,
		   radianCos 	= 0.0,
		   radianSin 	= 0.0;
	float vX 			= 0.0;
	float vY 			= 0.0;
	const float velMax 	= 4;
	const float velSpeed= 2;
	float thrustSpeed 	= 0.0;
	float vYMax			= 10;
	bool camlocked 		= true;
	bool moveleft 		= false,
		 moveright 		= false,
		 moveup 		= false,
		 movedown		= false,
		 rotateLeft 	= false,
		 rotateRight 	= false;
	bool thrust 		= false;
	int thrustT 		= 0;
	bool shift 			= false;
	int timer2 			= 0;
	double time 		= 0;
	bool deathScreen	= false;
	bool alive;
	bool returned		= false;

	/*
	 * Parrying small blue Bullet from Boss:		score += 1;
	 * Parrying small red Bullet from Boss:			score += 2;
	 * Parrying large red Bullet from Boss:			score += 4;
	 * Hitting Boss directly w/ Slash Projectile: 	score += 5
	 * Hitting Boss directly w/ Sword: 				score += 5
	 * Hitting Boss with counter is:				score += 10
	 */
	unsigned int score	= 0;
	unsigned int highscore = 0;

	// Fire-rate
	//string power-up 		= "LAZER";
	int newmx			= 0;	// new mouse position
	int newmy			= 0;	// new mouse position
	int powerup 		= 1;
	double barrelX		= 0;	// gun barrel position
	double barrelY		= 0;
	double barrelW		= 0;	// whether to add width or subtract from gun barrel position
	double barrelH		= 0;
	double bulletW 		= 92;	// position the bullet will come out of the player ONLY if player
	double bulletH 		= 21;	// is centered (have angle at 0.0 to test)
	double particleW 	= 48;
	double particleH 	= 48;
	float delayT 		= 0;
	bool delay 			= false;
	bool initialshot 	= false;
	bool trigger 		= false;
	bool renderFlash	= false; // shuts off right after 1 frame
	bool playSFXDownStabHittingFloor	= true;
	double recoilX = 0;			 // gun recoil
	double recoilY = 0;

	// Lives
	int lives 			= 1;
	int health			= 100;
	const int healthMax	= 100;

	double tempc, temps;
	SDL_Rect continueButton[3];

	// High-Score display
	std::string highList[10];
	int indx 		= 0;
	float travel 	= 0.0;
	int position	= 0;
	int position2	= 0;
	int dir 		= 1;
	int indexSaved 	= -1;

	/* This will bring the player
	 * to life. At least, that's what
	 * it'll look like. This will rustle
	 * the player's armor as they move
	 */
	float rustleW = 0;
	float rustleH = 0;
	float rustleDirX = 1;
	float rustleDirY = 1;

	bool moving = false;

public: // attack variables
	std::string facing;
	SDL_RendererFlip flipW;
	int jumps;
	int attackTimer;
	int attackTimerSpe = 3;	// default: 1
	int attackFrame;
	int attackType;	// 0: slash, 1: down stab
	int stunTimer;
	bool attack;
	bool stunned;
	bool playSlash;
	bool clash;
	bool spawnAttack;
    //float *tempXX;
    //float *tempYY;

public:	// functions
	// Functions

	// Reset High Scores
	void ResetHighScore(int LevelWeLoaded);
	void Load(SDL_Renderer* gRenderer);
	void Free();
	void Init(float spawnX, float spawnY, std::string newName, bool respawn);
	bool checkCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);

	// Get audio files
	//void getAudio(Mix_Chunk* sLazer);

	// Player shoot
	void fire(Particle particle[], Particle &p_dummy, Mix_Chunk* sCastSFX, int mx, int my);

	void Update(Map &map,
				Enemy enemy[], Enemy &e_dummy,
				Particle particle[], Particle &p_dummy,
				Tile &tl, Tile tile[],
				Tilec &tlc, Tilec tilec[],
				Object &obj, Object object[],
				int mx, int my, int camx, int camy,
				float spawnX, float spawnY,
				LWindow gWindow, SDL_Renderer* gRenderer,
				LTexture gText, TTF_Font *gFont, SDL_Color color,
				Mix_Chunk *sAtariBoom, bool &RestartLevel,
				int LevelWeLoaded);

	void Render(int mx, int my, int camX, int camY, LWindow gWindow,
				SDL_Renderer* gRenderer,
				SDL_Color color, int &PARTICLES, LTexture gText);

	void RenderUI(SDL_Renderer *gRenderer, int camX, int camY);

	void RenderDebug(SDL_Renderer* gRenderer, int camX, int camY);

private:	// Private variables

	// Walktimer for sprite usage
	double walkTimer;

	// Walktimer visual effects
	double walkTimerVFX;

private:	// Private variables
			// Save these player stats in a file. This will be the players save data.

	float AttackSpeed;		// Attack speed

	float maxMana;			// Max mana
	float mana;				// Mana
	float manaRegenSpeed;	// Mana
	float manaRegenTimer;	// 1 second timer for mana regen;
	float manaGainOnParry;	// Gain mana if you parry successfully

	float damage;			// Sword damage
	float damageMultipler;	// Parrying will increase damage multiplier
	float castDamage;		// Cast damage

	float knockBackPower;	// Knock back power


	// Parry state
	bool parry;
	float parryTimer;		// Parry timer
	float parryCDTimer;		// Parry cooldown timer tracker
	float parryCDMax;		// Parry cooldown, default: 60*3

	// Dash state
	bool dash;							// Dash ability
	float dashSpeed;					// default: 15
	float dashLength;					// default: 10
	const float dashCooldown 	= 60;	// default: 60
	float dashCounter;					// default: 0
	float dashCoolCounter;				// default: 0

	// Invurnerable state
	int invurnerableFrame;
	int invurnerableTick;
	int invurnerableT;
	bool invurnerable;


public:	// Functions to do stuff?

	void SlashAttack();

	void ActivateParry();

	void ActivateDash();

public:	// Mutator functions

	// Restore Player for the Level
	void RestorePlayer(float spawnX, float spawnY);

	// Move x pos
	float moveX(float value);

	// Move y pos
	float moveY(float value);

	// Applies invurnerability to Player
	void ActivateInvurnerability();

	// Stops parry but goes on cool down
	void StopParrying();

	// Stops parry but reset cool down to 0
	void ResetParry();

	// Extend parry duration, and Gain mana back
	void ExtendParryDuration();

	// Health
	void IncreaseHealth(float value);

	// Score
	void IncreaseScore(float value);

	// Shorten parry cool down
	void ShortenParryCD(float value);

	// Load previous high score from PlayGame.cpp
	void ApplyHighScore(float previousHighScore);

	// Stop movement
	void StopMovement();

	// Stop Slash Attack
	void StopSlashAttack();

	// Stop dashing
	void StopDashing();

	// Reset dashing
	void ResetDashing();

public:	// Accessor functions

	// Get x pos
	float getX();

	// Get y pos
	float getY();

	// Get width
	float getW();

	// Get height
	float getH();

	// Get left pos
	float getLeftSide();

	// Get right pos
	float getRightSide();

	// Get top pos
	float getTopSide();

	// Get bottom pos
	float getBottomSide();

	// Get player damage
	float getDamage();

	// Get player cast damage
	float getCastDamage();

	// Get x center of player
	float getCenterX();

	// Get y center of player
	float getCenterY();

	// Get value depending on direction facing
	float getXDir();

	// Get knockback power
	float getKnockBackPower();

	// Get status of invurnerability
	float getInvurnerableStatus();

	// Get status of parrying
	float getParryStatus();

	// Get status of dashing
	float getDashStatus();

	// Get score
	float getScore();

public:	// Controls
	// Player controls
	int controls;		// [0] Keyboard, [1] Xbox 360 Controller
	SDL_Joystick *joy;
	bool A;
	bool B;
	bool X;
	bool Y;
	bool D_UP;
	bool D_DOWN;
	bool D_LEFT;
	bool D_RIGHT;
	bool LB;
	bool RB;
	bool BACK;
	bool START;
	bool L3;
	bool R3;
	double LAngle;
	double RAngle;
	double zAxisLeft;
	double zAxisRight;
	bool leftclick;
	bool rightclick;
	bool test;

public:

	// Save current highs-core for current Level
	void SaveHighScore(int LevelToLoad);

};

#endif /* LOCAL_PLAYERS_H_ */
