#include "Game.h"

int main(int argc, char *argv[]) {
	Game game;

	game.Start();

	return 0;
}


// Random stuff

/*
 * ShowSplashScreen() -> ShowMenu()
 * 						 -> mainMenu.Show(gWindow, gRenderer, result);
 *						 -> mainMenu.Render() - its crashing here, why?
 *						 -> [x] - Fixed: it was rendering some stuff from Joystick variables that were
 *						 	      not initialized so we just commented it out.
 *
 * Other Notes:
 * - the screen is being render at 1920x1080, but we shrinking it to 1280x720
 *
 *	How Saving Works**** (3-11-2022)
 *	- Part I: Save 		Tile count.
 *	- Part II save: 	Save Tile data.
 *	- Part III save: 	Save Tilec count.
 *	- Part IV save: 	Save Tilec data.
 *	- Part V save: 		Save Slime data.
 *	- Part VI save: 	Save Player spawn point
 *
 *
 */




/* PlayGame */
// TODO [ ] - fix players class so that we can easily create more players
// TODO [x] (3-8-2022) - Make hit box on player's attack wider, but less height
// TODO [x] (3-8-2022) - Check collision for player's attack hit-box and Slime's hit-box with a square check collision rather than distance collision


/* Customize Character */
// TODO [ ] - how to save Texture as a PNG file format? possible solution:: find another library for loading png
// TODO [ ] - replace in-game-text-buttons with drawn buttons from photoshop

// TODO [ ] - get input from player so they can input their names
// TODO [x] - make pause menu

/* Tilebar */
// TODO [x] - write code so we don't render tile-bar if it's not on screen

/* Options */
// TODO [x] - Code an Options Menu inside options to be used in both MainMenu and during GamePlay
//			  After coding of Options Menu, the goal is to remove the Pause.cpp class.

/* Tiles */
// TODO [x] - I fixed the delete underlying by decreasing mouse size by 4
// TODO [x] - find out why program crashes when trying to make more than 512 tiles
//			- I never found out why, but it just started working after changing some code :/

/* Player */
// TODO [x] - fix players position when they Attack facing the Left side
// TODO [x] - fix player size so that they may be able to go through a tight squeeze of 48x48



// TODO [x] - attacking a slime makes it look weird when you try to hit
//			  a slime out of the world and it gets stuck on the orange collision block


// TODO [x] (3-7-2022) - Save and load Slime data to files for loading
// TODO [x] (3-7-2022) - Replace Title Menu screen from our previous Dungeon Game,
//						 and also get pause-menu from previous Dungeon Game

/* No specific class */
// TODO [x] - Go left and right
// TODO [x] - Create monsters class (i.e. slime, goblin)
// TODO [x] (3-8-2022) - Create TextNFont.cpp class - This class can be included by any other classes that want to render text
// TODO [x] - Create sound effect for down stab hitting the ground
// TODO [x] - Change sound effect for hitting a slime with player slash attack
// TODO [x] - Down stab hit-box is not aligned with players sword
// TODO [x] (3-10-2022) - Slime.cpp -> Add code to play sound effect when slime attacks
// TODO [ ] (3-12-2022) - Create animation attack for Slime in photoshop
// TODO [ ] (3-12-2022) - Add more monsters
// TODO [ ] (3-12-2022) - Add objective to game
// TODO [x] (3-15-2022) - Make walk VFX nicer
// TODO [x] (3-15-2022) - Give dash a sound effect
// TODO [x] (3-15-2022) - Fix collision check with Slimer and Tiles
// TODO [x] (3-15-2022) - Create sprite in photoshop for Player that animates when the Player uses teh Dash ability
// TODO [x] (3-16-2022) - Make player invulnerable when dashing
// TODO [x] (3-18-2022) - Change Player bullet texture to Slash Textures instead of Bullet textures


// TODO (2) [x] (3-10-2022) - Create level selection Scene
// TODO (2) [x] (3-10-2022) - Dash ability

// [ ] reanable Highscore render in Player class
/*Reanable:
 * [ ] applying damage multiplier for player
 * [ ] rendering text for damage multiplier above player
 * [ ] Players.cpp - Play SFX when Player has no more mana, but is still trying to shoot out SlashAttack particles
 * [ ] Players.cpp (2) - Create summary page after defeating the boss and add things like:
 * 					 1. How many bullets the Player shot
 * 					 2. How many bullets the Player took
 * 					 3. How many bullets the Player destroyed
 * 					 4. How many bullets the Player blocked/parried
 * 					 5. How many times the Player died
 * [ ] - SaveHighScore in Player.cpp not working
 * [ ] - re enable Keyboard key P and H to show Debug and Enable Editor mode
 * [ ] - Set Editor, Debug, and camlock back to true upon Init() of PlayGame.cpp
 * [ ] - Set saving map and loading map SDLK_0 and _1
 * [ ] Line 1288 PlayGame.cpp, show congrats screen when Boss dies, and do for Slash attack also
 *
 */


