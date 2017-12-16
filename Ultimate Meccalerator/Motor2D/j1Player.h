#ifndef __J1PLAYER__H
#define __J1PLAYER__H

#include "j1App.h"
#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;

class j1Player : public j1Module
{

public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	//Before render avaiable
	bool Awake(pugi::xml_node&);

	//Before first frame
	bool Start();

	//Before each frame
	bool PreUpdate();

	//Each frame
	bool Update(float dt);

	//After Each frame
	bool PostUpdate();

	//CleanUp
	bool CleanUp();

	//Saving and Loading
	bool Load(pugi::xml_node&);
		
	bool Save(pugi::xml_node&) const;

	//Teleport mode
	void TpMode();

public:

	int tp_counter = 0;
	float speed_x;
	float speed_y = 0;
	bool win = false;
	bool dead = false;
	bool CheckCol(iPoint pos) const;
	mutable bool diesoundplayed = false;
	j1Rect player_col;
	fPoint pos;
	SDL_Texture* playerText = nullptr;
	SDL_Rect Tp_circle_rect;

private:
	
	fPoint Startingpos;

	uint jumps = 1;
	bool grounded = true;
	
	float standard_speed_x = 0;

	float tiles_sec_jump;
	bool alreadyLoaded = false;

	Animation* current_anim = nullptr;
	Animation IdleRight;
	Animation IdleLeft;
	Animation GoRight;
	Animation GoLeft;
	Animation DieGoingRight;
	Animation DieGoingLeft;
	Animation DoubleJump_GoingRight;
	Animation DoubleJump_GoingLeft;

	void CheckFalls(float dt);
	void CheckAccels(float dt);
	void CheckWin();
	void CheckMovements(float dt);

	bool CheckDieCol(iPoint pos) const;

	void SetStartingPos();
	float getAccelY(iPoint pos) const;
	float getAccelX(iPoint pos) const;
	uint getDownYCol(iPoint pos) const;

	int dieCounter = 0;
};
#endif