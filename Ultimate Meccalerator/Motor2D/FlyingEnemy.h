#ifndef __FLYINGENEMY_H__
#define __FLYINGENEMY_H__

#include "p2Point.h"
#include "j1Enemies.h"
#include "Animation.h"

struct SDL_Texture;

class FlyingEnemy : public Enemy
{
private:
	fPoint position;
	Animation std_anim;
	Animation* main_Anim;

	bool wait = false;
	float timeWaited = 0.0f;
	j1Timer waitTimer;
public:
	FlyingEnemy(fPoint);
	void Move(float dt);
	void Draw(SDL_Texture* enemyTex) const;
};
#endif
