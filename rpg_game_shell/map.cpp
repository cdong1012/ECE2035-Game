#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */

struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map;
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y)
{
    return X*(map.h)+Y; // X is width, Y is Height, multiply X by the height of the map
    // and add Y to get a unique value
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    //map.items is a hashTable and contains num_buckets
    return key%(NUMBUCKETS);
}

void maps_init()
{
    map.items = createHashTable(map_hash, NUMBUCKETS);
    map.w = WIDTH;
    map.h = HEIGHT;
}

Map* get_active_map()
{
    return &map;
}

Map* set_active_map(int m)
{
    active_map = m;
    return &map;
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'B'};
    for(int y = 0; y < map_height(); y++) {
        for (int x = 0; x < map_width(); x++) {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    Map *map = get_active_map();
    return map->w;

}

int map_height()
{
    Map *map = get_active_map();
    return map->h;
}

int map_area()
{
    Map *map = get_active_map();
    return (map->h * map->w);
}

MapItem* get_north(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x,y-1); //north is y-1
    return (MapItem*) getItem(map->items,key);
}

MapItem* get_south(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x,y+1); //south is y+1
    return (MapItem*) getItem(map->items,key);
}

MapItem* get_east(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x+1,y); //east is x+1
    return (MapItem*) getItem(map->items,key);
}

MapItem* get_west(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x-1,y);//west is x-1
    return (MapItem*) getItem(map->items,key);
}

MapItem* get_here(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x,y);
    return (MapItem*) getItem(map->items,key);
}


void map_erase(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x,y);
    MapItem* checkItem = (MapItem*) getItem(map->items,key);
    if(checkItem->type == BURGER || checkItem ->type == BABY || checkItem -> type == SODA) deleteItem(map->items,key); //check if item exists, then delete it
   
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++) {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_wall2(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++) {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall2;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_smiley(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SMILEY;
    w1->draw = draw_smiley;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_fryman(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = FRY;
    w1->draw = draw_fryman;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_burger(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BURGER;
    w1->draw = draw_burger;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}



void add_baby1(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BABY;
    w1->draw = draw_baby1;
    w1->walkable = false;
    w1->data = (char*)"baby1";
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_baby2(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BABY;
    w1->draw = draw_baby2;
    w1->walkable = false;
    w1->data = (char*)"baby2";
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_soda(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SODA;
    w1->draw = draw_soda;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
