#include "j1Player.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Scene.h"


j1Player::j1Player() : j1Module()
{
	name.create("player");
	standard_anim.PushBack({ 16, 34, 23, 46 });
	standard_anim.PushBack({81, 35, 18, 45});
	standard_anim.PushBack({137, 37, 23, 44});
	standard_anim.PushBack({201, 35, 19, 45});
	//standard_anim.PushBack({256, 34, 23, 46});
	standard_anim.speed = 0.05f;

	GoRight.PushBack({16, 216, 29, 40});
	GoRight.PushBack({76, 216, 29, 44});
	GoRight.PushBack({133, 216, 31, 45});
	GoRight.PushBack({190, 216, 33, 44});
	GoRight.PushBack({248, 216, 33, 45});
	GoRight.PushBack({307, 216, 36, 44});
	GoRight.PushBack({14, 276, 32, 40});
	GoRight.PushBack({72, 276, 32, 43});
	GoRight.PushBack({132, 276, 29, 44});
	GoRight.PushBack({188, 276, 35, 44});
	GoRight.PushBack({248, 276, 36, 45});
	GoRight.PushBack({311, 276, 34, 44});
	GoRight.speed = 0.15f;
}

// Destructor
j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& playernode)
{
	speed_x = (playernode.child("speed").attribute("tiles_sec").as_float());
	tiles_sec_jump = playernode.child("speed_jump").attribute("tiles_sec").as_float();
	current_anim = &standard_anim;
	return true;
}

bool j1Player::Start()
{
	SetStartingPos();
	pos = Startingpos;
	speed_x = (speed_x * App->map->data.tile_width) / 60;
	playerText = App->tex->Load("textures/Player_SpriteSheet.png");
	


	return true;
}

bool j1Player::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		if(current_anim!=&GoRight)
		current_anim = &GoRight;
		pos.x += speed_x;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		if (current_anim != &standard_anim)
		current_anim = &standard_anim;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		pos.x -= speed_x;
	}

	if (jumps>0 && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		jumps--;
		speed_y = (tiles_sec_jump*App->map->data.tile_height) / 60;
		grounded = false;
	}
	

	return true;
}


bool j1Player::Update(float dt)
{
	if(grounded==false)
	{
		pos.y -= speed_y;
		speed_y -= App->scene->Gravity;
	}
	if (CheckDownPos({ (int)pos.x, (int)pos.y + App->map->data.tile_height / 2 }) == false)
		grounded = false;
	if (grounded == false && speed_y<0 && CheckDownPos({ (int)pos.x, (int)pos.y+App->map->data.tile_height/2 }))
	{
		
		grounded = true;
		jumps = 2;
		speed_y = 0;
	}
	App->render->Blit(playerText, pos.x, pos.y, &current_anim->GetCurrentFrame());
	return true;
}



bool j1Player::PostUpdate()
{
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

bool j1Player::Load(pugi::xml_node&)
{
	return true;
}

bool j1Player::Save(pugi::xml_node&) const
{

	return true;
}
void j1Player::SetStartingPos()
{
	for (p2List_item<TileSet*>* TileSet = App->map->data.tilesets.start; TileSet != nullptr; TileSet = TileSet->next)
	{
		for (p2List_item<MapLayer*>* layer = App->map->data.LayerList.start; layer != nullptr; layer = layer->next)
		{
			int x = 0, y = 0;
			for (int num_tile = 0; num_tile < layer->data->size_data; ++num_tile)
			{
				if (*(layer->data->data + num_tile) == 2603)
				{
					Startingpos = fPoint(x, y+App->map->data.tile_height/2);
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

bool j1Player::CheckDownPos(iPoint pos) const
{
	iPoint pos_tile=App->map->World_to_Map(pos);
	
	for (p2List_item<TileSet*>* TileSet = App->map->data.tilesets.start; TileSet != nullptr; TileSet = TileSet->next)
	{
		for (p2List_item<MapLayer*>* layer = App->map->data.LayerList.start; layer != nullptr; layer = layer->next)
		{
			if (strcmp(layer->data->name.GetString(), "logical debug") != 0)
				continue;
				int x = 0, y = 0;
			for (int num_tile = 0; num_tile < layer->data->size_data; ++num_tile)
			{
				
				if(x== pos_tile.x * TileSet->data->tile_width && (y==(pos_tile.y+1)*TileSet->data->tile_height))
					if (*(layer->data->data + num_tile) == 2601+8)
					{
						return true;
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
	return false;


}


