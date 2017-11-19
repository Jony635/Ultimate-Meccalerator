#include "j1Enemies.h"
#include "j1Map.h"
#include "j1Player.h"
#include "GroundedEnemy.h"
#include "j1Textures.h"
#include "j1FileSystem.h"
#include "FlyingEnemy.h"
#include "Brofiler\Brofiler.h"



EnemyData::EnemyData(int x, int y) : position(fPoint(x, y)) {}
Enemy::Enemy() {}

void Enemy::Move(float dt) {}

void Enemy::Draw(SDL_Texture* enemyTex) const {}


j1Enemies::j1Enemies() : j1Module()
{
	name.create("enemies");
}

j1Enemies::~j1Enemies(){}

bool j1Enemies::Awake(pugi::xml_node& node)
{
	return true;
}

bool j1Enemies::Start()
{
	EnemyTex = App->tex->Load("Resources/textures/enemies.png");
	return true;
}

bool j1Enemies::CleanUp()
{

	//Clear EnemyDataList
	p2List_item<EnemyData*>* data = EnemyDataList.start;
	while (data != nullptr)
	{
		RELEASE(data->data);
		data = data->next;
	}
	EnemyDataList.clear();

	//Clear EnemyList
	p2List_item<Enemy*>* enemy = EnemyList.start;
	while (enemy != nullptr)
	{
		RELEASE(enemy->data);
		enemy = enemy->next;
	}
	EnemyList.clear();

	return true;
}

bool j1Enemies::PreUpdate()
{
	SpawnEnemies();

	return true;
}

bool j1Enemies::Update(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);

	if(!App->tp_mode_enabled)
	MoveEnemies(dt);

	return true;
}

bool j1Enemies::PostUpdate()
{
	DrawEnemies();
	return true;
}

bool j1Enemies::Load(pugi::xml_node& node)
{
	return true;
}

bool j1Enemies::Save(pugi::xml_node& node) const
{
	return true;
}

void j1Enemies::FillEnemiesData()
{
	for (p2List_item<MapLayer*>* layer_it = App->map->data.LayerList.start; layer_it != nullptr; layer_it = layer_it->next)
	{
		if (strcmp(layer_it->data->name.GetString(), "logical debug") != 0)
			continue;

		for (int y = 0; y < layer_it->data->height; ++y)
		{
			for (int x = 0; x < layer_it->data->width; ++x)
			{
				int tile_id = layer_it->data->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? App->map->GetTilesetFromTileId(tile_id) : NULL;
				if (tileset && tile_id - tileset->firstgid == 4)
				{
					EnemyData* data = new EnemyData(x*tileset->tile_width, y*tileset->tile_height);
					EnemyDataList.add(data);
				}
			}
		}
	}
}

void j1Enemies::SpawnEnemies()
{
	p2List_item<EnemyData*>* data = EnemyDataList.start;
	while (data)
	{
		if (abs(data->data->position.x - App->player->pos.x) < 40 * App->map->data.tile_width)
		{
			Enemy* enemy = nullptr;
			if (App->actual_lvl == Levels::FIRST_LEVEL)
				enemy = new GroundedEnemy(data->data->position);
			else
				enemy = new FlyingEnemy(data->data->position);
			
			RELEASE(data->data);
			EnemyDataList.del(data);
			
			if(enemy)
				EnemyList.add(enemy);
		}
		data = data->next;
	}
}

void j1Enemies::MoveEnemies(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);

	p2List_item<Enemy*>* enemy = EnemyList.start;
	while (enemy)
	{
		enemy->data->Move(dt);
		enemy = enemy->next;
	}
}

void j1Enemies::DrawEnemies()
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);

	p2List_item<Enemy*>* enemy = EnemyList.start;
	while (enemy)
	{
		enemy->data->Draw(EnemyTex);
		enemy = enemy->next;
	}
}