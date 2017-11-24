#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "j1Module.h"
#include "j1App.h"
#include "p2Point.h"
#include "p2List.h"
#include "p2SString.h"
#include "p2DynArray.h"

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

struct dev_Char
{
	char _char;
	iPoint coord;
	int width;
};

class UI_Font
{
public:
	char* name = nullptr;
private:
	SDL_Texture* texture;
	p2DynArray<dev_Char*> chars;
	
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
	UI_Font* font = nullptr;
public:
	Label(UI_ElemType type, iPoint position, char* string, UI_Font* font);
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
	virtual bool Update();

	bool LoadFonts();
	bool DestroyFonts();

public:
	UI_Elem* CreateUIElem(UI_ElemType type, iPoint pos, UI_ButtonType btype = NO_BUTTONTYPE, char* string = nullptr, UI_Font* font = nullptr);
	UI_Font* getFontbyName(char* name) const;

private:
	SDL_Texture* UI_Texture; //Texture for buttons and checkboxes
	p2List<UI_Elem*> UI_ElemList;
	p2List<UI_Font*> UI_FontList;
};




#endif