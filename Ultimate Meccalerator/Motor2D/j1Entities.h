#ifndef __j1ENTITIES_H__
#define __j1ENTITIES_H__

#include "p2Point.h"
#include "j1App.h"
#include "j1Module.h"
#include "p2List.h"



struct SDL_Texture;

enum EntityType
{
	NO_ENTITY_TYPE = -1,
	GEAR
};



class Entity
{
	friend class EntityManager;
public:
	Entity(fPoint pos, j1Rect collider);
	virtual ~Entity();
protected:
	bool enabled = false;
	fPoint pos;
	j1Rect collider;
	EntityType type = NO_ENTITY_TYPE;
public:
	virtual bool Update(float dt);
	virtual void Draw();
};

class Gear : public Entity
{
public:
	Gear(fPoint pos, j1Rect collider);
	~Gear();

public:
	bool Update(float dt);
	void Draw();
};

class EntityManager : public j1Module
{
public:

	EntityManager();
	virtual ~EntityManager();

	bool Start();
	bool Awake(pugi::xml_node& uimnode);
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	
public:
	Entity* CreateEntity(EntityType type, fPoint pos);
	const SDL_Texture* getCollectibleObjectsTex() const;
	const SDL_Texture* getEnemiesTex() const;
	const SDL_Texture* getPlayerTex() const;

public:
	

private:
	SDL_Texture* collectible_objects_texture = nullptr;
	SDL_Texture* enemies_texture = nullptr;
	SDL_Texture* player_texture = nullptr;
	p2List<Entity*> EntitiesList;

};




#endif