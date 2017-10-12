#include "j1Player.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");
}

// Destructor
j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& playernode)
{
	
	return true;
}

bool j1Player::Start()
{
	return true;
}

bool j1Player::PreUpdate()
{

	return true;
}


bool j1Player::Update(float dt)
{


	return true;
}



bool j1Player::PostUpdate()
{
	return true;
}

bool j1Player::CleanUp()
{
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



