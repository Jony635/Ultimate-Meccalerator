#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "p2SString.h"
#include "j1App.h"
#include "p2DynArray.h"

#define CURSOR_WIDTH 2

struct SDL_Texture;
class TTF_Font;

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


//------------UI_ELEM HERITAGE-----------------------------------------------

class UI_Elem
{
public:
	iPoint position;
	UI_ElemType type;
public:
	UI_Elem(UI_ElemType type, iPoint position);
	virtual bool Update();
};

class NO_InteractuableElem : public UI_Elem
{
public:
	NO_InteractuableElem(UI_ElemType type, iPoint position);
	virtual bool Update();
};

class Image : public NO_InteractuableElem
{
private:
	SDL_Texture* texture;
public:
	Image(UI_ElemType type, iPoint position, SDL_Texture* texture);
	bool Update();
};

class Label : public NO_InteractuableElem
{
private:
	p2SString string;
	TTF_Font* font = nullptr;
public:
	Label(UI_ElemType type, iPoint position, char* string, TTF_Font* font);
	bool Update();
};

class InteractuableElem : public UI_Elem
{
private:
	j1Rect collider;
public:
	InteractuableElem(UI_ElemType type, iPoint position, j1Rect col);
	virtual bool Update();
	bool CheckWithMouse();
	virtual void Do();
};

class Button : public InteractuableElem
{
private:
	UI_ButtonType btype;
	Label* text;
public:
	Button(UI_ElemType type, iPoint position, j1Rect col, UI_ButtonType btype, Label* text = nullptr);
};

class CheckBox : public InteractuableElem
{
private:
	Label* text;
public:
	CheckBox(UI_ElemType type, iPoint position, j1Rect col, Label* text = nullptr);
};

//------------UI_MANAGER MODULE--------------------------------------

class UI_Manager : public j1Module
{
public:
	UI_Manager();
	virtual ~UI_Manager();

	bool Start();
	bool Awake(pugi::xml_node& uimnode);
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

public:
	UI_Elem* CreateUIElem(UI_ElemType type, iPoint pos, UI_ButtonType btype = NO_BUTTONTYPE, char* string = nullptr, TTF_Font* font = nullptr);
	const SDL_Texture* GetAtlas() const;
private:
	SDL_Texture* atlas = nullptr; //Texture that has everything
	p2SString atlas_file_name;
	p2List<UI_Elem*> UI_ElemList;
};




#endif