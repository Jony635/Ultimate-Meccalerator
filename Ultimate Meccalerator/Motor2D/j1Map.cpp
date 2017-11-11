#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>
#include "j1Input.h"
#include "j1Player.h"
#include "j1FileSystem.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

int Properties::Get(const char* value, int default_value, int index) const
{
	if (index == -1)
	{
		p2List_item<Property*>* item = list.start;

		while (item)
		{
			if (item->data->name == value)
				return item->data->value;
			item = item->next;
		}

		return default_value;
	}
	else
	{
		p2List_item<Property*>* item = list.start;

		while (item)
		{
			if (item->data->name == value && item->data->index == index)
				return item->data->value;
			item = item->next;
		}

		return default_value;
	}
	
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 5: Prepare the loop to draw all tilesets + Blit
	
	for (p2List_item<TileSet*>* TileSet = data.tilesets.start; TileSet != nullptr; TileSet = TileSet->next)
	{
		for (p2List_item<MapLayer*>* layer_to_draw = data.LayerList.start; layer_to_draw != nullptr; layer_to_draw = layer_to_draw->next)
		{
			if (layer_to_draw->data->visible == false)
				continue;

			int x = layer_to_draw->data->pos.x, y = 0;
			for (int num_tile = 0; num_tile < layer_to_draw->data->size_data; ++num_tile)
			{
				if (strcmp(TileSet->data->name.GetString(), "Logical_tileset") == 0)
				{
					SDL_SetTextureAlphaMod(TileSet->data->texture, layer_to_draw->data->alpha*255);
				}
				App->render->Blit(TileSet->data->texture, x, y, &TileSet->data->GetTileRect(*(layer_to_draw->data->data+num_tile)));
				x += TileSet->data->tile_width;

				if (x % ((layer_to_draw->data->width * TileSet->data->tile_width) + (int)layer_to_draw->data->pos.x) == 0)
				{
					x = layer_to_draw->data->pos.x;
					y += TileSet->data->tile_height;
				}
			}
		}
	}
		// TODO 9: Complete the draw function

}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint j1Map::World_to_Map(iPoint world_coordinates)
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = world_coordinates.x / data.tile_width;
		ret.y = world_coordinates.y / data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int((world_coordinates.x / half_width + world_coordinates.y / half_height) / 2) - 1;
		ret.y = int((world_coordinates.y / half_height - (world_coordinates.x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = world_coordinates.x; ret.y = world_coordinates.y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// TODO 2: clean up all layer data
	
	if (data.LayerList.count() != 0)
	{
		p2List_item<MapLayer*>* item_layer;
		item_layer = data.LayerList.start;
		while (item_layer != NULL)
		{
			RELEASE(item_layer->data);
			item_layer = item_layer->next;
		}
		data.LayerList.clear();
	}
	if(LogicalTileset!=nullptr)
	{
		delete LogicalTileset;
		LogicalTileset = nullptr;
	}
	
	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	char* buf;
	int size = App->fs->Load(tmp.GetString(), &buf);
	pugi::xml_parse_result result = map_file.load_buffer(buf, size);

	RELEASE(buf);

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// TODO 4: Iterate all layers and load each of them
	// Load layer info ----------------------------------------------
	for (pugi::xml_node Layer_It = map_file.child("map").child("layer"); Layer_It != nullptr; Layer_It = Layer_It.next_sibling())
	{
		MapLayer* layer=new MapLayer;
		LoadLayer(Layer_It, layer);
		this->data.LayerList.add(layer);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// TODO 4: Add info here about your loaded layers
		// Adapt this vcode with your own variables
		
		p2List_item<MapLayer*>* item_layer = data.LayerList.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		data.camera_starting_y = map.child("properties").find_child_by_attribute("name", "Camera Starting Tile Y").attribute("value").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	if (set->name == "Logical_tileset")
	{
		this->LogicalTileset = new pugi::xml_node;
		*this->LogicalTileset = tileset_node;
	}
		
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	LoadProperties(tileset_node, set->properties, TILESET_PROPERTY);
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

// TODO 3: Create the definition for a function that loads a single layer
bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	layer->name.create(node.attribute("name").value());
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	layer->visible = node.attribute("visible").as_bool(true);
	layer->alpha = node.attribute("opacity").as_float(1);
	LoadProperties(node, layer->properties);
	layer->speed = node.child("properties").find_child_by_attribute("name", "speed").attribute("value").as_float()*App->map->data.tile_width / 60;

	for (pugi::xml_node TileToCopy = node.child("data").child("tile"); TileToCopy != nullptr; TileToCopy = TileToCopy.next_sibling())
	{
		layer->size_data++;
	}

	layer->data = new uint[layer->size_data];
	memset(layer->data, 0, layer->width*layer->height);

	int i = 0;
	for (pugi::xml_node TileToCopy = node.child("data").child("tile"); TileToCopy != nullptr; TileToCopy = TileToCopy.next_sibling(), ++i)
	{
		*(layer->data + i) = TileToCopy.attribute("gid").as_uint();
	}
	return true;
}

bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties, PropertyType type)
{
	bool ret = false;
	if (type == LAYER_PROPERTY)
	{
		pugi::xml_node data = node.child("properties");

		if (data != NULL)
		{
			pugi::xml_node prop;

			for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
			{
				Properties::Property* p = new Properties::Property();

				p->name = prop.attribute("name").as_string();
				p->value = prop.attribute("value").as_int();

				properties.list.add(p);
			}
		}
	}
	else if (type == TILESET_PROPERTY)
	{
		pugi::xml_node data = node.child("tile");
		uint index = 0;
		if (data != NULL)
		{
			pugi::xml_node prop;
			for (data; data != nullptr; data = data.next_sibling())
			{
				index = data.attribute("id").as_uint();
				for (prop = data.child("properties").child("property"); prop; prop = prop.next_sibling("property"))
				{
					Properties::Property* p = new Properties::Property();

					p->name = prop.attribute("name").as_string();
					p->value = prop.attribute("value").as_int(0);
					p->index = index;

					properties.list.add(p);
				}
			}
		}
	}
	
	return ret;
}

void j1Map::UpdateLayers(char* direction)
{
	if (direction == "right")
	{
		for (p2List_item<MapLayer*>* layertoupdate = data.LayerList.start; layertoupdate != nullptr; layertoupdate = layertoupdate->next)
		{

			layertoupdate->data->pos.x += layertoupdate->data->speed;
		}
	}
	else
	{
		for (p2List_item<MapLayer*>* layertoupdate = data.LayerList.start; layertoupdate != nullptr; layertoupdate = layertoupdate->next)
		{

			layertoupdate->data->pos.x -= layertoupdate->data->speed;
		}
	}
	
}

bool j1Map::Load(pugi::xml_node& mapnode)
{
	pugi::xml_node layernode = mapnode.child("layers").child("layer");
	for (p2List_item<MapLayer*>* layer = data.LayerList.start; layer!=nullptr; layernode=layernode.next_sibling(), layer=layer->next)
	{
		layer->data->pos.x = layernode.attribute("x").as_float();
	}
	App->actual_lvl=(Levels)mapnode.child("ActualLvl").attribute("name").as_int();
	App->RestartScene();
	return true;
}

bool j1Map::Save(pugi::xml_node& mapnode) const
{
	pugi::xml_node layers = mapnode.append_child("layers");
	
	for (p2List_item<MapLayer*>* layer = data.LayerList.start; layer!=nullptr; layer=layer->next)
	{
		layers.append_child("layer").append_attribute("x") = layer->data->pos.x;
	}
	mapnode.append_child("ActualLvl").append_attribute("name") = App->actual_lvl;
	return true;
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	p2List_item<MapLayer*>* item;
	item = data.LayerList.start;

	for (item = data.LayerList.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->name != "pathfinding")
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = tileset->properties.Get("walkability", 0, tile_id - tileset->firstgid);
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
					map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}
		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}