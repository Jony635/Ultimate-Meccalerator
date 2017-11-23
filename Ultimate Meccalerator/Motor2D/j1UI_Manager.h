#ifndef __j1UI_MANAGER_H__
#define __j1UI_MANAGER_H__

#include "j1Module.h"
#include "j1App.h"
#include "p2Point.h"
#include "p2List.h"
#include "p2SString.h"

struct SDL_Texture;

//------------ENUMS---------------------------------------------------------

enum UI_ElemType
{
	NO_ELEMTYPE = -1,
	IMAGE,
	LABEL,
	BUTTON,
	CHECKBOX
};

enum UI_ButtonType
{
	NO_BUTTONTYPE = -1,
	CONFIG,
	EXIT
};

//------------FONTS----------------------------------------------------------

class UI_Font
{

};

//------------UI_ELEM HERITAGE-----------------------------------------------

class UI_Elem
{
public:
	iPoint position;
	UI_ElemType type;
public:
	virtual bool Update();
};

class NO_InteractuableElem : public UI_Elem
{
public:
	virtual bool Update();
};

class Image : public NO_InteractuableElem
{
private:
	SDL_Texture* texture;
public:
	bool Update();
};

class Label : public NO_InteractuableElem
{
	p2SString string;

};

class Interactuable : public UI_Elem
{
private:
	j1Rect collider;
public:
	virtual bool Update();
	bool CheckWithMouse();
	virtual void Do();
};


//------------UI_MANAGER MODULE--------------------------------------

class UI_Manager : public j1Module
{
public:
	UI_Manager();
	virtual ~UI_Manager();
	virtual bool Update();


public:
	UI_Elem* CreateUIElem(UI_ElemType type, UI_ButtonType btype = NO_BUTTONTYPE, char* string = nullptr);


private:
	p2List<UI_Elem*> UI_ElemList;

	
};




#endif