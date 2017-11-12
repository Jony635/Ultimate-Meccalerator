#ifndef __GROUNDEDENEMY_H__
#define __GROUNDEDENEMY_H__

#include "p2Point.h"
#include "j1Enemies.h"
#include "Animation.h"


class GroundedEnemy : public Enemy
{
private:
	iPoint position;
	Animation main_Anim;
public:
	GroundedEnemy(iPoint);
	void Move(float dt);
	void Draw() const;
};




#endif