#ifndef __J1PLAYER__H
#define __J1PLAYER__H

#include "j1App.h"
#include "j1Module.h"
#include "p2Point.h"

class j1Player : public j1Module
{

public:

	j1Player();

	// Destructor
	virtual ~j1Player();



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

private:
	iPoint StartingPosition;
	int speed;
};




#endif