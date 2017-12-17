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
#include "j1Entities.h"

#define SCREEN_MIDDLE_X_FOR_BUTTON 1024/2-190/2//window.w/2-button.w/2
#define SCREEN_MIDDLE_X_FOR_TITLE 1024/2-870/2//window.w/2-button.w/2

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
	App->ui_manager->Activate();
	App->map->Activate();
	App->pathfinding->Activate();

	App->player->tp_counter = 3;

	switch (App->actual_lvl)
	{
		case Levels::MENU:
		{
			//----------------------------------------------Load background-----------------------
			App->render->camera.x = 0;
			App->render->fcamera.x = 0;
			App->map->Load("Menu_background.tmx");
			
			//---------------------------------------------Buttons Rect---------------------------
			j1Rect atlasrec[Button_State::MAX_STATE] = { j1Rect(0,192,190,49), j1Rect(190,49,190,49), j1Rect(190,0,190,49) };
			
			//---------------------------------------------Images Rect----------------------------
			j1Rect title_rect = { 0,266,870,165 };
			j1Rect logo_rect = { 457,2,91,105 };

			//---------------------------------------------Buttons Positions--------------------------------
			fPoint play_pos = { SCREEN_MIDDLE_X_FOR_BUTTON,340};
			fPoint continue_pos = { SCREEN_MIDDLE_X_FOR_BUTTON,399};
			fPoint htp_pos = { SCREEN_MIDDLE_X_FOR_BUTTON,458 };
			fPoint credits_pos = { SCREEN_MIDDLE_X_FOR_BUTTON,517 };
			fPoint exit_pos = { SCREEN_MIDDLE_X_FOR_BUTTON,576 };
			fPoint settings_pos = { 20,694 };

			//--------------------------------------------Image Positions-----------------------------------
			fPoint title_pos = { SCREEN_MIDDLE_X_FOR_TITLE,60 };
			fPoint logo_pos = { 913,598+60 };

			//---------------------------------------------Buttons Colliders--------------------------------
			j1Rect play_col(play_pos, atlasrec[Button_State::DEFAULT].rec.w, atlasrec[Button_State::DEFAULT].rec.h);
			j1Rect continue_col(continue_pos, atlasrec[Button_State::DEFAULT].rec.w, atlasrec[Button_State::DEFAULT].rec.h);
			j1Rect htp_col(htp_pos, atlasrec[Button_State::DEFAULT].rec.w, atlasrec[Button_State::DEFAULT].rec.h);
			j1Rect credits_col(credits_pos, atlasrec[Button_State::DEFAULT].rec.w, atlasrec[Button_State::DEFAULT].rec.h);
			j1Rect exit_col(exit_pos, atlasrec[Button_State::DEFAULT].rec.w, atlasrec[Button_State::DEFAULT].rec.h);
			j1Rect settings_col(settings_pos, atlasrec[Button_State::DEFAULT].rec.w, atlasrec[Button_State::DEFAULT].rec.h);

			//---------------------------------------------Buttons------------------------------------------
			App->ui_manager->CreateUIElem(UI_ElemType::BUTTON, play_pos, &atlasrec[0], play_col, UI_ButtonType::PLAY, "play",App->fonts->getFontbyName("kenvector_future"));
			App->ui_manager->CreateUIElem(UI_ElemType::BUTTON, continue_pos, &atlasrec[0], continue_col, UI_ButtonType::CONTINUE, "continue", App->fonts->getFontbyName("kenvector_future"));
			App->ui_manager->CreateUIElem(UI_ElemType::BUTTON, htp_pos, &atlasrec[0], htp_col, UI_ButtonType::HOW_TO_PLAY, "how to play", App->fonts->getFontbyName("kenvector_future"));
			App->ui_manager->CreateUIElem(UI_ElemType::BUTTON, credits_pos, &atlasrec[0], credits_col, UI_ButtonType::CREDITS, "credits", App->fonts->getFontbyName("kenvector_future"));
			App->ui_manager->CreateUIElem(UI_ElemType::BUTTON, exit_pos, &atlasrec[0], exit_col, UI_ButtonType::EXIT, "quit", App->fonts->getFontbyName("kenvector_future"));
			App->ui_manager->CreateUIElem(UI_ElemType::BUTTON, settings_pos, &atlasrec[0], settings_col, UI_ButtonType::SETTINGS, "settings", App->fonts->getFontbyName("kenvector_future"));

			//---------------------------------------------Images-------------------------------------------
			App->ui_manager->CreateUIElem(UI_ElemType::IMAGE, title_pos, &title_rect);
			App->ui_manager->CreateUIElem(UI_ElemType::IMAGE, logo_pos, &logo_rect);


		}
		break;
		case Levels::SETTINGS_SCREEN:
		{
			//----------------------------------------------Load background-----------------------
			App->render->camera.x = 0;
			App->render->fcamera.x = 0;

			App->map->Load("Menu_background.tmx");
			fPoint title_pos = { SCREEN_MIDDLE_X_FOR_TITLE,60 };
			j1Rect title_rect = { 0,266,870,165 };

			App->ui_manager->CreateUIElem(UI_ElemType::IMAGE, title_pos, &title_rect);

			App->ui_manager->CreateUIElem(UI_ElemType::SLIDEBAR, { 200, 500 }, &j1Rect(41, 464, 559, 68), j1Rect(200+25, 500, 559-100, 68), NO_BUTTONTYPE, "Music Volume", App->fonts->getFontbyName("kenvector_future"));
			App->ui_manager->CreateUIElem(UI_ElemType::SLIDEBAR, { 200, 700 }, &j1Rect(41, 464, 559, 68), j1Rect(200 + 25, 700, 559 - 100, 68), NO_BUTTONTYPE, "FX Volume", App->fonts->getFontbyName("kenvector_future"));
		}
		break;
		case Levels::FIRST_LEVEL:
		{
			App->audio->PlayMusic("Resources/audio/music/BSO.ogg");
			pugi::xml_document doc;
			App->map->Load(App->LoadConfig(doc).child("map").child("file").text().as_string());
			
			App->player->Activate();
			App->enemies->Activate();
			App->entities_manager->Activate();

			App->enemies->FillEnemiesData();
			LoadCollectibleObjects();

			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data))
				App->pathfinding->SetMap(w, h, data);
			RELEASE_ARRAY(data);

			App->render->camera.x = 0;
			App->render->fcamera.x = 0;
		}
		break;
		case Levels::SECOND_LEVEL:
		{
			App->audio->PlayMusic("Resources/audio/music/BSO.ogg");
			App->map->Load("Level_2_x2.tmx");

			App->player->Activate();
			App->enemies->Activate();
			App->entities_manager->Activate();
			App->map->Activate();
			App->ui_manager->Activate();
			App->pathfinding->Activate();

			App->enemies->FillEnemiesData();
			LoadCollectibleObjects();

			App->render->camera.x = 0;
			App->render->fcamera.x = 0;


			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data))
				App->pathfinding->SetMap(w, h, data);
			RELEASE_ARRAY(data);

			

		}
		break;
	}
	return true;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->player->DeActivate();
	App->enemies->DeActivate();
	App->ui_manager->DeActivate();
	App->map->DeActivate();
	App->pathfinding->DeActivate();
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
		App->scene->Reset();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->scene->Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		App->actual_lvl = Levels::SECOND_LEVEL;
		App->scene->Reset();
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
		
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN && App->player->tp_counter>0)
		App->tp_mode_enabled = true;

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_UP)
		App->tp_mode_enabled = false;
	
	if(App->map)
		App->map->Draw();

	//-------------------------------debug die
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		App->player->dead = true;

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

bool j1Scene::UI_Do(const UI_Elem* elem, Events* event)
{
	if (elem->type == UI_ElemType::BUTTON)
	{
		Button* button = (Button*)elem;
		switch (button->btype)
		{
			case UI_ButtonType::NO_BUTTONTYPE:
			{
				return false;
			}
			break;
			case UI_ButtonType::PLAY:
			{
				if (*event == Events::LEFT_UNCLICKED)
				{
					App->actual_lvl = Levels::FIRST_LEVEL;
					App->scene->Reset();
				}
			}
			break;
			case UI_ButtonType::SETTINGS:
			{
				if (*event == Events::LEFT_UNCLICKED)
				{
					App->actual_lvl = Levels::SETTINGS_SCREEN;
					App->scene->Reset();
				}
			}
			break;
			case UI_ButtonType::EXIT:
			{
				if (*event == Events::LEFT_UNCLICKED)
				{
					return false;
				}
			}
			break;
		}
	}
}

void j1Scene::LoadCollectibleObjects()
{
	for (p2List_item<MapLayer*>* layer_it = App->map->data.LayerList.start; layer_it != nullptr; layer_it = layer_it->next)
	{
		if (strcmp(layer_it->data->name.GetString(), "logical debug") != 0)
			continue;

		for (int y = 0; y < layer_it->data->height; ++y)
		{
			for (int x = 0; x < layer_it->data->width; ++x)
			{
				int tile_id = layer_it->data->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? App->map->GetTilesetFromTileId(tile_id) : NULL;
				if (tileset && tile_id - tileset->firstgid == 10)
				{
					App->entities_manager->CreateEntity(EntityType::GEAR, { (float)x*tileset->tile_width, (float)y*tileset->tile_height });
				}
			}
		}
	}
}