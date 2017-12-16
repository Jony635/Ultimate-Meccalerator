#include "j1Entities.h"
#include "j1Render.h"


//------------ENTITY_MANAGER METHODS-----------------------------------------------------

EntityManager::EntityManager()	{}

EntityManager::~EntityManager() {}

bool EntityManager:: Awake(pugi::xml_node& entitiesnode)
{
	return true;
}

bool EntityManager::Start()
{
	return true;
}

bool EntityManager::CleanUp()
{
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

const SDL_Texture* EntityManager::getEntitiesAtlas() const
{
	return entities_Atlas;
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
	return true;
}
void Gear::Draw()
{
	if (enabled)
		App->render->Blit((SDL_Texture*)App->entities_manager->getEntitiesAtlas(), this->pos.x, this->pos.y, &(SDL_Rect)j1Rect(0,0,0,0).rec );
}