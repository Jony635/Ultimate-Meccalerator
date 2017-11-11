#include "j1Enemies.h"
#include "j1Map.h"



EnemyData::EnemyData(int x, int y) : position(iPoint(x, y)) {}


void Enemy::Move()
{

}

void Enemy::Draw() const
{

}

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

	return true;
}

bool j1Enemies::CleanUp()
{
	return true;
}

bool j1Enemies::PreUpdate()
{
	return true;
}

bool j1Enemies::Update(float dt)
{
	return true;
}

bool j1Enemies::PostUpdate()
{
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
				if (tile_id - tileset->firstgid == 4)
				{
					EnemyData* data = new EnemyData(x, y);
					EnemyDataList.add(data);
				}
			}
		}
	}
}