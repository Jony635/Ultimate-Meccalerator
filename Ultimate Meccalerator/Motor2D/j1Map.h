#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Enemies.h"


struct SDL_Texture;

enum PropertyType
{
	NO_TYPE = -1,
	LAYER_PROPERTY,
	TILESET_PROPERTY
};

struct Properties
{
	struct Property
	{
		p2SString name;
		int value;
		uint index = 0;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0, int index = -1) const;


	p2List<Property*>	list;
};

// TODO 1: Create a struct for the map layer
// ----------------------------------------------------
struct MapLayer
{
	p2SString name = "no_name";
	uint width = 0;
	uint height = 0;
	uint* data = nullptr;
	uint size_data = 0;
	bool visible = true;
	fPoint pos = { 0,0 };
	float speed = 0.0f;
	float alpha = 0;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// TODO 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};
	

// ----------------------------------------------------
struct TileSet
{
	// TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;
	Properties properties;
	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	// TODO 2: Add a list/array of layers to the map!
	p2List <MapLayer*> LayerList;
	int camera_starting_y = 0;
};
// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	iPoint World_to_Map(iPoint world_coordinates);

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	void UpdateLayers(char*);

	TileSet* GetTilesetFromTileId(int id) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);

	// TODO 3: Create a method that loads a single laye
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties, PropertyType type = LAYER_PROPERTY);

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

public:
	
	pugi::xml_node* LogicalTileset = nullptr;
	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__