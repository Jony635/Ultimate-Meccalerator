#include "j1Player.h"
#include "j1Textures.h"
#include "j1Render.h"




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
	pos = { 51,377 };
	
	return true;
}

bool j1Player::Start()
{
	playerText = App->tex->Load("textures/TestingPlayer.png");
	return true;
}

bool j1Player::PreUpdate()
{

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



