#ifndef __j1ENEMIES_H__
#define __j1ENEMIES_H__

#include "p2Point.h"
#include "p2List.h"
#include "j1App.h"
#include "j1Module.h"


class EnemyData
{
public:
	EnemyData(int x, int y);
	iPoint position;
};


class Enemy
{
private:
	int speed = 3;
public:
	Enemy();
	
	virtual void Move(float dt);
	virtual void Draw() const;
};


class j1Enemies : public j1Module
{
private:

	p2List<Enemy*> EnemyList;
	p2List<EnemyData*> EnemyDataList;

public:

	j1Enemies();

	// Destructor
	virtual ~j1Enemies();

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

	//Read enemies spawn from the tmx of the current map
	void FillEnemiesData();

	//Check enemies data list to spawn them when closer to the player to 50 pixels (only in x)
	void SpawnEnemies();

	//Blit all enemies in their respective positions
	void DrawEnemies();

	//Calculate each enemy movement by his path
	void MoveEnemies(float dt);
};










#endif
