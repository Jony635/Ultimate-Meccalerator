#include "GroundedEnemy.h"
#include "p2DynArray.h"
#include "j1Pathfinding.h"
#include "j1Player.h"

GroundedEnemy::GroundedEnemy(iPoint data_pos) : position(data_pos) {}

void GroundedEnemy::Move(float dt)
{
	App->pathfinding->CreatePath(this->position, iPoint(App->player->pos.x, App->player->pos.y), 1);
	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
	



}

void GroundedEnemy::Draw() const
{

}