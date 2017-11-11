#include "j1Enemies.h"


void Enemy::Move()
{

}



void Enemy::Draw() const
{

}

bool j1Enemies::Awake(pugi::xml_node& node)
{
	return true;
}

bool j1Enemies::Start()
{
	return true;
}

bool j1Enemies::CleanUp()
{
	return true;
}

bool j1Enemies::PreUpdate()
{
	return true;
}

bool j1Enemies::Update(float dt)
{
	return true;
}

bool j1Enemies::PostUpdate()
{
	return true;
}

bool j1Enemies::Load(pugi::xml_node& node)
{
	return true;
}

bool j1Enemies::Save(pugi::xml_node& node) const
{
	return true;
}
