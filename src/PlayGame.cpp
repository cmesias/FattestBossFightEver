/*
 * PlayGame.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Carl
 */

#include <fstream>
#include <iostream>
#include <limits>
#include <math.h>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "LTexture.h"
#include "Particless.h"

#include "PlayGame.h"

void PlayGame::Init() {
	// Upon entry
	place_type 			= 0;
    debug 				= false;
	editor	 			= false;
	quit 				= false;
	leftClick 			= false;
	shift 				= false;
	camx 				= 0;
	camy 				= 0;
	camlock				= true;
	frame 				= 0;
    cap 				= true;
	int i = 0;


	// Tiles
	{
		int HowManyTilesVertical   = 12, HowManyTilesHorizontal = 24;
		int endW = 16, endH = 16;
		for (int h = 0; h < HowManyTilesVertical; h++) {
			for (int w = 0; w < HowManyTilesHorizontal; w++) {
				rTiles[i].x = 0 + w * endW;
				rTiles[i].y = 0 + (h * endH);
				rTiles[i].w = endW;
				rTiles[i].h = endH;
				i++;
			}
		}
	}

	// Initial camera center
	//camx = map.x + map.w / 2 - screenWidth/2;
	//camy = map.y + map.h / 2 - screenHeight/2;
	camx = map.x;
	camy = map.y + screenHeight/2 + 100;

	// Initialize
	sli.Init(boss);
	part.init(particles);
	enem.init(enemy);
	spaw.init(spawner);
	player.Init(map.x+map.w/2-player.w/2, map.y+map.h/2-player.h/2, "Player1", false);
	player.loadScore();
	tex.init(text);
	tl.initTile(tile);
	tlc.Init(tilec);
	tb.init(tilebar);
	tb.placeTileBar(tilebar);
	obj.init(object);
}
/*
void PlayGame::saveCFG(std::string fileName){
	std::ofstream fileSettings;
	fileSettings.open( fileName.c_str() );
		std::stringstream tempss;
		tempss << MASTER_VOL << " "
			   << MUSIC_VOL  << " "
			   << SFX_VOL 	 << " "
			   << RESOLUTION << " "
			   << FULLSCREEN << " "
			   << VSYNC;
		fileSettings << tempss.str().c_str();
	fileSettings.close();
}

void PlayGame::loadCFG(std::string fileName){
	std::fstream fileSettings( fileName.c_str() );
	if (fileSettings.is_open()) {
		fileSettings >> MASTER_VOL >> MUSIC_VOL >> SFX_VOL >> RESOLUTION >> FULLSCREEN >> VSYNC;
		std::cout << "Loaded config.cfg" << std::endl;
	}else{
		std::cout << "Error opening config.cfg." << std::endl;
		std::cout << "\tCreating default config.cfg..." << std::endl;
		std::ofstream newConfigFile("cfg/config.cfg");
		if (newConfigFile.is_open()) {
			std::cout << "\t\tSuccessfully created config.cfg." << std::endl;
			newConfigFile << "100 128 128 2 2 2";
		}else{
			std::cout << "\t\tError creating config.cfg." << std::endl;
			std::cout << "\t\t\tManually setting default config settings." << std::endl;
			MASTER_VOL			= 100;
			MUSIC_VOL			= 128;
			SFX_VOL				= 128;
			RESOLUTION			= 2;
			FULLSCREEN			= 2;
			VSYNC				= 2;
		}
		newConfigFile.close();
	}
	fileSettings.close();

	// Set Audio Settings
	Mix_VolumeMusic(MUSIC_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sRockBreak, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sCast, SFX_VOL*(MASTER_VOL*0.01));
	Mix_VolumeChunk(sAtariBoom, SFX_VOL*(MASTER_VOL*0.01));

	// Set Video Settings
	gWindow.applySettings(RESOLUTION, FULLSCREEN, VSYNC);
}*/

// Load
void PlayGame::Load(LWindow &gWindow, SDL_Renderer *gRenderer)
{
	// load textures
	gBG.loadFromFile(gRenderer, 		"resource/gfx/bg.png");
	gBG2.loadFromFile(gRenderer, 		"resource/gfx/bg2.png");
	gCircle.loadFromFile(gRenderer, 	"resource/gfx/circle.png");
	gCursor.loadFromFile(gRenderer, "resource/gfx/cursor.png");

	// load fonts
	gFont 	= TTF_OpenFont("fonts/Viga-Regular.ttf", 18);
	gFont13 = TTF_OpenFont("fonts/Viga-Regular.ttf", 13);
	gFont26 = TTF_OpenFont("fonts/Viga-Regular.ttf", 26);

	// load audio
	sAmbientMusic 		= Mix_LoadMUS("sounds/ambient_space.mp3");
	sRockBreak 			= Mix_LoadWAV("sounds/bfxr/rock_break.wav");
	sAtariBoom 			= Mix_LoadWAV("sounds/bfxr/atari_boom.wav");
	sDownStabHitTilec	= Mix_LoadWAV("sounds/bfxr/snd_downStabHitTilec.wav");
	sSlashHitBoss		= Mix_LoadWAV("sounds/bfxr/snd_slashHitBoss.wav");
	sCastHitBoss		= Mix_LoadWAV("sounds/bfxr/snd_castHitBoss.wav");
	sSlashTilec			= Mix_LoadWAV("sounds/bfxr/snd_slashTilec.wav");
	sParrySuccess		= Mix_LoadWAV("sounds/bfxr/snd_parrySuccess.wav");

	// load particle textures
	gParticles.loadFromFile(gRenderer, "img/particle/particles.png");
	gParticles.setBlendMode(SDL_BLENDMODE_ADD);

	//Load texture target
	gTargetTexture.createBlank( gRenderer, screenWidth, screenHeight, SDL_TEXTUREACCESS_TARGET );

	// load media for other classes
	sli.Load(gRenderer);
	part.load(gRenderer);
	enem.load(gRenderer);
	spaw.load(gRenderer);
	player.Load(gRenderer);
	tl.load(gRenderer);
	tb.load(gRenderer);
}

void PlayGame::Free() {
	// free textures
	gParticles.free();
	gText.free();
	gBG.free();
	gBG2.free();
	gCircle.free();
	gCursor.free();

	// free fonts
	TTF_CloseFont(gFont);
	TTF_CloseFont(gFont13);
	TTF_CloseFont(gFont26);
	gFont = NULL;
	gFont13 = NULL;
	gFont26 = NULL;

	// free audio
	Mix_FreeChunk(sRockBreak);
	Mix_FreeMusic(sAmbientMusic);
	Mix_FreeChunk(sAtariBoom);
	Mix_FreeChunk(sDownStabHitTilec);
	Mix_FreeChunk(sSlashHitBoss);
	Mix_FreeChunk(sCastHitBoss);
	Mix_FreeChunk(sSlashTilec);
	Mix_FreeChunk(sParrySuccess);

	sRockBreak 			= NULL;
	sAmbientMusic 		= NULL;
	sAtariBoom 			= NULL;
	sDownStabHitTilec	= NULL;
	sSlashHitBoss		= NULL;
	sCastHitBoss		= NULL;
	sSlashTilec			= NULL;
	sParrySuccess		= NULL;

	// free media from other classes
	sli.Free();
	player.Free();
	part.free();
	enem.free();
	spaw.free();
}

/* 5-31-2017
 * Change asteroid spawns to zombies
 */


void PlayGame::Show(LWindow &gWindow, SDL_Renderer *gRenderer,
					PlayGame::Result &result, int LevelToLoad) {

	// Set level to load to a local variable
	// that is only available inside PlayGame.cpp
	this->LevelToLoad = LevelToLoad;

	// Initialize
	Init();

	// Load resources
	Load(gWindow, gRenderer);

	// Load save configurations
	LoadCFG();

	// Load Player last high score for current Level
	LoadHighScore();

	// Load level selected from ActSelection.cpp
	LoadLevel();

    // Play Music, looped
	//Mix_FadeInMusic(sAmbientMusic, -1, 0);

	//SDL_ShowCursor(false);

	// While loop
	while (!quit) {

		// Start FPS cap
		fps.start();

		// Handle Events
		while (SDL_PollEvent(&event)) {

			// Handle window events
			gWindow.handleEvent(gRenderer, event);

			// Close program
			if (event.type == SDL_QUIT) {
				result = Exit;
				quit = true;
				Free();
				return;
			}else{
				/// Key Pressed
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
				{
					/* Global Controls */
					switch (event.key.keysym.sym)
					{
						case SDLK_LSHIFT:
							shift = true;
							break;
						case SDLK_RSHIFT:
							shift = true;
							break;
						case SDLK_p:
							//editor = (!editor);
							/*if (editor) {
								// Disable editor
								editor = false;
								camlock = true;
							} else {
								// Enable editor
								editor = true;
								camlock = false;
							}*/
							break;
						case SDLK_h:
							//debug = (!debug);
							break;
						case SDLK_ESCAPE:	// pause menu
							start(gWindow, gRenderer);
							break;
						case SDLK_F1:							// Set render size 1
							//SDL_RenderSetLogicalSize(gRenderer,1920,1080);
							break;
						case SDLK_F2:							// Set render size 2
							//SDL_RenderSetLogicalSize(gRenderer,1600,900);
							break;
						case SDLK_F3:							// Set render size 3
							//SDL_RenderSetLogicalSize(gRenderer,1280,720);
							break;
						case SDLK_F4:							// Set render size 4
							//SDL_RenderSetLogicalSize(gRenderer,800,600);
							break;
					}

					/* Editor Controls */
					if (editor) {
						// Local
						switch (event.key.keysym.sym)
						{
							//
						}
						editorOnKeyDown(event.key.keysym.sym);
					}
					/* Player Controls */
					else{
						// Local
						switch (event.key.keysym.sym)
						{
							case SDLK_ESCAPE:	// pause menu
								//start(gWindow, gRenderer);
								break;
						}

						// Player key down
						player.OnKeyDown(event.key.keysym.sym);
					}	// end editor check
				}       // end key pressed
				// Key Released
				else if (event.type == SDL_KEYUP && event.key.repeat == 0)
				{
					/* Global Controls */
					switch (event.key.keysym.sym) {
					case SDLK_LSHIFT:
						shift = false;
						break;
					case SDLK_RSHIFT:
						shift = false;
						break;
					}

					/* Editor Controls */
					if (editor) {
						// Local
						switch (event.key.keysym.sym)
						{
							//
						}
						editorOnKeyUp(event.key.keysym.sym);
					}
					/* Player Controls */
					else{
						// Local
						switch (event.key.keysym.sym)
						{
							//
						}

						// Player Keys Released, categorized
						player.OnKeyUp(event.key.keysym.sym);
					} // end editor
				}	  // end key released

				//////////////////////////////////////

				/////////////////////////////////////

				// Update Player click state
				if (!editor) {
					// Player Mouse Click state
					player.mouseClickState(event);

					// Update Xbox 360 controls
					player.updateJoystick(event);
				}

				// Global mouse presses
				mousePressed(event);

				// Mouse Released
				//result = mouseReleased(event);

				// Customize Character results
				switch (optionsResult)  {
				case Options::Back:				// Back (to Main Menu)
					quit = true;
					break;
				case Options::Exit:				// Back (to Main Menu)
					result = Exit;
					quit = true;
					Free();
					return;
					break;
				}
			}
		}

		// update everything
		Update(gWindow, gRenderer);

		//Set our Texture as our render target
		gTargetTexture.setAsRenderTarget(gRenderer);

		// Clear render screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

			//SDL_Rect backgroundRect = {0, 0, screenWidth, screenHeight};
			//SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
			//SDL_RenderFillRect(gRenderer, &backgroundRect);

			// Render particles
			part.renderStarParticle(particles, camx, camy, 1, gRenderer);
			part.renderBulletParticle(particles, camx, camy, 1, gRenderer);

			// Render UI
			//player.renderUI();
			//konoko.renderUI(npc[1], textBoxPrincess[indexP]);

			// Render foreground
			/*RenderFG(gRenderer, gWindow);*/

			// Render objects
			Render(gRenderer, gWindow);

			// TODO [ ] - fix lights 3/17/2022
			///gBG2.setBlendMode(SDL_BLENDMODE_ADD);
			//gBG2.render( gRenderer, 0, 0, 1280, 720, NULL, 0.0);

			//gBG.setBlendMode(SDL_BLENDMODE_BLEND);
			//gBG.render( gRenderer, 0, 0, 1280, 720, NULL, 0.0);

			// Render background
			//gBG2.setBlendMode(SDL_BLENDMODE_ADD);
			//gBG2.render( gRenderer, 0, 0, 1280, 720, NULL, 0.0);

			// Render UI for objects
			RenderUI(gRenderer, gWindow);

			// Render text
			RenderDebug(gRenderer);

			// Render text
			RenderText(gRenderer, gWindow);

			// Original box, untouched
			SDL_Rect tempRect = {0, 0, screenWidth, screenHeight};
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(gRenderer, &tempRect);

		//Reset render target
		SDL_SetRenderTarget( gRenderer, NULL );

		//Show rendered to texture
		gTargetTexture.render( gRenderer, 0, 0, screenWidth, screenHeight, NULL, 0.0);
		//gTargetTexture.render( gRenderer, 0, 0, gWindow.getWidth(), gWindow.getHeight(), NULL, 0.0);

		// Update screen
		SDL_RenderPresent(gRenderer);

		// fps
		frame++;
		if((cap == true ) && (fps.get_ticks() < 1000 / FRAMES_PER_SECOND ))
			SDL_Delay((1000 / FRAMES_PER_SECOND ) - fps.get_ticks());
	}
	// Free everything
	Free();
	//tl.free();
	//tb.free();
}

// Update everything
void PlayGame::Update(LWindow &gWindow, SDL_Renderer *gRenderer) {
	// Variable limits
	if (tl.layer < -1) {
		tl.layer = 6;
	}
	if (tl.layer > 6) {
		tl.layer = -1;
	}
	if (tlc.layer < 0) {
		tlc.layer = 0;
	}
	if (tlc.layer > 2) {
		tlc.layer = 0;
	}
	if (tl.id < 0) {
		tl.id = tb.TILES_UNIQUE;
	}
	if (tl.id > tb.TILES_UNIQUE) {
		tl.id = 0;
	}
	if (tlc.id > 2) {
		tlc.id = 0;
	}
	if (place_type > 2) {
		place_type = 0;
	}

	// Get mouse position
	SDL_GetMouseState(&mx, &my);
	/* get render width and height
	 * but (mostly used to get actual render height)
	 */
	int renderW = 0;
	int renderHDummy = 0;
	SDL_GetRendererOutputSize(gRenderer,&renderW,&renderHDummy);
	int en = renderW * 0.4375;
	int renderH = renderW - en;

	// Grid like coordinates of mouse
	// get new mouse coordinates based on render size, and actual screen size
	/*mx = (screenWidth*mx)/gWindow.getWidth();	// New mouse coordinates, no relation to camera
	my = (screenHeight*my)/gWindow.getHeight();	// New mouse coordinates, no relation to camera

	//int mex = (1280*mx)/gWindow.getWidth();
	//int mey = (720*my)/gWindow.getHeight();
	int oldMX = mx+camx;								// New mouse coordinates, relation to camera
	int oldMY = my+camy;								// New mouse coordinates, relation to camera
	int clampSize = 32;									// Magnet pixel size
	int remainderW = oldMX % clampSize;
	int remainderH = oldMY % clampSize;
	newMx = mx - remainderW;						// New mouse coordinates, locked on x32 coordinates
	newMy = my - remainderH;						// New mouse coordinates, locked on x32 coordinates*/

	// Grid like coordinates of mouse
	mex = (screenWidth*mx)/gWindow.getWidth();				// New mouse coordinates, no relation to camera
	mey = (screenHeight*my)/gWindow.getHeight();				// New mouse coordinates, no relation to camera
	//int mex = (1280*mx)/gWindow.getWidth();
	//int mey = (720*my)/gWindow.getHeight();
	int oldMX = mex+camx;								// New mouse coordinates, relation to camera
	int oldMY = mey+camy;								// New mouse coordinates, relation to camera
	int clampSize = tl.tilew;									// Magnet pixel size
	if (editor) {
		if (place_type == 0) {
			clampSize = tl.tilew;
		}else if (place_type == 1) {
			if (shift) {
				clampSize = 4;
			}else{
				clampSize = tl.tilew;
			}
		}
	}
	int remainderW = oldMX % clampSize;
	int remainderH = oldMY % clampSize;
	newMx = mex - remainderW;						// New mouse coordinates, locked on x32 coordinates
	newMy = mey - remainderH;						// New mouse coordinates, locked on x32 coordinates

	// Update Variables
	bool test;
	test = true;

	// Generate Stars
	if (test){
	//	part.genStars(particle, gWindow.getWidth(), gWindow.getHeight());
	}

	// Update tiles
	tl.updateTile(tile, gWindow, player.getX(), player.getY(), player.getW(), player.getH(),
			newMx+camx, newMy+camy,
			mex+camx, mey+camy,
			camx, camy,
			&rTiles[0]);

	// Update tiles
	tlc.Update(tilec, map, newMx+camx, newMy+camy, mex+camx, mey+camy, camx, camy);

	// Update Tile bar
	tb.update(tilebar, gWindow, mex, mey, camx, camy);

	// Editor
	if (editor) {

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//------------------------------------------------------------------------------------------------//
		//--------------------------- Update editor Updates from other classes ---------------------------//

		// Boss.cpp class

		// Update Editor boss
		sli.UpdateEditor(boss, mex+camx, mey+camy, camx, camy);

		//--------------------------- Update editor Updates from other classes ---------------------------//
		//------------------------------------------------------------------------------------------------//
		////////////////////////////////////////////////////////////////////////////////////////////////////

		// If Left Mouse Button is being held down
		if (leftClick) {
			// If not on Tile-bar, place other tiles
			if (!tb.touching) {

				// If removing Tiles
				if (place_type == 0) {

					// Remove Tiles if left clicking, "0" in second argument
					// Also removes them by how many Tiles we are going to place horiz. and verti.
					tl.removeTile(tile, 0);
				}

				// If removing Tilecs
				else if (place_type == 1) {
					//tlc.SpawnAll(tilec, newMx, newMy, camx, camy);
				}

				// If removing Bosses
				else if (place_type == 2) {
					sli.Remove(boss);
				}
			}else{
				if (shift) {
					tb.selectBlockMultiple(tilebar, tl.id, mex, mey);
				// Pen Tool, select a Tile from the TileBar
				}else{
					tb.selectBlock(tilebar, tl.id);
				}
			}
		}

		// If Right Mouse Button is being held down
		if (rightClick)
		{
			// If not on Tile-bar, place other tiles
			if (!tb.touching) {
				// Tiles
				if (place_type == 0) {
					tl.spawnTile(tile, newMx, newMy, camx, camy, &rTiles[0]);
				}

				// Tilecs
				else if (place_type == 1) {
					// Do nothing here
				}

				// Boss
				else if (place_type == 2) {
					// Do nothing here
				}
			}else{
				// Pen Tool, select a Tile from the TileBar
				tb.selectBlock(tilebar, tl.id);
			}
		}
	}

	// Update Particles
	part.updateStarParticles(particles, map.x, map.y, map.w, map.h);
	part.updateBulletParticles(particles, map.x, map.y, map.w, map.h);

	// Update Asteroids
	/*aste.updateAsteroid(asteroid, particles, part,
								  player.alive, player.getInvurnerableStatus(), player.score, enem.mileScore, player.health,
								  mx, my, camx, camy, gWindow.getWidth(), gWindow.getHeight(),
								  map.x, map.y, map.w, map.h);*/

	// Update Spawner: spawns an Asteroid
	//spaw.update(spawner, asteroid, aste, player.x+player.w/2, player.y+player.h/2, mx, my, camx, camy);


	// Check collision between Player & Tiles
	//tl.checkCollision(tile, player.x, player.y, player.realw, player.realh, player.jumpstate, player.vX, player.vY, player.jumps);

	// If not in editor mode
	if (!editor) {

		// If Player is alive
		if (player.alive) {
			// update zombies
			/*zom.update(zombie, particles, part, map, player, sAtariBoom, camx, camy);

			// Update Enemy
			enem.update(enemy,
					particles, part,
						player.alive,
						player.x+player.w/2, player.y+player.h/2, player.w/2,
						player.score, map.x, map.y, map.w, map.h,
						sAtariBoom, sAtariBoom);*/
		}

		// update zombie manually
		/*for (int i = 0; i < 128; i++) {
			if (asteroid[i].alive){
				int rands = rand() % 3 + 1;
				double newX = asteroid[i].x+asteroid[i].w/2;
				double newY = asteroid[i].y+asteroid[i].h/2;
				part.spawnExplosion(particles, newX, newY, rands);
			}
		}*/

		// Update Player & Asteroid collision check
		//checkCollisionPlayerAsteroid(player, asteroid);

		// Update Player Particle & Asteroid collision check
		//checkCollisionParticleAsteroid(particles, part, asteroid, player);

		// Update Player Particle & Enemy collision check
		//checkCollisionParticleEnemy(particles, part, enemy, player);

		// Collision, particle & zombie
		/*checkCollisionParticleZombie();

		// Update Asteroids: Wave re-spawn
		spawnAsteroidsNow2();*/

		/* Notes on order:
		 *
		 * 0. obj.Update()
		 * 1. sli.GetDistanceOfPlayer()
		 * 2. sli.Update()
		 * 3. player.Update()
		 *
		 *
		 */

		// Move boss towards player
		sli.GetDistanceOfPlayer(boss, player.getX(), player.getY(), player.getW(), player.getH(), &player.x2, &player.y2);

		// Update boss
		sli.Update(boss, obj, object, particles, part, map, mex+camx, mey+camy, camx, camy, player.alive);

		// Update Player
		player.Update(map,
					  enemy, enem,
					  particles, part,
					  tl, tile,
					  tlc, tilec,
					  obj, object,
					  mex, mey, camx, camy,
					  spawnX, spawnY,
					  gWindow, gRenderer,
					  gText, gFont26, {255,255,255},
					  sAtariBoom, RestartLevel,
					  LevelToLoad);

		// If we get a true from Player.cpp that we should restart the levelm then restart the level
		if (RestartLevel) {
			RestartLevel = false;

			// Load level selected from ActSelection.cpp
			LoadLevel();
		}

		// Update objects
		obj.update(object);

		// Check collision between Particle & Tile
		checkCollisionParticleTile();

		// Check collision between Particle & Boss
		checkCollisionParticleBoss();

		// Check collision between Boss & Tile
		checkBossTileCollision();

		// Check collision between Player attacks & Boss
		checkPlayerAttacksCollisionBoss();

		// Check collision between Player attacks & Tile
		checkPlayerAttacksTileCollision();

		// Check collision between Player attacks & Boss Particle
		checkPlayerAttacksBossParticleCollision();

		// Check collision between Boss attacks & Player
		checkBossAttacksCollisionPlayer();

		// Check collision between Boss & Player
		//checkCollisionBossPlayer();

		// Update Enemy Particle & Player collision check
		checkCollisionParticlePlayer();

		// Check collision between Player Particle & Boss Particle
		checkCollisionParticleParticle();

		// Damage text: for zombie
		tex.updateDamageText(text);
	}

	// Damage text: for zombie
	tex.updateDebugText(text);

	// Update camera
	if (camLeft) {
		camx -= 8;
	}
	if (camRight) {
		camx += 8;
	}
	if (camUp) {
		camy -= 8;
	}
	if (camDown) {
		camy += 8;
	}
	//camx = map.x + map.w / 2 - screenWidth/2;
	//camy = map.y + map.h / 2 - screenHeight/2;

	// center camera on target
	{
		//camx = player.x + player.w / 2 - gWindow.getWidth()/2;
		//camy = player.y + player.h / 2 - gWindow.getHeight()/2;
		if (camlock) {
			float bmx, bmy;
			bmx  = player.x+player.w/2-screenWidth/2;
			bmy  = player.y+player.h/2-screenHeight/2;
			float distance = sqrt((bmx - camx) * (bmx - camx)+
								  (bmy - camy) * (bmy - camy));

			// Camera target
			float vX, vY;
			if (distance >= 1){
				vX 	= 2 * (10*distance/256) * (bmx - camx) / distance;
				vY 	= 2 * (10*distance/256) * (bmy - camy) / distance;
				camx += vX;
				camy += vY;
			}
		}else{
			if (editor) {
				if (mex <= 0) {
				//	camx -= 10;
				}
				if (mey <= 0) {
				//	camy -= 10;
				}
				if (mex + 2 > screenWidth) {
				//	camx += 10;
				}
				if (mey + 2 > screenHeight) {
				//	camy += 10;
				}
			}
		}

		// Camera bounds
		if (!editor) {
			if( camx < 0 ){
				camx = 0;
			}
			if( camy < 0 ){
				camy = 0;
			}
			if( camx+screenWidth > map.w ){
				camx = map.w-screenWidth;
			}
			if( camy+screenHeight > map.h ){
				camy = map.h-screenHeight ;
			}
		}
	}
}

// Render foreground
void PlayGame::RenderFG(SDL_Renderer *gRenderer, LWindow &gWindow) {
	// render map
	for (int j=0; j<6; j++) {
		for (int i=0; i<6; i++) {
			gBG.render(gRenderer, map.x+i*837-camx, map.y+j*837-camy, 837, 837);
		}
	}
}

// Render everything
void PlayGame::Render(SDL_Renderer *gRenderer, LWindow &gWindow) {

	// Render Colonies that spawn Asteroids
	spaw.render(spawner, camx, camy, gRenderer);

	// Render Enemies
	enem.render(enemy, camx, camy, gRenderer);

	// Render different layers
	{

		/* Layers
		 *
		 * 0: Floor
		 * 1: Appliance			- Only rendered on top of Player when player.y+player.h < tile.x+tile.w
		 * 2: Top of Appliance	- Always rendered on top of Player
		 * 3: Roof 				- Always rendered on top of Player
		 */


		// Render tile, ground below ground
		tl.renderTile(gRenderer, tile, -1, camx, camy);


		// Render tile, ground
		tl.renderTile(gRenderer, tile, 0, camx, camy);

		// Render tile, appliances
		//tl.renderTile(gRenderer, tile, 2, camx, camy);

		// Render Tile in back of player
		tl.RenderBack(gRenderer, tile, camx, camy);

		// Render Boss
		sli.RenderBack(gRenderer, boss, gFont13, gText, camx, camy);

		// Render our player
		player.Render(mex, mey, camx, camy, gWindow,
					  gRenderer, {255,255,255}, part.count, gText);

		// Render Boss
		sli.RenderFront(gRenderer, boss, gFont13, gText, camx, camy);

		// Render Star particles
		part.renderStarParticle(particles, camx, camy, 1, gRenderer);

		// Render Particles
		part.renderBulletParticle(particles, camx, camy, 1, gRenderer);

		// Render Tile in front of player
		tl.RenderFront(gRenderer, tile, camx, camy);

		// Render tile, ceiling
		tl.renderTile(gRenderer, tile, 3, camx, camy);

	}
}


// Render everything
void PlayGame::RenderUI(SDL_Renderer *gRenderer, LWindow &gWindow)
{

	// Render Boss Health
	sli.RenderUI(gRenderer, boss, camx, camy);

	// Render Player Health
	player.RenderUI(gRenderer, camx, camy);
}

// Render debug information
void PlayGame::RenderDebug(SDL_Renderer *gRenderer)
{
	// If debugging, show debug info
	if (debug)
	{
		// Render Player debug
		player.RenderDebug(gRenderer, camx, camy);

		// Render Particle debug
		part.renderBulletParticleDebug(particles, camx, camy, gRenderer);

		// Render player attack collisions
		obj.render(gRenderer, object, camx, camy);

		// Render collision tiles
		tlc.Render(gRenderer, tilec, camx, camy);

		// Render Boss text
		sli.RenderDebug(gRenderer, boss, gFont13, gText, camx, camy);

		// Render Player spawn point
		SDL_Rect tempRect = {spawnX-camx, spawnY-camy, 48, 48};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 200, 255);
		SDL_RenderDrawRect(gRenderer, &tempRect);

		// Render which collision-tile we have selected for placement]
		int NumberOfCollisionTilesWidth = 32 * 3;
		for (int i=0; i<3; i++)
		{
			if (i == 0) {
				SDL_Rect tempr = {screenWidth * 0.80 - NumberOfCollisionTilesWidth  + i*32, screenHeight-34, 32, 32};
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}else if (i == 1) {
				SDL_Rect tempr = {screenWidth * 0.80 - NumberOfCollisionTilesWidth + i*32, screenHeight-34, 32, 32};
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}else if (i == 2) {
				SDL_Rect tempr = {screenWidth * 0.80 - NumberOfCollisionTilesWidth + i*32, screenHeight-34, 32, 32};
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
				SDL_RenderDrawRect(gRenderer, &tempr);
			}
		}

		if (place_type == 1) {
			int newWidth = tlc.tilew*tlc.multiW;
			int newHeight = tlc.tileh*tlc.multiH;
			for (int j = 0; j < tlc.multiW; j++) {
				for (int h = 0; h < tlc.multiH; h++) {
					// Render mouse coordinates snapped to grid
					if (tlc.id == 0) {
						SDL_Rect tempr = {newMx, newMy, newWidth, newHeight};
						SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
						SDL_RenderDrawRect(gRenderer, &tempr);
					}else if (tlc.id == 1) {
						SDL_Rect tempr = {newMx, newMy, newWidth, newHeight};
						SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
						SDL_RenderDrawRect(gRenderer, &tempr);
					}else if (tlc.id == 2) {
						SDL_Rect tempr = {newMx, newMy, newWidth, newHeight};
						SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 255);
						SDL_RenderDrawRect(gRenderer, &tempr);
					}
				}
			}
		}

		// Render level size
		SDL_Rect tempr = {map.x-camx, map.y-camy, map.w, map.h};
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(gRenderer, &tempr);

		// Render object text
		for (int i = 0; i < obj.max; i++)
		{
			if (object[i].alive)
			{
				if (object[i].id == 1)
				{
					// Render text
					std::stringstream tempss;
					tempss << "x: " << *object[i].xFollow << ", y: " << *object[i].yFollow;
					gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont26);
					gText.setAlpha(255);
					gText.render(gRenderer, object[i].x-camx, object[i].y-gText.getHeight()-camy, gText.getWidth(), gText.getHeight());

				}

			}
		}
		// Render Tile debug
		tl.renderTileDebug(gRenderer, tile, newMx, newMy, mex, mey, camx, camy, &rTiles[0]);
	}

	// Editor debug menu
	if (editor) {

		// Render hand debug info
		std::stringstream tempss;
		tempss << "Tiles: " 				<< tl.tileCount 		<< ", Tilecs: " 	<< tlc.count 		<< ", Bosss: " << sli.count;
		tempss << "place_type: " 			<< place_type 			<< ", tl.id: " 		<< tl.id 			<< ", tlc.id: " << tlc.id;
		tempss << ", tl.collisionTile: " 	<< tl.collisionTile 	<< ", layer: " 		<< tl.layer;
		tempss << ", tilew: " 				<< tl.tilew 			<< ", tileh: " 		<< tl.tileh;
				/*	   << ", layer: " 		<< tl.layer<< ", tlc.layer: " << tlc.layer << ", editor: " << editor
			   << ", tl.multiW: " 	<< tl.multiW << ", tl.multiH: " << tl.multiH << ", tl.count: " << tl.tileCount;
		tempss << ", tlc.multiW: " 	<< tlc.multiW << ", tlc.multiH: " << tlc.multiH << ", tlc.count: " << tlc.count;*/
		gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255,255,255}, gFont26, 200);
		gText.setAlpha(255);
		gText.render(gRenderer, 0+screenWidth-gText.getWidth(), 100, gText.getWidth(), gText.getHeight());

		// Render tile debug
		if (debug){
			if (place_type == 0) {
				// Render Tile in hand
				tl.RenderHand(gRenderer, tile, newMx, newMy, mex, mey, camx, camy, &rTiles[0]);
			}else if (place_type == 1) {
				// Render collision tiles
				//rendercTile(ctile);
				// Render Collision Tile debug
				//rendercTileDebug(ctile, newMx, newMy);
			}else if (place_type == 2) {
				// Render collision tiles
				//rendercTile(ctile);
				// Render Collision Tile debug
				sli.RenderHand(gRenderer, boss, newMx, newMy, mex, mey, camx, camy);
			}
		}

		// Render Tile Bar
		tb.render(gRenderer, tilebar, tl.id);
	}
}

// Render text
void PlayGame::RenderText(SDL_Renderer *gRenderer, LWindow &gWindow)
{
	// Render map
	//map.render(gRenderer, camx, camy);

	// Render text that spawn into world then dissapear after x amount of time
	for (int i = 0; i < 100; i++)
	{
		if (text[i].alive)
		{
			// Render text
			std::stringstream tempss;
			tempss << text[i].textfield.c_str();

			// original but commented out
			//gText.render(gRenderer, 0, 0+i*28, gText.getWidth(), gText.getHeight());
			//gText.render(gRenderer, text[i].x-camx, text[i].y+i*28-camy, gText.getWidth(), gText.getHeight());

			// Render always on the top left corner of the screen (general debug information)
			if (text[i].type == 0) {
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 255, 255}, gFont26);
				gText.setAlpha(text[i].alpha);
				gText.render(gRenderer, 0, 0 + i*28, gText.getWidth(), gText.getHeight());
			}

			// These texts have an x & y coordinate, render these (like damage text)
			else if (text[i].type == 1) {
				gText.loadFromRenderedText(gRenderer, tempss.str().c_str(), {255, 255, 0, 255}, gFont26);
				gText.setAlpha(text[i].alpha);
				gText.render(gRenderer, text[i].x-camx, text[i].y-camy, gText.getWidth(), gText.getHeight());
			}
		}
	}

	// Render mouse location
	gCursor.render(gRenderer, mex, mey, 20, 20);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// Global Functions ////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------//

void PlayGame::checkCollisionParticleEnemy(Particle particle[], Particle &part, Enemy enemy[], Players &player) {
	// Enemy
	for (int j = 0; j < 8; j++) {
		if (enemy[j].alive){
			// Particle
			for (int i = 0; i < part.max; i++) {
				if (particle[i].alive) {
					if (particle[i].type == 0){
						if (particle[i].x + particle[i].w > enemy[j].x && particle[i].x < enemy[j].x + enemy[j].w
								&& particle[i].y + particle[i].h > enemy[j].y
								&& particle[i].y < enemy[j].y + enemy[j].h) {
							// remove particle
							//particle[i].time = 0;
							//particle[i].alive = false;
							//part.count--;

							// reduce enemy health
							enemy[j].damaged = true;
							enemy[j].health -= particle[i].damage;
						}
					}
				}
			}	// end Particle
		}
	}	// end Enemy
}

void PlayGame::checkCollisionParticleTile()
{
	// Particle
	for (int i = 0; i < part.max; i++)
	{
		if (particles[i].alive)
		{
			if (particles[i].type == -1 || particles[i].type == 0 || particles[i].type == 1)
			{
				for (int j = 0; j < tl.max; j++)
				{
					if (tile[j].alive)
					{
						if (tile[j].collisionTile)
						{
							// Particle target center
							float bmx = tile[j].x + tile[j].w/2;
							float bmy = tile[j].y + tile[j].h/2;

							// Particle center
							float bmx2 = particles[i].x + particles[i].w/2;
							float bmy2 = particles[i].y + particles[i].h/2;
							float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
												  (bmy - bmy2) * (bmy - bmy2));

							// Circle Collision
							if (distance < tile[j].w/2 + particles[i].w/2)
							{
								// Square Collision
								// Check collision between particle and tile
								/*if (checkCollision(particles[i].x, particles[i].y, particles[i].w, particles[i].h, tile[j].x, tile[j].y, tile[j].w, tile[j].h)) {
									particles[i].collide = true;
								} else {
									particles[i].collide = false;
								}*/

								// If there was a collision
								//if (particles[i].collide)
								//{
									// Remove particle
									particles[i].time = 0;
									particles[i].alive = false;
									part.count--;


									// Spawn particle effect
									for (double h=0.0; h< 180.0; h+=rand() % 10 + 40){
										//for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
										int rands = rand() % 3 + 8;
										float newX = tile[j].x+tile[j].w/2;
										float newY = tile[j].y+tile[j].h/2;

										part.spawnParticleAngle(particles, 2,
															newX-rands/2,
															newY-rands/2,
														   rands, rands,
														   i, randDouble(2.1, 5.1),
														   0.0, 0, 0,
														   {210, 144, 40, 255}, 1,
														   1, 1,
														   rand() % 100 + 150, rand() % 2 + 5,
														   rand() % 50 + 90, 0,
														   true, randDouble(0.1, 0.7),
														   100, 10);
									}

									// Play hit sound effect
					                //Mix_PlayChannel(-1, sSlashTilec, 0);
								//}
							}
						}
					}
				}
			}	// end Particle
		}
	}		// end Zombie
}

void PlayGame::checkCollisionParticleBoss()
{

	for (int j = 0; j < part.max; j++)
	{
		if (particles[j].alive)
		{
			if (particles[j].type == -1 || particles[j].type == 0)
			{
				for (int i = 0; i < sli.max; i++)
				{
					if (boss[i].alive)
					{
						// Get center of attack-particle (spawned by the player attacking)
						float bmx = boss[i].x+boss[i].w/2;
						float bmy = boss[i].y+boss[i].h/2;

						// Get center of particles
						float bmx2 = particles[j].x+particles[j].w/2;
						float bmy2 = particles[j].y+particles[j].h/2;

						// Get distance
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));

						// If distance is less than 50 pixels
						//if (distance < 50)
						//{
							// Get angle of boss relative to attack-particle
							float angle = atan2(bmy - bmy2,bmx - bmx2);
							angle = angle * (180 / 3.1416);
							if (angle < 0) {
								angle = 360 - (-angle);
							}

							// Handle radians, cos, sin
							float radians = (3.1415926536/180)*(angle);
							float Cos = floor(cos(radians)*10+0.5)/10;
							float Sin = floor(sin(radians)*10+0.5)/10;

							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
							//----------------------------- Collision Detection based on player-attack hit-box and Boss hit-box -----------------------------//
							// If particle slash if within X distance then move the boss away
							// collision occurred
							if (distance < boss[i].w/2 + particles[j].w/2)
							{
								// Flash Bosses sprite
								boss[i].flash = true;

								// Remove particle
								particles[j].time = 0;
								particles[j].alive = false;
								part.count--;

								// New velocity going away from Bullet Particle
								float newvX = 0.25 * (bmx - bmx2) / distance;
								float newvY = 0.25 * (bmy - bmy2) / distance;

								// Move the boss in someway
								boss[i].vX += newvX;
								boss[i].vY += newvY;
								//boss[i].vX += player.getKnockBackPower()/2 * xDir;

								// Play hit sound effect
				                Mix_PlayChannel(-1, sCastHitBoss, 0);

				                // Subtract boss health
				                boss[i].health -= player.getCastDamage();

				                // If Boss died, go back to ActSelection
				                if (boss[i].health <= 0) {
				    				// Show congrats screen
				                }

				                // Increase player score
				                if (particles[j].type == -1) {
					                player.IncreaseScore(10);
				                }
				                if (particles[j].type == 0) {
					                player.IncreaseScore(5);
				                }

				                // Show damage text (it will print how much damage the player did to the boss)
				    			std::stringstream tempss;
				    			tempss << player.getCastDamage();
				    			tex.spawn(text, boss[i].x+boss[i].w/2, boss[i].y+boss[i].w/2-15, 0.0, -0.5, 150, tempss.str().c_str(), 1);
							}
							//----------------------------- Collision Detection based on player-attack hit-box and Boss hit-box -----------------------------//
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
						//}
					}
				}
			}
		}
	}
}

void PlayGame::checkBossTileCollision()
{
	// Boss and Tile Collision
	// X Axis Collision

	// Boss
	for (int i = 0; i < sli.max; i++) {
		if (boss[i].alive) {
			// Tiles
			for (int j = 0; j < tl.max; j++) {
				if (tile[j].alive){
					if (tile[j].collisionTile)
					{
						// Get center of attack-particle (spawned by the player attacking)
						float bmx = boss[i].x+boss[i].w/2;
						float bmy = boss[i].y+boss[i].h/2;

						// Get center of particles
						float bmx2 = tile[j].x+particles[j].w/2;
						float bmy2 = tile[j].y+particles[j].h/2;

						// Get distance
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));

						// If distance is less than 50 pixels
						//if (distance < 50)
						//{
							// Get angle of boss relative to attack-particle
							float angle = atan2(bmy - bmy2,bmx - bmx2);
							angle = angle * (180 / 3.1416);
							if (angle < 0) {
								angle = 360 - (-angle);
							}

							// Handle radians, cos, sin
							float radians = (3.1415926536/180)*(angle);
							float Cos = floor(cos(radians)*10+0.5)/10;
							float Sin = floor(sin(radians)*10+0.5)/10;

							////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------//
							//-------------------- Circle Collision Detection --------------------//
							// If particle slash if within X distance then move the boss away
							// collision occurred
							if (distance < boss[i].w/2 + particles[j].w/2)
							{
								// New velocity going away from Bullet Particle
								float newvX = 2 * (bmx - bmx2) / distance;
								float newvY = 2 * (bmy - bmy2) / distance;

								// Move the boss away from Tile
								boss[i].vX = boss[i].vX * -1;
								boss[i].vY = boss[i].vY * -1;
							}
					}
				}
			}
			//-------------------- Circle Collision Detection --------------------//
			//--------------------------------------------------------------------//
			////////////////////////////////////////////////////////////////////////


			// X Collision
			{
				// Player Velocity Y Axis
				boss[i].x += boss[i].vX;
				/*
				// Correct boss position
				SDL_Rect rectA;
				rectA.x = boss[i].x;
				rectA.y = boss[i].y;
				rectA.w = boss[i].w;
				rectA.h = boss[i].h;
				bool moveBack;
				moveBack = false;

				// Handle normal collision
				for (int j = 0; j < tl.max; j++) {
					if (tile[j].alive){
						if (tile[j].collisionTile) {
							// Boss target center
							float bmx = tile[j].x + tile[j].w/2;
							float bmy = tile[j].y + tile[j].h/2;

							// Boss center
							float bmx2 = boss[i].x + boss[i].w/2;
							float bmy2 = boss[i].y + boss[i].h/2;
							float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
												  (bmy - bmy2) * (bmy - bmy2));

							// If distance of tile is less than 50 pixels
							if (distance < 50) {
								SDL_Rect rectB;
								rectB.x = tile[j].x;
								rectB.y = tile[j].y;
								rectB.w = tile[j].w;
								rectB.h = tile[j].h;
								if  ( checkCollisionRect( rectA, rectB )) {
									moveBack = true;
									boss[i].collision = true;
								}else{
									boss[i].collision = false;
								}
							}
						}
					}
				}
				if (moveBack){
					// Reverse Boss walking direction
					boss[i].vX -= boss[i].vX;
				}*/
			}

			// Y Collision
			{
				// Player Velocity Y Axis
				boss[i].y += boss[i].vY;
				/*
				// Correct boss position
				SDL_Rect rectA;
				rectA.x = boss[i].x;
				rectA.y = boss[i].y;
				rectA.w = boss[i].w;
				rectA.h = boss[i].h;
				bool moveBack;
				moveBack = false;

				// Handle normal collision
				for (int j = 0; j < tl.max; j++) {
					if (tile[j].alive){
						if (tile[j].collisionTile) {
							// Boss target center
							float bmx = tile[j].x + tile[j].w/2;
							float bmy = tile[j].y + tile[j].h/2;

							// Boss center
							float bmx2 = boss[i].x + boss[i].w/2;
							float bmy2 = boss[i].y + boss[i].h/2;
							float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
												  (bmy - bmy2) * (bmy - bmy2));

							// If distance of tile is less than 50 pixels
							if (distance < 50) {
								SDL_Rect rectB;
								rectB.x = tile[j].x;
								rectB.y = tile[j].y;
								rectB.w = tile[j].w;
								rectB.h = tile[j].h;
								if  ( checkCollisionRect( rectA, rectB )) {
									moveBack = true;
									boss[i].collision = true;
								}else{
									boss[i].collision = false;
								}
							}
						}
					}
				}
				if (moveBack){
					// Reverse Boss walking direction
					boss[i].vY -= boss[i].vY;
				}*/
			}
		}
	}
}

void PlayGame::checkPlayerAttacksCollisionBoss() {
	// Objects
	for (int j = 0; j < obj.max; j++)
	{
		if (object[j].alive)
		{
			if (object[j].id != 2)
			{
				// Bosss
				for (int i = 0; i < sli.max; i++)
				{
					if (boss[i].alive)
					{
						// Get center of attack-particle (spawned by the player attacking)
						float bmx = boss[i].x+boss[i].w/2;
						float bmy = boss[i].y+boss[i].h/2;

						// Get center of object
						float bmx2 = object[j].x+object[j].w/2;
						float bmy2 = object[j].y+object[j].h/2;

						// Get distance
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));

						// If distance is less than 50 pixels
						if (distance < 384/2 +  50)
						{
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
							//----------------------------- Collision Detection based on player-attack hit-box and Boss hit-box -----------------------------//
							// Check collision between object and boss
							if (checkCollision(object[j].x, object[j].y, object[j].w, object[j].h, boss[i].x, boss[i].y, boss[i].w, boss[i].h))
							{
								// If attack-type: Slash
								if (player.attackType == 0)
								{
									// Play hit sound effect: Slash attack
					                Mix_PlayChannel(-1, sSlashHitBoss, 0);
								}

								// If attack-type: Down-stab
								else if (player.attackType == 1)
								{
									// Play hit sound effect: Down-stab attack
					                Mix_PlayChannel(-1, sDownStabHitTilec, 0);

									// Knockback player back in the air (only if doing down-stab)
									//player.y -= 10;
									//player.vY = -8;
								}

								// Knockback Boss
								{
									float distanceW = sqrt((bmx - bmx2) * (bmx - bmx2));
									float distanceH = sqrt((bmy - bmy2) * (bmy - bmy2));
									float tempVX 	= 0.8 * (bmx - bmx2) / distanceW;
									float tempVY 	= 0.8 * (bmy - bmy2) / distanceH;

									boss[i].vX += tempVX;
									boss[i].vY += tempVY;
								}

								// Knockback Boss - Depracated
								{
									/*// If particle is coming from the right of Boss
									int xDir, yDir;
									if (player.getCenterX() > object[j].x+object[j].w/2) {
										xDir = -1;
									}
									// If particle is coming from the left of Boss
									else {
										xDir = 1;
									}
									if (player.getCenterY() > object[j].y+object[j].h/2) {
										yDir = -1;
									}
									// If particle is coming from the left of Boss
									else {
										yDir = 1;
									}

									// Knockback enemy
									boss[i].vX += player.getKnockBackPower() * xDir;
									boss[i].vY += player.getKnockBackPower() * yDir;*/
								}

				                // Subtract boss health
				                boss[i].health -= player.getDamage();

				                // Increase player score
				                player.IncreaseScore(10);

				                // Show damage text (it will print how much damage the player did to the boss)
				    			std::stringstream tempss;
				    			tempss << player.getDamage();
				    			tex.spawn(text, boss[i].x+boss[i].w/2, boss[i].y-15, 0.0, -0.5, 150, tempss.str().c_str(), 1);
							}
							//----------------------------- Collision Detection based on player-attack hit-box and Boss hit-box -----------------------------//
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
						}
					}
				}
			}
		}
	}
}

void PlayGame::checkPlayerAttacksTileCollision() {
	for (int j = 0; j < obj.max; j++) {
		if (object[j].alive) {
			for (int i = 0; i < tl.max; i++) {
				if (tile[i].alive) {
					if (tile[i].collisionTile) {
						// Get ccenter of object's target
						float bmx = tile[i].x+tile[i].w/2;
						float bmy = tile[i].y+tile[i].h/2;

						// Get center of object
						float bmx2 = object[j].x+object[j].w/2;
						float bmy2 = object[j].y+object[j].h/2;

						// Distance of tile relative to attack-object
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));

						// If it goes less than 0, the game crashes
						if (distance <= 1) {
							distance = 1;
						}

						// If distance is less than 50 pixels
						if (distance < 50)
						{
							// Get angle of tile relative to attack-object
							float angle = atan2(bmy - bmy2,bmx - bmx2);
							angle = angle * (180 / 3.1416);
							if (angle < 0) {
								angle = 360 - (-angle);
							}

							// Handle radians, cos, sin
							float radians = (3.1415926536/180)*(angle);
							float Cos = floor(cos(radians)*10+0.5)/10;
							float Sin = floor(sin(radians)*10+0.5)/10;

							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
							//----------------------------- Collision Detection based on player-attack hit-box and tile hit-box ------------------------------//
							// Check collision between object and tile
							if (checkCollision(object[j].x, object[j].y, object[j].w, object[j].h, tile[i].x, tile[i].y, tile[i].w, tile[i].h))
							{
								// Spawn particle effect when player-attack object collides with tile
								for (double h=0.0; h< 180.0; h+=rand() % 10 + 40)
								{
									//for (double h=0.0; h< 360.0; h+=rand() % 10 + 10){
									int rands = rand() % 3 + 8;
									float newX;
									float newY;

									// Slash attack
									if (player.attackType == 0)
									{
										newX = object[j].x+object[j].w/2;
										newY = object[j].y+object[j].h/2;
									}

									// Down-stab attack
									else if (player.attackType == 1)
									{
										newX = object[j].x+object[j].w/2;
										newY = tile[i].y;
									}

									part.spawnParticleAngle(particles,
													   3,									// Type
													   newX-rands/2,						// X
													   newY-rands/2,						// Y
													   rands, rands,						// width, height
													   -h, randDouble(0.1, 1.4),				// angle, speed
													   0.0, 0, 0,									// damage
													   {200, 144, 40, 255}, 1,				// color and layer
													   1, 1,								// angleSpe, angleDir
													   rand() % 100 + 150, rand() % 2 + 5,	// alpha, alphaSpeed
													   rand() % 50 + 20, 0,					// deathtimer, deathtimerSpeed
													   false, 0,							// size death, death speed
													   rand() % 35 + 5);
								}

								// Remove Object
								object[j].alive = false;
								obj.count--;

								// If attack-type: Slash
								if (player.attackType == 0)
								{
									// Play hit sound effect: Slash attack
					                Mix_PlayChannel(-1, sSlashTilec, 0);
								}

								// If attack-type: Down-stab
								else if (player.attackType == 1)
								{
									// Play hit sound effect: Down-stab attack
					                Mix_PlayChannel(-1, sDownStabHitTilec, 0);
								}
							}
							//----------------------------- Collision Detection based on player-attack hit-box and tile hit-box ------------------------------//
							//--------------------------------------------------------------------------------------------------------------------------------//
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						}
					}
				}
			}
		}
	}
}

void PlayGame::checkPlayerAttacksBossParticleCollision() {
	for (int j = 0; j < obj.max; j++) {
		if (object[j].alive) {
			for (int i = 0; i < part.max; i++) {
				if (particles[i].alive) {
					if (particles[i].type == 1) {
						// Get ccenter of object's target
						float bmx = particles[i].x+particles[i].w/2;
						float bmy = particles[i].y+particles[i].h/2;

						// Get center of object
						float bmx2 = object[j].x+object[j].w/2;
						float bmy2 = object[j].y+object[j].h/2;

						// Distance of particles relative to attack-object
						float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
											  (bmy - bmy2) * (bmy - bmy2));

						// If it goes less than 0, the game crashes
						if (distance <= 1) {
							distance = 1;
						}

						// If distance is less than 50 pixels
						if (distance < 600)
						{
							// Get angle of particles relative to attack-object
							float angle = atan2(bmy - bmy2,bmx - bmx2);
							angle = angle * (180 / 3.1416);
							if (angle < 0) {
								angle = 360 - (-angle);
							}

							// Handle radians, cos, sin
							float radians = (3.1415926536/180)*(angle);
							float Cos = floor(cos(radians)*10+0.5)/10;
							float Sin = floor(sin(radians)*10+0.5)/10;

							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//--------------------------------------------------------------------------------------------------------------------------------//
							//----------------------------- Collision Detection based on player-attack hit-box and particles hit-box ------------------------------//
							// Check collision between object and particles
							//if (checkCollision(object[j].x, object[j].y, object[j].w, object[j].h, particles[i].x, particles[i].y, particles[i].w, particles[i].h))
							//{


							// Circle Collision
							if (distance < object[j].h/2 + particles[i].w/2)
							{

								// Reduce health of Enemey Particle
								particles[i].health -= player.getDamage();;

								// Show damage text (it will print how much damage the player did to the boss)
								std::stringstream tempss;
								tempss << player.getDamage();
								tex.spawn(text, particles[i].x+particles[i].w/2, particles[i].y-15, 0.0, -0.5, 150, tempss.str().c_str(), 1);

								// Remove Object
								object[j].alive = false;
								obj.count--;

								// If attack-type: Slash
								if (player.attackType == 0)
								{
									// Play hit sound effect: Slash attack
					                Mix_PlayChannel(-1, sSlashHitBoss, 0);
								}
							}
							//----------------------------- Collision Detection based on player-attack hit-box and particles hit-box ------------------------------//
							//--------------------------------------------------------------------------------------------------------------------------------//
							////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						}
					}
				}
			}
		}
	}
}


void PlayGame::checkBossAttacksCollisionPlayer() {
	for (int j = 0; j < obj.max; j++)
	{
		if (object[j].alive)
		{
			if (object[j].id == 2)
			{
				// Get ccenter of object's target
				float bmx = player.getCenterX();
				float bmy = player.getCenterY();

				// Get center of object
				float bmx2 = object[j].x+object[j].w/2;
				float bmy2 = object[j].y+object[j].h/2;

				// Distance of tile relative to attack-object
				float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
									  (bmy - bmy2) * (bmy - bmy2));

				// If it goes less than 0, the game crashes
				if (distance <= 1) {
					distance = 1;
				}

				// If distance is less than 50 pixels
				if (distance < 50)
				{
					// If Player is currently invurnerable state on
					if (player.getInvurnerableStatus())
					{
						// Remove object
						object[j].alive = false;
						obj.count--;

						// Spawn particle effect
						for (double i=0.0; i< 360.0; i+=rand() % 10 + 40){
							int rands = rand() % 11 + 3;
							float newX = player.x+player.w/2;
							float newY = player.y+player.h/2;
							part.spawnParticleAngle(particles, 2,
												newX-rands/2,
												newY-rands/2,
											   rands, rands,
											   i, randDouble(2.1, 5.1),
											   0.0, 0, 0,
											   {144, 144, 144, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, randDouble(0.1, 0.7),
											   100, 10);
						}

						// Reference to exact boss (this was stored in the object when it was created
						int i = object[j].CreatorIndex;

						// If that boss is still alive
						if (boss[i].alive)
						{
							// If particle is coming from the right of Boss
							int xDir;

							// Determine which side the player is from the Boss
							if (player.getCenterX() > boss[i].x+boss[i].w/2) xDir = -1; else xDir = 1;

							// Knockback boss
							boss[i].vX += player.getKnockBackPower() * xDir;
							boss[i].vY = -4;

							// play sound effect
							Mix_PlayChannel(-1, sParrySuccess, 0);
						}
					}

					// If player is currently parrying
					else if (player.getParryStatus())
					{
						// Remove object
						object[j].alive = false;
						obj.count--;

						// Immediately stop parrying, and spawn a counter attack
						{
							// Stop parry, and reset its cool down
							player.ResetParry();

							// Activate player shield
							player.ActivateInvurnerability();

							// Launch attack
							player.SlashAttack();
						}

						// Spawn particle effect
						for (double i=0.0; i< 360.0; i+=rand() % 10 + 40){
							int rands = rand() % 11 + 3;
							float newX = player.x+player.w/2;
							float newY = player.y+player.h/2;
							/*part.spawnParticleAngle(particles, 2,
												newX-rands/2, newY-rands/2,
											   rands, rands,
											   i, randDouble(2.1, 5.1),
											   0.0,
											   {210, 144, 40, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, 0.1,
											   rand() % 15 + 2, 1);*/
							part.spawnParticleAngle(particles, 2,
												newX-rands/2,
												newY-rands/2,
											   rands, rands,
											   i, randDouble(2.1, 5.1),
											   0.0, 0, 0,
											   {210, 144, 40, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, randDouble(0.1, 0.7),
											   100, 10);
						}

						// play sound effect
						Mix_PlayChannel(-1, sParrySuccess, 0);
					}

					////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//--------------------------------------------------------------------------------------------------------------------------------//
					//----------------------------- Collision Detection based on Boss-attack hit-box and Player hit-box -----------------------------//
					// Check collision between object and boss
					else if (checkCollision(object[j].x, object[j].y, object[j].w, object[j].h, player.x, player.y, player.w, player.h))
					{
						// If player is on right side of object
						int xDir;
						if (player.getCenterX() > object[j].x+object[j].w/2) {
							xDir = 1;
						}
						// If player is on left side of object
						else {
							xDir = -1;
						}

						// Knockback player
						//player.stunned = true;
						player.vX += player.getKnockBackPower() * xDir;
						player.vY = -4;

						// Subtract player health
						player.health -= 20;

						// spawn blood particle effect
						//for (double i=0.0; i< 360.0; i+=rand() % 10 + 25){
							for (double i=0.0; i< 360.0; i+=rand() % 10 + 25){
							int rands = rand() % 11 + 3;
							float newX = player.x+player.w/2;
							float newY = player.y+player.h/2;
							/*part.spawnParticleAngle(particles, 2,
												newX-rands/2,
												newY-rands/2,
											   rands, rands,
											   i, randDouble(2.1, 5.1),
											   0.0,
											   {240, 0, 0, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, 0.1,
											   rand() % 15 + 2, 1);*/
							part.spawnParticleAngle(particles, 2,
												newX-rands/2,
												newY-rands/2,
											   rands, rands,
											   i, randDouble(2.1, 5.1),
											   0.0, 0, 0,
											   {240, 0, 0, 255}, 1,
											   1, 1,
											   rand() % 100 + 150, rand() % 2 + 5,
											   rand() % 50 + 90, 0,
											   true, randDouble(0.1, 0.7),
											   100, 10);
						}

						// play sound effect
						Mix_PlayChannel(-1, sSlashHitBoss, 0);

						// Show damage text (it will print how much damage the player did to the boss)
						std::stringstream tempss;
						tempss << 20;
						tex.spawn(text, player.x+player.w/2, player.y-15, 0.0, -0.5, 150, tempss.str().c_str(), 1);
					}
					//----------------------------- Collision Detection based on Boss-attack hit-box and Player hit-box -----------------------------//
					////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//--------------------------------------------------------------------------------------------------------------------------------//
				}
			}
		}
	}
}

void PlayGame::checkCollisionBossPlayer() {

	// boss circle collision check with other bosss
	for (int i = 0; i < sli.max; i++) {
		if (boss[i].alive) {

			// Center of Boss target for collision
			float bmx = player.x+player.w/2;
			float bmy = player.y+player.h/2;

			// Center of Boss
			float bmx2 = boss[i].x+boss[i].w/2;
			float bmy2 = boss[i].y+boss[i].h/2;

			// Angel stuff
			float angle = atan2(bmy - bmy2,bmx - bmx2);
			angle = angle * (180 / 3.1416);
			if (angle < 0) {
				angle = 360 - (-angle);
			}

			// Radiands stuff
			float radians = (3.1415926536/180)*(angle);
			float Cos = floor(cos(radians)*10+0.5)/10;
			float Sin = floor(sin(radians)*10+0.5)/10;

			// Distance
			float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
								  (bmy - bmy2) * (bmy - bmy2));


			float distanceW = sqrt((bmx - bmx2) * (bmx - bmx2));
			float distanceH = sqrt((bmy - bmy2) * (bmy - bmy2));

			// Do this or you will crash
			if (distance <= 0.01) {
				distance = 0.01;
			}
			if (distanceW <= 0.01) {
				distanceW = 0.01;
			}
			if (distanceH <= 0.01) {
				distanceH = 0.01;
			}

			//float tempVX = (cos( (3.14159265/180)*(angle) ));;
			//float tempVY = (sin( (3.14159265/180)*(angle) ));
			//float tempVel = 5 * (bmx - bmx2) / distance;

			////////////////////////////////////////////////////////////////////////
			//--------------------------------------------------------------------//
			//-------------------- Circle Collision Detection --------------------//
			{
				// Get new direction for Player to go
				float tempVX 	= player.velMax * (bmx - bmx2) / distanceW;
				float tempVY 	= player.velMax * (bmy - bmy2) / distanceH;

				// If distance between both is lower than X, then have them go away from each other
				if (distance < boss[i].w/4 + player.w/2)
				{
					// Apply new direction to Player's x & y velocity
					player.vX = tempVX;
					//player.x += tempVX;

					player.vY = tempVY;
					//player.y += tempVY;
				}
			}
			//-------------------- Circle Collision Detection --------------------//
			//--------------------------------------------------------------------//
			////////////////////////////////////////////////////////////////////////
		}
	}
}

// Check collision: Enemy Particle and Player
void PlayGame::checkCollisionParticlePlayer() {
	// Player
	if (player.alive && !player.getInvurnerableStatus() && !player.getDashStatus()){
		// Particle
		for (int i = 0; i < part.max; i++)
		{
			if (particles[i].alive)
			{
				// enemy particle hitting Player
				if (particles[i].type == 1)
				{
					// particle collision with player using a CIRCLE
					float bmx = player.x + player.w/2;
					float bmy = player.y + player.h/2;
					float bmx2 = particles[i].x + particles[i].w/2;
					float bmy2 = particles[i].y + particles[i].h/2;
					float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
										  (bmy - bmy2) * (bmy - bmy2));
					// collision occurred
					if (distance < player.getW()/2 + particles[i].w/2)
					{
						///////////////////////////////////////////////////////////////////
						//---------------------------------------------------------------//
						//------------------------ Parry Successful ---------------------//

						// If player is currently parrying
						if (player.getParryStatus())
						{
							// Spawn Counter Attack: Slash Attack Wave
							{
								// Change Enemy bullet into a Player bullet!
								particles[i].type = -1;

								// Knock back enemy bullet particles
								{
									float distanceW = sqrt((bmx - bmx2) * (bmx - bmx2));
									float distanceH = sqrt((bmy - bmy2) * (bmy - bmy2));
									float tempVX 	= 5 * (bmx - bmx2) / distanceW;
									float tempVY 	= 5 * (bmy - bmy2) / distanceH;

									particles[i].vX += particles[i].vX * -2;
									particles[i].vY += particles[i].vY * -2;
								}
							}

							// Extend Parry duration
							player.ExtendParryDuration();

							// Increase score depending on size of bullet parried
							{
								// Small blue bullet from Boss
								if (particles[i].w == 20)
								{
									// Increase health
									player.IncreaseHealth(1);

					                // Increase player score
					                player.IncreaseScore(1);
								}

								// Small red bullet from Boss
								if (particles[i].w == 32)
								{
									// Increase health
									player.IncreaseHealth(2);

					                // Increase player score
					                player.IncreaseScore(4);
								}

								// Large red bullet from Boss
								if (particles[i].w == 96)
								{
									// Increase health
									player.IncreaseHealth(25);

					                // Increase player score
					                player.IncreaseScore(4);
								}
							}

							// Spawn particle VFX
							for (double i=0.0; i< 360.0; i+=rand() % 10 + 40){
								int rands = rand() % 11 + 3;
								float newX = player.x+player.w/2;
								float newY = player.y+player.h/2;
								part.spawnParticleAngle(particles, 2,
													newX-rands/2,
													newY-rands/2,
												   rands, rands,
												   i, randDouble(2.1, 5.1),
												   0.0, 0, 0,
												   {210, 144, 40, 255}, 1,
												   1, 1,
												   rand() % 100 + 150, rand() % 2 + 5,
												   rand() % 50 + 90, 0,
												   true, randDouble(0.1, 0.7),
												   100, 10);
							}

							// play sound effect
							Mix_PlayChannel(-1, sParrySuccess, 0);
						}
						//------------------------ Parry Successful ---------------------//
						//---------------------------------------------------------------//
						///////////////////////////////////////////////////////////////////

						///////////////////////////////////////////////////////////////////
						//---------------------------------------------------------------//
						//---------------- NOT Parrying, so hurt the Player -------------//
						else {
							// Knock back player
							{
								float distanceW = sqrt((bmx - bmx2) * (bmx - bmx2));
								float distanceH = sqrt((bmy - bmy2) * (bmy - bmy2));
								float tempVX 	= 5 * (bmx - bmx2) / distanceW;
								float tempVY 	= 5 * (bmy - bmy2) / distanceH;

								player.vX += tempVX;
								player.vY += tempVY;
							}

							// spawn blood particle effect
							for (double i=0.0; i< 360.0; i+=rand() % 10 + 10){
								int rands = rand() % 11 + 3;
								float newX = player.x+player.w/2;
								float newY = player.y+player.h/2;
								part.spawnParticleAngle(particles, 2,
													newX-rands/2,
													newY-rands/2,
												   rands, rands,
												   i, randDouble(2.1, 5.1),
												   0.0, 0, 0,
												   {255, 0, 0, 255}, 1,
												   1, 1,
												   rand() % 100 + 150, rand() % 2 + 5,
												   rand() % 50 + 90, 0,
												   true, 0.1,
												   rand() % 15 + 2, 1);
							}

							// play sound effect
							Mix_PlayChannel(-1, sSlashHitBoss, 0);

							// Subtract player health
							player.health -= particles[i].damage;

			                // Show damage text (it will print how much damage the player did to the boss)
			    			std::stringstream tempss;
			    			tempss << particles[i].damage;
			    			tex.spawn(text, player.x+player.w/2, player.y-15, 0.0, -0.5, 150, tempss.str().c_str(), 1);

							// remove particle
							particles[i].alive = false;
							part.count--;

						}
						//---------------- NOT Parrying, so hurt the Player -------------//
						//---------------------------------------------------------------//
						///////////////////////////////////////////////////////////////////
					}
				}
			}
		}	// end Particle
	}	// end Player
}


void PlayGame::checkCollisionParticleParticle() {
	// Player particle
	for (int i = 0; i < part.max; i++)
	{
		if (particles[i].alive)
		{
			if (particles[i].type == 0)
			{
				for (int j = 0; j < part.max; j++)
				{
					// Make sure we are not comparing the same particle
					if (i != j) {
						if (particles[j].alive)
						{
							if (particles[j].type == 1)
							{
								// Particle target center
								float bmx = particles[j].x + particles[j].w/2;
								float bmy = particles[j].y + particles[j].h/2;

								// Particle center
								float bmx2 = particles[i].x + particles[i].w/2;
								float bmy2 = particles[i].y + particles[i].h/2;
								float distance = sqrt((bmx - bmx2) * (bmx - bmx2)+
													  (bmy - bmy2) * (bmy - bmy2));


								// Circle Collision
								if (distance < particles[j].w/2 + particles[i].w/2)
								{
									// Reduce health of Enemey Particle
									particles[j].health -= particles[i].dmgToParticles;

									// Show damage text (it will print how much damage the player did to the boss)
									std::stringstream tempss;
									tempss << particles[i].dmgToParticles;
									tex.spawn(text, particles[j].x+particles[j].w/2, particles[j].y-15, 0.0, -0.5, 150, tempss.str().c_str(), 1);

									// Remove Player particle next
									particles[i].time = 0;
									particles[i].alive = false;
									part.count--;

									// Spawn particle effect
									for (double i=0.0; i< 360.0; i+=rand() % 10 + 40){
										int rands = rand() % 11 + 3;
										float newX = bmx;
										float newY = bmy;
										part.spawnParticleAngle(particles, 2,
															newX-rands/2,
															newY-rands/2,
														   rands, rands,
														   i, randDouble(2.1, 5.1),
														   0.0, 0, 0,
														   {210, 144, 40, 255}, 1,
														   1, 1,
														   rand() % 100 + 150, rand() % 2 + 5,
														   rand() % 50 + 90, 0,
														   true, randDouble(0.1, 0.7),
														   100, 10);
									}

									// play sound effect
									Mix_PlayChannel(-1, sParrySuccess, 0);
								}
							}
						}
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------ Save Functions ------------------------------------------------------//

// Save player spawn point
std::string PlayGame::saveSpawnPoint(){
	/*std::ofstream fileSpawnSave;
	fileSpawnSave.open( "data/maps/spawn.mp" );
		std::stringstream tempss;
		tempss << spawnX << " " << spawnY;
		fileSpawnSave << tempss.str().c_str();

		fileSpawnSave.close();*/

	std::stringstream tempss;
	tempss << spawnX << " " << spawnY;
	return tempss.str().c_str();
}

// Load player spawn point
void PlayGame::loadSpawnPoint(){

	// Set file path and name
	std::stringstream tempss;
	tempss << "data/maps/level";
	tempss << this->LevelToLoad;
	tempss << "/spawn.mp";

	// open config file
	std::fstream fileSpawn(tempss.str().c_str());
	if (fileSpawn.is_open()) {
		fileSpawn >>  spawnX >> spawnY;
		player.x		= spawnX;
		player.y		= spawnY;
	}
	// Error opening spawn.mp file
	else{
		// Set file path and name
		std::stringstream tempss;
		tempss << "data/maps/level";
		tempss << this->LevelToLoad;
		tempss << "/spawn.mp";

		// Creating default spawn.mp
		std::ofstream newSpawnFile(tempss.str().c_str());
		// If successfully created spawn.mp file
		if (newSpawnFile.is_open()) {
			newSpawnFile << "0 0";
		}
		// Opening and creating default config file failed, set manually
		else{
			spawnX			= 0;
			spawnY			= 0;
			player.x		= spawnX;
			player.y		= spawnY;
		}
		newSpawnFile.close();
	}
	fileSpawn.close();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------- Controls ---------------------------------------------------------//

// Mouse Pressed
PlayGame::Result PlayGame::mousePressed(SDL_Event event){
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = true;
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick = true;
			//zom.spawn(zombie, mx+camx-80/2, my+camy-80/2, 80, 80, 0.0, randDouble(3.6, 4.4), 0);
			//spaw.spawn(spawner, newMx+camx-48/2, newMy+camy-48/2, 48, 48);


			// If we are on editor mode
			if (editor) {
				// If not on Tile-bar, place other tiles
				if (!tb.touching) {

					// If bosss is our placement selection
					if (place_type == 2) {

						// Spawn bosss
						sli.Spawn(boss, mex+camx, mey+camy, 256, 256, 0.0, randDouble(3.6, 4.4), 0);
					}
				}
			}
		}
	}
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick = false;
		}
	}
	return Nothing;
}

// Mouse Released
PlayGame::Result PlayGame::mouseReleased(SDL_Event event){
	PlayGame::Result result = Nothing;
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftClick = false;
			// Perform actions
			/*for (int i=0; i<3; i++) {
				// Check Mouse hover w/ Menu item
				SDL_Rect a = {mx, my, 1, 1};
				SDL_Rect b = {button[i].x, button[i].y, button[i].w, button[i].h};
				// If mouse is hovering over menu item then render specifically
				if (checkCollision(a, b)) {
					if (i == 0) {			// Back
						result = Back;
					}else if (i == 1) {		// Save Character
						// function here to save customized character to file
						result = Nothing;
					}else if (i == 2) {		// Start Game
						result = StartGame;
					}
				}
			}*/
		}
		if (event.button.button == SDL_BUTTON_RIGHT) {
			rightClick = false;
		}
	}
	return result;
}


// Editor controls
void PlayGame::editorOnKeyDown( SDL_Keycode sym )
{
	switch (sym) {
	case SDLK_UP:				// camera up
		camUp = true;
		break;
	case SDLK_DOWN:				// camera down
		camDown = true;
		break;
	case SDLK_LEFT:				// camera left
		camLeft = true;
		break;
	case SDLK_RIGHT:			// camera right
		camRight = true;
		break;
	case SDLK_y:				// camera lock
		camlock = (!camlock);
		break;
	case SDLK_w:
		if (shift) {
			tb.moveBarSelection(tilebar, "up");
		}else{
			camy-=10;
		}
		break;
	case SDLK_x:				// Save spawn point
		spawnX = newMx+camx;
		spawnY = newMy+camy;
		if (shift) {
			//loadSpawnPoint();
		}else{
			//saveSpawnPoint();
		}
		break;
	case SDLK_s:
		if (shift) {
			tb.moveBarSelection(tilebar, "down");
		}else{
			camy+=10;
		}
		break;
	case SDLK_a:
		if (shift) {
			tb.moveBarSelection(tilebar, "left");
		}else{
			camx-=10;
		}
		break;
	case SDLK_d:
		if (shift) {
			tb.moveBarSelection(tilebar, "right");
		}else{
			camx+=10;
		}
		break;
	case SDLK_q:								// Change place type (i.e. Tiles or Collision Tiles)
		place_type++;
		break;
	case SDLK_TAB:								// Toggle hide other layers
		tl.hideOtherLayers = (!tl.hideOtherLayers);
		break;
	case SDLK_k:{								// Change collisionTile, if we should place a Tile with Collision

			tl.collisionTile = (!tl.collisionTile);
			break;
		}
	case SDLK_l:{								// Change layer, if shoudl place a Tile at this certain layer
			if (shift){
				if (place_type==0) {
					tl.layer--;
				}else if (place_type==1) {
					tlc.layer--;
				}
			}else{
				if (place_type==0) {
					tl.layer++;
				}else if (place_type==1) {
					tlc.layer++;
				}
			}
			break;
		}
	case SDLK_i:								// Change tile id to place
		if (shift) {
			if (place_type==0) {
				tl.id--;
			}else if (place_type==1) {
				tlc.id--;
			}
		}else{
			if (place_type==0) {
				tl.id++;
			}else if (place_type==1) {
				tlc.id++;
			}
		}
		break;
	case SDLK_COMMA:							// Toggle if the Tile should be a collision tile
		tl.changeTileCollision(tile);
		break;
	case SDLK_PERIOD:							// Change layer of the Tile our mouse is hovering over
		if (shift)
			tl.changeTileLayer(tile, -1);
		else
			tl.changeTileLayer(tile, 1);
		break;
	case SDLK_c:								// Tile, copy Tile
		if (editor) {
			if (place_type == 0 ) {
				tl.copyTile(tile);
			}else if (place_type == 1) {
				tlc.Copy(tilec);
			}
		}
		break;
	case SDLK_SPACE:							// Tile, remove all Tiles (and Collision Tiles)
		if (editor)
		{
			// If holding shift, only delete the currently selected place_type
			if (shift)
			{
				// Currently selected: Textured tiles
				if (place_type == 0 )
				{
					// Remove textured tiles
					tl.RemoveAll(tile);
				}

				// Currently selected: Collision tiles
				else if (place_type == 1)
				{
					// Remove collision tiles
					tlc.RemoveAll(tilec);
				}

				// Currently selected: Boss Objects
				else if (place_type == 2)
				{
					// Remove bosss
					sli.RemoveAll(boss);
				}
			}
			// Remove all tiles
			else {
				// Remove textured tiles
				tl.RemoveAll(tile);

				// Remove collision tiles
				//tlc.Clear(tilec);

				// Remove bosss
				sli.RemoveAll(boss);
			}
		}
		break;
	case SDLK_LEFTBRACKET:						// Tile, subtract parry in x-axis
		if (place_type == 0) {
			if (tl.multiW > 1) {
				tl.multiW -= 1;
			}
		}else if (place_type == 1) {
			if (tlc.multiW > 1) {
				tlc.multiW -= 1;
			}
		}
		break;
	case SDLK_RIGHTBRACKET:						// Tile, add parry in x-axis
		if (place_type == 0) {
			tl.multiW += 1;
		}else if (place_type == 1) {
			tlc.multiW += 1;
		}
		break;
	case SDLK_MINUS:							// Tile, subtract parry in y-axis
		if (place_type == 0) {
			if (tl.multiH> 1) {
				tl.multiH -= 1;
			}
		}else if (place_type == 1) {
			if (tlc.multiH> 1) {
				tlc.multiH -= 1;
			}
		}
		break;
	case SDLK_EQUALS:							// Tile, add parry in y-axis
		if (place_type == 0) {
			tl.multiH += 1;
		}else if (place_type == 1) {
			tlc.multiH += 1;
		}
		break;
	case SDLK_9: {								// Load Room

			// This will hold all our data
			LoadLevel();

			// Print message to screen
			std::stringstream tempss;
			tempss << "Loading Data...";
			tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str(), 0);

			break;
		}
	case SDLK_0: {								// Save Room

			// This will hold all our data
			std::stringstream aVeryLongString;

			{
				// Save Tile data
				aVeryLongString << tl.SaveData(tile);

				// Save Tilec data
				aVeryLongString << tlc.SaveData(tilec);

				// Save Boss data
				aVeryLongString << sli.SaveData(boss);

				// Save Player spawn point
				aVeryLongString << saveSpawnPoint();

				std::ofstream fileParrys2;
				std::stringstream tempNameParryNum2;
				//tempNameParryNum << "data/maps/" << tempLevelSaveName.str().c_str();
				//tempNameParryNum << saveFileName.str().c_str() << "TileNum.mp";
				tempNameParryNum2 << "data/maps/";
				tempNameParryNum2 << "Level";
				tempNameParryNum2 << this->LevelToLoad;
				tempNameParryNum2 << ".mp";

				// Open file
				fileParrys2.open(tempNameParryNum2.str().c_str());

				// Save tile count
				fileParrys2 << aVeryLongString.str().c_str();

				// Close file
				fileParrys2.close();
			}

			// Print message to screen
			std::stringstream tempss;
			tempss << "Saving data...";
			tex.spawn(text, 0, 0, 0.0, 0.0, 255, tempss.str().c_str(), 0);
			break;
		}
	case SDLK_1:							// increase or decrease width of tile being placed
		if (shift) {
			if (place_type == 0) {
				tl.tilew -= 8;
			}else if (place_type == 1) {
				tlc.tilew -= 8;
			}
		}else{
			if (place_type == 0) {
				tl.tilew += 8;
			}else if (place_type == 1) {
				tlc.tilew += 8;
			}
		}
		break;
	case SDLK_2:							// increase or decrease height of tile being placed
		if (shift) {
			if (place_type == 0) {
				tl.tileh -= 8;
			}else if (place_type == 1) {
				tlc.tileh -= 8;
			}
		}else{
			if (place_type == 0) {
				tl.tileh += 8;
			}else if (place_type == 1) {
				tlc.tileh += 8;
			}
		}
		break;
		// Set render size
	case SDLK_F5:							// Spawn player on spawner
		player.x = spawnX;
		player.y = spawnY;
		break;
	}
}
void PlayGame::editorOnKeyUp( SDL_Keycode sym ) {
	switch (sym) {
	case SDLK_UP:
		camUp = false;
		break;
	case SDLK_DOWN:
		camDown = false;
		break;
	case SDLK_LEFT:
		camLeft = false;
		break;
	case SDLK_RIGHT:
		camRight = false;
		break;
	}
}


/* Global functions used in World */


bool PlayGame::checkCollisionRect( SDL_Rect a, SDL_Rect b )
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

// Load saved configurations
void PlayGame::LoadCFG() {

	// Load Video settings from file
	loadVideoCFG();

	// Load Audio settings from file
	loadAudioCFG();

	// Apply audio configurations
	loadAudioCFG();						// Load Audio settings from file
	applyMasterAudioCFG();				// Apply audio configurations

	// Apply video configurations
	//applyVideoCFG(gWindow);
}

void PlayGame::LoadHighScore() {
	std::stringstream filePath;
	filePath << "data/maps/highscore";
	filePath << LevelToLoad;
	filePath << ".txt";

	int previousHighScore = -1;
	std::fstream fileTileDataL(filePath.str().c_str());

	// File exists, load last high score
	if (fileTileDataL.is_open()) {
		fileTileDataL >> previousHighScore;
		fileTileDataL.close();

		// Apply that high score to current level for Render
		player.ApplyHighScore(previousHighScore);
	}

	// File does NOT exist, create file with default of 0 high score
	else {
		std::cout<< "File does not exist on Loading Highscore, creating new\n";
		{
			std::stringstream filePath;
			filePath << "data/maps/highscore";
			filePath << LevelToLoad;
			filePath << ".txt";

			std::ofstream fileSave;
			fileSave.open(filePath.str().c_str());
			fileSave << 0;
			fileSave.close();

			// Apply that high score to current level for Render
			player.ApplyHighScore(0);
		}
	}
}

// Load level
void PlayGame::LoadLevel()
{

	// Remove everything
	{
		// Remove particles tiles
		part.RemoveAll(particles);

		// Remove textured tiles
		tl.RemoveAll(tile);

		// Remove collision tiles
		tlc.RemoveAll(tilec);

		// Remove bosss
		sli.RemoveAll(boss);
	}

	// Set file path and name
	std::stringstream tempss;
	tempss << "data/maps/";
	tempss << "Level";
	tempss << LevelToLoad;
	tempss << ".mp";
	std::fstream fileTileDataL(tempss.str().c_str());

	// While file is open
	while( fileTileDataL.good() )
	{
		// Load Tile data
		tl.LoadData(tile, fileTileDataL);

		// Load Tilec data
		tlc.LoadData(tilec, fileTileDataL);

		// Load Boss data
		sli.LoadData(boss, fileTileDataL);

		// Load Player spawn point
		fileTileDataL >>  spawnX >> spawnY;
		player.x		= spawnX;
		player.y		= spawnY;

		// Break out of file
		break;
	}
	fileTileDataL.close();
}
