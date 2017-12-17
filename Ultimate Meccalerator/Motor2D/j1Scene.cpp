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
#include "j1FileSystem.h"

#define SCREEN_W 1024.0f
#define SCREEN_H 768.0f
#define MARGIN 10.0f
#define LABEL_X_MARGIN 8.0f
#define LABEL_Y_MARGIN 10.0f
#define SCREEN_MIDDLE_X_FOR_BUTTON SCREEN_W/2-190.0f/2//window.w/2-button.w/2
#define SCREEN_MIDDLE_X_FOR_TITLE SCREEN_W/2-870.0f/2//window.w/2-button.w/2

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
	bar_colour = 0;

	App->ui_manager->Activate();
	App->map->Activate();
	App->pathfinding->Activate();

	App->player->tp_counter = 3;

	switch (App->actual_lvl)
	{
		case Levels::MENU:
		{
			App->audio->PlayMusic("Resources/audio/music/MenuBSO.mp3");

			App->player->lifes = 3;

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
			fPoint play_pos = { SCREEN_MIDDLE_X_FOR_BUTTON,399};
			fPoint continue_pos = { SCREEN_MIDDLE_X_FOR_BUTTON,340};
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
			if(!App->fs->IsFileEmpty("save/save_game.xml"))
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

			//---------------------------------------------Buttons Rect---------------------------
			j1Rect atlasrec[Button_State::MAX_STATE] = { j1Rect(0,192,190,49), j1Rect(190,49,190,49), j1Rect(190,0,190,49) };

			//---------------------------------------------Images Rect----------------------------
			j1Rect setings_rect = { 13,600,507,79 };
			j1Rect label_rect = { 0,4,190,49 };
			j1Rect square_label_rect = { 190,198,49,49};

			//---------------------------------------------Buttons Positions--------------------------------
			fPoint back_pos = { 1024.0f - label_rect.rec.w - 10,768.0f - label_rect.rec.h - 10.0f };

			//--------------------------------------------Image Positions-----------------------------------
			fPoint settings_pos = { 1024.0f / 2 - 507.0f / 2,60.0f };

			//---------------------------------------------Buttons Colliders--------------------------------
			j1Rect back_col(back_pos, atlasrec[Button_State::DEFAULT].rec.w, atlasrec[Button_State::DEFAULT].rec.h);

			//---------------------------------------------Buttons------------------------------------------
			App->ui_manager->CreateUIElem(UI_ElemType::BUTTON, back_pos, &atlasrec[0], back_col, UI_ButtonType::BACK, "return", App->fonts->getFontbyName("kenvector_future"));

			//---------------------------------------------Images-------------------------------------------
			App->ui_manager->CreateUIElem(UI_ElemType::IMAGE, settings_pos, &setings_rect);

			//---------------------------------------------Slides-------------------------------------------
			App->ui_manager->CreateUIElem(UI_ElemType::SLIDEBAR, { 200, 300 }, &j1Rect(41, 464, 559, 68), j1Rect(200 + 25, 300, 559 - 100, 68), NO_BUTTONTYPE, "Music Volume", App->fonts->getFontbyName("zorque"));
			App->ui_manager->CreateUIElem(UI_ElemType::SLIDEBAR, { 200, 500 }, &j1Rect(41, 464, 559, 68), j1Rect(200 + 25, 500, 559 - 100, 68), NO_BUTTONTYPE, "FX Volume", App->fonts->getFontbyName("zorque"));

		}
		break;
		case Levels::CREDITS_SCREEN:
		{
			//----------------------------------------------Load background-----------------------
			App->render->camera.x = 0;
			App->render->fcamera.x = 0;
			App->map->Load("Menu_background.tmx");

			//---------------------------------------------Buttons Rect---------------------------
			j1Rect atlasrec[Button_State::MAX_STATE] = { j1Rect(0,192,190,49), j1Rect(190,49,190,49), j1Rect(190,0,190,49) };

			//---------------------------------------------Images Rect----------------------------
			j1Rect title_rect = { 571,595,477,90 };
			j1Rect label_rect = { 0,4,190,49 };
			j1Rect square_label_rect = { 190,198,49,49 };

			//---------------------------------------------Buttons Positions--------------------------------
			fPoint back_pos = { 1024.0f - label_rect.rec.w - 10,768.0f - label_rect.rec.h - 10.0f };

			//--------------------------------------------Image Positions-----------------------------------
			fPoint title_pos = { 1024.0f / 2 - 507.0f / 2,60.0f };

			//---------------------------------------------Buttons Colliders--------------------------------
			j1Rect back_col(back_pos, atlasrec[Button_State::DEFAULT].rec.w, atlasrec[Button_State::DEFAULT].rec.h);

			//---------------------------------------------Buttons------------------------------------------
			App->ui_manager->CreateUIElem(UI_ElemType::BUTTON, back_pos, &atlasrec[0], back_col, UI_ButtonType::BACK, "return", App->fonts->getFontbyName("kenvector_future"));

			//---------------------------------------------Images-------------------------------------------
			App->ui_manager->CreateUIElem(UI_ElemType::IMAGE, title_pos, &title_rect);

			//---------------------------------------------Labels-------------------------------------------
			App->ui_manager->CreateUIElem(UI_ElemType::LABEL, { 50.0f,200.0f }, NULL, j1Rect(), NO_BUTTONTYPE, "Jonathan Molina and Oriol de Dios, members of FlanStudio, are the authors of this game.\n\n- Fonts from :\n http://www.1001fonts.com/video-game-fonts.html \n -Zorque font Made by Raymond Larabie\n- kenvector_future Made by Kenney\n\n Sprites from : https://www.gameart2d.com/free-sci-fi-platformer-tileset.html and https://mobilegamegraphics.com/product/monster-heroes-free-assets/ \n -Monster Heroes - Free Assets\n- FREE SCI - FI PLATFORMER TILESET\n\n- Music from:\nhttps://www.youtube.com/watch?v=6ukEy6FOxZE&list=PLfP6i5T0 \n-[No Copyright Music] Artificial.Music - Herbal Tea[Lo - fi]", App->fonts->getFontbyName("zorque_mini"));
		}
		break;
		case Levels::HOW_TO_PLAY_SCREEN:
		{
			//----------------------------------------------Load background-----------------------
			App->render->camera.x = 0;
			App->render->fcamera.x = 0;
			App->map->Load("Menu_background.tmx");

			//---------------------------------------------Buttons Rect---------------------------
			j1Rect atlasrec[Button_State::MAX_STATE] = { j1Rect(0,192,190,49), j1Rect(190,49,190,49), j1Rect(190,0,190,49) };

			//---------------------------------------------Images Rect----------------------------
			j1Rect title_rect = { 37,753,730,112 };
			j1Rect label_rect = { 0,4,190,49 };
			j1Rect square_label_rect = { 190,198,49,49 };

			//---------------------------------------------Buttons Positions--------------------------------
			fPoint back_pos = { 1024.0f - label_rect.rec.w - 10,768.0f - label_rect.rec.h - 10.0f };

			//--------------------------------------------Image Positions-----------------------------------
			fPoint title_pos = { 1024.0f / 2 - 753.0f / 2,60.0f };

			//---------------------------------------------Buttons Colliders--------------------------------
			j1Rect back_col(back_pos, atlasrec[Button_State::DEFAULT].rec.w, atlasrec[Button_State::DEFAULT].rec.h);

			//---------------------------------------------Buttons------------------------------------------
			App->ui_manager->CreateUIElem(UI_ElemType::BUTTON, back_pos, &atlasrec[0], back_col, UI_ButtonType::BACK, "return", App->fonts->getFontbyName("kenvector_future"));

			//---------------------------------------------Images-------------------------------------------
			App->ui_manager->CreateUIElem(UI_ElemType::IMAGE, title_pos, &title_rect);

			//---------------------------------------------Labels-------------------------------------------
			App->ui_manager->CreateUIElem(UI_ElemType::LABEL, { 50.0f,200.0f }, NULL, j1Rect(), NO_BUTTONTYPE, "- A to go left\n- D to go right\n- SPACEBAR to jump\n- SPACEBAR(AGAIN) to double jump(it only works if you have jumped once before from the floor)\n- TAB to pause the game and make a teleport (you only have 3)\n\nWIN : Reach the end of the levels and cross the final door\n\nLOSE : Fall into the acid or touch a spike kills the player and respawns it at the begin of the level\n\nACCELERATION AREAS give more velocity to the player in a particular direction", App->fonts->getFontbyName("zorque_mini"));}
		break;
		case Levels::FIRST_LEVEL:
		{
			App->map->Activate();
			App->map->Load("Level_1_x2.tmx");
			App->audio->PlayMusic("Resources/audio/music/BSO.ogg");
			App->enemies->Activate();
			App->entities_manager->Activate();
			App->player->Activate();

			App->enemies->FillEnemiesData();
			LoadCollectibleObjects();

			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data))
				App->pathfinding->SetMap(w, h, data);
			RELEASE_ARRAY(data);

			App->render->camera.x = 0;
			App->render->fcamera.x = 0;

			CreateIngameUI();

			label_time = (Label*)App->ui_manager->SearchElem(UI_ElemType::LABEL, UI_ButtonType::NO_BUTTONTYPE, nullptr, "time:");

		}
		break;
		case Levels::SECOND_LEVEL:
		{
			App->audio->PlayMusic("Resources/audio/music/BSO.ogg");

			App->map->Activate();
			App->map->Load("Level_2_x2.tmx");


			App->enemies->Activate();
			App->entities_manager->Activate();
			App->ui_manager->Activate();
			App->pathfinding->Activate();

			App->enemies->FillEnemiesData();
			LoadCollectibleObjects();

			App->render->camera.x = 0;
			App->render->fcamera.x = 0;

			App->player->Activate();


			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data))
				App->pathfinding->SetMap(w, h, data);
			RELEASE_ARRAY(data);

			CreateIngameUI();
			label_time = (Label*)App->ui_manager->SearchElem(UI_ElemType::LABEL, UI_ButtonType::NO_BUTTONTYPE, nullptr, "time:");
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
	App->entities_manager->DeActivate();
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

	if (reset)
	{
		this->Reset();
		reset = false;
	}

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
					App->gameON.Start();
					App->actual_lvl = Levels::FIRST_LEVEL;
					App->scene->reset = true;
				}
			}
			break;
			case UI_ButtonType::SETTINGS:
			{
				if (*event == Events::LEFT_UNCLICKED)
				{
					App->actual_lvl = Levels::SETTINGS_SCREEN;
					App->scene->reset = true;
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
			case UI_ButtonType::BACK:
			{
				if (*event == Events::LEFT_UNCLICKED)
				{
					App->actual_lvl = Levels::MENU;
					App->scene->reset = true;
				}
			}
			break;
			case UI_ButtonType::CREDITS:
			{
				if (*event == Events::LEFT_UNCLICKED)
				{
					App->actual_lvl = Levels::CREDITS_SCREEN;
					App->scene->reset = true;
				}
			}
			break;
			case UI_ButtonType::HOW_TO_PLAY:
			{
				if (*event == Events::LEFT_UNCLICKED)
				{
					App->actual_lvl = Levels::HOW_TO_PLAY_SCREEN;
					App->scene->reset = true;
				}
			}
			break;
		}
	}

	return true;
}

void j1Scene::CreateIngameUI()
{
	//------------------------------------Labels square rect------------------------------
	j1Rect label_square_rect = {0, 4, 190, 49};

	//------------------------------------Images position---------------------------------
	fPoint lifes_pos = { MARGIN,MARGIN};
	fPoint timer_pos = { MARGIN,SCREEN_H - label_square_rect.rec.h - MARGIN};
	fPoint tp_pos = { SCREEN_W - label_square_rect.rec.w - MARGIN,MARGIN};
	fPoint upgrade_bar_pos = { SCREEN_W - actual_bar_upgrade->rec.w - MARGIN, 150.0f };

	//------------------------------------Labels positions--------------------------------
	fPoint lifes_label_pos = { lifes_pos.x + LABEL_X_MARGIN,lifes_pos.y + LABEL_Y_MARGIN };
	fPoint timer_label_pos = { timer_pos.x + LABEL_X_MARGIN,timer_pos.y + LABEL_Y_MARGIN };
	fPoint tp_label_pos = { tp_pos.x + LABEL_X_MARGIN,tp_pos.y + LABEL_Y_MARGIN };
	fPoint time_label_pos = { timer_pos.x + LABEL_X_MARGIN + 50.0f,timer_pos.y + LABEL_Y_MARGIN };

	//------------------------------------Images------------------------------------------
	App->ui_manager->CreateUIElem(UI_ElemType::IMAGE, lifes_pos, &label_square_rect);
	App->ui_manager->CreateUIElem(UI_ElemType::IMAGE, timer_pos, &label_square_rect);
	App->ui_manager->CreateUIElem(UI_ElemType::IMAGE, tp_pos, &label_square_rect);

	//------------------------------------labels------------------------------------------
	App->ui_manager->CreateUIElem(UI_ElemType::LABEL, lifes_label_pos, NULL, label_square_rect,UI_ButtonType::NO_BUTTONTYPE,"lifes", App->fonts->getFontbyName("kenvector_future"));
	App->ui_manager->CreateUIElem(UI_ElemType::LABEL, timer_label_pos, NULL, label_square_rect, UI_ButtonType::NO_BUTTONTYPE, "time:", App->fonts->getFontbyName("kenvector_future"));
	App->ui_manager->CreateUIElem(UI_ElemType::LABEL, tp_label_pos, NULL, label_square_rect, UI_ButtonType::NO_BUTTONTYPE, "tp", App->fonts->getFontbyName("kenvector_future"));

	//------------------------------------Images------------------------------------------
	//App->ui_manager->CreateUIElem(UI_ElemType::IMAGE, upgrade_bar_pos, actual_bar_upgrade);
}

void j1Scene::Update_upgrade_bar(int upgrade_lvl)
{
	switch (upgrade_lvl)
	{
	case EMPTY:
		actual_bar_upgrade = &upgrade_lvl_empty_rect;
		App->player->Tp_circle_rect = TP_CIRCLE_RECT_DEFAULT;
		App->player->playerText = &App->player->playerWhite;
		break;
	case BLUE:
		actual_bar_upgrade = &upgrade_lvl_blue_rect;
		App->player->Tp_circle_rect = TP_CIRCLE_RECT_BLUE;
		App->player->playerText = &App->player->playerBlue;
		break;
	case YELLOW:
		actual_bar_upgrade = &upgrade_lvl_yellow_rect;
		App->player->Tp_circle_rect = TP_CIRCLE_RECT_YELLOW;
		App->player->playerText = &App->player->playerYellow;
		break;
	case RED:
		actual_bar_upgrade = &upgrade_lvl_red_rect;
		App->player->Tp_circle_rect = TP_CIRCLE_RECT_RED;
		App->player->playerText = &App->player->playerRed;
		break;
	default:
		break;
	}
	bar_colour = upgrade_lvl;
	App->render->Blit((SDL_Texture*)App->ui_manager->GetAtlas(), SCREEN_W - actual_bar_upgrade->rec.w - MARGIN - App->render->camera.x, 150.0f - App->render->camera.y,&actual_bar_upgrade->rec);
}
void j1Scene::Update_lifes(int lifes)
{
	SDL_Rect hard_rect = { 458,164,24,22 };
	if (lifes >= 1)
	{
		App->render->Blit((SDL_Texture*)App->ui_manager->GetAtlas(), 100 - App->render->camera.x, 20 - App->render->camera.y, &hard_rect);
		if (lifes >= 2)
		{
			App->render->Blit((SDL_Texture*)App->ui_manager->GetAtlas(), 130 - App->render->camera.x, 20 - App->render->camera.y, &hard_rect);
			if (lifes == 3)
			{
				App->render->Blit((SDL_Texture*)App->ui_manager->GetAtlas(), 160 - App->render->camera.x, 20- App->render->camera.y, &hard_rect);
			}
		}
	}
}
void j1Scene::Update_tp(int tp_left)
{
	SDL_Rect tp_rect = { 457,126,26,27 };
	if (tp_left >= 1)
	{
		App->render->Blit((SDL_Texture*)App->ui_manager->GetAtlas(), 900 - App->render->camera.x, 17 - App->render->camera.y, &tp_rect);
		if (tp_left >= 2)
		{
			App->render->Blit((SDL_Texture*)App->ui_manager->GetAtlas(), 930 - App->render->camera.x, 17 - App->render->camera.y, &tp_rect);
			if (tp_left == 3)
			{
				App->render->Blit((SDL_Texture*)App->ui_manager->GetAtlas(), 960 - App->render->camera.x, 17 - App->render->camera.y, &tp_rect);
			}
		}
	}
}
void j1Scene::Update_timer(int time)
{
	if(App->scene->label_time)
		App->scene->label_time->string.create("Time: %i", time);
}

void j1Scene::Update_UI(int upgrade_lvl,int lifes,int tp_left,int time)
{
	Update_upgrade_bar(upgrade_lvl);
	Update_lifes(lifes);
	Update_tp(tp_left);
	Update_timer(time);
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

