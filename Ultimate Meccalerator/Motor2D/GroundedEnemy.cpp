#include "GroundedEnemy.h"
#include "j1Pathfinding.h"
#include "j1Player.h"
#include "j1FileSystem.h"
#include "j1Render.h"
#include "j1Map.h"

GroundedEnemy::GroundedEnemy(fPoint data_pos) : position(data_pos) 
{
	std_anim.PushBack({ 3,0,50,29 });
	std_anim.PushBack({ 60,0,50,29 });
	std_anim.PushBack({ 115,1,50,29 });
	std_anim.PushBack({ 171,1,50,29 });
	std_anim.PushBack({ 137,33,50,29 });
	std_anim.speed = 0.2f;

	main_Anim = &std_anim;
	last_PlayerPos = { 0,0 };
}

void GroundedEnemy::Move(float dt)
{
	p2Point<int> pos_mapped = App->map->World_to_Map(iPoint(position.x, position.y));
	p2Point<int> playerpos_mapped = App->map->World_to_Map(iPoint(App->player->pos.x, App->player->pos.y));

	if (!wait)
	{
		if (last_PlayerPos != playerpos_mapped)
			pathFounded = App->pathfinding->CreatePath(pos_mapped, playerpos_mapped, 2) != -1;
		if (pathFounded != -1)
		{
			last_PlayerPos = playerpos_mapped;
			const p2DynArray<iPoint>* newPath = App->pathfinding->GetLastPath();
			/*if (!previous_path || previous_path != newPath)
			{
				previous_path = newPath;
				tile_it = 1;
			}
			else if (previous_path->Count() != newPath->Count())
				previous_path = newPath;*/
			
			const iPoint* tile_mapped = newPath->At(1);
			if (tile_mapped)
			{
				iPoint tile_world = App->map->MapToWorld(tile_mapped->x, tile_mapped->y);
				position.x += (tile_world.x - position.x) * 2 * dt;
				position.y += (tile_world.y - position.y) * 2 * dt;
			}
			const iPoint* tile_mapped2 = newPath->At(2);
			if (tile_mapped2)
			{
				iPoint tile_world2 = App->map->MapToWorld(tile_mapped2->x, tile_mapped2->y);
				position.x += (tile_world2.x - position.x) * 2 * dt;
				position.y += (tile_world2.y - position.y) * 2 * dt;
			}

			//works

			/*const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

			const iPoint* tile_mapped = path->At(1);
			if (tile_mapped)
			{
				iPoint tile_world = App->map->MapToWorld(tile_mapped->x, tile_mapped->y);
				position.x += (tile_world.x - position.x) * 2 * dt;
				position.y += (tile_world.y - position.y) * 2 * dt;
			}
			const iPoint* tile_mapped2 = path->At(2);
			if (tile_mapped2)
			{
				iPoint tile_world2 = App->map->MapToWorld(tile_mapped2->x, tile_mapped2->y);
				position.x += (tile_world2.x - position.x) * 2 * dt;
				position.y += (tile_world2.y - position.y) * 2 * dt;
			}*/
		}
	}
	else
	{
		if (timeWaited == 0.0f)
			waitTimer.Start();
		timeWaited = waitTimer.ReadSec()+0.0000000001f;

		if (timeWaited > 0.00000002f)
		{
			timeWaited = 0;
			wait = false;
		}

		
	}
}

void GroundedEnemy::Draw(SDL_Texture* enemyTex) const
{
	App->render->Blit(enemyTex, position.x, position.y, &main_Anim->GetCurrentFrame());
}