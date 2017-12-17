#include "UI_Manager.h"
#include "j1FileSystem.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Player.h"

//------------DEFINES----------------------------------------------------------------
#define BUTTON_RECT_W						190
#define BUTTON_RECT_H						45
#define PIXELS_DOWN_FOR_CLICKED_ANIMATION	4


//------------UI_ELEM HERITAGE METHODS-----------------------------------------------

	//-------CONSTRUCTORS-------------
	UI_Elem::UI_Elem(UI_ElemType type, fPoint position) : type(type), position(position) {}
	NO_InteractuableElem::NO_InteractuableElem(UI_ElemType type, fPoint position) : UI_Elem(type, position) {}
	InteractuableElem::InteractuableElem(UI_ElemType type, fPoint position, j1Rect col) : UI_Elem(type, position), collider(col){}

	Label::Label(UI_ElemType type, fPoint position, p2SString string, TTF_Font* font) : NO_InteractuableElem(type, position),font(font)
	{
		this->string.create(string.GetString());
	}

	Image::Image(UI_ElemType type, fPoint position, j1Rect rec) : NO_InteractuableElem(type, position), rec(rec) {}
	Button::Button(UI_ElemType type, fPoint position, const j1Rect& col, UI_ButtonType btype, j1Rect* atlasRec, Label* text) : InteractuableElem(type, position, col), btype(btype), text(text)
	{
		this->atlasRec[Button_State::DEFAULT] = atlasRec[Button_State::DEFAULT];
		this->atlasRec[Button_State::MOUSE_ON] = atlasRec[Button_State::MOUSE_ON];
		this->atlasRec[Button_State::CLICKED] = atlasRec[Button_State::CLICKED];
		this->state = Events::MOUSE_LEAVE;
	}
	CheckBox::CheckBox(UI_ElemType type, fPoint position, j1Rect col, Label* text) : InteractuableElem(type, position, col), text(text){}
	SlideBar::SlideBar(UI_ElemType type, fPoint position, const j1Rect& col, j1Rect atlasRec, Label* title) : InteractuableElem(type, position, col), title(title), atlasRec(atlasRec) 
	{
		this->gearPos.y = this->position.y-2;
		this->gearPos.x = (this->collider.rec.x + this->collider.rec.w) / 2 + 100;
		
		p2SString value ("%.f", this->percent_value);
		this->percent = new Label(LABEL, { (float)collider.rec.x + collider.rec.w + 100, (float)collider.rec.y + 20 }, value, App->fonts->getFontbyName("zorque"));
		value.Clear();
	}
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
	Button::~Button() 
	{
		if (this->text)
		{
			RELEASE(this->text);
		}
	}
	CheckBox::~CheckBox() {}
	SlideBar::~SlideBar()
	{
		if (this->title)
		{
			RELEASE(this->title);
		}
		if (this->percent)
		{
			RELEASE(this->percent);
		}
	}

	//--------------------------------

	//-------UPDATES------------------
	bool UI_Elem::Update(float dt) { return true;}
	bool NO_InteractuableElem::Update(float dt) { return true; }
	bool InteractuableElem::Update(float dt) { return true; }
	//--------------------------------

	//-------USEFUL METHODS-----------
	bool InteractuableElem::Do(float dt) { return true; }
	void UI_Elem::Move(fPoint distance)
	{
		this->position += distance;
	}
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
	atlas = App->tex->Load(string);

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

	//Cleaning UI_MobileElem List
	p2List_item<Mobile_Elem*>* elem_ui2 = UI_MobileElemList.start;
	while (elem_ui2)
	{
		RELEASE(elem_ui2->data);
		elem_ui2 = elem_ui2->next;
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
	bool ret = true;
	p2List_item<UI_Elem*>* elem = UI_ElemList.start;
	while (elem!=nullptr && elem->data != nullptr)
	{
		if (elem->data->Update(dt) == false)
		{
			return false;
		}
		elem = elem->next;
	}
	//------------------------------Updating UI Scene-------
	if(App->actual_lvl == Levels::FIRST_LEVEL || App->actual_lvl == Levels::SECOND_LEVEL)
		App->scene->Update_UI(App->scene->bar_colour, App->player->lifes, App->player->tp_counter, 0);

	MoveElems(dt); //Maybe bugged for conflicts

	return true;
}

void UI_Manager::MoveElems(float dt)
{
	p2List_item<Mobile_Elem*>* mobile_elem = UI_MobileElemList.start;
	while (mobile_elem)
	{
		fPoint* distance = &mobile_elem->data->distance;	//Distance left to move
		UI_Elem* elem = mobile_elem->data->elem;		
		float* time = &mobile_elem->data->time;				//Time left to move

		fPoint dpos((distance->x * dt) / *time, (distance->y * dt) / *time);
		elem->Move(dpos);

		*distance -= dpos;	//Update de amount of pixels left to move
		*time -= dt;		//Update de amount of time left to move

		mobile_elem = mobile_elem->next;
		
		if (*time <= 0)
			UI_MobileElemList.del(mobile_elem->prev); //Remove Elems from the list when finished the movement
	}
}

bool UI_Manager::PostUpdate()
{	
	return true;
}

const SDL_Texture* UI_Manager::GetAtlas() const
{
	return atlas;
}

UI_Elem* UI_Manager::CreateUIElem(UI_ElemType type, fPoint pos, j1Rect* atlasRec, const j1Rect& col, UI_ButtonType btype, char* string, TTF_Font* font)
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
			elem = new Image(type, pos, atlasRec[0]);
			break;
	
		//-----CHECKBOX--------------------------------------------------------
		case UI_ElemType::CHECKBOX: //Non Updated Constructor, because of this won't be used for now
		{
			Label* label = nullptr;
			if (string)
			{
				label = new Label(LABEL, pos, string, App->fonts->getFontbyName("generic_font"));
				if (label)
					UI_ElemList.add(label);
			}
			elem = new CheckBox(type, pos, j1Rect(pos, 23, 57), label);
		}
		break;

		//-----BUTTON----------------------------------------------------------
		case UI_ElemType::BUTTON:
		{
			if (string)
			{	
				int string_w, string_h = 0;
				TTF_SizeText(font, string, &string_w, &string_h);
				fPoint label_position = fPoint(pos.x + (BUTTON_RECT_W / 2) - string_w / 2, pos.y + (BUTTON_RECT_H / 2) - string_h / 2);
				label = new Label(LABEL, label_position, string, font);
			}
			elem = new Button(type, pos, col, btype, atlasRec, label); 

			InteractuableElem* button = (InteractuableElem*)elem;
			button->listeners.add(App->audio);
			button->listeners.add(App->scene);
		}
		break;
		case UI_ElemType::SLIDEBAR:
		{
			if (string)
			{
				fPoint label_position = fPoint((col.rec.x + col.rec.w) / 2 + 40, col.rec.y - 50);
				label = new Label(LABEL, label_position, string, font);
			}
			elem = new SlideBar(type, pos, col, *atlasRec, label);

			InteractuableElem* slidebar = (InteractuableElem*)elem;
			slidebar->listeners.add(App->audio);
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


void UI_Manager::Move(const fPoint& distance, float secs, const UI_Elem* elem) 
{
	p2List_item<UI_Elem*>* elem_it = UI_ElemList.start;
	while (elem_it)
	{
		if (elem)
		{
			if (elem == elem_it->data)
			{
				Mobile_Elem* mobile_elem = new Mobile_Elem(elem_it->data, secs, distance);
				UI_MobileElemList.add(mobile_elem);

				if (elem_it->data->type == UI_ElemType::BUTTON)
				{
					Button* button = (Button*)elem_it->data;
					if (button->text)
					{
						Mobile_Elem* mobile_elem2 = new Mobile_Elem(button->text, secs, distance);
						UI_MobileElemList.add(mobile_elem2);
					}
				}
				break;
			}
			else
			{
				elem_it = elem_it->next;
				continue;
			}
		}

		Mobile_Elem* mobile_elem = new Mobile_Elem(elem_it->data, secs, distance);
		UI_MobileElemList.add(mobile_elem);

		if (elem_it->data->type == UI_ElemType::BUTTON)
		{
			Button* button = (Button*)elem_it->data;
			if (button->text)
			{
				Mobile_Elem* mobile_elem2 = new Mobile_Elem(button->text, secs, distance);
				UI_MobileElemList.add(mobile_elem2);
			}
		}
		

		elem_it = elem_it->next;
	}
	
}		

void UI_Manager::Move_to(const iPoint& destination, float secs, const UI_Elem* elem) 
{


}

UI_Elem* UI_Manager::SearchElem(UI_ElemType elemtype, UI_ButtonType btype, j1Rect* rect,char* string) const
{
	p2List_item<UI_Elem*>* elem_it = UI_ElemList.start;
	while (elem_it)
	{
		if (elem_it->data->type != elemtype)
		{
			elem_it = elem_it->next;
			continue;
		}
		switch (elemtype)
		{
			case UI_ElemType::BUTTON:
			{
				Button* button = (Button*)elem_it;

				if (button->btype != btype)
				{
					elem_it = elem_it->next;
					continue;
				}
				return elem_it->data;
			}
				break;

			case UI_ElemType::IMAGE:
			{
				Image * image = (Image*)elem_it;

				if (image->rec != *rect)
				{
					elem_it = elem_it->next;
					continue;
				}
				return elem_it->data;
			}
				break;

			case UI_ElemType::LABEL:
			{
				Label* label = (Label*)elem_it->data;

				if (label->string != string)
				{
					elem_it = elem_it->next;
					continue;
				}

				return elem_it->data;
			}
				break;
		}
		elem_it = elem_it->next;
	}

	return nullptr;
}


//------------UI_ELEM METHODS--------------------------------------------------------

bool Button::CheckWithMouse(float dt)
{
	int mouse_x, mouse_y;
	App->input->GetMousePosition(mouse_x, mouse_y);

	//Reset label pos if button was pressed
	if (this->state == Events::LEFT_CLICKED || this->state == Events::LEFT_CONTINUES)
	{
		Button* button = (Button*)this;
		button->text->position.y -= PIXELS_DOWN_FOR_CLICKED_ANIMATION;
	}

	//Check Mouse Col with Collider
	if ((state == Events::MOUSE_ENTER || state == Events::LEFT_UNCLICKED) && this->collider.Collides(j1Rect(mouse_x, mouse_y, 0, 0)))
	{
		this->state = Events::MOUSE_CONTINUES;
	}
	
	else if (!this->collider.Collides(j1Rect(mouse_x, mouse_y, 0, 0)))
	{
		this->state = Events::MOUSE_LEAVE;
	}

	else if ((state==Events::MOUSE_LEAVE) && this->collider.Collides(j1Rect(mouse_x, mouse_y, 0, 0)))
	{
		this->state = Events::MOUSE_ENTER;
	}
	

	//Check MouseButtons
	if (state == Events::MOUSE_ENTER || state == Events::MOUSE_CONTINUES || state == Events::LEFT_CLICKED || state == Events::LEFT_CONTINUES)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			state = Events::LEFT_CLICKED;
			Button* button = (Button*)this;
			button->text->position.y += PIXELS_DOWN_FOR_CLICKED_ANIMATION;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			state = Events::LEFT_CONTINUES;
			Button* button = (Button*)this;
			button->text->position.y += PIXELS_DOWN_FOR_CLICKED_ANIMATION;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			state = Events::LEFT_UNCLICKED;
		}
		/*if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			state = Events::RIGHT_CLICKED;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
		{
			state = Events::RIGHT_UNCLICKED;
		}*/
	}

	if (state != Events::NO_EVENT)
		if (Do(dt) == false)
			return false;

	return true;
}

bool Button::Do(float dt)
{
	bool ret = true;

	switch (state)
	{
	case MOUSE_ENTER:
	case MOUSE_CONTINUES:
		this->BlitRec = atlasRec[Button_State::MOUSE_ON].rec;
		break;
	case MOUSE_LEAVE:
	case NO_EVENT:
	case LEFT_UNCLICKED:
		this->BlitRec = atlasRec[Button_State::DEFAULT].rec;
		break;
	case LEFT_CLICKED:
	case LEFT_CONTINUES:
		this->BlitRec = atlasRec[Button_State::CLICKED].rec;
		break;
	case RIGHT_CLICKED:
		break;
	case RIGHT_UNCLICKED:
		break;
	default:
		break;
	}

	p2List_item<j1Module*>* listener = listeners.start;
	while (listener && (state != Events::MOUSE_LEAVE))
	{
		if (listener->data->UI_Do(this, &state) == false)
		{
			ret = false;
		}
		listener = listener->next;
	}

	return ret;
}

bool Image::Update(float dt)
{
	world_position.y = -App->render->fcamera.y + this->position.y;
	world_position.x = -App->render->fcamera.x + this->position.x;
	App->render->Blit((SDL_Texture*)App->ui_manager->GetAtlas(), world_position.x, world_position.y, &this->rec.rec);

	return true;
}

bool Label::Update(float dt)
{

	world_position.y = -App->render->fcamera.y + this->position.y;
	world_position.x = -App->render->fcamera.x + this->position.x;
  SDL_Texture* string_texturized;
	if (this->font == App->fonts->getFontbyName("zorque") || this->font == App->fonts->getFontbyName("zorque_mini"))
		string_texturized = App->fonts->Print(this->string.GetString(), { 255, 255, 255, 255 }, this->font);
	else
		string_texturized = App->fonts->Print(this->string.GetString(), {102, 0, 0, 255}, this->font);
	
	if (!App->render->Blit(string_texturized, world_position.x, world_position.y))

	LOG("Error Printing Label: %s", this->string.GetString());
	SDL_DestroyTexture(string_texturized);

	return true;
}

p2SString* Label::getString()
{
	return &string;
}

bool Button::Update(float dt)
{
	if (CheckWithMouse(dt) == false)
	{
		return false;
	}

	App->render->Blit((SDL_Texture*)App->ui_manager->GetAtlas(), this->position.x, this->position.y, &this->BlitRec);

	if (this->text)
		this->text->Update(dt);

	this->collider.rec.x = this->position.x;
	this->collider.rec.y = this->position.y;

	return true;
}

bool SlideBar::Update(float dt)
{
	UpdateValue(dt);

	//Blit the Bar
	App->render->Blit((SDL_Texture*)App->ui_manager->GetAtlas(), this->position.x, this->position.y, &this->atlasRec.rec);

	App->render->Blit((SDL_Texture*)App->ui_manager->GetAtlas(), this->gearPos.x, this->gearPos.y, &(SDL_Rect)j1Rect(639, 460, 66, 73).rec); //Set gear atlas coordinates here

	this->percent->Update(dt);
	this->title->Update(dt);

	return true;
}

void SlideBar::UpdateValue(float dt)
{
	int mouse_x, mouse_y;
	App->input->GetMousePosition(mouse_x, mouse_y);
	if (this->collider.Collides(j1Rect(mouse_x, mouse_y, 0, 0)))
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			this->percent_value = (mouse_x-this->collider.rec.x) * 100 / (this->collider.rec.w);
			this->gearPos.x = this->collider.rec.x + (this->percent_value*this->collider.rec.w / 100);
			this->percent->getString()->create("%.f", percent_value);
			Do(dt);
		}
	}
}

bool SlideBar::Do(float dt)
{
	p2List_item<j1Module*>* listener = this->listeners.start;
	while (listener)
	{
		Events event = NO_EVENT;
		listener->data->UI_Do(this, &event);
		listener = listener->next;
	}
	return true;
}

bool CheckBox::Do(float dt)
{
	return true;
}

