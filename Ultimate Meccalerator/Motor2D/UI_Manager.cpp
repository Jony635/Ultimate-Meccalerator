#include "UI_Manager.h"
#include "j1FileSystem.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Textures.h"

//------------UI_ELEM HERITAGE METHODS-----------------------------------------------
UI_Elem::UI_Elem(UI_ElemType type, iPoint position) : type(type), position(position) {}
NO_InteractuableElem::NO_InteractuableElem(UI_ElemType type, iPoint position) : UI_Elem(type, position) {}
Label::Label(UI_ElemType type, iPoint position, char* string, UI_Font* font) : NO_InteractuableElem(type, position), string(string), font(font) {}
Image::Image(UI_ElemType type, iPoint position, SDL_Texture* texture) : NO_InteractuableElem(type, position), texture(texture) {}

UI_Manager::UI_Manager() : j1Module()
{
	name.create("UI_Manager");
}
UI_Elem* UI_Manager::CreateUIElem(UI_ElemType type, iPoint pos, UI_ButtonType btype, char* string, UI_Font* font)
{
	UI_Elem* elem = nullptr;
	switch (type)
	{
	//-----LABEL----------------------------------------------------------
	case UI_ElemType::LABEL:
		if (string && font)
		{
			elem = new Label(type, pos, string, font);
		}
		else
		{
			if (!string)
				LOG("Invalid string creating Label");
			if(!font)
				LOG("Invalid font creating Label");
		}
		break;
	//-----IMAGE-----------------------------------------------------------
	case UI_ElemType::IMAGE:
		if (string)
			elem = new Image(type, pos, App->tex->Load(string));
		else
			LOG("Invalid string creating Image");
		break;
	////-----BUTTON--------------------------------------------------------
	case UI_ElemType::BUTTON:

		break;
	//-----CHECKBOX--------------------------------------------------------
	case UI_ElemType::CHECKBOX:

		break;
	}

	if (elem)
	{
		UI_ElemList.add(elem);
		return elem;
	}
	else
	{
		LOG("ERROR: UNVALID_ELEM. Creating UI_Elem failed");
	}
}