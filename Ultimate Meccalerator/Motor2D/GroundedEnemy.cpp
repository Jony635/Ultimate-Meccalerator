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
	if (accumulated_time >= 0.01f) //Don't do maths and paths every frame. (dt in secs)
	{
		accumulated_time = 0.0f;

		p2Point<int> pos_mapped = App->map->World_to_Map(iPoint(position.x, position.y));
		p2Point<int> playerpos_mapped = App->map->World_to_Map(iPoint(App->player->pos.x, App->player->pos.y));

		//Check Falls
		
		if (!App->player->CheckCol({ (int)position.x + 20, (int)(position.y + 31) })) //Checks the tile under the enemy pos, if doesn't collide, fall down.
		{
			position.y += speed_y * 12 * dt;
			speed_y += App->scene->Gravity;
		}
		else
		{
			speed_y = 0.0f;
		}
		//Check Paths
		if (tile_to_go == iPoint() || tile_to_go == pos_mapped)
		{
			if (App->pathfinding->CreatePath(pos_mapped, playerpos_mapped, 1) != -1)
			{
				tile_to_go = *App->pathfinding->GetLastPath()->At(1);
			}
		}
			
		else if (tile_to_go != pos_mapped)
		{
			const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

			const iPoint* tile_mapped = path->At(1);
			if (tile_mapped)
			{
				iPoint tile_world = App->map->MapToWorld(tile_mapped->x, tile_mapped->y);
				position.x += (tile_world.x - position.x) * 2 * dt;
				position.y += (tile_world.y - position.y) * 2 * dt;
			}
		}
	
		rec.rec.x = position.x;
		rec.rec.y = position.y;
		if (App->player->player_col.Collides(rec))
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