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
enum Events
{
	NO_EVENT = -1,
	MOUSE_ENTER,
	MOUSE_LEAVE,
	LEFT_CLICKED,
	LEFT_UNCLICKED,
	RIGHT_CLICKED,
	RIGHT_UNCLICKED
};

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
	PLAY,
	CONTINUE,
	EXIT
};

enum Button_State
{
	DEFAULT,
	MOUSE_ON,
	CLICKED,
	MAX_STATE
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
	friend class UI_Manager;
protected:
	j1Rect collider;
	Events state = Events::NO_EVENT;
protected:
	p2List<j1Module*> listeners;

public:
	InteractuableElem(UI_ElemType type, iPoint position, j1Rect col);
	virtual ~InteractuableElem();

	virtual bool Update(float dt);
	bool CheckWithMouse(float dt);
	virtual bool Do(float dt);
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
	friend class j1Scene;
	friend class j1Audio;
private:
	UI_ButtonType btype;
	Label* text;
	j1Rect atlasRec[Button_State::MAX_STATE];
public:
	SDL_Rect BlitRec;
public:
	Button(UI_ElemType type, iPoint position, const j1Rect& col, UI_ButtonType btype, j1Rect* atlasRec = nullptr, Label* text = nullptr);
	bool Update(float dt);
	virtual ~Button();
	bool Do(float dt);
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
	bool Do(float dt);
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
	UI_Elem* CreateUIElem(UI_ElemType type, iPoint pos, j1Rect* atlasRec = nullptr, const j1Rect& col = j1Rect(), UI_ButtonType btype = NO_BUTTONTYPE, char* string = nullptr, TTF_Font* font = nullptr);
	const SDL_Texture* GetAtlas() const;
private:
	SDL_Texture* atlas = nullptr; //Texture that has everything
	p2SString atlas_file_name;
	p2List<UI_Elem*> UI_ElemList;
public:
	bool Button_Clicked = true;
	Events last_event;
	iPoint last_elem_pos = { 0,0 };
};




#endif