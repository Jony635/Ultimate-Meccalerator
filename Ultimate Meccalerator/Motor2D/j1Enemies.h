#ifndef __j1ENEMIES_H__
#define __j1ENEMIES_H__

#include "Animation.h"
#include "p2Point.h"
#include "p2List.h"
#include "j1App.h"
#include "j1Module.h"



class Enemy
{
private:
	Animation main_Anim;
	iPoint position;
	int speed;

public:
	void Move();
	void Draw() const;
};


class j1Enemies : public j1Module
{
	p2List<Enemy*> EnemyList;

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



};










#endif