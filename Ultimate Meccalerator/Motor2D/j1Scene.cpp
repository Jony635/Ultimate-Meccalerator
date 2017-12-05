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
#include "UI_Manager.h"

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
	if(!Tp_circle_texture)
		Tp_circle_texture = App->tex->Load("Resources/textures/Tp_Circle.png");
	if(!Player_shape)
		Player_shape = App->tex->Load("Resources/textures/Player_shape.png");

	tp_counter = 3;

	switch (App->actual_lvl)
	{
		case Levels::MENU:
		{
			/*j1Rect atlasrec [Button_State::MAX_STATE] = { j1Rect(22,55,45,65), j1Rect(22,55,45,65), j1Rect(22,55,45,65)};
			iPoint pos;
			j1Rect col(pos, 23, 12);*/
		}
		break;
		case Levels::FIRST_LEVEL:
		{
			pugi::xml_document doc;
			App->map->Load(App->LoadConfig(doc).child("map").child("file").text().as_string());
			App->enemies->FillEnemiesData();

			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data))
				App->pathfinding->SetMap(w, h, data);
			RELEASE_ARRAY(data);

			App->render->camera.x = 0;
			App->render->fcamera.x = 0;

			if (App->player->playerText == nullptr)
				App->player->Start();
		}
		break;
		case Levels::SECOND_LEVEL:
		{
			App->render->camera.x = 0;
			App->render->fcamera.x = 0;
			App->map->Load("Level_2_x2.tmx");

			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data))
				App->pathfinding->SetMap(w, h, data);
			RELEASE_ARRAY(data);

			App->enemies->FillEnemiesData();
			App->player->Start();
		}
		break;
	}
	return true;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->tex->UnLoad(Tp_circle_texture);
	App->tex->UnLoad(Player_shape);
	App->player->CleanUp();
	App->map->CleanUp();
	App->enemies->CleanUp();
	App->ui_manager->CleanUp();
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
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

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && App->player->CheckCol({ (int)App->player->pos.x + 20, (int)(App->player->pos.y + 42)}))
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		App->godmode = !App->godmode;

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (App->framerate_cap == 30)
			App->framerate_cap = 0;
		else
			App->framerate_cap = 30;
	}
		

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

//Teleport Mode
void j1Scene::TpMode()
{
	//LOG("Tp paused mode");
	int mouse_x, mouse_y;
	
	App->input->GetMousePosition(mouse_x, mouse_y);
	App->render->Blit(Tp_circle_texture, App->player->pos.x-114+18, App->player->pos.y-114+29);

	if (mouse_x > (App->render->camera.x) + App->player->pos.x - 114 + 18 &&
		mouse_x < (App->render->camera.x) + App->player->pos.x + 114 + 18 &&
		mouse_y >(App->render->camera.y) + App->player->pos.y - 114 + 29 &&
		mouse_y < (App->render->camera.y) + App->player->pos.y + 114 + 29 )
	{
		if (App->player->CheckCol(iPoint((App->render->camera.x*-1) + mouse_x, (App->render->camera.y*-1) + mouse_y)) == false)
		{
			App->render->Blit(Player_shape, (App->render->camera.x*-1) + mouse_x - 18, (App->render->camera.y*-1) + mouse_y - 29);

			if (App->input->GetMouseButtonDown(RI_MOUSE_LEFT_BUTTON_DOWN) == KEY_DOWN)
			{
				App->player->pos.x = (App->render->camera.x*-1) + mouse_x;
				App->player->pos.y = (App->render->camera.y*-1) + mouse_y;
				tp_counter--;
				App->tp_mode_enabled = false;
			}
		}	
	}
}

bool j1Scene::UI_Do(const UI_Elem* elem, Events* event)
{
	if (elem->type == UI_ElemType::BUTTON)
	{
		Button* button = (Button*)elem;
		switch (button->btype)
		{
			case UI_ButtonType::EXIT:
			{
				return false;
			}
			break;
			case UI_ButtonType::PLAY:
			{
				if (*event == Events::LEFT_CLICKED)
				{
					App->actual_lvl = Levels::FIRST_LEVEL;
					App->RestartScene();
				}
			}
			break;
		}
	}
}
