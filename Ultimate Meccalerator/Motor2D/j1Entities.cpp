#include "j1Entities.h"
#include "j1Map.h"
#include "j1Player.h"
#include "GroundedEnemy.h"
#include "j1Textures.h"
#include "j1FileSystem.h"
#include "FlyingEnemy.h"
#include "Brofiler\Brofiler.h"

EnemyData::EnemyData(int x, int y) : position(fPoint(x, y)) {}

//Virtual Entity Methods Definition
Entity::Entity(fPoint pos) : position(pos) {}
Entity::~Entity(){}
void Entity::Move(float dt) { }
void Entity::Draw(SDL_Texture*) {}
void Entity::Save(pugi::xml_node& node){}
void Entity::Load(pugi::xml_node& node) {}
void Entity::Awake(pugi::xml_node& node){}
void Entity::PostMove() {}
void Entity::CleanUp(){}
void Entity::Start() {}

//j1Entities Methods
j1Entities::j1Entities() : j1Module()
{
	name.create("entities");
}

j1Entities::~j1Entities() {}

bool j1Entities::Awake(pugi::xml_node& node)
{
	//Player creation
	fPoint player_pos;
	SetStartingPos(player_pos);
	player = (j1Player*)AddEntity(EntityTypes::PLAYER, player_pos);

	p2List_item<Entity*>* _Entity = Entities.start;
	while (_Entity)
	{
		switch (_Entity->data->entity_Type)
		{
		case EntityTypes::PLAYER: 
			_Entity->data->Awake(node.child("player"));
			break;
		}
		_Entity = _Entity->next;
	}
	return true;
}

bool j1Entities::Start()
{
	//Loading Textures
	if(!Player_tex)
		Player_tex = App->tex->Load("Resources/textures/Player_SpriteSheet.png");
	if(!GroundedEnemy_tex)
		GroundedEnemy_tex = App->tex->Load("Resources/textures/Grounded_Enemy.png");
	if(!FlyingEnemy_tex)
		FlyingEnemy_tex = App->tex->Load("Resources/textures/Flying_Enemy.png");

	player->Start();

	return true;
}

bool j1Entities::CleanUp()
{
	if (Player_tex)
	{
		//Clear EnemyDataList
		p2List_item<EnemyData*>* data = EnemyDataList.start;
		while (data != nullptr)
		{
			RELEASE(data->data);
			data = data->next;
		}
		EnemyDataList.clear();

		//Clear Entities List
		p2List_item<Entity*>* _Entity2 = Entities.start;
		while (_Entity2 != nullptr)
		{
			//RELEASE(_Entity2->data);
			_Entity2 = _Entity2->next;
		}
		Entities.clear();

		if (Player_tex)
			App->tex->UnLoad(Player_tex);
		if (GroundedEnemy_tex)
			App->tex->UnLoad(GroundedEnemy_tex);
		if (FlyingEnemy_tex)
			App->tex->UnLoad(FlyingEnemy_tex);

	}
	
	return true;
}

bool j1Entities::PreUpdate()
{
	SpawnEnemies();
	return true;
}

bool j1Entities::Update(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);

	if (!App->tp_mode_enabled)
		MoveEntities(dt);

	return true;
}

bool j1Entities::PostUpdate()
{
	DrawEntities();
	return true;
}

bool j1Entities::Load(pugi::xml_node& node)
{
	p2List_item<Entity*>* _Entity = Entities.start;
	while (_Entity)
	{
		switch (_Entity->data->entity_Type)
		{
		case EntityTypes::PLAYER: //Enemies don't load
			_Entity->data->Load(node.child("player"));
			break;
		}
		_Entity = _Entity->next;
	}
	return true;
}

bool j1Entities::Save(pugi::xml_node& node) const
{
	p2List_item<Entity*>* _Entity = Entities.start;
	while (_Entity)
	{
		switch (_Entity->data->entity_Type)
		{
		case EntityTypes::PLAYER: //Enemies don't save
			_Entity->data->Save(node.append_child("player"));
			break;
		}
		_Entity = _Entity->next;
	}
	return true;
}

Entity* j1Entities::AddEntity(EntityTypes type, fPoint pos)
{
	Entity* _Entity = nullptr;
	switch (type)
	{
		case EntityTypes::PLAYER:
			_Entity = new j1Player(pos);
			break;
		case EntityTypes::GROUNDED_ENEMY:
			_Entity = new GroundedEnemy(pos);
			break;
		case EntityTypes::FLYING_ENEMY:
			_Entity = new FlyingEnemy(pos);
			break;
	}
	_Entity->entity_Type = type;
	Entities.add(_Entity);
	return _Entity;
}

void j1Entities::FillEnemiesData()
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

void j1Entities::SpawnEnemies()
{
	p2List_item<EnemyData*>* data = EnemyDataList.start;
	while (data)
	{
		if (abs(data->data->position.x - App->entities->player->position.x) < 40 * App->map->data.tile_width)
		{
			Entity* _Entity = nullptr;
			if (App->actual_lvl == Levels::FIRST_LEVEL)
			{
				_Entity = AddEntity(EntityTypes::GROUNDED_ENEMY, data->data->position);
				_Entity->Start();
			}
			else
			{
				_Entity = AddEntity(EntityTypes::FLYING_ENEMY, data->data->position);
				_Entity->Start();
			}

			RELEASE(data->data);
			EnemyDataList.del(data);

			if (_Entity)
				Entities.add(_Entity);
		}
		data = data->next;
	}
}

void j1Entities::MoveEntities(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);

	p2List_item<Entity*>* _Entity = Entities.start;
	while (_Entity)
	{
		_Entity->data->Move(dt);
		_Entity = _Entity->next;
	}
}

void j1Entities::DrawEntities()
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);
	SDL_Texture* tex = nullptr;
	p2List_item<Entity*>* _Entity = Entities.start;
	while (_Entity)
	{
		switch (_Entity->data->entity_Type)
		{
		case EntityTypes::PLAYER:
			tex = Player_tex;
			break;
		case EntityTypes::GROUNDED_ENEMY:
			tex = GroundedEnemy_tex;
			break;
		case EntityTypes::FLYING_ENEMY:
			tex = FlyingEnemy_tex;
			break;
		}
		if(tex!=nullptr)
			_Entity->data->Draw(tex);
		_Entity = _Entity->next;
	}
}

void j1Entities::SetStartingPos(fPoint& pos)
{
	for (p2List_item<TileSet*>* TileSet = App->map->data.tilesets.start; TileSet != nullptr; TileSet = TileSet->next)
	{
		for (p2List_item<MapLayer*>* layer = App->map->data.LayerList.start; layer != nullptr; layer = layer->next)
		{
			if (strcmp(layer->data->name.GetString(), "logical debug") != 0)
				continue;
			int x = 0, y = 0;
			for (int num_tile = 0; num_tile < layer->data->size_data; ++num_tile)
			{
				if (*(layer->data->data + num_tile) == 5195)
				{
					pos = fPoint(x, y + App->map->data.tile_height / 2);
					break;
				}
				x += TileSet->data->tile_width;

				if (x % (layer->data->width * TileSet->data->tile_width) == 0)
				{
					x = 0;
					y += TileSet->data->tile_height;
				}
			}
		}
	}
}
