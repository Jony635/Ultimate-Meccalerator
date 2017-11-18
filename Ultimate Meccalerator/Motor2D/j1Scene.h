#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Teleport mode
	void TpMode();

public:
	SDL_Texture* Tp_circle_texture=nullptr;
	SDL_Texture* Player_shape=nullptr;

	SDL_Rect Tp_circle_rect;
	SDL_Rect Player_shape_rect;
	float Gravity = 9.8 / 60;
};

#endif // __j1SCENE_H__