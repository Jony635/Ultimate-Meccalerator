#include "j1Player.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Input.h"


j1Player::j1Player() : j1Module()
{
	name.create("player");
	standard_anim.PushBack({ 0,0,30,90 });
}

// Destructor
j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& playernode)
{
	speed = (playernode.child("speed").attribute("tiles_sec").as_int());
	
	return true;
}

bool j1Player::Start()
{
	speed = (speed * App->map->data.tile_width) / 60;
	playerText = App->tex->Load("textures/TestingPlayer.png");
	return true;
}

bool j1Player::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		pos.x += speed;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		pos.x -= speed;
	}
	return true;
}


bool j1Player::Update(float dt)
{

	App->render->Blit(playerText, pos.x, pos.y, &standard_anim.GetCurrentFrame());
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



