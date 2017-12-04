#ifndef __j1FONTS_H__
#define __j1FONTS_H__

#include "j1Module.h"
#include "SDL\include\SDL_pixels.h"

#include "SDL_TTF\include\SDL_ttf.h"

#define DEFAULT_FONT "Resources/fonts/open_sans/OpenSans-Regular.ttf"
#define DEFAULT_FONT_SIZE 12

#include <map>
#include <iostream>

struct SDL_Texture;

class j1Fonts : public j1Module
{
public:

	j1Fonts();

	// Destructor
	virtual ~j1Fonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, char* name , int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = {255, 255, 255, 255}, _TTF_Font* font = NULL);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

	TTF_Font* getFontbyName(char* name) const;

public:

	std::map<char*, _TTF_Font*> named_fonts;

	_TTF_Font*			default;
};


#endif // __j1FONTS_H__