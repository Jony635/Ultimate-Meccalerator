#ifndef __GROUNDEDENEMY_H__
#define __GROUNDEDENEMY_H__

#include "p2Point.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "j1Entities.h"

struct SDL_Texture;

class GroundedEnemy : public Entity
{
private:
	fPoint position;
	Animation std_anim;
	Animation* main_Anim;
	
	SDL_Texture* GroundedEnemy_tex = nullptr;

	bool wait = false;
	float accumulated_time = 0.0f;

	float speed_y = 0.0f;

public:
	GroundedEnemy(fPoint);
	void Move(float dt);
	void Draw(SDL_Texture* enemyTex) const;
};




#endif