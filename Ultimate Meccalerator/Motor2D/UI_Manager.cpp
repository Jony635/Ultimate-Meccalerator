#include "UI_Manager.h"
#include "j1FileSystem.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Audio.h"


//------------UI_ELEM HERITAGE METHODS-----------------------------------------------

	//-------CONSTRUCTORS-------------
	UI_Elem::UI_Elem(UI_ElemType type, iPoint position) : type(type), position(position) {}
	NO_InteractuableElem::NO_InteractuableElem(UI_ElemType type, iPoint position) : UI_Elem(type, position) {}
	InteractuableElem::InteractuableElem(UI_ElemType type, iPoint position, j1Rect col) : UI_Elem(type, position), collider(col){}
	Label::Label(UI_ElemType type, iPoint position, char* string, TTF_Font* font) : NO_InteractuableElem(type, position), string(string), font(font) {}
	Image::Image(UI_ElemType type, iPoint position, SDL_Rect rec) : NO_InteractuableElem(type, position), rec(rec) {}
	Button::Button(UI_ElemType type, iPoint position, j1Rect col, UI_ButtonType btype, j1Rect* atlasRec, Label* text) : InteractuableElem(type, position, col), btype(btype), text(text) 
	{
		this->atlasRec[Button_State::DEFAULT] = atlasRec[Button_State::DEFAULT];
		this->atlasRec[Button_State::MOUSE_ON] = atlasRec[Button_State::MOUSE_ON];
		this->atlasRec[Button_State::CLICKED] = atlasRec[Button_State::CLICKED];
	}
	CheckBox::CheckBox(UI_ElemType type, iPoint position, j1Rect col, Label* text) : InteractuableElem(type, position, col), text(text){}
	//--------------------------------

	//-------DESTRUCTORS--------------
	UI_Elem::~UI_Elem(){}
	NO_InteractuableElem::~NO_InteractuableElem() {}
	InteractuableElem::~InteractuableElem() {}
	Image:: ~Image() {}
	Label::~Label()
	{
		this->string.Clear();
	}
	Button::~Button() {}
	CheckBox::~CheckBox() {}
	//--------------------------------

	//-------UPDATES------------------
	bool UI_Elem::Update(float dt) { return true;}
	bool NO_InteractuableElem::Update(float dt) { return true; }
	bool InteractuableElem::Update(float dt) { return true; }
	//-------UPDATES------------------

	//-------USEFUL METHODS-----------
	void InteractuableElem::Do(float dt) {}
	//--------------------------------


//------------UI_MANAGER METHODS-----------------------------------------------------

UI_Manager::UI_Manager() : j1Module()
{
	name.create("UI_Manager");
}
UI_Manager::~UI_Manager() {  }

bool UI_Manager::Awake(pugi::xml_node& uimnode)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = uimnode.child("atlas").attribute("file").as_string("");

	return ret;
}
bool UI_Manager::Start()
{
	const char* string = atlas_file_name.GetString();
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

bool UI_Manager::CleanUp()
{
	//Freeing Atlas Image
	if (atlas)
	{
		App->tex->UnLoad(atlas);
		atlas = nullptr;
	}

	//Cleaning UI_Elem List
	p2List_item<UI_Elem*>* elem_ui = UI_ElemList.start;
	while (elem_ui)
	{
		RELEASE(elem_ui->data);
		elem_ui = elem_ui->next;
	}
	UI_ElemList.clear();


	return true;
}

//Update all elems
bool UI_Manager::PreUpdate()
{
	return true;
}

bool UI_Manager::Update(float dt)
{
	p2List_item<UI_Elem*>* elem = UI_ElemList.start;
	while (elem)
	{
		elem->data->Update(dt);
		elem = elem->next;
	}
	return true;
}

bool UI_Manager::PostUpdate()
{
	return true;
}

// const getter for atlas
const SDL_Texture* UI_Manager::GetAtlas() const
{
	return atlas;
}

UI_Elem* UI_Manager::CreateUIElem(UI_ElemType type, iPoint pos, const SDL_Rect& rec, UI_ButtonType btype, char* string, TTF_Font* font)
{
	UI_Elem* elem = nullptr;
	Label* label = nullptr;
	switch (type)
	{
		//-----LABEL----------------------------------------------------------
		case UI_ElemType::LABEL:
			if (string)
			{
				elem = new Label(type, pos, string, (font) ? font : App->fonts->default);
			}
			else
			{
				if (!string)
					LOG("Invalid string creating Label");
				if(!font)
					LOG("Invalid font creating Label, using default one");
			}
			break;

		//-----IMAGE-----------------------------------------------------------
		case UI_ElemType::IMAGE:
			elem = new Image(type, pos, rec);
			break;
	
		//-----CHECKBOX--------------------------------------------------------
		case UI_ElemType::CHECKBOX:
		{
			Label* label = nullptr;
			if (string)
			{
				label = new Label(LABEL, pos, string, App->fonts->getFontbyName("generic_font")); //This is just an example.
				if (label)
					UI_ElemList.add(label);
			}
			elem = new CheckBox(type, pos, j1Rect(pos, 23, 57), label);
		}
		break;

		//-----BUTTON----------------------------------------------------------
		case UI_ElemType::BUTTON: //NOTE: EACH BUTTON MUST SET HERE HIS COLLIDER SIZE AND HIS LABEL POS
		{
			switch (btype)
			{
				case UI_ButtonType::EXIT:
				{
					if (string)
					{
						label = new Label(LABEL, pos, string, App->fonts->getFontbyName("generic_font")); //This is just an example.
						if (label)
							UI_ElemList.add(label);
					}
					elem = new Button(type, pos, j1Rect(pos, 45, 53), btype, atlasRec, label); //This is just an example.
					label = nullptr;
				}
				break;
				case UI_ButtonType::CONFIG:
				{

					if (string)
					{
						label = new Label(LABEL, pos, string, App->fonts->getFontbyName("generic_font")); //This is just an example.
						if (label)
							UI_ElemList.add(label);
					}
					elem = new Button(type, pos, j1Rect(pos, 23, 57), btype, j1Rect(), label); //This is just an example.
				}
				break;
			}
			InteractuableElem* button = (InteractuableElem*) elem;
			button->listeners.add(App->audio);
		}
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

//------------UI_ELEM METHODS--------------------------------------------------------

bool InteractuableElem::CheckWithMouse(float dt)
{
	int mouse_x, mouse_y;
	App->input->GetMousePosition(mouse_x, mouse_y);


	//Check Mouse Col with Collider
	if (state != Events::MOUSE_ENTER && this->collider.Collides(j1Rect(mouse_x, mouse_y, 0, 0)))
	{
		this->state = Events::MOUSE_ENTER;
	}
	else if (state == Events::MOUSE_ENTER && !this->collider.Collides(j1Rect(mouse_x, mouse_y, 0, 0)))
	{
		this->state = Events::MOUSE_LEAVE;
	}

	//Check MouseButtons
	if (state == Events::MOUSE_ENTER)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			state = Events::LEFT_CLICKED;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			state = Events::LEFT_UNCLICKED;
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			state = Events::RIGHT_CLICKED;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
		{
			state = Events::RIGHT_UNCLICKED;
		}
	}

	if (state != Events::NO_EVENT)
		Do(dt);

	return true;
}

void Button::Do(float dt)
{
	if (state == Events::LEFT_CLICKED)
	{
		this->atlasRec = j1Rect();

		p2List_item<j1Module*>* listener = listeners.start;
		while (listener)
		{
			listener->data->UI_Do(this, state);
			listener = listener->next;
		}

	}
	else if (state == Events::LEFT_UNCLICKED)
	{
		this->atlasRec = j1Rect();
	}
}

bool Image::Update(float dt)
{
	App->render->Blit((SDL_Texture*)App->ui_manager->GetAtlas(), this->position.x, this->position.y, &this->rec);

	return true;
}

bool Label::Update(float dt)
{
	if (!App->render->Blit(App->fonts->Print(this->string.GetString(), { 255,0,0, 255 }, this->font), this->position.x, this->position.y))
		LOG("Error Printing Label: %s", this->string.GetString());
	
	return true;
}



void CheckBox::Do(float dt)
{
	
}