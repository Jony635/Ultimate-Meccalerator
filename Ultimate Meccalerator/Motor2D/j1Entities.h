#ifndef __j1ENTITIES_H__
#define __j1ENTITIES_H__

#include "p2Point.h"
#include "p2List.h"
#include "j1App.h"
#include "j1Module.h"

struct SDL_Texture;

class EnemyData
{
public:
	EnemyData(int x, int y);
	fPoint position;
};

enum EntityTypes
{
	NO_VALUE = -1,
	PLAYER,
	FLYING_ENEMY,
	GROUNDED_ENEMY
};

class Entity
{
public:
	EntityTypes entity_Type=NO_VALUE;
	fPoint position;

public:
	Entity(fPoint);
	virtual ~Entity();
	virtual void Move(float dt);
	virtual void Draw(SDL_Texture*);
	virtual void Save(pugi::xml_node& node);
	virtual void Load(pugi::xml_node& node);
	virtual void Awake(pugi::xml_node& node);
	virtual void PostMove();
	virtual void CleanUp();
	virtual void Start();
};

class j1Entities: public j1Module
{
private:

	p2List<Entity*> Entities;
	p2List<EnemyData*> EnemyDataList;
	

public:
	j1Player* player = nullptr;
	SDL_Texture* Player_tex = nullptr;
	SDL_Texture* GroundedEnemy_tex = nullptr;
	SDL_Texture* FlyingEnemy_tex = nullptr;
public:

	j1Entities();

	// Destructor
	virtual ~j1Entities();

	//Before render avaiable
	bool Awake(pugi::xml_node&);

	//Before first frame
	bool Start();

	//Before each frame
	bool PreUpdate();

	//Each frame
	bool Update(float dt);

	//After Each frame
	bool PostUpdate();

	//CleanUp
	bool CleanUp();

	//Saving and Loading
	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

public:

	//Add one Entity
	Entity* AddEntity(EntityTypes type, fPoint pos);

	//Read enemies spawn from the tmx of the current map
	void FillEnemiesData();

	//Check enemies data list to spawn them when closer to the player to 50 pixels (only in x)
	void SpawnEnemies();

	//Blit all entities in their respective positions
	void DrawEntities();

	//Calculate each entity movement
	void MoveEntities(float dt);

	//Set Player Starting Pos
	void SetStartingPos(fPoint&);

};
#endif
