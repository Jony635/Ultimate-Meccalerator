#include "GroundedEnemy.h"
#include "p2DynArray.h"
#include "j1Pathfinding.h"
#include "j1Player.h"
#include "j1FileSystem.h"
#include "j1Render.h"
#include "j1Map.h"

GroundedEnemy::GroundedEnemy(iPoint data_pos) : position(data_pos) 
{
	std_anim.PushBack({ 0,33,30,31 });
	std_anim.PushBack({ 31,33,29,31 });
	std_anim.PushBack({ 61,33,30,31 });
	std_anim.PushBack({ 93,33,32,31 });
	std_anim.speed = 0.5f;

	main_Anim = &std_anim;
}

void GroundedEnemy::Move(float dt)
{
	iPoint pos_mapped = App->map->World_to_Map(position);
	iPoint playerpos_mapped = App->map->World_to_Map(iPoint(App->player->pos.x, App->player->pos.y));
	/*App->pathfinding->CreatePath(pos_mapped, playerpos_mapped, 1);



	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
	
	const iPoint* tile_mapped = path->At(1);
	iPoint tile_world = App->map->MapToWorld(tile_mapped->x, tile_mapped->y);
	
	position.x += (tile_world.x - position.x) * 60 * dt;
	position.y += (tile_world.y - position.y) * 60 * dt;*/


}

void GroundedEnemy::Draw(SDL_Texture* enemyTex) const
{
	App->render->Blit(enemyTex, position.x, position.y, &main_Anim->GetCurrentFrame());
}