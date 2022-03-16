/*
 * Monster.h
 *
 *  Created on: Jul 21, 2017
 *      Author: Carl
 */

#ifndef GAME_MONSTER_H_
#define GAME_MONSTER_H_

class Monster {
public:

	float x2, y2;
	int radius;
	float x,y,w,h;
	float angle;
	float vX,vY;
	float speed;
	int type;
	int health;
	int damage;
	int timer;			// Used for shooting
	float distance;
	bool alive;
	bool collision;		// whether or not the monster is touching ANYTHING at THIS one time
	bool onScreen;

public:	// States

	bool alert;			// If true, slime sees the player (within 100 pixels)

public: // actions

	std::string jumpstate;
	int jumps;
	bool jump;

public: // animations
	double animSpeed;
	double animTimer;
	int animFrame;
};

#endif /* GAME_MONSTER_H_ */
