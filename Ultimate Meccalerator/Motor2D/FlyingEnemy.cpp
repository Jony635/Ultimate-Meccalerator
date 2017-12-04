#include "FlyingEnemy.h"
#include "p2DynArray.h"
#include "j1Pathfinding.h"
#include "j1Player.h"
#include "j1FileSystem.h"
#include "j1Render.h"
#include "j1Map.h"

FlyingEnemy::FlyingEnemy(fPoint data_pos) : position(data_pos)
{
	std_anim.PushBack({ 3,0,50,29 });
	std_anim.PushBack({ 60,0,50,29 });
	std_anim.PushBack({ 115,1,50,29 });
	std_anim.PushBack({ 171,1,50,29 });
	std_anim.PushBack({ 137,33,50,29 });
	std_anim.speed = 6.0f;

	rec.rec.w = 50;
	rec.rec.h = 29;

	main_Anim = &std_anim;
}

void FlyingEnemy::Move(float dt)
{
	p2Point<int> pos_mapped = App->map->World_to_Map(iPoint(position.x, position.y));
	p2Point<int> playerpos_mapped = App->map->World_to_Map(iPoint(App->player->pos.x, App->player->pos.y));

	if (accumulated_time >= 0.01f)
	{
		accumulated_time = 0.0f;
		if (App->pathfinding->CreatePath(pos_mapped, playerpos_mapped, 2) != -1)
		{
			const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();


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
				iPoint tile_world = App->map->MapToWorld(tile_mapped2->x, tile_mapped2->y);
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

void FlyingEnemy::Draw(SDL_Texture* enemyTex) const
{
	App->render->Blit(enemyTex, position.x, position.y, &main_Anim->GetCurrentFrame());
}