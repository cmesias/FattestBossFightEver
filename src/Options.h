/*
 * Options.h
 *
 *  Created on: May 24, 2017
 *      Author: Carl
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_


#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "Game/Players.h"

#include "Helper.h"

#include "LTexture.h"
#include "LWindow.h"

class Options {
public:
	Helper helper;
	//enum OptionsResult { Back, Nothing, ShowingMenu, Exit };
	enum OptionsResult { Back, Exit, Nothing, StartGame, ShowingMenu };
	OptionsResult optionsResult = Nothing;

public:	// Exclusive for this game

	// Textures
	LTexture gParticles;
	SDL_Rect cParticles = {32, 0, 8, 8};

	// Audio
	Mix_Music *sAmbientMusic 		= NULL;
	Mix_Chunk *sRockBreak 			= NULL;
	Mix_Chunk *sAtariBoom 			= NULL;
	Mix_Chunk *sDownStabHitTilec 	= NULL;
	Mix_Chunk *sSlashHitBoss 		= NULL;
	Mix_Chunk *sCastHitBoss 		= NULL;
	Mix_Chunk *sSlashTilec 			= NULL;
	Mix_Chunk *sParrySuccess 		= NULL;

public:
	// don't declare
	int mx, my;
	int mex, mey;
	// declare these
	bool pauseLoop;
	bool leftclick;
	int index;
	int key;
	bool confirm;
	int timer;
	const int revertSettingsTimer = 600;

	// Default directory for audio and video config files
	const std::string defDir = "data/cfg/";

public:	// Bar variables
	const int barWidth = 320;
	const int barHeight = 56;
	const int menuTitleNamesMarginsH = 66;		// Options menu -> this is the space between each option in the Options Menu

	struct SFX {
		Mix_Chunk *sSFX 	= NULL;
		std::string name;
		SFX (std::string newName, std::string path) {
			name = newName;
			sSFX = Mix_LoadWAV(path.c_str());
		}
		Mix_Chunk *getChunk() {
			return this->sSFX;
		}
		std::string getName() {
			return name;
		}
	};

	/*void playSFX(std::vector<SFX> &s, std::string targetName)
	{
		for (unsigned int i=0; i<s.size(); i++)
		{
			if (s[i].getName() == targetName)
			{
				Mix_PlayChannel(-1, s[i].getChunk(), 0);
			}
		}
	};*/

public:	// Settings that can be changed by user
	// Textures
	LTexture gText;
	// Fonts
	TTF_Font *gFont 			= NULL;
	TTF_Font *gFont13 			= NULL;
	TTF_Font *gFont26 			= NULL;

	// Audio, Video Settings
	int MASTER_VOL;
	int MUSIC_VOL;
	int SFX_VOL;
	int RESOLUTION;
	int ANTI_ALIAS;
	int VSYNC;
	int FULLSCREEN;

public: // Bar sliders and text

	/*
	 * type 0:
	 *    - typeShown:
	 *    	0: Resume
	 *    	1: How To Play
	 *    	2: Settings
	 *    	3: Main Menu
	 *    	4: Exit To Desktop
	 * type 1:
	 *    - typeShown:
	 *      0: Audio
	 *    	1: Video
	 *    	2: back
	 *    	3: Apply Audio
	 *    	4: Apply Video
	 */
	int type;

	// Sub menu options
	int HIGHLIGHT_INDEX;

	// Set Game items
	bool mouseTitle[5];

	// Determine which applied button pressed
	std::string button;

	// Apply Bar Settings button
	SDL_Rect applyButton[2];		// 0: audio, 1: video (buttons)
	bool applyMouse[2];				// 0: audio, 1: video (buttons)

	// keep & Revert button
	SDL_Rect confirmButton[2];
	bool confirmMouse[2];

	// Items
	class Title{
	public:
		std::string name;
		int x;
		int y;
		int w;
		int h;
	};

	// Max number of items in the bar
	static const int barMax = 7;

	// Bar class
	class Bar{
	public:
		SDL_Rect rect;
		bool mouse;
		bool alive;
		bool highlight;
		int value;
		std::string name;

		void init(Bar bar[]){
			for (int i=0; i<barMax; i++){
				bar[i].rect.w   	= 100;
				bar[i].rect.h   	= 20;
				bar[i].highlight 	= false;
				bar[i].alive 		= false;
			}
		}
		void create(Bar bar[], std::string name){
			for (int i=0; i<barMax; i++){
				if (!bar[i].alive){
					bar[i].name 	= name;
					bar[i].mouse 	= false;
					bar[i].alive 	= true;
					break;
				}
			}
		}
		void update(Bar bar[], int posX, int posY, int mx, int my, bool leftclick, int HIGHLIGHT_INDEX){

			// Set position for all audio sliders
			const int audioSliderSpacing = 32;
			for (int i=0; i<3; i++){
				bar[i].rect.x = posX;
				bar[i].rect.y = posY+i*(bar[i].rect.h+audioSliderSpacing);
			}

			// Master volume sliders
			if (bar[0].alive){

				if (mx>=bar[0].rect.x && mx <= bar[0].rect.x+bar[0].rect.w && my>=bar[0].rect.y && my <= bar[0].rect.y+bar[0].rect.h){
					bar[0].mouse = true;
				}else{
					bar[0].mouse = false;
				}

				if (bar[0].mouse){
					if (leftclick){
						bar[0].value = 100*(mx - bar[0].rect.x)/bar[0].rect.w;
					}
				}
			}

			// Music and SFX volume sliders
			for (int i=1; i<3; i++){
				if (bar[i].alive){

					if (mx>=bar[i].rect.x && mx <= bar[i].rect.x+bar[i].rect.w && my>=bar[i].rect.y && my <= bar[i].rect.y+bar[i].rect.h){
						bar[i].mouse = true;
					}else{
						bar[i].mouse = false;
					}

					if (bar[i].mouse){
						if (leftclick){
							bar[i].value = 128*(mx - bar[i].rect.x)/bar[i].rect.w;
						}
					}
				}
			}

			// Video sliders and switches
			const int videoSliderSpacing = 32;
			for (int i=3; i<barMax; i++){
				if (bar[i].alive){
					bar[i].rect.x = posX;
					bar[i].rect.y = posY+i*(bar[i].rect.h+videoSliderSpacing);

					if (mx>bar[i].rect.x && mx < bar[i].rect.x+bar[i].rect.w && my>bar[i].rect.y && my < bar[i].rect.y+bar[i].rect.h){
						bar[i].mouse = true;
					}else{
						bar[i].mouse = false;
					}

					if (bar[i].mouse){
						if (leftclick){
							//bar[3].value = 4*(mx - bar[i].rect.x)/bar[i].rect.w;
						}
					}
				}
			}

			/* RESOLUTION BAR */
			if (leftclick){
				// Actual pixel length of resolution bar
				int individualBoxWidth = bar[0].rect.w;
				if (mx>bar[3].rect.x && mx < bar[3].rect.x+individualBoxWidth && my>bar[3].rect.y && my < bar[3].rect.y+bar[3].rect.h){
					bar[3].value = 1;
				}else if (mx>bar[3].rect.x && mx < (bar[3].rect.x+individualBoxWidth)+individualBoxWidth && my>bar[3].rect.y && my < bar[3].rect.y+bar[3].rect.h){
					bar[3].value = 2;
				}else if (mx>bar[3].rect.x && mx < (bar[3].rect.x+individualBoxWidth*2)+individualBoxWidth && my>bar[3].rect.y && my < bar[3].rect.y+bar[3].rect.h){
					bar[3].value = 3;
				}else if (mx>bar[3].rect.x && mx < (bar[3].rect.x+individualBoxWidth*3)+individualBoxWidth && my>bar[3].rect.y && my < bar[3].rect.y+bar[3].rect.h){
					bar[3].value = 4;
				}else if (mx>bar[3].rect.x && mx < (bar[3].rect.x+individualBoxWidth*4)+individualBoxWidth && my>bar[3].rect.y && my < bar[3].rect.y+bar[3].rect.h){
					bar[3].value = 5;
				}
			}

			/* ANTI_ALIASING BAR */
			if (leftclick){
				// Bar variables neater
				int x = bar[4].rect.x;
				int w = bar[4].rect.w;
				int y = bar[4].rect.y;
				int h = bar[4].rect.h;
				if (mx>x && mx < x+w/2 && my>y && my < y+h){
					bar[4].value = 1;
				}else if (mx>x && mx < (x+w/2)+w/2 && my>y && my < y+h){
					bar[4].value = 0;
				}
			}

			/* VSYNC BAR */
			if (leftclick){
				// Bar variables neater
				int x = bar[5].rect.x;
				int w = bar[5].rect.w;
				int y = bar[5].rect.y;
				int h = bar[5].rect.h;
				if (mx>x && mx < x+w/2 && my>y && my < y+h){
					bar[5].value = 1;
				}else if (mx>x && mx < (x+w/2)+w/2 && my>y && my < y+h){
					bar[5].value = 0;
				}
			}

			/* FULLSCREEN BAR */
			if (leftclick){
				// Bar variables neater
				int x = bar[6].rect.x;
				int w = bar[6].rect.w;
				int y = bar[6].rect.y;
				int h = bar[6].rect.h;
				if (mx>x && mx < x+w/2 && my>y && my < y+h){
					bar[6].value = 1;
				}else if (mx>x && mx < (x+w/2)+w/2 && my>y && my < y+h){
					bar[6].value = 0;
				}
			}

			// Highlights
			if (HIGHLIGHT_INDEX == 0){
				for (int i=0; i<3; i++){
					bar[i].highlight = true;
				}
				for (int i=3; i<barMax; i++){
					bar[i].highlight = false;
				}
			}else if (HIGHLIGHT_INDEX == 1){
				for (int i=0; i<3; i++){
					bar[i].highlight = false;
				}
				for (int i=3; i<barMax; i++){
					bar[i].highlight = true;
				}
			}else {
				for (int i=0; i<barMax; i++){
					bar[i].highlight = false;
				}
			}
		}
	};

public:	// classes
	Title title[5];
	Bar bar[7];
	Bar au;

public: // Load Resources

	// Load audio files
	void LoadAudioFiles();

	// Free audio files
	void FreeAudioFiles();

public:	// functions

	// Load video configurations
	void loadVideoCFG();

	// Load audio configurations
	void loadAudioCFG();

	// Save video configurations
	void saveVideoCFG();

	// Save audio configurations
	void saveAudioCFG();

	// Apply video configurations
	void applyVideoCFG(LWindow &gWindow);

	// Apply audio configurations
	void applyCustomAudioCFG(int MUSIC_VOL, int SFX_VOL);
	void applyOldAudioCFG();
	void applyMasterAudioCFG();

	// when you press the button, this action should be called
	void actionApplyAudio();
	void actionApplyVideo(LWindow &gWindow);
	void actionKeep();
	void actionRevert(LWindow &gWindow);

	// Load Options menu
	void start(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Save a level to a directory
	/*void SaveLevel(LWindow &gWindow, SDL_Renderer *gRenderer, bool &mainLoop,
				   std::string SpawnCoordinatesData,
				   std::string TileSaveData,
				   std::string ItemSaveData,
				   std::string MonsterSaveData);*/

	// Return a string the user has entered (hint is used to give the Editor an idea of what to write)
	std::string GetInput(LWindow &gWindow, SDL_Renderer *gRenderer, bool &mainLoop, std::string hint);

	// User input
	void OnKeyDown(SDL_Keycode sysm);
	void OnKeyUp(SDL_Keycode sysm);
	void mousePressed();
	void mouseReleased(LWindow &gWindow);

	// Render
	void RenderII(LWindow &gWindow, SDL_Renderer *gRenderer);

	// Render Text
	void RenderTextII(SDL_Renderer *gRenderer);

private:
	void updateJoystick(SDL_Renderer *gRenderer, LWindow &gWindow, SDL_Event *e);

	bool A;									// XBOX a button
	bool LAnalogTrigger;					// this will let us use the joystick as a trigger rather than an axis
	bool RAnalogTrigger;					// this will let us use the joystick as a trigger rather than an axis
	const int JOYSTICK_DEAD_ZONE = 8000;
	const int LTRIGGER_DEAD_ZONE = 25000;
	const int RTRIGGER_DEAD_ZONE = 25000;
	int joyStickID;
	double LAngle;
	double RAngle;
	bool confirmKey;
	// Used in type: 2
	// 0: If we are in the Audio, Video, back, Apply Audio, Apply Video indexX
	// 1: we will cycle through the Master volume and other option's index, which is indexOther
	// going left and right through Settings menu, where master volume is
	int indexX;
	int maxIndexX = 1; // will go up to 3 if we are confirming an action
	/*
	 * 0: master volume
	 * 1: music volume
	 * 2: sfx volume
	 * 3: resolution
	 * 4: anti-aliasing
	 * 5: vsync
	 * 6: fullscreen
	 * 7: apply audio
	 * 8: apply video
	 */
	int indexOther;
	const int maxIndexOther = 8;
	bool focusedOther;	// controller focused on an indexOther item, indexX should not move in this state

	/* change values in indexX == 1
	 * Used by these keys:
	 * Left-DPAD key
	 * Keyboard_A key
	 * Left_Arrow key
	 */
	void minusValues();

	/* change values in indexX == 1
	 * Used by these keys:
	 * Right-DPAD key
	 * Keyboard_D key
	 * RIGHT_Arrow key
	 */
	void addValues();

};

#endif /* OPTIONS_H_ */
