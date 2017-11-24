#include "UI_Manager.h"
#include "j1FileSystem.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Textures.h"

//------------UI_ELEM HERITAGE METHODS-----------------------------------------------

	//-------CONSTRUCTORS-------------
	UI_Elem::UI_Elem(UI_ElemType type, iPoint position) : type(type), position(position) {}
	NO_InteractuableElem::NO_InteractuableElem(UI_ElemType type, iPoint position) : UI_Elem(type, position) {}
	InteractuableElem::InteractuableElem(UI_ElemType type, iPoint position, j1Rect col) : UI_Elem(type, position), collider(col){}
	Label::Label(UI_ElemType type, iPoint position, char* string, UI_Font* font) : NO_InteractuableElem(type, position), string(string), font(font) {}
	Image::Image(UI_ElemType type, iPoint position, SDL_Texture* texture) : NO_InteractuableElem(type, position), texture(texture) {}
	Button::Button(UI_ElemType type, iPoint position, j1Rect col, UI_ButtonType btype, Label* text) : InteractuableElem(type, position, col), btype(btype), text(text) {}
	CheckBox::CheckBox(UI_ElemType type, iPoint position, j1Rect col, Label* text = nullptr) : InteractuableElem(type, position, col), text(text){}
	//--------------------------------

//------------UI_MANAGER METHODS-----------------------------------------------------

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
	case UI_ElemType::BUTTON: //NOTE: EACH BUTTON MUST SET HERE HIS COLLIDER SIZE AND HIS LABEL POS
		switch (btype)
		{
			Label* label = nullptr;
		case UI_ButtonType::EXIT:
			if (string)
			{
				label = new Label(LABEL, pos, string, getFontbyName("generic_font")); //This is just an example.
				if (label)
					UI_ElemList.add(label);
			}
			elem = new Button(type, pos, j1Rect(pos, 45, 53), btype, label); //This is just an example.
			break;
		case UI_ButtonType::CONFIG:
			if (string)
			{
				label = new Label(LABEL, pos, string, getFontbyName("generic_font")); //This is just an example.
				if (label)
					UI_ElemList.add(label);
			}
			elem = new Button(type, pos, j1Rect(pos, 23, 57), btype, label); //This is just an example.
			break;
		}
		break;
	//-----CHECKBOX--------------------------------------------------------
	case UI_ElemType::CHECKBOX:
		Label* label = nullptr;
		if (string)
		{
			label = new Label(LABEL, pos, string, getFontbyName("generic_font")); //This is just an example.
			if (label)
				UI_ElemList.add(label);
		}
		elem = new CheckBox(type, pos, j1Rect(pos, 23, 57), label); 
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

//Update all elems
bool UI_Manager::Update()
{
	p2List_item<UI_Elem*>* elem = UI_ElemList.start;
	while (elem)
	{
		elem->data->Update();
		elem = elem->next;
	}
}

//Search in Font List by a char* name
UI_Font* UI_Manager::getFontbyName(char* string) const
{
	if (string)
	{
		p2List_item<UI_Font*>* font = UI_FontList.start;
		while (font)
		{
			if (font->data->name == string)
				return font->data;
			font = font->next;
		}
	}
}