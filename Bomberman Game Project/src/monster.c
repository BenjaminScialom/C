#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <monster.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <monster.h>
#include <list_monster.h>

struct monster {
	int x, y;
	int life; // 1=monster en vie ; 0=monster dead
	enum direction current_direction;
	int time_mvt;
	int move;
	int next_mvt;
	//int id;

	//struct monster *next_m;
};

struct monster *monster_init (int x, int y, int inc) {
	struct monster* monster = malloc(sizeof(*monster));
	if (!monster)
		error("Memory error");

	monster->x = x;
	monster->y = y;
	monster->life = 1;
	monster->current_direction = SOUTH;
	monster->next_mvt = 1000 - (100*inc);
	monster->time_mvt = SDL_GetTicks() + monster->next_mvt;
	//monster->next_m = NULL;
	//monster->id = id;

	return monster;
}

void monster_free( struct monster* monster ) {
	assert(monster);
	free(monster);
}

int monster_get_x( struct monster* monster ) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y( struct monster* monster ) {
	assert(monster != NULL);
	return monster->y;
}

void monster_set_life ( struct monster* monster, int killed ) {
	assert(monster);
	monster->life = killed;
}

int monster_get_life ( struct monster* monster ) {
	assert(monster);
	return monster->life;
}


void monster_random (struct monster *monster) {

	srand(time(NULL));
	int nb_aleatoire = SDL_GetTicks() + rand();
	monster->move = nb_aleatoire%4;
	//return monster->move;
}

int monster_fear_door( struct game *game, struct map *map, int x, int y ) {
	if (!map_is_inside(map, x, y))
	{
		printf("not in the map\n");
		return 0;
	}

	if (map_is_inside(map, x-1, y) && map_get_cell_type(map, x-1, y) == CELL_DOOR)
	{ //WEST
		return 0;
	}
	else if (map_is_inside(map, x+1, y) && map_get_cell_type(map, x+1, y) == CELL_DOOR)
	{  //EAST
		return 0;
	}
	else if (map_is_inside(map, x, y-1) && map_get_cell_type(map, x, y-1) == CELL_DOOR)
	{ //NORTH
		return 0;
	}
	else if (map_is_inside(map, x, y+1) && map_get_cell_type(map, x, y+1) == CELL_DOOR)
	{ //SOUTH
		return 0;
	}
	else if (map_is_inside(map, x-1, y) && map_get_cell_type(map, x-1, y) == CELL_PRINCESS)
	{ //WEST
		return 2;
	}
	else if (map_is_inside(map, x+1, y) && map_get_cell_type(map, x+1, y) == CELL_PRINCESS)
	{  //EAST
		return 2;
	}
	else if (map_is_inside(map, x, y-1) && map_get_cell_type(map, x, y-1) == CELL_PRINCESS)
	{ //NORTH
		return 2;
	}
	else if (map_is_inside(map, x, y+1) && map_get_cell_type(map, x, y+1) == CELL_PRINCESS)
	{ //SOUTH
		return 2;
	}

	else
		return 1;


}

int monster_move_aux( struct game *game, struct map *map, int x, int y ) {
	if (!map_is_inside(map, x, y))
	{
		printf("not in the map\n");
		return 0;
	}
	struct player *player = game_get_player(game);

	switch (map_get_cell_type(map, x, y))
	{

	case CELL_PRINCESS:
		return 2;
		break;

	case CELL_SCENERY:
		return 0;
		break;

	case CELL_BOX:
		return 0;
		break;

	case CELL_DOOR:
		return 0;
		break;

	case CELL_BONUS:
		return 0;
		break;

	case CELL_KEY:
		return 0;
		break;

	case CELL_PLAYER:
		if ( player_get_wounds(player) == 0 ) {
			player_inc_wounds( player );
		}
		return 0;
		break;

	case CELL_MONSTER:
		return 0;
		break;

	case CELL_BOMB:
		return 0;
		break;

	case CELL_EMPTY:
		//return 1;
		return monster_fear_door(game, map, x, y);
		break;

	default:
		break;
	}
	return 1;
}

void monster_move ( struct game *game, struct map *map, struct monster *monster ) {
	//int s_x = select_m->x;
	//int s_y = select_m->y;
	//struct monster *monster = select_monster( game_get_llist_monster(game), s_x, s_y );
	int x = monster->x;
	int y = monster->y;
	int deplaced = 0;



	switch ( monster->move ) {
	case 0: //NORTH
		if ( monster_move_aux(game, map, monster->x, monster->y - 1) == 1 )
		{
			monster->y--;
			deplaced = 1;
		}
		else if( monster_move_aux(game, map, monster->x, monster->y - 1) == 2)
		{
			if(map_get_cell_type(map, monster->x, monster->y - 1) == CELL_EMPTY)
			{
				monster->y--;
			}
			monster->life = 2;
			deplaced = 1;
		}
		break;
	case 1: //SOUTH
		if ( monster_move_aux(game, map, monster->x, monster->y + 1) == 1 )
		{
				monster->y++;
				deplaced = 1;
		}
		else if ( monster_move_aux(game, map, monster->x, monster->y + 1) == 2 )
		{
			if(map_get_cell_type(map, monster->x, monster->y + 1) == CELL_EMPTY)
			{
				monster->y++;
			}
			monster->life = 2;
			deplaced = 1;
		}
		break;
	case 2: //WEST
		if ( monster_move_aux(game, map, monster->x - 1, monster->y) == 1 )
		{
				monster->x--;
				deplaced = 1;
		}
		else if ( monster_move_aux(game, map, monster->x - 1, monster->y) == 2 )
		{
			if(map_get_cell_type(map, monster->x - 1, monster->y) == CELL_EMPTY)
			{
				monster->x--;
			}
			monster->life = 2;
			deplaced = 1;
		}
		break;
	case 3: //EAST
		if ( monster_move_aux(game, map, monster->x + 1, monster->y) == 1 )
		{
				monster->x++;
				deplaced = 1;
		}
		else if ( monster_move_aux(game, map, monster->x + 1, monster->y) == 2 )
		{
			if(map_get_cell_type(map, monster->x + 1, monster->y) == CELL_EMPTY)
			{
				monster->x++;
			}
			monster->life = 2;
			deplaced = 1;
		}
		break;
	}
	monster->current_direction = monster->move;
	monster->time_mvt = SDL_GetTicks() + monster->next_mvt;

	if ( deplaced ) {
		map_set_cell_type( map, x, y, CELL_EMPTY );
		map_set_cell_type( map, monster->x, monster->y, CELL_MONSTER );
	}


}

void monster_display ( struct game *game, struct map *map, int x, int y ) {
	//en fct du chiffre sorti (intervalle de 0 a 3) -> donne dans monster_move qui fera le mvmt monster selon le chiffre
	assert(game);

	struct llist_monster *llist_monster = game_get_llist_monster(game);
	//on cree un pointeur qui reprend notre llist_monster
	//printf("M1 \n");
	struct monster *monster = select_monster(llist_monster, x, y);
	//on selectionne par pointeur le monstre de notre liste avec coordonnees (x,y)
	//printf("M1.1 \n");

	if (monster->life == 0)
	{
		printf("M_DEAD \n");
		monster_free(monster);
	}
	else if( monster->life == 2)
	{

	}
	else
	{
		if ( SDL_GetTicks() > (monster->time_mvt) )
		{
			printf("M_LIVED \n");
			monster_random(monster);
			monster_move( game, map, monster );
		}
	}



	//struct monster *monster = game_get_monster(game);


	window_display_image( sprite_get_monster(monster->current_direction),
			monster_get_x(monster) * SIZE_BLOC, monster_get_y(monster) * SIZE_BLOC );


}
