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
	//TODO: Fill the path here:
	//collectible_objects_texture = App->tex->Load("");
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
	return true;
}

bool EntityManager::PostUpdate()
{
	return true;
}

Entity* EntityManager::CreateEntity(EntityType type, fPoint pos)
{
	Entity* entity = nullptr;
	switch (type)
	{
		case EntityType::GEAR:
			entity = new Entity(pos, j1Rect(pos, 50, 50));
			entity->type = GEAR;
			EntitiesList.add(entity);
		break;
	}
	return entity;
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
		//TODO: Set player to the new Tier.
		//TODO: Delete Gear from the list.
	}
	return true;
}

void Gear::Draw()
{
	if (enabled)
		App->render->Blit((SDL_Texture*)App->entities_manager->getCollectibleObjectsTex(), this->pos.x, this->pos.y, &(SDL_Rect)j1Rect(0,0,0,0).rec );
}