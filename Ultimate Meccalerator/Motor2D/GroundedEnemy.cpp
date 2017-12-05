#include "GroundedEnemy.h"
#include "j1Pathfinding.h"
#include "j1Player.h"
#include "j1FileSystem.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Scene.h"

GroundedEnemy::GroundedEnemy(fPoint data_pos) : position(data_pos) 
{
	std_anim.PushBack({ 0,33,30,31 });
	std_anim.PushBack({ 31,33,29,31 });
	std_anim.PushBack({ 61,33,30,31 });
	std_anim.PushBack({ 93,33,32,31 });
	std_anim.speed = 4.0f;
	
	rec.rec.w = 30;
	rec.rec.h = 31;

	main_Anim = &std_anim;
}

void GroundedEnemy::Move(float dt)
{
	if (1) //Introduce timer condition here
	{
		accumulated_time = 0.0f;

		//Check Falls

		if (!App->player->CheckCol({ (int)position.x + 20, (int)(position.y + 31) })) //Checks the tile under the enemy pos, if doesn't collide, fall down.
		{
			position.y += speed_y * 19 * dt;
			speed_y += App->scene->Gravity * 19 * dt;
		}
		else
		{
			speed_y = 0.0f;

			p2Point<int> pos_mapped = App->map->World_to_Map(iPoint(position.x, position.y));
			p2Point<int> playerpos_mapped = App->map->World_to_Map(iPoint(App->player->pos.x, App->player->pos.y));

			//Check Paths
			if (!tile_to_go)
			{
				if (App->pathfinding->CreatePath(pos_mapped, playerpos_mapped, 1) != -1)
				{
					tile_to_go = (iPoint*)App->pathfinding->GetLastPath()->At(1);
				}
			}

			if(tile_to_go)
			{
				iPoint tile_world = App->map->MapToWorld(tile_to_go->x, tile_to_go->y);
				if (abs(tile_world.y - position.y) < 0.0001)
				{
					position.x = tile_world.x;
					position.y = tile_world.y;
					tile_to_go = nullptr;
				}

				position.x += (tile_world.x - position.x) * 2 * dt;
				position.y += (tile_world.y - position.y) * 2 * dt;
			}
		}
		rec.rec.x = position.x;
		rec.rec.y = position.y;
		if (App->player->player_col.Collides(rec) && !App->godmode)
		{
			App->player->dead = true;
		}
	}
	
	accumulated_time += dt;
}

void GroundedEnemy::Draw(SDL_Texture* enemyTex) const
{
	App->render->Blit(enemyTex, position.x, position.y, &main_Anim->GetCurrentFrame());
}