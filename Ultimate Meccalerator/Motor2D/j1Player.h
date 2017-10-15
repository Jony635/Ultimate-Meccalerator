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

public:

	fPoint pos;
	float speed_x;
	bool win = false;


private:
	
	fPoint Startingpos;

	uint jumps = 1;
	bool grounded = true;
	
	float speed_y=0;
	float standard_speed_x = 0;

	float tiles_sec_jump;
	SDL_Texture* playerText = nullptr;

	Animation* current_anim = nullptr;
	Animation IdleRight;
	Animation IdleLeft;
	Animation GoRight;
	Animation GoLeft;
	Animation DieGoingRight;
	Animation DieGoingLeft;
	Animation DoubleJump_GoingRight;
	Animation DoubleJump_GoingLeft;

	void CheckFalls();
	void CheckAccels();
	void CheckWin();


	bool CheckDownPos(iPoint pos)const;
	bool CheckRightPos(iPoint pos)const;
	bool CheckLeftPos(iPoint pos)const;
	



	void SetStartingPos();
	float getAccelY(iPoint pos) const;
	float getAccelX(iPoint pos) const;
	uint getDownYCol(iPoint pos) const;
};




#endif