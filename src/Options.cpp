/*
 * Options.cpp
 *
 *  Created on: May 24, 2017
 *      Author: Carl
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Options.h"

// TODO [x] - Code an Options Menu inside options to be used in both MainMenu and during GamePlay
//			  After coding of Options Menu, the goal is to remove the Pause.cpp class.
// TODO [x] - fix keep and revert buttons
// TODO [x] - fix how "Render Apply Audio/Video Buttons" looks like, fix how when you resize the screen,
//			 it doesnt match when you put the mouse on top of the buttons

void Options::LoadAudioFiles() {
	//sElement = Mix_LoadMUS("sounds/Trevor Lentz/Pixel River.mp3");
	//sRockBreak = Mix_LoadWAV("sounds/rock_break.wav");
}

void Options::FreeAudioFiles() {
	//Mix_FreeChunk(sRockBreak);
	//Mix_FreeMusic(sElement);
}

// Load video configurations
void Options::loadVideoCFG(){
	// open config file
	std::stringstream tempss;
	// set default directory
	tempss << defDir;
	// set filename
	tempss << "video.cfg";
	std::fstream fileSettings( tempss.str().c_str() );
	if (fileSettings.is_open()) {
		fileSettings >>  RESOLUTION >> ANTI_ALIAS >> VSYNC >> FULLSCREEN;
		//std::cout << "Loaded video.cfg" << std::endl;
	}else{
		std::cout << "Error opening video.cfg." << std::endl;
		std::cout << "\tCreating default video.cfg..." << std::endl;
		std::ofstream newConfigFile("cfg/video.cfg");
		if (newConfigFile.is_open()) {
			std::cout << "\t\tSuccessfully created video.cfg." << std::endl;
			newConfigFile << "2 1 0 0";
		}
		// Opening and creating default config file failed, set Settings manually
		else{
			std::cout << "\t\tError creating default video.cfg." << std::endl;
			std::cout << "\t\t\tManually setting default video.cfg settings." << std::endl;
			RESOLUTION			= 2;
			ANTI_ALIAS			= 1;
			VSYNC				= 0;
			FULLSCREEN			= 0;
		}
		newConfigFile.close();
	}
	fileSettings.close();
}

// Load audio configurations
void Options::loadAudioCFG(){
	// open config file
	std::stringstream tempss;
	// set default directory
	tempss << defDir;
	// set filename
	tempss << "audio.cfg";
	std::fstream fileSettings( tempss.str().c_str() );
	if (fileSettings.is_open()) {
		fileSettings >>  MASTER_VOL >> MUSIC_VOL >> SFX_VOL;
		//std::cout << "Loaded audio.cfg" << std::endl;
	}else{
		std::cout << "Error opening audio.cfg." << std::endl;
		std::cout << "\tCreating default audio.cfg..." << std::endl;
		std::ofstream newConfigFile("cfg/audio.cfg");
		if (newConfigFile.is_open()) {
			std::cout << "\t\tSuccessfully created audio.cfg." << std::endl;
			newConfigFile << "100 128 128";
		}
		// Opening and creating default config file failed, set Settings manually
		else{
			std::cout << "\t\tError creating default audio.cfg." << std::endl;
			std::cout << "\t\t\tManually setting default audio.cfg settings." << std::endl;
			MASTER_VOL			= 100;
			MUSIC_VOL			= 128;
			SFX_VOL				= 128;
		}
		newConfigFile.close();
	}
	fileSettings.close();
}

void Options::saveVideoCFG(){
	// open config file
	std::stringstream tempss;
	// set default directory
	tempss << defDir;
	// set filename
	tempss << "video.cfg";
	std::ofstream fileSettings( tempss.str().c_str() );
	if (fileSettings.is_open()) {
		fileSettings <<  RESOLUTION << " " << ANTI_ALIAS << " " << VSYNC << " " << FULLSCREEN;
	}
	fileSettings.close();
}

void Options::saveAudioCFG(){
	// open config file
	std::stringstream tempss;
	// set default directory
	tempss << defDir;
	// set filename
	tempss << "audio.cfg";
	std::ofstream fileSettings( tempss.str().c_str() );
	if (fileSettings.is_open()) {
		fileSettings << MASTER_VOL << " " << MUSIC_VOL << " " << SFX_VOL;
	}
	fileSettings.close();
}

void Options::applyVideoCFG(LWindow &gWindow) {
	gWindow.applySettings(RESOLUTION, ANTI_ALIAS, VSYNC, FULLSCREEN);
}

void Options::applyCustomAudioCFG(int MUSIC_VOL, int SFX_VOL) {
	//Mix_VolumeMusic(MUSIC_VOL*(MASTER_VOL*0.01));
	//Mix_VolumeChunk(sRockBreak, SFX_VOL*(MASTER_VOL*0.01));
}

void Options::applyOldAudioCFG() {
	//Mix_VolumeMusic(MUSIC_VOL);
	//Mix_VolumeChunk(sRockBreak, SFX_VOL);
}

void Options::applyMasterAudioCFG() {
	//Mix_VolumeMusic(MUSIC_VOL*(MASTER_VOL*0.01));
	//Mix_VolumeChunk(sRockBreak, SFX_VOL*(MASTER_VOL*0.01));
}

//Get's input from user and returns it
void Options::start(LWindow &gWindow, SDL_Renderer *gRenderer)
{
	// Mouse cursor
	LTexture gCursor;
	gCursor.loadFromFile(gRenderer, "resource/gfx/cursor.png");

	SDL_Event e;

	// Create title bar names
	au.init(bar);
	au.create(bar, "MASTER VOLUME");
	au.create(bar, "MUSIC VOLUME");
	au.create(bar, "SFX VOLUME");
	au.create(bar, "RESOLUTION");
	au.create(bar, "ANTI ALIASING");
	au.create(bar, "VSYNC");
	au.create(bar, "FULLSCREEN");

	// Reset upon entering
	for (int i=0; i<5; i++) {mouseTitle[i] = false;}
	title[0].name 		= "RESUME";
	title[1].name 		= "TBA";
	title[2].name 		= "SETTINGS";
	title[3].name 		= "EXIT MAIN MENU";
	title[4].name 		= "EXIT TO DESKTOP";
	pauseLoop 			= true;
	index 				= 0;
	key 				= 0;
	type 				= 0;
	HIGHLIGHT_INDEX		= -1;
	leftclick			= false;
	applyMouse[0]		= false;
	applyMouse[1]		= false;
	confirmMouse[0]		= false;
	confirmMouse[1]		= false;
	confirm				= false;
	confirmKey			= false;
	A					= false;
	LAnalogTrigger		= false;
	RAnalogTrigger		= false;
	focusedOther		= false;
	//timer				= revertSettingsTimer;
	timer				= revertSettingsTimer;
	indexX				= 0;
	indexOther = 0;
	//frame 				= 0;
	//SDL_ShowCursor(true);

	// Load Bar values
	bar[0].value 	= MASTER_VOL;
	bar[1].value 	= MUSIC_VOL;
	bar[2].value 	= SFX_VOL;

	// Load Video values
	bar[3].value 	= RESOLUTION;
	bar[4].value 	= ANTI_ALIAS;
	bar[5].value 	= VSYNC;
	bar[6].value 	= FULLSCREEN;

	while (pauseLoop) {

		// Start FPS cap
		helper.fps.start();

		// Start FPS cap
		// fps.start();
		// Main x and y position of Options menu options
		for (int i=0; i<5; i++) {
			title[i].x = helper.screenWidth * 0.05;
			//title[i].y = helper.screenHeight/2-14/2-130+i*29;
			title[i].y = 80+i*menuTitleNamesMarginsH;
		}

		// Apply Audio button
		applyButton[0].w = barWidth;
		applyButton[0].h = barHeight/2;
		applyButton[0].x = helper.screenWidth * 0.80 - applyButton[0].w;				// Set x pos to 20% of the screen minus applybutton width
		//applyButton[0].y = helper.screenHeight-applyButton[0].h*2 - 3 - 2;
		applyButton[0].y = helper.screenHeight * 0.80 - applyButton[0].h - barHeight;	// Set y pos to 80% of the screen minus applybutton height

		// Apply Video button
		applyButton[1].w = barWidth;
		applyButton[1].h = barHeight/2;
		applyButton[1].x = helper.screenWidth * 0.80 - applyButton[0].w;
		applyButton[1].y = helper.screenHeight * 0.80 - applyButton[1].h;

		while (SDL_PollEvent(&e) != 0) {

			if (e.type == SDL_QUIT) {
				optionsResult	= Exit;
				pauseLoop 	= false;
				//gameLoop 	= false;
				//selection 	= -1;
				gCursor.free();
			}

			// switch key if controller moved
			if (e.type == SDL_JOYAXISMOTION) {
				key = 0;
			}
			// Controller button down
			if (e.type == SDL_JOYBUTTONDOWN){
				key = 0;
			}

			//Handle window events
			gWindow.handleEvent(gRenderer, e);

			if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
				key = 0;
				OnKeyDown(e.key.keysym.sym);
			}
			//If Key Released
			else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
				case SDLK_UP:
					//
					break;
				case SDLK_DOWN:
					//
					break;
				}
				OnKeyUp(e.key.keysym.sym);
			}

			// Mouse clicked
			if (e.type==SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					leftclick = true;
					mousePressed();
				}
			}
			if (e.type==SDL_MOUSEBUTTONUP) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					leftclick = false;

					mouseReleased(gWindow);
				}
			}

			// Mouse motion?
			if (e.type == SDL_MOUSEMOTION) {
				key = 1;
			}
			// Get title index from keyboard or xbox controller
			updateJoystick(gRenderer, gWindow, &e);
		}

		// Get mouse coordinates
		SDL_GetMouseState(&mx, &my);

		/* get render width and height
		 * but (mostly used to get actual render height)
		 */
		int renderW = 0;
		int renderHDummey = 0;
		SDL_GetRendererOutputSize(gRenderer,&renderW,&renderHDummey);
		int en = renderW * 0.4375;
		int renderH = renderW - en;

		// Grid like coordinates of mouse
		// get new mouse coordinates based on render size, and actual screen size
		mex = (helper.screenWidth*mx)/gWindow.getWidth();				// New mouse coordinates, no relation to camera
		mey = (helper.screenHeight*my)/gWindow.getHeight();				// New mouse coordinates, no relation to camera


		// Update
		int randomSpacingFromCenter = 40;
		//int yPositionForAudioVideoSliders = 40;
		au.update(bar,
					//helper.screenWidth/2-bar[0].rect.w/2 + randomSpacingFromCenter,
					helper.screenWidth * 0.32,		// x position will be 32% from the left side of the screen
					80,
					mex, mey, leftclick,
					HIGHLIGHT_INDEX);

		for (int i=0; i<5; i++) {
			if (helper.checkCollision(mex, mey, 1, 1, title[i].x-3, title[i].y-3, title[i].w+3, title[i].h+3)) {
				mouseTitle[i] = true;
				index=i;
			}else{
				mouseTitle[i] = false;
			}
		}

		/* Mouse on Apply Bar or Video button */
		for (int i=0; i<5; i++) {
			if (helper.checkCollision(mex, mey, 1, 1, applyButton[i].x, applyButton[i].y, applyButton[i].w, applyButton[i].h)) {
				applyMouse[i] = true; }else{ applyMouse[i] = false;
			}
		}

		// Generate Stars
		//part.genStars(particle, helper.screenWidth, helper.screenHeight);

		// Update Particles
		//part.updateStarParticles(particle, 0, 0, helper.screenWidth, helper.screenHeight);

		// if confirm button appeared, change max to 3 so we can go to the keep/revert buttons
		if (confirm) {
			maxIndexX = 3;
		}else {
			maxIndexX = 1;
		}

		// Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			// Render everything
			RenderII(gWindow, gRenderer);

			// Render mouse location
			gCursor.render(gRenderer, mex, mey, 20, 20);

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		helper.frame++;
		if((helper.cap == true ) && (helper.fps.get_ticks() < 1000 / helper.FRAMES_PER_SECOND ))
			SDL_Delay((1000 / helper.FRAMES_PER_SECOND ) - helper.fps.get_ticks());

		// fps
		/*frame++;
		if (cap && fps.get_ticks() < 1000 / FRAMES_PER_SECOND) {
			SDL_Delay((1000/FRAMES_PER_SECOND ) - fps.get_ticks());
		}*/
	}
	gCursor.free();
}

/*
//Get's input from user and returns it
void Options::SaveLevel(LWindow &gWindow, SDL_Renderer *gRenderer, bool &mainLoop,
						std::string SpawnCoordinatesData,
						std::string TileSaveData,
						std::string ItemSaveData,
						std::string MonsterSaveData){

	// Loop bool
	bool getInputSave = true;

	// Store user input in string
	std::string inputDirectory = "file name...";

	// What you are currently typing
	std::string typing = "nothing";
	int typingAnim = 0;
	std::string slash = "";

	//Start getting input from user
	SDL_StartTextInput();

	//Mouse properties
	int mex, mey;

	//input text box
	SDL_Rect customRect = {0,0,0,0};
	SDL_Event e;

	while (getInputSave) {

		// Start FPS cap
		helper.fps.start();

		//Mouse Position
		SDL_GetMouseState(&mex, &mey);

		// get new mouse coordinates based on render size, and actual screen size
		int renderW = 0;
		int renderHDummey = 0;
		SDL_GetRendererOutputSize(gRenderer,&renderW,&renderHDummey);
		int en = renderW * 0.4375;
		int renderH = renderW - en;
		mex = (helper.screenWidth*mex)/renderW;	// New mouse coordinates, no relation to camera
		mey = (helper.screenHeight*mey)/renderH;	// New mouse coordinates, no relation to camera

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				getInputSave = false;
				optionsResult	= Exit;
			}

			if (typing == "filename"){
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_BACKSPACE&& inputDirectory.length() > 0) {
						inputDirectory.pop_back();
					}
					else if (e.key.keysym.sym== SDLK_c&& SDL_GetModState() & KMOD_CTRL) {
						SDL_SetClipboardText(inputDirectory.c_str());
					}
					else if (e.key.keysym.sym== SDLK_v&& SDL_GetModState() & KMOD_CTRL) {
						inputDirectory = SDL_GetClipboardText();
					}
				}
				else if (e.type == SDL_TEXTINPUT) {
					if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C')&& (e.text.text[0] == 'v' || e.text.text[0] == 'V')&& SDL_GetModState() & KMOD_CTRL)) {
						inputDirectory += e.text.text;
					}
				}
			}
			//Handle window events
			gWindow.handleEvent(gRenderer, e);

			//If Key Pressed
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					getInputSave = false;			//quit current loop
					break;
				case SDLK_RETURN:
					std::stringstream defaultDir;
					defaultDir << "resource/data/maps/";
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//----------------------------- Save Spawn Point ----------------------------//
					//Save # of Blocks
					std::ofstream spawnPointFile;
					std::stringstream PathDir;
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "spawn.mp";
					// Open File
					spawnPointFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					spawnPointFile << SpawnCoordinatesData;
					// Close
					spawnPointFile.close();
					//----------------------------- Save Spawn Point ----------------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//------------------------------ Save Tile Data -----------------------------//
					//Save # of Blocks
					std::ofstream newTileFile;
					PathDir.str(std::string());
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "Tile.mp";
					// Open File
					newTileFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					newTileFile << TileSaveData;
					// Close
					newTileFile.close();
					//------------------------------ Save Tile Data -----------------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//-------------------------- Save Collision Tile Data -----------------------//
					///Save # of Blocks
					std::ofstream newTileCFile;
					PathDir.str(std::string());
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "TileC.mp";
					// Open File
					newTileCFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					newTileCFile << CollisionTileSaveData;
					// Close
					newTileCFile.close();
					//-------------------------- Save Collision Tile Data -----------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//------------------------------ Save Object Data ---------------------------//
					//Save # of Blocks
					std::ofstream newObjectFile;
					PathDir.str(std::string());
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "Item.mp";
					// Open File
					newObjectFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					newObjectFile << ItemSaveData;
					// Close
					newObjectFile.close();
					//------------------------------ Save Object Data ---------------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					//----------------------------- Save Monster Data ---------------------------//
					//Save # of Blocks
					std::ofstream newMonsterFile;
					PathDir.str(std::string());
					// Set directory to save
					PathDir << defaultDir.str().c_str();
					// Set file name and extension
					PathDir << inputDirectory.c_str() << "Monster.mp";
					// Open File
					newMonsterFile.open(PathDir.str().c_str());
					// Store data given from Editor to store in File
					newMonsterFile << MonsterSaveData;
					// Close
					newMonsterFile.close();
					//----------------------------- Save Monster Data ---------------------------//
					///////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////
					getInputSave = false;
				}
			}

			//Left mouse click
			if (e.type == SDL_MOUSEBUTTONUP) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					//
				}
				if ( helper.checkCollision(mex, mey, 1, 1, customRect.x,customRect.y, customRect.w, customRect.h)  ){
					if (typing != "filename"){
						typingAnim = 30;
						typing = "filename";
					}
				} else{
					typingAnim = 0;
					typing = "nothing";
				}
			}
		}

		//Typing timer animation
		if (typing == "filename"){
			typingAnim += 1;
			if (typingAnim >= 0 && typingAnim < 60) {
				slash = "_";
			}
			if (typingAnim >= 60 && typingAnim <= 120) {
				slash = " ";
			}
			if (typingAnim > 120) {
				typingAnim = 0;
			}
		}
		//Add the slash animation only when the player is typing the file name
		std::stringstream tempSS;
		if (typing == "filename") {
			tempSS << inputDirectory.c_str() << slash;
		} else {
			tempSS << inputDirectory.c_str();
		}

		//Clear screen for render
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 100);
		SDL_RenderClear(gRenderer);

			//Render text tip
			gText.loadFromRenderedText(gRenderer, "Enter level directory (ending with a '/'):", {255,255,255}, gFont13);
			int newWidth = gText.getWidth()/2;
			int newHeight = gText.getHeight()/2;
			gText.render(gRenderer, 5, 5, newWidth, newHeight);

			//Render text input
			gText.loadFromRenderedText(gRenderer, tempSS.str().c_str(), {0,255,0}, gFont13);
			customRect.x = 10;
			customRect.y = 15;
			newWidth = gText.getWidth()/2;
			newHeight = gText.getHeight()/2;
			gText.render(gRenderer, customRect.x+4, customRect.y +newHeight - 2, newWidth, newHeight);
			customRect.w = newWidth+8;
			customRect.h = newHeight * 2;
			if ( helper.checkCollision(mex, mey, 1, 1, customRect.x,customRect.y, customRect.w, customRect.h)  ){
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &customRect);
			}else{
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &customRect);
			}

		//Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		helper.frame++;
		if((helper.cap == true ) && (helper.fps.get_ticks() < 1000 / helper.FRAMES_PER_SECOND ))
			SDL_Delay((1000 / helper.FRAMES_PER_SECOND ) - helper.fps.get_ticks());
	}
}*/

std::string Options::GetInput(LWindow &gWindow, SDL_Renderer *gRenderer, bool &mainLoop, std::string hint){

	// Loop bool
	bool getInputSave = true;

	// Store hint hear to give the Editor an idea of what they are typing
	std::string textField = "128 128";

	// What you are currently typing
	std::string typing = "nothing";
	int typingAnim = 0;
	std::string slash = "";

	//Start getting input from user
	SDL_StartTextInput();

	//Mouse properties
	int mex, mey;

	//input text box
	SDL_Rect customRect = {0,0,0,0};
	SDL_Event e;

	while (getInputSave) {

		// Start FPS cap
		helper.fps.start();

		//Mouse Position
		SDL_GetMouseState(&mex, &mey);

		// get new mouse coordinates based on render size, and actual screen size
		int renderW = 0;
		int renderHDummey = 0;
		SDL_GetRendererOutputSize(gRenderer,&renderW,&renderHDummey);
		int en = renderW * 0.4375;
		int renderH = renderW - en;
		mex = (helper.screenWidth*mex)/renderW;	// New mouse coordinates, no relation to camera
		mey = (helper.screenHeight*mey)/renderH;	// New mouse coordinates, no relation to camera

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				getInputSave = false;
				optionsResult	= Exit;
			}

			if (typing == "inputBox"){
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_BACKSPACE&& textField.length() > 0) {
						textField.pop_back();
					}
					else if (e.key.keysym.sym== SDLK_c&& SDL_GetModState() & KMOD_CTRL) {
						SDL_SetClipboardText(textField.c_str());
					}
					else if (e.key.keysym.sym== SDLK_v&& SDL_GetModState() & KMOD_CTRL) {
						textField = SDL_GetClipboardText();
					}
				}
				else if (e.type == SDL_TEXTINPUT) {
					if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C')&& (e.text.text[0] == 'v' || e.text.text[0] == 'V')&& SDL_GetModState() & KMOD_CTRL)) {
						textField += e.text.text;
					}
				}
			}
			//Handle window events
			gWindow.handleEvent(gRenderer, e);

			//If Key Pressed
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					getInputSave = false;			//quit current loop
					break;
				case SDLK_RETURN:
					getInputSave = false;
				}
			}

			//Left mouse click
			if (e.type == SDL_MOUSEBUTTONUP) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					//
				}
				if ( helper.checkCollision(mex, mey, 1, 1, customRect.x,customRect.y, customRect.w, customRect.h)  ){
					if (typing != "inputBox"){
						typingAnim = 30;
						typing = "inputBox";
					}
				} else{
					typingAnim = 0;
					typing = "nothing";
				}
			}
		}

		//Typing timer animation
		if (typing == "inputBox"){
			typingAnim += 1;
			if (typingAnim >= 0 && typingAnim < 60) {
				slash = "_";
			}
			if (typingAnim >= 60 && typingAnim <= 120) {
				slash = " ";
			}
			if (typingAnim > 120) {
				typingAnim = 0;
			}
		}
		//Add the slash animation only when the player is typing the file name
		std::stringstream tempSS;
		if (typing == "inputBox") {
			tempSS << textField.c_str() << slash;
		} else {
			tempSS << textField.c_str();
		}

		//Clear screen for render
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 100);
		SDL_RenderClear(gRenderer);

			//Render text tip
			gText.loadFromRenderedText(gRenderer, hint.c_str(), {255,255,255}, gFont13);
			gText.render(gRenderer, 5, 5,gText.getWidth(), gText.getHeight());

			//Render text input
			gText.loadFromRenderedText(gRenderer, tempSS.str().c_str(), {0,255,0}, gFont13);
			customRect.x = 10;
			customRect.y = 20;
			gText.render(gRenderer, customRect.x+4, customRect.y,gText.getWidth(), gText.getHeight());
			customRect.w = gText.getWidth()+8;
			customRect.h = gText.getHeight();
			if ( helper.checkCollision(mex, mey, 1, 1, customRect.x,customRect.y, customRect.w, customRect.h)  ){
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &customRect);
			}else{
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(gRenderer, &customRect);
			}

		//Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		helper.frame++;
		if((helper.cap == true ) && (helper.fps.get_ticks() < 1000 / helper.FRAMES_PER_SECOND ))
			SDL_Delay((1000 / helper.FRAMES_PER_SECOND ) - helper.fps.get_ticks());
	}
	return textField.c_str();
}

// Key Pressed
void Options::OnKeyDown(SDL_Keycode sym) {
	switch (sym) {
	case SDLK_RETURN:
		confirmKey = true;
		if (type == 2) {
			if (focusedOther) {					// if we are currently focused on a Bar item
				focusedOther = false;			// stop focus
			}

			// if we are not focused on a Bar item
			else {
				// If we are in the Bar selecting index (indexX == 1), focus on that bar
				if (indexX == 1) {
					focusedOther = true;
				}
			}
		}
		break;
	case SDLK_LEFT:
		if (type == 2) {
			if (!focusedOther) {
				if (indexX > 0) {
					indexX -= 1;				// change indexX
				}
			}
			minusValues();						// change values in type==2 (volume sliders/resolutions
		}
		break;
	case SDLK_RIGHT:
		if (type == 2) {
			if (!focusedOther) {
				if (indexX < maxIndexX) {
					indexX += 1;				// change indexX
				}
			}
			addValues();						// change values in type==2 (volume sliders/resolutions
		}
		break;
	case SDLK_a:
		if (type == 2) {
			if (!focusedOther) {
				if (indexX > 0) {
					indexX -= 1;				// change indexX
				}
			}
			minusValues();						// change values in type==2 (volume sliders/resolutions
		}
		break;
	case SDLK_d:
		if (type == 2) {
			if (!focusedOther) {
				if (indexX < maxIndexX) {
					indexX += 1;				// change indexX
				}
			}
			addValues();						// change values in type==2 (volume sliders/resolutions
		}
		break;
	case SDLK_UP:
		if (indexX == 0) {
			if (index > 0) {
				index -= 1;
			}
		}else if (indexX == 1 && !focusedOther) {
			if (indexOther > 0) {
				indexOther -= 1;
			}
		}
		break;
	case SDLK_DOWN:
		if (indexX == 0) {
			int typeShown;
			if (type==0) {
				typeShown=4;
			}else if (type==2) {
				typeShown=2;
			}
			if (index < typeShown) {
				index += 1;
			}
		}else if (indexX == 1 && !focusedOther) {
			if (indexOther < maxIndexOther) {
				indexOther += 1;
			}
		}
		break;;
	case SDLK_w:
		if (indexX == 0) {
			if (index > 0) {
				index -= 1;
			}
		}else if (indexX == 1 && !focusedOther) {
			if (indexOther > 0) {
				indexOther -= 1;
			}
		}
		break;
	case SDLK_s:
		if (indexX == 0) {
			int typeShown;
			if (type==0) {
				typeShown=4;
			}else if (type==2) {
				typeShown=2;
			}
			if (index < typeShown) {
				index += 1;
			}
		}else if (indexX == 1 && !focusedOther) {
			if (indexOther < maxIndexOther) {
				indexOther += 1;
			}
		}
		break;
	}
}

void Options::OnKeyUp(SDL_Keycode sym) {
	switch (sym) {
	case SDLK_RETURN:
		confirmKey = false;
		if (indexX == 0) {
			// PauseMenu
			if (type==0) {
				if (index==0) {
					pauseLoop 		= false;
				}
				// Enter Settings
				else if (index==2) {
					// Change type
					type = 2;

					// Change options
					title[0].name = "AUDIO";
					title[1].name = "VIDEO";
					title[2].name = "BACK";
					/*title[2].name = "CONTROLLER";
					title[3].name = "MOUSE AND KEYBOARD";
					title[4].name = "BACK";*/
					HIGHLIGHT_INDEX = -1;
				}
				// Exit To Main Menu
				else if (index==3) {
					optionsResult	= Back;
					pauseLoop 		= false;
					//gameLoop 		= false;
					//selection 	= 0;
				}
				// Exit To Desktop
				else if (index==4) {
					optionsResult	= Exit;
					pauseLoop 		= false;
					//gameLoop 	= false;
					//selection = -1;
				}
			}
			// SubMenu
			else if (type==2) {
				if (index==0) {
					HIGHLIGHT_INDEX = 0;
				}else if (index==1) {
					HIGHLIGHT_INDEX = 1;
				}else if (index==2) {
					// Change type
					type = 0;

					// Change options
					title[0].name = "RESUME";
					title[1].name = "TBA";
					title[2].name = "SETTINGS";
					title[3].name = "EXIT MAIN MENU";
					title[4].name = "EXIT TO DESKTOP";
					HIGHLIGHT_INDEX = -1;
				}
			}
		}
		break;
	}
}

// Mouse Pressed
void Options::mousePressed() {
}

// Update buttons
void Options::mouseReleased(LWindow &gWindow) {
	for (int i=0; i<5; i++) {
		if (helper.checkCollision(mex, mey, 1, 1, title[i].x-3, title[i].y-3, title[i].w+3, title[i].h+3)) {
			// PauseMenu controls
			if (type==0) {
				// Resume Game
				if (index==0) {
					pauseLoop 		= false;
				}
				// Go into 'Settings'
				else if (index==2) {
					// Change type
					type = 2;
					// Change options
					title[0].name = "AUDIO";
					title[1].name = "VIDEO";
					title[2].name = "BACK";
					/*title[2].name = "CONTROLLER";
					title[3].name = "MOUSE AND KEYBOARD";
					title[4].name = "BACK";*/
					HIGHLIGHT_INDEX = -1;
				}
				// Exit To Main Menu
				else if (index==3) {
					optionsResult	= Back;
					pauseLoop 		= false;
					//gameLoop 		= false;
					//selection 	= 0;
				}
				// Exit To Desktop
				else if (index==4) {
					optionsResult	= Exit;
					pauseLoop 		= false;
					//gameLoop 	= false;
					//selection = -1;
				}
			// Settings controls
			}else if (type==2) {
				if (index==0) {
					HIGHLIGHT_INDEX = 0;
				}else if (index==1) {
					HIGHLIGHT_INDEX = 1;
				}else if (index==2) {
					HIGHLIGHT_INDEX = 2;
					// Change type
					type = 0;
					// Change options
					title[0].name = "RESUME";
					title[1].name = "TBA";
					title[2].name = "SETTINGS";
					title[3].name = "EXIT MAIN MENU";
					title[4].name = "EXIT TO DESKTOP";
					HIGHLIGHT_INDEX = -1;
				}
			}
		}
	}

	/* Mouse on Keep or Revert button */
	for (int i=0; i<2; i++) {
		if (helper.checkCollision(mex, mey, 1, 1, confirmButton[i].x, confirmButton[i].y, confirmButton[i].w, confirmButton[i].h)) {
			confirmMouse[i] = true;
		}
		else{
			confirmMouse[i] = false;
		}
	}

	/* Apply Audio Button */
	if (helper.checkCollision(mex, mey, 1, 1, applyButton[0].x, applyButton[0].y, applyButton[0].w, applyButton[0].h)) {
		actionApplyAudio();
	}

	/* Apply Video Button */
	if (helper.checkCollision(mex, mey, 1, 1, applyButton[1].x, applyButton[1].y, applyButton[1].w, applyButton[1].h)) {
		actionApplyVideo(gWindow);
	}

	/* Keep Button */
	if (helper.checkCollision(mex, mey, 1, 1, confirmButton[0].x, confirmButton[0].y, confirmButton[0].w, confirmButton[0].h)) {
		actionKeep();
	}

	/* Revert Button */
	if (helper.checkCollision(mex, mey, 1, 1, confirmButton[1].x, confirmButton[1].y, confirmButton[1].w, confirmButton[1].h)) {
		actionRevert(gWindow);
	}
}

void Options::RenderII(LWindow &gWindow, SDL_Renderer *gRenderer) {

	// Render particles (some particles are used as actual bullets)
	//part.renderStarParticle(particle, (60*distanceW)/500, (60*distanceH)/500, 1, gRenderer);

	// Welcome text
	std::stringstream tempss;
	tempss << "index: " << index << ", indexX: " << indexX << ", indexOther: " << indexOther << ", focusedOther: " << focusedOther;
	gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
		int newWidth = gText.getWidth();
		int newHeight = gText.getHeight();
	gText.render(gRenderer, helper.screenWidth-newWidth, 0, newWidth, newHeight);

	// Render Game items
	int typeShown;
	if (type==0) {
		typeShown = 5;
	}if (type==2) {
		typeShown = 3;
	}
	for (int i=0; i<typeShown; i++) {
		if (key==0)
		{
			// Get PauseMenu item name
			std::stringstream tempss;
			tempss << title[i].name;

			// Set index if selection if over a title item
			if (((index == i && confirmKey) || (index == i && A)) && indexX == 0) {
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {0,200,0}, gFont);
			}else if (index == i && indexX == 0) {
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {244,144,20}, gFont);
			}
			else{
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
			}

			// Render text
			title[i].w = gText.getWidth();
			title[i].h = gText.getHeight();
			gText.render(gRenderer, title[i].x,title[i].y, title[i].w, title[i].h);
		}else if (key==1) {

			// Get PauseMenu item name
			std::stringstream tempss;
			tempss << title[i].name;
			// Set index if mouse if over a title item
			if (mouseTitle[i] && leftclick) {
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {0,200,0}, gFont);
			}else if (mouseTitle[i]) {
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {244,144,20}, gFont);
			}
			else{
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
			}

			// Render text
			title[i].w = gText.getWidth();
			title[i].h = gText.getHeight();
			gText.render(gRenderer, title[i].x,title[i].y, title[i].w, title[i].h);
		}
	}

	// Render audio options
	if (type==2) {
		for (int i=0; i<barMax; i++) {
			// Draw bg
			SDL_Rect tempr {bar[i].rect.x, bar[i].rect.y,
				 bar[i].rect.w, bar[i].rect.h};
			SDL_SetRenderDrawColor(gRenderer, 25, 25, 25, 155);
			SDL_RenderFillRect(gRenderer, &tempr);

			////////////////////////////////////////////////////////////////////////////////////////
			//-------------------------------------- Volume Sliders ------------------------------//
			if (i==0) {
				// Render Volume sliders
				SDL_Rect tempr {bar[i].rect.x, bar[i].rect.y, bar[i].rect.w*(bar[i].value)/100, bar[i].rect.h};
				SDL_SetRenderDrawColor(gRenderer, 55, 55, 55, 155);
				SDL_RenderFillRect(gRenderer, &tempr);

				// Render Volume number
				tempss.str(std::string());
				tempss << bar[i].value;
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
				int newWidth = gText.getWidth();
				int newHeight = gText.getHeight();
				gText.render(gRenderer, bar[i].rect.x+bar[i].rect.w+2,
											 bar[i].rect.y+bar[i].rect.h/2-newHeight/2,
											 newWidth, newHeight);
			}
			if (i>=1 && i<3) {
				// Render Volume sliders
				SDL_Rect tempr {bar[i].rect.x, bar[i].rect.y,
					bar[i].rect.w*(bar[i].value)/128, bar[i].rect.h};
				SDL_SetRenderDrawColor(gRenderer, 55, 55, 55, 155);
				SDL_RenderFillRect(gRenderer, &tempr);

				// Render Volume number
				tempss.str(std::string());
				tempss << bar[i].value;
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
				int newWidth = gText.getWidth();
				int newHeight = gText.getHeight();
				gText.render(gRenderer, bar[i].rect.x+bar[i].rect.w+2,
											 bar[i].rect.y+bar[i].rect.h/2-newHeight/2,
											 newWidth, newHeight);
			}

			// Render PauseMenu text
			std::stringstream tempss;
			tempss.str(std::string());
			tempss << bar[i].name;
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);
			int newWidth = gText.getWidth();
			int newHeight = gText.getHeight();
			gText.render(gRenderer, bar[i].rect.x-newWidth-10,
										 bar[i].rect.y+bar[i].rect.h/2-newHeight/2,
										 newWidth, newHeight);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////// Resolution Bar /////////////////////////////////////////////////
		//-----------------------------------------------------------------------------------------------------------------//
		// Actual value bar
		std::stringstream tempsss;
		int mAdd;
		if (bar[3].value==5) {
			tempsss << "1920x1080";
			mAdd = 4;
		}else if (bar[3].value==4) {
			tempsss << "1600x900";
			mAdd = 3;
		}else if (bar[3].value==3) {
			tempsss << "1336x768";
			mAdd = 2;
		}else if (bar[3].value==2) {
			tempsss << "1280x720";
			mAdd = 1;
		}else if (bar[3].value==1) {
			tempsss << "1024x576";
			mAdd = 0;
		}

		// Resolution Background BG
		SDL_Rect tempr {bar[3].rect.x+(bar[3].rect.w)*mAdd, bar[3].rect.y, bar[3].rect.w, bar[3].rect.h};
		SDL_SetRenderDrawColor(gRenderer, 55, 55, 55, 155);
		SDL_RenderFillRect(gRenderer, &tempr);
		// Render box in current resolution selection
		gText.loadFromRenderedText(gRenderer, tempsss.str().c_str(), {255,255,255}, gFont);
		int individualBoxWidth = bar[0].rect.w;
		//gText.render(gRenderer,  ((bar[3].rect.x+individualBoxWidth*4)+individualBoxWidth) - individualBoxWidth/2 - gText.getWidth()/2,
		//							 bar[3].rect.y+bar[3].rect.h/2-gText.getHeight()/2,
		//							 gText.getWidth(), gText.getHeight());
		int newWidth = gText.getWidth();
		int newHeight = gText.getHeight();
		gText.render(gRenderer,  bar[3].rect.x+bar[3].rect.w + 2,
									 bar[3].rect.y+bar[3].rect.h/2-newHeight/2,
									 newWidth, newHeight);
		// Resolution selection borders (5 selection)
		for (int i=0; i<5; i++) {
			SDL_Rect tempr {bar[3].rect.x+i*(bar[0].rect.w), bar[3].rect.y, bar[3].rect.w, bar[3].rect.h};
			SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 155);
			SDL_RenderDrawRect(gRenderer, &tempr);
		}
		//-----------------------------------------------------------------------------------------------------------------//
		//////////////////////////////////////////////////// Resolution Bar /////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/* FULLSCREEN & VSYNC BAR */
		// Actual value bar
		for (int j=4; j<barMax; j++) {
			std::stringstream ssF;
			if (bar[j].value==1) {
				ssF << "On";
				// Render if it is selected
				SDL_Rect tempr {bar[j].rect.x+bar[j].rect.w/2, bar[j].rect.y, bar[j].rect.w/2, bar[j].rect.h};
				SDL_SetRenderDrawColor(gRenderer, 55, 55, 55, 155);
				SDL_RenderFillRect(gRenderer, &tempr);
			}else if (bar[j].value==0) {
				ssF << "Off";
				SDL_Rect tempr {bar[j].rect.x, bar[j].rect.y, bar[j].rect.w/2, bar[j].rect.h};
				SDL_SetRenderDrawColor(gRenderer, 55, 55, 55, 155);
				SDL_RenderFillRect(gRenderer, &tempr);
			}
			gText.loadFromRenderedText(gRenderer, ssF.str().c_str(), {255,255,255}, gFont);
			int newWidth = gText.getWidth();
			int newHeight = gText.getHeight();
			gText.render(gRenderer, bar[j].rect.x+bar[j].rect.w + 2,
										 bar[j].rect.y+bar[j].rect.h/2-newHeight/2,
										 newWidth, newHeight);

			// Border
			/*for (int i=0; i<2; i++) {
				SDL_Rect tempr {bar[j].rect.x+i*(bar[0].rect.w/2), bar[j].rect.y, (bar[0].rect.w/2), bar[j].rect.h};
				SDL_SetRenderDrawColor(gRenderer, 99, 99, 99, 155);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}*/
		}

		// Applies to all PauseMenu text
		for (int i=0; i<barMax; i++) {
			// PauseMenu option highlighted
			if (bar[i].highlight) {
				SDL_Rect tempr {bar[i].rect.x-2, bar[i].rect.y, 2, bar[i].rect.h};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 155);
				SDL_RenderFillRect(gRenderer, &tempr);
			}

			// If we are using a controller, or a keyboard for selection
			if (key == 0) {
				if (indexX == 1) {
					if (indexOther == i) {
						if (focusedOther) {
							SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 155);
							SDL_RenderDrawRect(gRenderer, &bar[i].rect);
						}else{
							SDL_SetRenderDrawColor(gRenderer, 255, 144, 20, 155);
							SDL_RenderDrawRect(gRenderer, &bar[i].rect);
						}
					}
				}
			}
			// Using mouse for selection
			else{
				// Mouse on PauseMenu text
				if (bar[i].mouse) {
					SDL_SetRenderDrawColor(gRenderer, 255, 144, 20, 155);
					SDL_RenderDrawRect(gRenderer, &bar[i].rect);
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////// Render Apply Audio/Video Buttons //////////////////////////////////////////////
		//----------------------------------------------------------------------------------------------------------------------------//
		for (int i=0; i<2; i++) {
			SDL_Rect temprV {applyButton[i].x, applyButton[i].y, applyButton[i].w, applyButton[i].h};
			// Hover
			if ( ((applyMouse[i] || (indexOther-7) == i) && A) && indexX == 1 ) {
				SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 155);
			}
			// Hover
			else if ((applyMouse[i] || (indexOther-7) == i) && indexX == 1 ) {
				SDL_SetRenderDrawColor(gRenderer, 244, 144, 20, 155);
			}

			// No Hover
			else if (!applyMouse[i]) {
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 155);
			}
			SDL_RenderDrawRect(gRenderer, &temprV);
			if (i==0) {
				gText.loadFromRenderedText(gRenderer, "APPLY AUDIO", {255,255,255}, gFont);
			}
			if (i==1) {
				gText.loadFromRenderedText(gRenderer, "APPLY VIDEO", {255,255,255}, gFont);
			}
			int newWidth = gText.getWidth();
			int newHeight = gText.getHeight();
			gText.render(gRenderer, applyButton[i].x+applyButton[i].w/2-newWidth/2,
					applyButton[i].y+applyButton[i].h/2-newHeight/2,
					newWidth, newHeight);
		}
		//----------------------------------------------------------------------------------------------------------------------------//
		//////////////////////////////////////////////// Render Apply Audio/Video Buttons //////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////// Render Keep/Revert Buttons /////////////////////////////////////////////////
		//----------------------------------------------------------------------------------------------------------------------------//
		//
		if (confirm) {

			int confirmButtonWidth = 160;
			// Keep button
			confirmButton[0].w = confirmButtonWidth;
			confirmButton[0].h = barHeight/2;
			confirmButton[0].x = helper.screenWidth - confirmButtonWidth*2 - 40;
			confirmButton[0].y = helper.screenHeight - barHeight;

			// Revert button
			confirmButton[1].w = confirmButtonWidth;
			confirmButton[1].h = barHeight/2;
			confirmButton[1].x = helper.screenWidth - confirmButtonWidth - 40;
			confirmButton[1].y = helper.screenHeight - barHeight;

			/* Mouse on Keep or Revert button */
			for (int i=0; i<2; i++) {
				if (helper.checkCollision(mex, mey, 1, 1, confirmButton[i].x, confirmButton[i].y, confirmButton[i].w, confirmButton[i].h)) {
					confirmMouse[i] = true;
				}
				else{
					confirmMouse[i] = false;
				}
			}

			// Start timer
			timer -= 1;

			/* Player response time out */
			if (timer <= 0) {
				timer = revertSettingsTimer;
				confirm = false;

				// Revert to old Bar Settings
				if (button=="a") {
					applyOldAudioCFG();
					bar[0].value	= MASTER_VOL;
					bar[1].value	= MUSIC_VOL;
					bar[2].value	= SFX_VOL;
				}

				// Revert to old Video Settings
				if (button=="v") {
					bar[3].value	= RESOLUTION;
					bar[4].value	= ANTI_ALIAS;
					bar[5].value	= VSYNC;
					bar[6].value	= FULLSCREEN;
					gWindow.applySettings(RESOLUTION, ANTI_ALIAS, VSYNC, FULLSCREEN);
				}
			}
			// Render confirmation text
			std::stringstream tempss;
			tempss << "Reverting settings in " << timer/60 << ".";
			gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont);

			// Set text width and height
			int newWidth = gText.getWidth();
			int newHeight = gText.getHeight();

			// Render text
			gText.render(gRenderer, confirmButton[0].x,
									confirmButton[0].y-newHeight,
									newWidth, newHeight);

			//gText.render(gRenderer, helper.screenWidth-newWidth,
			//						helper.screenHeight-newHeight - barHeight,
			//						newWidth, newHeight);

			/* Keep & Revert button */
			for (int i=0; i<2; i++) {

				// Render buttons, if using Controller
				if (key == 0) {
					//std::cout << "key0\n";
					if (indexX == 2 && i == 0) {
						SDL_SetRenderDrawColor(gRenderer, 244, 144, 20, 155);
						/*if (!confirmKey) {
							SDL_SetRenderDrawColor(gRenderer, 244, 144, 20, 155);
						}else{
						SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 155);
						}*/
					}
					else if (indexX == 3 && i == 1) {
						SDL_SetRenderDrawColor(gRenderer, 244, 144, 20, 155);
						/*if (!confirmKey) {
							SDL_SetRenderDrawColor(gRenderer, 244, 144, 20, 155);
						}else{
							SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 155);
						}*/
					}else{
						SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 155);
					}


					// Render Keep and Revert buttons as White, no selection
					SDL_RenderDrawRect(gRenderer, &confirmButton[i]);
					if (i==0) {
						gText.loadFromRenderedText(gRenderer, "Keep", {255,255,255}, gFont);
					}else{
						gText.loadFromRenderedText(gRenderer, "Revert", {255,255,255}, gFont);
					}
					int newWidth = gText.getWidth();
					int newHeight = gText.getHeight();
					gText.render(gRenderer, confirmButton[i].x+confirmButton[i].w/2-newWidth/2,
												 confirmButton[i].y+confirmButton[i].h/2-newHeight/2,
												 newWidth, newHeight);
				}

				// Render buttons, if using Mouse & Keyboard
				else if (key == 1) {

					// If mouse is hovering over Keep or Revert button
					if (confirmMouse[i]) {

						// Set box border color to green
						SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 155);


						// If left mouse button held down
						if (leftclick) {

							// Set box border color to darker green
							SDL_SetRenderDrawColor(gRenderer, 0, 125, 0, 155);
						}
					}

					// If mouse is NOT hovering over Keep or Revert button
					else if (!confirmMouse[i]) {

						// Set box border color to white
						SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 155);
					}

					// Render box around Keep and Revert button
					SDL_RenderDrawRect(gRenderer, &confirmButton[i]);

					// If for loop is on index "0", which is the Keep button
					if (i==0)
					{
						// Set text field as "Keep"
						gText.loadFromRenderedText(gRenderer, "Keep", {255,255,255}, gFont);
					}

					// If for loop is on index "1", which is the Revert buttone
					else
					{
						// Set text field as "Revert"
						gText.loadFromRenderedText(gRenderer, "Revert", {255,255,255}, gFont);
					}

					// Set width of text
					int newWidth = gText.getWidth();
					int newHeight = gText.getHeight();

					// Render text
					gText.render(gRenderer, confirmButton[i].x+confirmButton[i].w/2-newWidth/2,
											confirmButton[i].y+confirmButton[i].h/2-newHeight/2,
											newWidth, newHeight);
				}
			}
		}
		//----------------------------------------------------------------------------------------------------------------------------//
		/////////////////////////////////////////////////// Render Keep/Revert Buttons /////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

}

void Options::RenderTextII(SDL_Renderer *gRenderer) {

}

void Options::updateJoystick(SDL_Renderer *gRenderer, LWindow &gWindow, SDL_Event *e) {
	/*////////////////// Xbox 360 Controls /////////////
	if (e->type == SDL_CONTROLLERAXISMOTION) {
		//controls = 1;
	}
	// Xbox 360 Controls
	// Left Analog
	if ( ((SDL_JoystickGetAxis(joy, 0) < -8000) || (SDL_JoystickGetAxis(joy, 0) > 8000)) ||
		 ((SDL_JoystickGetAxis(joy, 1) < -8000) || (SDL_JoystickGetAxis(joy, 1) > 8000)) ){
		LAngle = atan2(SDL_JoystickGetAxis(joy, 1), SDL_JoystickGetAxis(joy, 0)) * ( 180.0 / M_PI );
	}
	// Right Analog
	if ( ((SDL_JoystickGetAxis(joy, 3) < -8000) || (SDL_JoystickGetAxis(joy, 3) > 8000)) ||
		 ((SDL_JoystickGetAxis(joy, 4) < -8000) || (SDL_JoystickGetAxis(joy, 4) > 8000)) ){
		RAngle = atan2(SDL_JoystickGetAxis(joy, 4), SDL_JoystickGetAxis(joy, 3)) * ( 180.0 / M_PI );
	}
	if (LAngle < 0) { LAngle = 360 - (-LAngle); }
	if (RAngle < 0) { RAngle = 360 - (-RAngle); }

	//// Left Analog/////
	// Move left, x-axis
	if (SDL_JoystickGetAxis(joy, 0) < -JOYSTICK_DEAD_ZONE){
		if (!RAnalogTrigger) {
			RAnalogTrigger = true;
			if (type == 2) {
				if (!focusedOther) {
					if (indexX > 0) {
						indexX -= 1;
					}
				}
				minusValues();
			}
		}
	}
	// Move right, x-axis
	else if (SDL_JoystickGetAxis(joy, 0) > JOYSTICK_DEAD_ZONE){
		if (!RAnalogTrigger) {
			RAnalogTrigger = true;
			if (type == 2) {
				if (!focusedOther) {
					if (indexX < maxIndexX) {
						indexX += 1;
					}
				}
				addValues();
			}
		}
	}else{
		RAnalogTrigger = false;
	}
	// joy range between -500 and 500, no moving
	if (SDL_JoystickGetAxis(joy, 0)/30 >= -500 && SDL_JoystickGetAxis(joy, 0)/30 <= 500){
		//
	}

	// Move up, y-axis
	if (SDL_JoystickGetAxis(joy, 1) < -JOYSTICK_DEAD_ZONE){
		if (!LAnalogTrigger) {
			LAnalogTrigger = true;
			if (indexX == 0) {
				if (index > 0) {
					index -= 1;
				}
			}
			else if (indexX == 1 && !focusedOther) {
				if (indexOther > 0) {
					indexOther -= 1;
				}
			}
		}
	}
	// Move down, y-axis
	else if (SDL_JoystickGetAxis(joy, 1) > JOYSTICK_DEAD_ZONE){
		if (!LAnalogTrigger) {
			LAnalogTrigger = true;
			if (indexX == 0) {
				int typeShown;
				if (type==0) {
					typeShown=4;
				}else if (type==2) {
					typeShown=2;
				}
				if (index < typeShown) {
					index += 1;
				}
			}
			else if (indexX == 1 && !focusedOther) {
				if (indexOther < maxIndexOther) {
					indexOther += 1;
				}
			}
		}
	}else{
		LAnalogTrigger = false;
	}
	// joy range between -500 and 500, no moving
	if (SDL_JoystickGetAxis(joy, 1)/30 >= -500 && SDL_JoystickGetAxis(joy, 1)/30 <= 500){
		//
	}

	//// Right Analog/////
	// Face left, x-axis
	if (SDL_JoystickGetAxis(joy, 3)/30 < -500){
	//	moveLeft = true;
	}
	// Face right, x-axis
	if (SDL_JoystickGetAxis(joy, 3)/30 > 500){
	//	moveRight = true;
	}
	// Face up, y-axis
	if (SDL_JoystickGetAxis(joy, 4)/30 < -500){
	//	moveUp = true;
	}
	// Face down, y-axis
	if (SDL_JoystickGetAxis(joy, 4)/30 > 500){
	//	moveDown = true;
	}

	//// Triggers Analog/////
	// Left Trigger
	if (SDL_JoystickGetAxis(joy, 2) > -LTRIGGER_DEAD_ZONE){
		//
	}
	// Right Trigger
	if (SDL_JoystickGetAxis(joy, 5) > -RTRIGGER_DEAD_ZONE){
		//
	}
	//// DPAD Triggers ////
	if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_UP) {
		key = 0;
		if (indexX == 0) {
			if (index > 0) {
				index -= 1;
			}
		}
		else if (indexX == 1 && !focusedOther) {
			if (indexOther > 0) {
				indexOther -= 1;
			}
		}
	}
	if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_DOWN) {
		key = 0;
		if (indexX == 0) {
			int typeShown;
			if (type==0) {
				typeShown=4;
			}else if (type==2) {
				typeShown=2;
			}

			// how far the index can go
			if (index < typeShown) {
				index += 1;
			}
		}
		else if (indexX == 1 && !focusedOther) {
			if (indexOther < maxIndexOther) {
				indexOther += 1;
			}
		}
	}
	if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_LEFT) {
		key = 0;
		if (type == 2) {
			if (!focusedOther) {
				if (indexX > 0) {
					indexX -= 1;
				}
			}
			minusValues();
		}
	}
	if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_RIGHT) {
		key = 0;
		if (type == 2) {
			if (!focusedOther) {
				if (indexX < maxIndexX) {
					indexX += 1;
				}
			}
			addValues();
		}
	}

	if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_LEFTUP) {
		//
	}
	else if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_RIGHTUP) {
		//
	}
	else if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_LEFTDOWN) {
		//
	}
	else if (SDL_JoystickGetHat(joy, 0) == SDL_HAT_RIGHTDOWN) {
		//
	}

	// Xbox 360 Controls
	if (e->type == SDL_JOYBUTTONDOWN && e->jbutton.state == SDL_PRESSED && e->jbutton.which == 0){
		key = 0;
		switch(e->jbutton.button){
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			//
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			//
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			//
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			//
			break;
		case SDL_CONTROLLER_BUTTON_A:
			A = true;
			if (indexOther < 7) {
				if (indexX == 1) {
					if (focusedOther) {
						focusedOther = false;
					}else{
						focusedOther = true;
					}
				}
			}
			break;
		case SDL_CONTROLLER_BUTTON_B:
			focusedOther = false;
			break;
		case SDL_CONTROLLER_BUTTON_X:
			//
			break;
		case SDL_CONTROLLER_BUTTON_Y:
			//
			break;
		}
	}else if (e->type == SDL_JOYBUTTONUP && e->jbutton.state == SDL_RELEASED && e->jbutton.which == 0){
		switch(e->jbutton.button){
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			//
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			//
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			//
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			//
			break;
		case SDL_CONTROLLER_BUTTON_A:
			A = false;
			if (indexX == 0) {
				// PauseMenu
				if (type==0)
				{
					if (index==0) {
						pauseLoop 	= false;
					}
					// Enter Settings
					else if (index==2) {
						// Change type
						type = 2;
						// Change options
						title[0].name = "AUDIO";
						title[1].name = "VIDEO";
						title[2].name = "BACK";
						HIGHLIGHT_INDEX = -1;
					}
					// Exit To Main Menu
					else if (index==3) {
						optionsResult	= Back;
						pauseLoop 		= false;
						//gameLoop 		= false;
						//selection 	= 0;
					}
					// Exit To Desktop
					else if (index==4) {
						optionsResult	= Exit;
						pauseLoop 		= false;
						//gameLoop 	= false;
						//selection = -1;
					}
				}
				// SubMenu
				else if (type==2) {
					// Press other buttons
					if (index==0) {
						HIGHLIGHT_INDEX = 0;
					}else if (index==1) {
						HIGHLIGHT_INDEX = 1;
					}else if (index==2) {
						// Change type
						type = 0;

						// Change options
						title[0].name = "RESUME";
						title[1].name = "TBA";
						title[2].name = "SETTINGS";
						title[3].name = "EXIT MAIN MENU";
						title[4].name = "EXIT TO DESKTOP";
						HIGHLIGHT_INDEX = -1;
					}
				}
			}
			else if (indexX == 1) {
				if (type==2) {
					// Press apply buttons
					if (indexOther==7) {
						actionApplyAudio();
					}else if (indexOther==8) {
						actionApplyVideo(gWindow);
					}
				}
			}
			else if (indexX == 2) {
				if (type==2) {
					actionKeep();
				}
			}
			else if (indexX == 3) {
				if (type==2) {
					actionRevert(gWindow);
				}
			}
			break;
		case SDL_CONTROLLER_BUTTON_B:
			//
			break;
		case SDL_CONTROLLER_BUTTON_X:
			//
			break;
		case SDL_CONTROLLER_BUTTON_Y:
			//
			break;
		}
	}*/
}

void Options::minusValues() {
	if (focusedOther) {
		if (indexOther == 0) {
			bar[indexOther].value -= 10;
		}
		else if (indexOther == 1) {
			bar[indexOther].value -= 8;
		}
		else if (indexOther == 2) {
			bar[indexOther].value -= 8;
		}
		else if (indexOther == 3) {
			if (bar[indexOther].value > 1) {
				bar[indexOther].value -= 1;
			}
		}
		else if (indexOther == 4) {
			bar[indexOther].value -= 1;
		}
		else if (indexOther == 5) {
			bar[indexOther].value -= 1;
		}
		else if (indexOther == 6) {
			bar[indexOther].value -= 1;
		}
		// fix going over
		if (bar[indexOther].value < 0) {
			bar[indexOther].value = 0;
		}
	}
}

void Options::addValues() {
	if (focusedOther) {
		if (indexOther == 0) {
			bar[indexOther].value += 10;
			if (bar[indexOther].value > 128) { bar[indexOther].value = 128; }
		}
		else if (indexOther == 1) {
			bar[indexOther].value += 8;
			if (bar[indexOther].value > 128) { bar[indexOther].value = 128; }
		}
		else if (indexOther == 2) {
			bar[indexOther].value += 8;
			if (bar[indexOther].value > 128) { bar[indexOther].value = 128; }
		}
		else if (indexOther == 3) {
			bar[indexOther].value += 1;
			if (bar[indexOther].value > 5) { bar[indexOther].value = 5; }
		}
		else if (indexOther == 4) {
			bar[indexOther].value += 1;
			if (bar[indexOther].value > 1) { bar[indexOther].value = 1; }
		}
		else if (indexOther == 5) {
			bar[indexOther].value += 1;
			if (bar[indexOther].value > 1) { bar[indexOther].value = 1; }
		}
		else if (indexOther == 6) {
			bar[indexOther].value += 1;
			if (bar[indexOther].value > 1) { bar[indexOther].value = 1; }
		}
	}
}

void Options::actionApplyAudio() {
	// Apply new Bar Settings and new SFX Bar Settings
	applyCustomAudioCFG(bar[1].value*(bar[0].value*0.01), bar[2].value*(bar[0].value*0.01));
	// player preview of new settings
	Mix_PlayChannel(-1, sRockBreak, 0);
	// Display confirm prompt
	confirm			= true;
	timer 			= revertSettingsTimer;
	button = "a";
}

void Options::actionApplyVideo(LWindow &gWindow) {
	// Apply new Video Settings
	gWindow.applySettings(bar[3].value, bar[4].value, bar[5].value, bar[6].value);
	// Display confirm prompt
	confirm			= true;
	timer 			= revertSettingsTimer;
	button = "v";
}

void Options::actionKeep() {
	indexX = 1;
	// file name .cfg
	std::string temps;
	// file data
	std::stringstream tempss;
	// Turn off confirm prompt
	confirm 		= false;
	timer 			= revertSettingsTimer;
	// Keep new Audio Settings
	if (button=="a") {
		MASTER_VOL		= bar[0].value;
		MUSIC_VOL		= bar[1].value;
		SFX_VOL			= bar[2].value;
		temps += defDir;
		temps += "audio.cfg";
		tempss << MASTER_VOL << " "
			   << MUSIC_VOL  << " "
			   << SFX_VOL;
		// Save Audio Settings
		saveAudioCFG();
	}
	// Keep new Video Settings
	if (button=="v") {
		RESOLUTION		= bar[3].value;
		ANTI_ALIAS		= bar[4].value;
		VSYNC			= bar[5].value;
		FULLSCREEN		= bar[6].value;
		temps == "";
		temps += defDir;
		temps += "video.cfg";
		tempss << RESOLUTION << " "
			   << ANTI_ALIAS << " "
			   << VSYNC 	 << " "
			   << FULLSCREEN;
		// Save Video Settings
		saveVideoCFG();
	}
	// save .cfg file
	std::ofstream fileSettings;
	fileSettings.open( temps.c_str() );
		fileSettings << tempss.str().c_str();
	fileSettings.close();
}

void Options::actionRevert(LWindow &gWindow) {
	indexX = 1;
	// Turn off confirm prompt
	confirm 		= false;
	timer 			= revertSettingsTimer;

	// Revert to old Bar Settings
	if (button=="a") {
		applyOldAudioCFG();
		// player preview of new settings
		Mix_PlayChannel(-1, sRockBreak, 0);
		bar[0].value	= MASTER_VOL;
		bar[1].value	= MUSIC_VOL;
		bar[2].value	= SFX_VOL;
	}

	// Revert to old Video Settings
	if (button=="v") {
		bar[3].value	= RESOLUTION;
		bar[4].value	= ANTI_ALIAS;
		bar[5].value	= VSYNC;
		bar[6].value	= FULLSCREEN;
		gWindow.applySettings(RESOLUTION, ANTI_ALIAS, VSYNC, FULLSCREEN);
	}
}
