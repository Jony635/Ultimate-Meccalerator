#include "j1Entities.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Textures.h"

//------------ENTITY_MANAGER METHODS-----------------------------------------------------

EntityManager::EntityManager()	{}

EntityManager::~EntityManager() {}

bool EntityManager:: Awake(pugi::xml_node& entitiesnode)
{
	return true;
}

bool EntityManager::Start()
{
	collectible_objects_texture = App->tex->Load("Resources/gui/Sprites/collectible_objects.png");
	return true;
}

bool EntityManager::CleanUp()
{
	//Freeing Textures
	App->tex->UnLoad(collectible_objects_texture);
	collectible_objects_texture = nullptr;

	//Freeing Entity List
	p2List_item<Entity*>* entity_it = EntitiesList.start;
	while (entity_it)
	{
		RELEASE(entity_it->data);
		entity_it = entity_it->next;
	}
	EntitiesList.clear();


	return true;
}

bool EntityManager::PreUpdate()
{
	return true;
}

bool EntityManager::Update(float dt)
{
	p2List_item<Entity*>* entity_it = EntitiesList.start;
	while (entity_it)
	{
		entity_it->data->Update(dt);
		entity_it = entity_it->next;
	}
	return true;
}

bool EntityManager::PostUpdate()
{
	p2List_item<Entity*>* entity_it = EntitiesList.start;
	while (entity_it)
	{
		entity_it->data->Draw();
		entity_it = entity_it->next;
	}
	return true;
}

Entity* EntityManager::CreateEntity(EntityType type, fPoint pos)
{
	Entity* entity = nullptr;
	switch (type)
	{
		case EntityType::GEAR:
			entity = new Gear(pos, j1Rect(pos, 50, 50));
			entity->type = GEAR;
			EntitiesList.add(entity);
		break;
	}
	return entity;
}

void EntityManager::DestroyEntity(p2List_item<Entity*>* entity)
{
	RELEASE(entity->data);
	EntitiesList.del(entity);
}


const SDL_Texture* EntityManager::getCollectibleObjectsTex() const
{
	return collectible_objects_texture;
}

const SDL_Texture* EntityManager::getEnemiesTex() const
{
	return enemies_texture;
}

const SDL_Texture* EntityManager::getPlayerTex() const
{
	return player_texture;
}

//------------ENTITIES METHODS-----------------------------------------------------
Entity::Entity(fPoint pos, j1Rect collider) : pos(pos), collider(collider) {}
Entity::~Entity() {}
bool Entity::Update(float dt) { return true; }
void Entity::Draw() {}

Gear::Gear(fPoint pos, j1Rect collider) : Entity(pos, collider) {}
Gear::~Gear() {}

bool Gear::Update(float dt)
{
	if (this->collider.Collides(App->player->player_col))
	{
		p2List<Entity*>* EntitiesList = &App->entities_manager->EntitiesList;
		App->entities_manager->DestroyEntity(EntitiesList->At(EntitiesList->find(this)));

	}
	return true;
}

void Gear::Draw()
{
	App->render->Blit((SDL_Texture*)App->entities_manager->getCollectibleObjectsTex(), this->pos.x, this->pos.y);
}
