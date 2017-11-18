#ifndef __GROUNDEDENEMY_H__
#define __GROUNDEDENEMY_H__

#include "p2Point.h"
#include "j1Enemies.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "j1Timer.h"


struct SDL_Texture;

class GroundedEnemy : public Enemy
{
private:
	fPoint position;
	iPoint last_PlayerPos;
	Animation std_anim;
	Animation* main_Anim;
	
	bool wait = false;
	float accumulated_time=0.0f;

	const p2DynArray<iPoint>* previous_path;
	int tile_it = 1;
	int pathFounded = 0;

public:
	GroundedEnemy(fPoint);
	void Move(float dt);
	void Draw(SDL_Texture* enemyTex) const;
};




#endif