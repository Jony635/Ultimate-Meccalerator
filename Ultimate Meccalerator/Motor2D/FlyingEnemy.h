#ifndef __FLYINGENEMY_H__
#define __FLYINGENEMY_H__

#include "p2Point.h"
#include "Animation.h"
#include "j1Entities.h"

struct SDL_Texture;

class FlyingEnemy : public Entity
{
private:
	fPoint position;
	Animation std_anim;
	Animation* main_Anim;

	SDL_Texture* FlyingEnemy_tex = nullptr;


	bool wait = false;
	float accumulated_time = 0.0f;

public:
	FlyingEnemy(fPoint);
	void Move(float dt);
	void Draw(SDL_Texture* enemyTex) const;
};
#endif
