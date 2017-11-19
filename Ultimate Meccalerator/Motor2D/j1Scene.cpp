#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "j1Entities.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	Tp_circle_texture = App->tex->Load("Resources/textures/Tp_Circle.png");
	Player_shape = App->tex->Load("Resources/textures/Player_shape.png");

	tp_counter = 3;

	if(App->actual_lvl==FIRST_LEVEL)
	{
		pugi::xml_document doc;
		App->map->Load(App->LoadConfig(doc).child("map").child("file").text().as_string());	
		App->entities->FillEnemiesData();

		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);
		RELEASE_ARRAY(data);

		App->render->camera.x = 0;
		App->render->fcamera.x = 0;

		if (App->entities->Player_tex == nullptr)
			App->entities->Start();
	}
	else
	{
		App->render->camera.x = 0;
		App->render->fcamera.x = 0;
		App->map->Load("Level_2_x2.tmx");

		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);
		RELEASE_ARRAY(data);

		App->entities->FillEnemiesData();
		App->entities->Start();
	}
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		for (p2List_item<MapLayer*>* layertoset = App->map->data.LayerList.start; layertoset != nullptr; layertoset = layertoset->next)
		{
			if (strcmp(layertoset->data->name.GetString(), "logical debug") != 0)
				continue;
			layertoset->data->visible = !layertoset->data->visible;
		}
	}
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->actual_lvl = Levels::FIRST_LEVEL;
		App->RestartScene();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->RestartScene();
	}
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		App->actual_lvl = Levels::SECOND_LEVEL;
		App->RestartScene();
	}

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN )
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && App->player->CheckCol({ (int)App->player->position.x + 20, (int)(App->entities->player->position.y + 42)}))
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN && tp_counter>0)
		App->tp_mode_enabled = true;

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_UP)
		App->tp_mode_enabled = false;

	App->map->Draw();
	
	if (App->tp_mode_enabled)
		TpMode();
	

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->tex->UnLoad(Tp_circle_texture);
	App->tex->UnLoad(Player_shape);
	App->map->CleanUp();
	App->entities->CleanUp();

	return true;
}

//Teleport Mode
void j1Scene::TpMode()
{
	LOG("Tp paused mode");
	int mouse_x, mouse_y;
	
	App->input->GetMousePosition(mouse_x, mouse_y);
	App->render->Blit(Tp_circle_texture, App->entities->player->position.x-114+18, App->entities->player->position.y-114+29);

	if (mouse_x > (App->render->camera.x) + App->entities->player->position.x - 114 + 18 &&
		mouse_x < (App->render->camera.x) + App->entities->player->position.x + 114 + 18 &&
		mouse_y >(App->render->camera.y) + App->entities->player->position.y - 114 + 29 &&
		mouse_y < (App->render->camera.y) + App->entities->player->position.y + 114 + 29 )
	{
		App->render->Blit(Player_shape, (App->render->camera.x*-1) + mouse_x - 18, (App->render->camera.y*-1) + mouse_y - 29);
		
		if (App->input->GetMouseButtonDown(RI_MOUSE_LEFT_BUTTON_DOWN) == KEY_DOWN)
		{
			App->entities->player->position.x = (App->render->camera.x*-1) + mouse_x;
			App->entities->player->position.y = (App->render->camera.y*-1) + mouse_y;
			tp_counter--;
			App->tp_mode_enabled = false;
		}
	}
}
