#include "Game.h"

int main(int argc, char *argv[]) {
	Game game;

	game.Start();

	return 0;
}

// Notes

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
 *	(Do not delete)
 *	- Part I: Save 		Tile count.
 *	- Part II save: 	Save Tile data.
 *	- Part III save: 	Save Tilec count.
 *	- Part IV save: 	Save Tilec data.
 *	- Part V save: 		Save Slime data.
 *	- Part VI save: 	Save Player spawn point
 *
 *
 */

/*	Stuff to do :)
 *
 *
 * [Game Features]
 *
 * // PlayGame.cpp
 * TODO [x] (3-8-2022) - Make hit box on player's attack wider, but less height
 * TODO [x] (3-8-2022) - Check collision for player's attack hit-box and Slime's
 * 						hit-box with a square check collision rather than distance collision
 *
 *
 *
 *
 *
 *
 * // Player.cpp
 * TODO [x] - fix players position when they Attack facing the Left side
 * TODO [x] - fix player size so that they may be able to go through a tight squeeze of 48x48
 * TODO [x] (3-7-2022) - Save and load Boss data to files for loading
 * TODO [x] (3-7-2022) - Replace Title Menu screen from our previous Dungeon Game,
 *						 and also get pause-menu from previous Dungeon Game
 * TODO [x] - Code an Options Menu inside options to be used in both MainMenu and during GamePlay
 *			  After coding of Options Menu, the goal is to remove the Pause.cpp class.
 * TODO [ ] - Create shadows under Player's texture
 *
 *
 *
 *
 *
 *
 * //Customize Character
 * TODO [ ] - how to save Texture as a PNG file format? possible solution:: find another library for loading png
 * TODO [ ] - replace in-game-text-buttons with drawn buttons from photoshop
 *
 *
 *
 *
 *
 *
 * // No specific class
 * TODO [x] - Go left and right, up and down
 * TODO [x] - Create monsters class (i.e. slime, goblin)
 * TODO [x] (3-8-2022) - Create TextNFont.cpp class - This class can be included by any other classes that want to render text
 * TODO [x] - Create sound effect for down stab hitting the ground
 * TODO [x] - Change sound effect for hitting a slime with player slash attack
 * TODO [x] - Down stab hit-box is not aligned with players sword
 * TODO [x] (3-10-2022) - Slime.cpp -> Add code to play sound effect when slime attacks
 * TODO [x] (3-10-2022) - Create level selection Scene
 * TODO [x] (3-10-2022) - Dash ability
 * TODO [x] (3-12-2022) - Add objective to game (defeat the boss)
 * TODO [x] (3-15-2022) - Make walk VFX nicer
 * TODO [x] (3-15-2022) - Give dash a sound effect
 * TODO [x] (3-15-2022) - Fix collision check with Slimer and Tiles
 * TODO [x] (3-15-2022) - Create sprite in photoshop for Player that animates when the Player uses teh Dash ability
 * TODO [x] (3-16-2022) - Make player invulnerable when dashing
 * TODO [x] (3-18-2022) - Change Player bullet texture to Slash Textures instead of Bullet textures
 * TODO [x] Create damage multiplier for player when " " happens
 * TODO [x] Render text for damage multiplier above player
 * TODO [ ] Players.cpp - Play SFX when Player has no more mana, but is still trying to shoot out SlashAttack particles
 * TODO [ ] Players.cpp (2) - Create summary page after defeating the boss and add things like:
 * 					 1. How many bullets the Player shot
 * 					 2. How many bullets the Player took
 * 					 3. How many bullets the Player destroyed
 * 					 4. How many bullets the Player blocked/parried
 * 					 5. How many times the Player died
 * TODO [ ] - Create re-bind keys section in Settings
 * TODO [ ] - Create End Screen after defeating Boss, and add a transition to go into next Level.
 * TODO [ ] - Also after defeating the boss, make sure to save into the save file "levelsUnlocked.mp" to save how many levels the Player has completed
 * TODO [ ] - When we have 2 or more Boss's spawned, they used up the max number of particles we have started with, making it look like the Boss is not shooting its full attack
 *
 *
 *
 *
 *
 *
 *
 * [Game Features (2)]
 *
 * TODO [ ] (3-12-2022) - Add more monsters
 * TODO [ ] (3-20-2022) - Get input from player so they can input their names
 *
 *
 *
 *
 *
 *
 * [Bugs]
 * TODO [x] - SaveHighScore in Player.cpp not working
 * TODO [ ] - Line 1288 PlayGame.cpp, show congrats screen when Boss dies, and do for Slash attack also
 *
 *
 *
 *
 *
 *
 * [Re-Enable if Editing]
 * TODO [ ] - Re-enable Keyboard key P and H to show Debug and Enable Editor mode
 * TODO [ ] - Re-enable Editor, Debug, and camlock back to true upon Init() of PlayGame.cpp
 * TODO [ ] - Re-enable saving Tile Map and loading Tile Map SDLK_0 and _1
 *
 *
 *
 *
 * [Contact these people!]
 * TODO [ ] - Contact resource authors and notify them of the game.
 *
 *
 *
 * TODO [ ]	- Resource Name: 		Knight set
 *			- Author: 				MASTER484
 *			- Author Website: 		http://m484games.ucoz.com/

 * TODO [ ]	- Resource Name:		Dungeon Tileset
 *			- Author: 				Rekkimaru
 *			- Author Website: 		https://rekkimaru.itch.io/dungeon-rpg-tileset
 *
 *
 * TODO [ ]	- Resource Name: 		Neon starlight by Necrophageon
 *			  Author: 				Necrophageon
 *			  Author Website: 		http://famitracker.com/forum/profile.php?id=2752
 *			  Author Website #2: 	https://soundcloud.com/necrophageon
 *
 *
 *
 *
 *
 *



 */


