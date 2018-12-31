#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <bomb.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <player.h>
#include <list_bomb.h>
#include <game.h>
#include <list_monster.h>


struct bomb {

	int x, y;	 //coordonnees de la bomb
	enum explosion exploded;
	int time; 	//time qu'on donne a la bomb, pr comparer ensuite avec time actuel
	int estate; 	//etats bomb (image avec les differentes meches) ; de 0 a 3
	int range;

};


struct bomb * bomb_init ( struct game *game, int x, int y) {

	struct bomb *new_bomb = malloc ( sizeof(struct bomb) );
	if (new_bomb != NULL)
	{
		new_bomb->x = x;
		new_bomb->y = y;
		new_bomb->exploded = CLOCK; //L'etat d'explosion est dans l'etat "CLOCK"
		new_bomb->time = SDL_GetTicks() + 1000;  //tmps actuel + 1 sec
		new_bomb->estate = 3;
		new_bomb->range = player_get_nb_range(game_get_player(game));
	}


	//sera appelee dans list_bomb
	return new_bomb;
}

void bomb_free(struct bomb *bomb) {
	assert(bomb);
	free(bomb);
}

int bomb_get_x( struct bomb* bomb ) {
	assert( bomb != NULL );
	return bomb->x;
}

int bomb_get_y( struct bomb* bomb ) {
	assert( bomb != NULL );
	return bomb->y;
}

int bomb_get_estate (struct bomb *bomb) {
	assert ( bomb != NULL );
	return bomb->estate;
}

int bomb_get_range (struct bomb *bomb) {
	assert ( bomb != NULL );
	return bomb->range;
}




static int bomb_explosion_aux( struct game *game, int x, int y, int x_expl, int y_expl ) {
	// comme pour la fct player_move_aux
	struct map *map_b = game_get_current_map(game);
	struct player *player_b = game_get_player(game);
	struct llist_bomb *llist_bomb = game_get_llist_bomb(game);
	struct bomb *bomb = select_bomb(llist_bomb, x, y);
	struct llist_monster *llist_monster = game_get_llist_monster(game);


	printf("1.10\n");

	if ( !map_is_inside(map_b, x_expl, y_expl) )
		return 0;
	printf("1.11\n");
	unsigned char type = map_get_bonus_type( map_b, x_expl, y_expl ); //type = les 4 bits de poids faible
	printf("1.12\n");
	switch ( map_get_cell_type(map_b, x_expl, y_expl) ) {
	//selon la case (x,y)
	case CELL_EMPTY:
		return 1;
		break;

	case CELL_BOX:
		window_display_image ( sprite_get_explosion(), x_expl * SIZE_BLOC, y_expl * SIZE_BLOC );
		if ( SDL_GetTicks() > bomb->time - 10 ) {
			//change the type of the case at the last ms !!
			if ( type == BONUS_BOMB_RANGE_DEC ) {
				map_set_cell_type( map_b, x_expl, y_expl, CELL_BONUS_RANGEDEC );
			}
			else if ( type == BONUS_BOMB_RANGE_INC ) {
				map_set_cell_type( map_b, x_expl, y_expl, CELL_BONUS_RANGEINC );
			}
			else if ( type == BONUS_BOMB_NB_DEC ) {
				map_set_cell_type( map_b, x_expl, y_expl, CELL_BONUS_BOMBDEC );
			}
			else if ( type == BONUS_BOMB_NB_INC ) {
				map_set_cell_type( map_b, x_expl, y_expl, CELL_BONUS_BOMBINC );
			}
			else if ( type == BONUS_LIFE ) {
				map_set_cell_type( map_b, x_expl, y_expl, CELL_BONUS_LIFE );
			}
			else if( type == BONUS_MONSTER )
			{
				map_set_cell_type( map_b, x_expl, y_expl, CELL_MONSTER );
			}
			else {
				map_set_cell_type( map_b, x_expl, y_expl, CELL_EMPTY );
			}
		}

		return 0;
		break;

	case CELL_PLAYER:
		if ( player_get_wounds(player_b) == 0 )
			player_inc_wounds( player_b );
		return 1;
		break;

	case CELL_BONUS:
		map_set_cell_type( map_b, x_expl, y_expl, CELL_EMPTY );
		return 1;
		break;

	case CELL_MONSTER:
		map_set_cell_type( map_b, x_expl, y_expl, CELL_EMPTY );
		printf("free monster into the llist :\n");
		llist_monster_free(llist_monster, select_monster(llist_monster, x_expl, y_expl) );
		printf("free monster done\n");
		//monster_free(monster);

		return 1;
		break;

	case CELL_BOMB:
		if ( player_get_x(player_b) == x_expl  &&  player_get_y(player_b) == y_expl ) {
			if ( player_get_wounds(player_b) == 0 )
				player_inc_wounds( player_b );
		}
		return 0;
		break;

	default:
		return 0;
		break;
	}

	return 1;
}


void bomb_explosion_display ( struct game *game, int x, int y ) {
	//Affichage des explosions (ici ce sont des cles)

	struct map *map = game_get_current_map(game);
	struct player *player_b = game_get_player(game);
	struct llist_bomb *llist_bomb = game_get_llist_bomb(game);
	struct bomb *bomb = select_bomb(llist_bomb, x, y);
	printf("1.0\n");
	if ( player_get_x(player_b) == bomb->x  && player_get_y(player_b) == bomb->y ) {
		if ( player_get_wounds(player_b) == 0 )
			player_inc_wounds( player_b );
	}
	window_display_image ( sprite_get_explosion, bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC );
	printf("1.1\n");
	int i = 1;
	while ( i <= bomb->range  &&  bomb_explosion_aux (game, x, y, bomb->x, bomb->y - i) ) { //NORTH
		printf("1.2\n");
		if ( bomb->y - i >= 0 )
		{
			printf("1.21\n");
			window_display_image ( sprite_get_explosion(), bomb->x * SIZE_BLOC, (bomb->y - i) * SIZE_BLOC );
		}

		i++;
	}

	i = 1;
	while ( i <= bomb->range  &&  bomb_explosion_aux (game, x, y, bomb->x - i, bomb->y) ) { //WEST
		if ( bomb->x - i >= 0 )
			window_display_image ( sprite_get_explosion, (bomb->x - i) * SIZE_BLOC, bomb->y * SIZE_BLOC );
		i++;
	}

	i = 1;
	while ( i <= bomb->range  &&  bomb_explosion_aux ( game, x, y, bomb->x, bomb->y + i ) ) { //SOUTH
		if ( bomb->y + i < map_get_height(map) )
			window_display_image ( sprite_get_explosion(), bomb->x * SIZE_BLOC, (bomb->y + i) * SIZE_BLOC );
		i++;
	}

	i = 1;
	while ( i <= bomb->range  &&  bomb_explosion_aux ( game, x, y, bomb->x + i, bomb->y ) ) { //EAST
		if ( bomb->x + i < map_get_width(map) )
			window_display_image ( sprite_get_explosion(), (bomb->x + i) * SIZE_BLOC, bomb->y * SIZE_BLOC );
		i++;
	}
}



void bomb_display ( struct game *game, int x, int y ) {
	//Affichage des bomb, la fonction est appelee dans la fct "game_diplay", qui se situe dans la boucle infini du jeu.
		//donc appelee plusieurs fois par sec.
	assert(game);

	struct map *map = game_get_current_map(game);
	struct player *player = game_get_player(game);
	struct llist_bomb *llist_bomb = game_get_llist_bomb(game);
	struct bomb *bomb = select_bomb(llist_bomb, x, y);
	printf("Bomb selected \n");


	switch ( bomb->exploded ) {

	case CLOCK:
			//cas CLOCK, j'affiche little by little l'etat de la bomb...
		if ( bomb->estate >= 0  &&  SDL_GetTicks() <= bomb->time ) {
			window_display_image( sprite_get_timer_bomb(bomb->estate),
					bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC );

			printf("Display the bomb in estate %d \n", bomb->estate);
		}
		else if ( bomb->estate >= 0  &&  SDL_GetTicks() > bomb->time ) {
			bomb->time = SDL_GetTicks() + 1000;
			window_display_image( sprite_get_timer_bomb(bomb->estate),
					bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC );
			bomb->estate--;
		}
		else {
			printf("Time's up ! \n");
			bomb->exploded = BOOM;
			bomb->time = SDL_GetTicks() + 500;
		}
		break;

	case BOOM:
			//cas BOOM , j'affiche quand je fais exploser la bombe. (via une autre function qui s'en chargera).
		if ( SDL_GetTicks() <= bomb->time ) {
			bomb_explosion_display(game, x, y);
			printf("Kabouuuum !\n");
		}
		else {

			if ( player_get_wounds( player ) != 0 ) { //Si player clignote tjrs, alors...
				player_inc_wounds( player );
			}

			else {
				if (player_get_nb_bomb(player) < 9 ) {
					player_inc_nb_bomb( player );
				}

				if ( bomb != NULL ) {
					printf("Can you free the bomb into the llist ? \n");
					game_set_llist_bomb( game, llist_bomb_free(game_get_llist_bomb(game), bomb) );
					printf("ok ? \n");
					bomb_free(bomb);
					printf("Bomb free into the llist ! \n\n");
					map_set_cell_type( map, x, y, CELL_EMPTY );
				}


			}
		}
		break;

	case WAIT:

		break;
	}


}
