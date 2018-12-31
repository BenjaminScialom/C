#ifndef MAP_H_
#define MAP_H_

#include <game.h>

enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000		--> a un enum scenery_type
	CELL_BOX=0x20,   		//  0010 0000		--> a un enum bonus_type
	CELL_DOOR=0x30,      	//  0011 0000		--> a un enum door_type
	CELL_KEY=0x40,       	//  0100 0000
	CELL_BONUS=0x50, 		// 	0101 0000		--> a un enum bonus_type
	CELL_MONSTER=0x60, 		// 	0110 0000
	CELL_BOMB=0x70, 	   	// 	0111 0000
	CELL_PLAYER=0x80, 		// 	1000 0000
	CELL_PRINCESS=0x90

};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC,
	BONUS_BOMB_NB_DEC,
	BONUS_BOMB_NB_INC,
	BONUS_MONSTER,
	BONUS_LIFE

};

enum scenery_type {
	SCENERY_TREE = 0,      // 0000 0000
	SCENERY_STONE = 4,     // 0000 0100
	//SCENERY_PRINCESS   // 0000 1000
};


enum door_type {
	DOOR_CLOSED, //0000 0000
	DOOR_OPENED  //0000 0001
};


enum level_type {
	LEVEL_0 = 0x00, //0000 000x
	LEVEL_1 = 0x02,	//0000 001x
	LEVEL_2 = 0x04, //0000 010x
	LEVEL_3 = 0x06, //0000 011x
	LEVEL_4 = 0x08, //0000 100x
	LEVEL_5 = 0x0a, //0000 101x
	LEVEL_6 = 0x0c, //0000 110x
	LEVEL_7 = 0x0e  //0000 111x
};

enum compose_type {
	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,
	//CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,

    CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_INC,
    CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
	CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_INC,
    CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_DEC,
    CELL_BOX_MONSTER  = CELL_BOX | BONUS_MONSTER,
	CELL_BOX_LIFE	  = CELL_BOX | BONUS_LIFE,

	CELL_BONUS_RANGEINC = CELL_BONUS | BONUS_BOMB_RANGE_INC,
	CELL_BONUS_RANGEDEC = CELL_BONUS | BONUS_BOMB_RANGE_DEC,
	CELL_BONUS_BOMBINC = CELL_BONUS | BONUS_BOMB_NB_INC,
	CELL_BONUS_BOMBDEC = CELL_BONUS | BONUS_BOMB_NB_DEC,
	CELL_BONUS_LIFE = CELL_BONUS | BONUS_LIFE,

	CELL_DOOR_0_CLOSED = CELL_DOOR | LEVEL_0 | DOOR_CLOSED,
	CELL_DOOR_0_OPENED = CELL_DOOR | LEVEL_0 | DOOR_OPENED,
	CELL_DOOR_1_CLOSED = CELL_DOOR | LEVEL_1 | DOOR_CLOSED,
	CELL_DOOR_1_OPENED = CELL_DOOR | LEVEL_1 | DOOR_OPENED,
	CELL_DOOR_2_CLOSED = CELL_DOOR | LEVEL_2 | DOOR_CLOSED,
	CELL_DOOR_2_OPENED = CELL_DOOR | LEVEL_2 | DOOR_OPENED,
	CELL_DOOR_3_CLOSED = CELL_DOOR | LEVEL_3 | DOOR_CLOSED,
	CELL_DOOR_3_OPENED = CELL_DOOR | LEVEL_3 | DOOR_OPENED,
	CELL_DOOR_4_CLOSED = CELL_DOOR | LEVEL_4 | DOOR_CLOSED,
	CELL_DOOR_4_OPENED = CELL_DOOR | LEVEL_4 | DOOR_OPENED,
	CELL_DOOR_5_CLOSED = CELL_DOOR | LEVEL_5 | DOOR_CLOSED,
	CELL_DOOR_5_OPENED = CELL_DOOR | LEVEL_5 | DOOR_OPENED,
	CELL_DOOR_6_CLOSED = CELL_DOOR | LEVEL_6 | DOOR_CLOSED,
	CELL_DOOR_6_OPENED = CELL_DOOR | LEVEL_6 | DOOR_OPENED,
	CELL_DOOR_7_CLOSED = CELL_DOOR | LEVEL_7 | DOOR_CLOSED,
	CELL_DOOR_7_OPENED = CELL_DOOR | LEVEL_7 | DOOR_OPENED,
};

struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

unsigned char map_get_grid(struct map* map, int x, int y);

void map_set_grid(struct map* map, int x, int y, int bidule);

// Return the state of the door (open/closed)
enum door_type map_get_door_type (struct map *map, int x, int y);

enum level_type map_get_level_type (struct map *map, int x, int y) ;

//Return the type of bonus
enum bonus_type map_get_bonus_type ( struct map *map, int x, int y );

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

enum cell_type map_get_cell(struct map* map, int x, int y);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default 12x12 static map
struct map* map_get_default();


void display_bonus(struct map* map, int x, int y, unsigned char type);

// Display the map on the screen
void map_display(struct map* map, struct game *game);

// Affiche la map au level indique par n
struct map* fill_map( int n );

#endif /* MAP_H_ */
