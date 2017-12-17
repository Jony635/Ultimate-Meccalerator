#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class Label;

enum upgrate_bar
{
	EMPTY,
	BLUE,
	YELLOW,
	RED
};

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

	//Creates UI in game elems
	void CreateIngameUI();

	//UI
	bool UI_Do(const UI_Elem* elem, Events* event);
	void Update_upgrade_bar(int upgrade_lvl);
	void Update_lifes(int lifes);
	void Update_tp(int lifes);
	void Update_timer(int time);

	void Update_UI(int upgrade_lvl, int lifes, int tp_left, int time);

public:
	float Gravity = 9.8 / 60;
	int time = 0;
	Label* label_time = nullptr;

	int bar_colour = upgrate_bar::EMPTY;

	//------------------------------------upgrade bar rect-------------------------------------
	j1Rect upgrade_lvl_empty_rect = { 923,13,75,527 };
	j1Rect upgrade_lvl_blue_rect = { 1016,13,75,527 };
	j1Rect upgrade_lvl_yellow_rect = { 1109,13,75,527 };
	j1Rect upgrade_lvl_red_rect = { 1203,13,75,527 };

	j1Rect actual_bar_upgrade = upgrade_lvl_empty_rect;//this will be changed depending from the upgrade lvl

};

#endif // __j1SCENE_H__