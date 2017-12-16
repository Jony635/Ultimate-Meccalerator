#include "j1Player.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "Brofiler\Brofiler.h"

#define PLAYER_H 58
#define PLAYER_W 36
#define TP_CIRCLE_RECT_DEFAULT {825,12,368,368}
#define TP_CIRCLE_RECT_WHITE {885,72,249,249}
#define TP_CIRCLE_RECT_RED {826,13,366,366}
#define TP_CIRCLE_RECT_YELLOW {853,40,312,312}
#define TP_CIRCLE_RECT_BLUE {868,55,281,281}
#define PLAYER_SHAPE_RECT {827,393,36,58}


j1Player::j1Player() : j1Module()
{
	name.create("player");

	IdleRight.PushBack({ 73, 14, 32, 50 });
	IdleRight.PushBack({ 133, 14, 32, 50 });
	IdleRight.PushBack({ 193, 14, 32, 50 });
	IdleRight.PushBack({ 253, 14, 32, 50 });
	IdleRight.speed = 7.0f;

	IdleLeft.PushBack({ 494, 14, 32, 50 });
	IdleLeft.PushBack({ 554, 14, 32, 50 });
	IdleLeft.PushBack({ 614, 14, 32, 50 });
	IdleLeft.PushBack({ 674, 14, 32, 50 });
	IdleLeft.speed = 7.0f;


	GoRight.PushBack({ 12, 196, 36, 44});
	GoRight.PushBack({ 72, 196, 36, 44 });
	GoRight.PushBack({ 132, 196, 36, 44 });
	GoRight.PushBack({ 192, 196, 36, 44 });
	GoRight.PushBack({ 252, 196, 36, 44 });
	GoRight.PushBack({ 312, 196, 36, 44 });
	GoRight.PushBack({ 12, 256, 36, 44 });
	GoRight.PushBack({ 72, 256, 36, 44 });
	GoRight.PushBack({ 132, 256, 36, 44 });
	GoRight.PushBack({ 192, 256, 36, 44 });
	GoRight.PushBack({ 252, 256, 36, 44 });
	GoRight.PushBack({ 312, 256, 36, 44 });
	GoRight.speed = 13.0f;

	GoLeft.PushBack({ 672, 196, 36, 44 });
	GoLeft.PushBack({ 612, 196, 36, 44 });
	GoLeft.PushBack({ 552, 196, 36, 44 });
	GoLeft.PushBack({ 492, 196, 36, 44 });
	GoLeft.PushBack({ 432, 196, 36, 44 });
	GoLeft.PushBack({ 372, 196, 36, 44 });
	GoLeft.PushBack({ 672, 256, 36, 44 });
	GoLeft.PushBack({ 612, 256, 36, 44 });
	GoLeft.PushBack({ 552, 256, 36, 44 });
	GoLeft.PushBack({ 492, 256, 36, 44 });
	GoLeft.PushBack({ 432, 256, 36, 44 });
	GoLeft.PushBack({ 372, 256, 36, 44 });
	GoLeft.speed = 13.0f;

	DieGoingRight.PushBack({ 7, 73, 46, 48 });
	DieGoingRight.PushBack({ 67, 73, 46, 48 });
	DieGoingRight.PushBack({ 127, 73, 46, 48 });
	DieGoingRight.PushBack({ 187, 73, 46, 48 });
	DieGoingRight.PushBack({ 247, 73, 46, 48 });
	DieGoingRight.PushBack({ 307, 73, 46, 48 });
	DieGoingRight.PushBack({ 7, 133, 46, 48 });
	DieGoingRight.PushBack({ 67, 133, 46, 48 });
	DieGoingRight.PushBack({ 127, 133, 46, 48 });
	DieGoingRight.PushBack({ 187, 133, 46, 48 });
	DieGoingRight.PushBack({ 247, 133, 46, 48 });
	DieGoingRight.PushBack({ 307, 133, 46, 48 });
	DieGoingRight.speed = 7.0f;
	DieGoingRight.loop = false;

	DieGoingLeft.PushBack({ 666, 73, 46, 48 });
	DieGoingLeft.PushBack({ 606, 73, 46, 48 });
	DieGoingLeft.PushBack({ 546, 73, 46, 48 });
	DieGoingLeft.PushBack({ 486, 73, 46, 48 });
	DieGoingLeft.PushBack({ 426, 73, 46, 48 });
	DieGoingLeft.PushBack({ 366, 73, 46, 48 });
	DieGoingLeft.PushBack({ 666, 133, 46, 48 });
	DieGoingLeft.PushBack({ 606, 133, 46, 48 });
	DieGoingLeft.PushBack({ 546, 133, 46, 48 });
	DieGoingLeft.PushBack({ 486, 133, 46, 48 });
	DieGoingLeft.PushBack({ 426, 133, 46, 48 });
	DieGoingLeft.PushBack({ 366, 133, 46, 48 });
	DieGoingLeft.speed = 7.0f;

	DoubleJump_GoingRight.PushBack({ 7, 344, 46, 48 });
	DoubleJump_GoingRight.PushBack({ 67, 344, 46, 48 });
	DoubleJump_GoingRight.PushBack({ 127, 344, 46, 48 });
	DoubleJump_GoingRight.PushBack({ 187, 344, 46, 48 });
	DoubleJump_GoingRight.PushBack({ 247, 344, 46, 48 });
	DoubleJump_GoingRight.PushBack({ 307, 344, 46, 48 });
	DoubleJump_GoingRight.PushBack({ 7, 413, 46, 68 });
	DoubleJump_GoingRight.PushBack({ 67, 398, 47, 81 });
	DoubleJump_GoingRight.PushBack({ 127, 397, 46, 81 });
	DoubleJump_GoingRight.PushBack({ 187, 414, 46, 68 });
	DoubleJump_GoingRight.PushBack({ 247, 427, 46, 48 });
	DoubleJump_GoingRight.PushBack({ 307, 435, 46, 46 });
	DoubleJump_GoingRight.speed = 20.0f;
	DoubleJump_GoingRight.loop = false;

	DoubleJump_GoingLeft.PushBack({ 666, 344, 46, 48 });
	DoubleJump_GoingLeft.PushBack({ 606, 344, 46, 48 });
	DoubleJump_GoingLeft.PushBack({ 546, 344, 46, 48 });
	DoubleJump_GoingLeft.PushBack({ 486, 344, 46, 48 });
	DoubleJump_GoingLeft.PushBack({ 426, 344, 46, 48 });
	DoubleJump_GoingLeft.PushBack({ 366, 344, 46, 48 });
	DoubleJump_GoingLeft.PushBack({ 666, 413, 46, 68 });
	DoubleJump_GoingLeft.PushBack({ 606, 398, 47, 81 });
	DoubleJump_GoingLeft.PushBack({ 546, 397, 46, 81 });
	DoubleJump_GoingLeft.PushBack({ 486, 414, 46, 68 });
	DoubleJump_GoingLeft.PushBack({ 426, 427, 46, 48 });
	DoubleJump_GoingLeft.PushBack({ 366, 435, 46, 46 });
	DoubleJump_GoingLeft.speed = 20.0f;
	DoubleJump_GoingLeft.loop = false;
}

// Destructor
j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& playernode)
{
	speed_x = (playernode.child("speed").attribute("tiles_sec").as_float());
	tiles_sec_jump = playernode.child("speed_jump").attribute("tiles_sec").as_float();
	current_anim = &IdleRight;
	return true;
}

bool j1Player::Start()
{
	playerText = App->tex->Load("Resources/textures/White_Player_SpriteSheet.png");

	Tp_circle_rect = TP_CIRCLE_RECT_WHITE;

	DieGoingRight.Reset();
	current_anim = &IdleRight;

	dieCounter = 0;
	dead = false;

	SetStartingPos();
	pos = Startingpos;

	player_col.rec.x = pos.x;
	player_col.rec.y = pos.y;
	player_col.rec.w = 32;
	player_col.rec.h = 48;
	
	if(!alreadyLoaded)
	{
		speed_x = (speed_x * App->map->data.tile_width) / 60;
		standard_speed_x = speed_x;
		alreadyLoaded = true;
	}
	speed_x = standard_speed_x;


	return true;
}

bool j1Player::PreUpdate()
{
	

	return true;
}

bool j1Player::Update(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::Orchid);

	if(!App->tp_mode_enabled)
		CheckAccels(dt);

	if (App->godmode==false && (CheckDieCol({ (int)pos.x+1, (int)pos.y + 15 }) || dead))
	{
		if (!diesoundplayed)
		{
			App->audio->PlayFx(App->audio->dieSound);
			diesoundplayed = true;
		}
		dead = true;
		tp_counter = 3;
	}
	
	if (!dead)
	{
		diesoundplayed = false;
		CheckMovements(dt);
	}
	else
	{
		if (current_anim != &DieGoingRight)
			current_anim = &DieGoingRight;
	}

	if(!App->tp_mode_enabled)
	CheckFalls(dt);

	player_col.rec.x = (int)pos.x;
	player_col.rec.y = (int)pos.y;

	if (App->tp_mode_enabled)
	TpMode();

	return true;
}

bool j1Player::PostUpdate()
{
	CheckWin();
	
	if (current_anim != &DieGoingRight || !current_anim->Finished())
	{
		App->render->Blit(playerText, pos.x, pos.y, &current_anim->GetCurrentFrame());
	}

	else
	{
		if (dieCounter > 120)
		{
			current_anim->Reset();
			App->RestartScene();
		}
		else
		{
			App->render->Blit(playerText, pos.x, pos.y, &current_anim->frames[current_anim->last_frame - 1]);
			dieCounter = dieCounter + 1;
		}
	}
	return true;
}

bool j1Player::CleanUp()
{
	if (playerText != nullptr)
	{
		App->tex->UnLoad(playerText);
		playerText = nullptr;
	}
	return true;
}

bool j1Player::Load(pugi::xml_node& playernode)
{
	pos.x = playernode.child("lastPos").attribute("x").as_int();
	pos.y = playernode.child("lastPos").attribute("y").as_int();
	return true;
}

bool j1Player::Save(pugi::xml_node& playernode) const
{
	pugi::xml_node lastPos = playernode.append_child("lastPos");
	lastPos.append_attribute("x") = pos.x;
	lastPos.append_attribute("y") = pos.y;
	return true;
}

void j1Player::SetStartingPos()
{
	for (p2List_item<TileSet*>* TileSet = App->map->data.tilesets.start; TileSet != nullptr; TileSet = TileSet->next)
	{
		for (p2List_item<MapLayer*>* layer = App->map->data.LayerList.start; layer != nullptr; layer = layer->next)
		{
			if (strcmp(layer->data->name.GetString(), "logical debug") != 0)
				continue;
			int x = 0, y = 0;
			for (int num_tile = 0; num_tile < layer->data->size_data; ++num_tile)
			{
				if (*(layer->data->data + num_tile) == 5195)
				{
					Startingpos = fPoint(x, y+App->map->data.tile_height/2);
					break;
				}
				x += TileSet->data->tile_width;

				if (x % (layer->data->width * TileSet->data->tile_width) == 0)
				{
					x = 0;
					y += TileSet->data->tile_height;
				}
			}
		}
	}
}

uint j1Player::getDownYCol(iPoint pos) const
{
	iPoint pos_tile = App->map->World_to_Map(pos);
	for (p2List_item<TileSet*>* TileSet = App->map->data.tilesets.start; TileSet != nullptr; TileSet = TileSet->next)
	{
		for (p2List_item<MapLayer*>* layer = App->map->data.LayerList.start; layer != nullptr; layer = layer->next)
		{
			if (strcmp(layer->data->name.GetString(), "logical debug") != 0)
				continue;
			int x = 0, y = 0;
			for (int num_tile = 0; num_tile < layer->data->size_data; ++num_tile)
			{

				if (x == pos_tile.x * TileSet->data->tile_width && (y >= pos_tile.y * TileSet->data->tile_height))
					if (*(layer->data->data + num_tile) == 5193 + 8)
					{
						return y;
					}
				x += TileSet->data->tile_width;

				if (x % (layer->data->width * TileSet->data->tile_width) == 0)
				{
					x = 0;
					y += TileSet->data->tile_height;
				}
			}
		}
	}
	return 0;
}

void j1Player::CheckFalls(float dt)
{
	if (grounded == false)
	{
		if (CheckCol({ (int)pos.x + 20, (int)(pos.y + 42 - speed_y * 75 * dt) }) == false)
		{
			pos.y -= speed_y * 75 * dt;
			speed_y -= App->scene->Gravity * 75 * dt;
		}
		else
		{
			if(speed_y<0)
			{
				DoubleJump_GoingRight.Reset();
				DoubleJump_GoingLeft.Reset();
				current_anim = &IdleRight;
				pos.y = getDownYCol({ (int)pos.x + 20, (int)pos.y + 42 }) -42;
				grounded = true;
				jumps = 1;
				speed_y = 0;
			}
			else
				if (speed_y > 0)
				{
					pos.y -= speed_y * 75 * dt;
					speed_y -= App->scene->Gravity * 75 * dt;
				}
		}
	}

	if (CheckCol({ (int)pos.x + 20, (int)(pos.y+42)}) == false)
	{
		grounded = false;
	}

	if (grounded == false && speed_y<0 && CheckCol({ (int)(pos.x + 20), (int)pos.y +42}))
	{
		DoubleJump_GoingRight.Reset();
		DoubleJump_GoingLeft.Reset();
		current_anim = &IdleRight;
		grounded = true;
		jumps = 1;
		speed_y = 0;
	}
}

float j1Player::getAccelY(iPoint pos) const
{
	iPoint pos_tile = App->map->World_to_Map(pos);
	for (p2List_item<MapLayer*>* layer = App->map->data.LayerList.start; layer != nullptr; layer = layer->next)
	{
		if (strcmp(layer->data->name.GetString(), "logical debug") != 0)
			continue;
		int gid = layer->data->data[App->map->getTileid(pos_tile)];
		if (gid == 5193 + 1 || gid == 5193 + 7)
		{
			if (gid == 5193 + 1)
				return App->map->LogicalTileset->find_child_by_attribute("id", "1").child("properties").find_child_by_attribute("name", "accel_y").attribute("value").as_int();
			else
				return App->map->LogicalTileset->find_child_by_attribute("id", "7").child("properties").find_child_by_attribute("name", "accel_y").attribute("value").as_int();
		}
	}
	return 0;
}

float j1Player::getAccelX(iPoint pos) const
{
	iPoint pos_tile = App->map->World_to_Map(pos);
	
	
	for (p2List_item<MapLayer*>* layer = App->map->data.LayerList.start; layer != nullptr; layer = layer->next)
	{
		if (strcmp(layer->data->name.GetString(), "logical debug") != 0)
			continue;
		int gid = layer->data->data[App->map->getTileid(pos_tile)];
		if (gid == 5193 + 3 || gid == 5193 + 5)
		{
			if (gid == 5193 + 3)
				return App->map->LogicalTileset->find_child_by_attribute("id", "3").child("properties").find_child_by_attribute("name", "accel_x").attribute("value").as_int();
			else
				return App->map->LogicalTileset->find_child_by_attribute("id", "5").child("properties").find_child_by_attribute("name", "accel_x").attribute("value").as_int();
		}
	}
	return 0;
}

void j1Player::CheckAccels(float dt)
{
	float accel_y = getAccelY({ (int)pos.x, (int)pos.y + 30});
	if (accel_y != 0)
	{
		speed_y -= (accel_y / 60) * 75 * dt;
		App->audio->PlayFx(App->audio->accelsound);
	}
		

	float accel_x = getAccelX({ (int)pos.x, (int)pos.y });
	if (accel_x != 0)
	{
		App->audio->PlayFx(App->audio->accelsound);
		speed_x += (accel_x / 60) * 75 * dt;
		if (!CheckCol({(int)(pos.x+speed_x + App->map->data.tile_width), (int)pos.x+30})) // Right
		pos.x += speed_x * 75 * dt;
	}
	else
		if (speed_x > standard_speed_x)
			speed_x -= 9.2 / 60 * 75 * dt;
		else
			if (speed_x < standard_speed_x)
				speed_x = standard_speed_x;
}

void j1Player::CheckWin()
{
	iPoint pos_tile = App->map->World_to_Map({ (int)pos.x, (int)pos.y });
	for (p2List_item<MapLayer*>* layer = App->map->data.LayerList.start; layer != nullptr; layer = layer->next)
	{
		if (strcmp(layer->data->name.GetString(), "logical debug") != 0)
			continue;
		if (layer->data->data[App->map->getTileid(pos_tile)] == 5193 + 6)
		{
			win = true;
		}
	}
	if (win)
	{
		if (App->actual_lvl == Levels::FIRST_LEVEL)
		{
			App->actual_lvl = Levels::SECOND_LEVEL;
			win = false;
			App->RestartScene();
			
		}
		else
		{
			App->scene->CleanUp();
			App->render->defWin = true;
		}
	}
}

void j1Player::CheckMovements(float dt) 
{
	if (!App->tp_mode_enabled)
	{
		if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) &&
			pos.x + App->map->data.tile_width <= App->map->data.width*App->map->data.tile_width &&
			!CheckCol({ (int)pos.x + 3 + App->map->data.tile_width, (int)pos.y + 40 })) //Right
		{
			if (current_anim != &GoRight && current_anim->Finished())
			{
				current_anim = &GoRight;
			}

			pos.x += speed_x * 75 * dt;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			if (current_anim != &IdleRight)
				current_anim = &IdleRight;
		}

		if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) && pos.x > 0 &&
			!CheckCol({ (int)pos.x - 4, (int)pos.y + 40 })) //Left
		{
			if (current_anim != &GoLeft && current_anim->Finished())
			{
				current_anim = &GoLeft;
			}
			pos.x -= speed_x * 75 * dt;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			if (current_anim != &IdleLeft)
				current_anim = &IdleLeft;
		}

		if (jumps > 0 && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (grounded == false)
			{
				App->audio->PlayFx(App->audio->doublejumpsound);
				jumps--;
				if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				{
					if (current_anim != &DoubleJump_GoingRight)
						current_anim = &DoubleJump_GoingRight;
				}
				else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				{
					if (current_anim != &DoubleJump_GoingLeft)
						current_anim = &DoubleJump_GoingLeft;
				}
				else
				{
					if (current_anim != &DoubleJump_GoingRight)
						current_anim = &DoubleJump_GoingRight;
				}


			}
			speed_y = (tiles_sec_jump*App->map->data.tile_height) / 60;
			grounded = false;
		}
	}
}

bool j1Player::CheckDieCol(iPoint pos) const
{
	iPoint pos_tile = App->map->World_to_Map(pos);
	for (p2List_item<MapLayer*>* layer = App->map->data.LayerList.start; layer != nullptr; layer = layer->next)
	{
		if (strcmp(layer->data->name.GetString(), "logical debug") != 0)
			continue;
		if (layer->data->data[App->map->getTileid(pos_tile)] == 5193 + 0)
		{
			return true;
		}
	}

	return false;
}

bool j1Player::CheckCol(iPoint pos) const
{
	iPoint pos_tile = App->map->World_to_Map(pos);
	for (p2List_item<MapLayer*>* layer = App->map->data.LayerList.start; layer != nullptr; layer = layer->next)
	{
		if (strcmp(layer->data->name.GetString(), "logical debug") != 0)
			continue;
		if (layer->data->data[App->map->getTileid(pos_tile)] == 5193 + 8)
		{
			return true;
		}
	}
	return false;
}

//Teleport Mode
void j1Player::TpMode()
{
	//LOG("Tp paused mode");
	int mouse_x, mouse_y;
	SDL_Rect Player_shape_rect = PLAYER_SHAPE_RECT;

	App->input->GetMousePosition(mouse_x, mouse_y);
	App->render->Blit(App->player->playerText,
		App->player->pos.x - Tp_circle_rect.w / 2 + PLAYER_W / 2,
		App->player->pos.y - Tp_circle_rect.h / 2 + PLAYER_H / 2,
		&Tp_circle_rect);

	if (mouse_x > (App->render->camera.x) + App->player->pos.x - Tp_circle_rect.w / 2 + PLAYER_W / 2 &&
		mouse_x < (App->render->camera.x) + App->player->pos.x + Tp_circle_rect.w / 2 + PLAYER_W / 2 &&
		mouse_y >(App->render->camera.y) + App->player->pos.y - Tp_circle_rect.h / 2 + PLAYER_H / 2 &&
		mouse_y < (App->render->camera.y) + App->player->pos.y + Tp_circle_rect.h / 2 + PLAYER_H / 2)
	{
		if (App->player->CheckCol(iPoint((App->render->camera.x*-1) + mouse_x, (App->render->camera.y*-1) + mouse_y)) == false)
		{
			App->render->Blit(App->player->playerText,
				(App->render->camera.x*-1) + mouse_x - PLAYER_W / 2,
				(App->render->camera.y*-1) + mouse_y - PLAYER_H / 2,
				&Player_shape_rect);

			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				App->player->pos.x = (App->render->camera.x*-1) + mouse_x;
				App->player->pos.y = (App->render->camera.y*-1) + mouse_y;
				tp_counter--;
				App->tp_mode_enabled = false;
			}
		}
	}
}