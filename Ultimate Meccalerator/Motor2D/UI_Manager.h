#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "p2SString.h"
#include "j1App.h"
#include "p2DynArray.h"
#include "j1Fonts.h"

#define CURSOR_WIDTH 2

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


//------------UI_ELEM HERITAGE-----------------------------------------------

class UI_Elem
{
public:
	iPoint position;
	UI_ElemType type;
public:
	UI_Elem(UI_ElemType type, iPoint position);
	virtual ~UI_Elem();
	virtual bool Update(float dt);
	
};

class NO_InteractuableElem : public UI_Elem
{
public:
	NO_InteractuableElem(UI_ElemType type, iPoint position);
	virtual ~NO_InteractuableElem();
	virtual bool Update(float dt);
};

class InteractuableElem : public UI_Elem
{
private:
	j1Rect collider;
public:
	InteractuableElem(UI_ElemType type, iPoint position, j1Rect col);
	virtual ~InteractuableElem();

	virtual bool Update(float dt);
	bool CheckWithMouse();
	virtual void Do(float dt);
};

class Image : public NO_InteractuableElem
{
private:
	SDL_Rect rec;
public:
	Image(UI_ElemType type, iPoint position, SDL_Rect rec);
	virtual ~Image();
	bool Update(float dt);
};

class Label : public NO_InteractuableElem
{
private:
	p2SString string;
	TTF_Font* font = nullptr;
public:
	Label(UI_ElemType type, iPoint position, char* string, TTF_Font* font);
	virtual ~Label();
	bool Update(float dt);
};

class Button : public InteractuableElem
{
private:
	UI_ButtonType btype;
	Label* text;
	SDL_Rect atlasRec;
public:
	Button(UI_ElemType type, iPoint position, j1Rect col, UI_ButtonType btype, Label* text = nullptr);
	virtual ~Button();
	void Do(float dt);
};

class CheckBox : public InteractuableElem
{
private:
	Label* text;
	bool state;
	SDL_Rect recTrue;
	SDL_Rect recFalse;
	SDL_Rect* actualRec;
public: 
	CheckBox(UI_ElemType type, iPoint position, j1Rect col, Label* text = nullptr);
	virtual ~CheckBox();
	void Do(float dt);
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
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:
	UI_Elem* CreateUIElem(UI_ElemType type, iPoint pos, const SDL_Rect& rec = SDL_Rect(), UI_ButtonType btype = NO_BUTTONTYPE, char* string = nullptr, TTF_Font* font = nullptr);
	const SDL_Texture* GetAtlas() const;
private:
	SDL_Texture* atlas = nullptr; //Texture that has everything
	p2SString atlas_file_name;
	p2List<UI_Elem*> UI_ElemList;
};




#endif