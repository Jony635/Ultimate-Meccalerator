#ifndef __J1PLAYER__H
#define __J1PLAYER__H

#include "j1App.h"
#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"

#define PLAYER_H 58
#define PLAYER_W 36
#define TP_CIRCLE_RECT_DEFAULT {825,12,368,368}
#define TP_CIRCLE_RECT_WHITE {885,72,249,249}
#define TP_CIRCLE_RECT_RED {826,13,366,366}
#define TP_CIRCLE_RECT_YELLOW {853,40,312,312}
#define TP_CIRCLE_RECT_BLUE {868,55,281,281}
#define PLAYER_SHAPE_RECT {827,393,36,58}


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
	int lifes = 3;
	int tp_counter = 0;
	float speed_x;
	float speed_y = 0;
	bool win = false;
	bool dead = false;
	bool CheckCol(iPoint pos) const;
	mutable bool diesoundplayed = false;
	j1Rect player_col;
	fPoint pos;
	SDL_Texture** playerText = nullptr;
	SDL_Texture* playerWhite = nullptr;
	SDL_Texture* playerBlue = nullptr;
	SDL_Texture* playerYellow = nullptr;
	SDL_Texture* playerRed = nullptr;
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