#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <bomb.h>
#include <list_bomb.h>
#include <game.h>
#include <monster.h>

struct map {
	int width;
	int height;
	unsigned char* grid;

};

#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);
	if ( x < map_get_width(map)  &&  x >= 0  &&  y < map_get_height(map)  &&  y >= 0 )
		return 1;
	else
		return 0;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

unsigned char map_get_grid(struct map* map, int x, int y) {
	assert(map);
	return map->grid[CELL(x,y)];
}

void map_set_grid(struct map* map, int x, int y, int bidule)
{
	assert (map);
	map->grid[CELL(x,y)] = bidule;
}

enum door_type map_get_door_type (struct map *map, int x, int y) { //New function pr etat des door :
	assert ( map && map_is_inside(map, x, y) );
	return map->grid[CELL(x,y)] & 0x01 ;
}

enum level_type map_get_level_type (struct map *map, int x, int y) {
	assert ( map && map_is_inside(map, x, y) );
	return map->grid[CELL(x,y)] & 0x0e ;
}

enum bonus_type map_get_bonus_type ( struct map *map, int x, int y ) { //New function pr type de bonus :
	assert ( map && map_is_inside(map, x, y) );
	return map->grid[CELL(x,y)] & 0x0f ;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert( map && map_is_inside(map, x, y) );
	return map->grid[CELL(x,y)] & 0xf0;
}

enum cell_type map_get_cell(struct map* map, int x, int y)
{
	assert( map && map_is_inside(map, x, y) );
	return map->grid[CELL(x,y)];
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert( map && map_is_inside(map, x, y) );
	map->grid[CELL(x,y)] = type;
}



void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;

	case BONUS_LIFE:
		window_display_image(sprite_get_bonus(BONUS_LIFE), x, y);
		break;

	default:
		break;
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0c) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	}
}

void display_door(struct map* map, int x, int  y, unsigned char type) // New fct -> Affichage des etats portes:
{
	switch (type & 0x01) { // sub-types are encoded with the bit b0
	case DOOR_CLOSED:
		window_display_image(sprite_get_door_closed(), x, y);
		break;

	case DOOR_OPENED:
		window_display_image(sprite_get_door_opened(), x, y);
		break;
	}
}


void map_display( struct map* map, struct game *game )
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];

	    switch (type & 0xf0) {
	    case CELL_PRINCESS:
	    	window_display_image( sprite_get_princess(), x, y );
	    	break;
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
	    case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);
	      break;
	    case CELL_BONUS:
	      display_bonus(map, x, y, type);
	      break;
	    case CELL_KEY:
	      window_display_image( sprite_get_key(), x, y );
	      break;
	    case CELL_DOOR:
	    	// gere l'etat de la porte avec le bit b0.
		   	display_door( map, x, y, type );
	      break;

	    case CELL_MONSTER:
	    	/*if ( game_get_monster(game) == NULL )
	    	{
	    		game_set_monster( game, monster_init(i, j) ); //remplie game->monster du monstre en (i,j)
	    		game_set_llist_monster( game, llist_put_monster(game_get_llist_monster(game), game_get_monster(game)) );
	    		//rajoute en fin de liste de game->llist_monster
	    	}
	    	else
	    	{
	    		monster_display( game, map, i, j );
	    	}
	    	*/
	    	monster_display( game, map, i, j );
	    	break;

	    case CELL_BOMB:
	    	//printf("I have found a  CELL_BOMB place \n");
	    	bomb_display( game, i, j );
	    	//printf("I display the bomb \n\n");
	    }
	  }
	}
}

struct map* map_get_default(void)
{
	struct map* map = map_new(MAP_WIDTH, MAP_HEIGHT);

	unsigned char themap[MAP_WIDTH * MAP_HEIGHT] = {
	  CELL_PLAYER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	  CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX_MONSTER, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX_RANGEINC, CELL_BONUS_RANGEDEC, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_BONUS_RANGEINC, CELL_EMPTY, CELL_STONE, CELL_BOX_RANGEDEC, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_MONSTER, CELL_EMPTY, CELL_BONUS_BOMBINC, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_MONSTER, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_BOX, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_MONSTER, CELL_PRINCESS, CELL_MONSTER, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_BOX, CELL_EMPTY,
	  CELL_BONUS_BOMBDEC,  CELL_BONUS_LIFE, CELL_DOOR, CELL_EMPTY, CELL_KEY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		};

	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
		map->grid[i] = themap[i];

	return map;
}


struct map* fill_map( int n ) { // n correspond au niveau

    char l_txt[8]; // Il y a 8 niveaux possible ( l pur level )
    char loc_map[30];
    strcpy( loc_map, "data/map_" ); // les maps sont situees dans le dossier data
    sprintf( l_txt, "%d", n );
    strcat ( loc_map, l_txt ); //concatene
    char *type = ".txt";	//fichier en .txt
    strcat ( loc_map, type );
    FILE * txt_map = NULL;
    txt_map = fopen ( loc_map, "r" ); // ouverture du fichier .txt en mode lecture
    int width, height;
    char temp[1];
    fscanf( txt_map, "%d", &width );
    fscanf( txt_map,"%1s", temp); // le deux-point ":"
    fscanf( txt_map, "%d", &height );
    
    //Creation de la new map.
    struct map * map = map_new( width, height );
    
    int i,j;
    for ( j=0 ; j < height ; j++ )   // on recopie chaque cellule du fichier txt pour que la map corresponde...
        {
            for ( i=0 ; i < width ; i++ )
            {
                int cell_val;
                fscanf( txt_map, "%d", &cell_val );
                map->grid[CELL(i,j)] = cell_val;
            }
        }
    fclose( txt_map );  // fermeture du fichier txt
    return map;  // on renvoie la map
}

