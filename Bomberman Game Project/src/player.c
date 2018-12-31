#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <game.h>
#include <map.h>


struct player {
	int x, y;
	enum direction current_direction;
	int nb_bombs;
	int nb_keys; //new variable.
	int nb_life;
	int wounds;
	int nb_range;
	int hodor; //porte dvt ou pas
};

struct player* player_init(int bomb_number) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_direction = SOUTH; //direction vue par player initial.
	player->nb_bombs = bomb_number;
	player->nb_keys = NB_KEYS;
	player->nb_life = NB_HEART;
	player->wounds = 0;
	player->nb_range = NB_RANGE;
	player->hodor = 0;

	return player;
}

void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

void player_set_x(struct player* player,int x)
{
	assert(player != NULL);
	player->x = x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_y(struct player* player,int y)
{
	assert(player != NULL);
	player->y = y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->current_direction = way;
}


int player_get_nb_key(struct player* player) { //new fct -> donne le nb de cles:
	assert(player);
	return player->nb_keys;
}


void player_set_nb_keys(struct player* player,int key)
{
	assert(player);
	player->nb_keys = key;
}


void player_set_wounds (struct player* player, int zero) {
	assert(player);
	player->wounds = zero;
}

int player_get_wounds (struct player* player) { //new fct -> donne la blessure player
	assert(player);
	return player->wounds;
}

void player_inc_wounds(struct player* player) {
	assert(player);
	player->wounds += 1;
}

void player_dec_wounds(struct player* player) {
	assert(player);
	player->wounds -= 1;
}

int player_get_nb_life (struct player* player) { //new fct -> donne le nb de vies:
	assert(player);
	return player->nb_life;
}

void player_set_nb_life(struct player * player, int life)
{
	assert(player);
	player->nb_life = life;
}

void player_inc_nb_life(struct player* player) {
	assert(player);
	player->nb_life += 1;
}

void player_dec_nb_life(struct player* player) {
	assert(player);
	player->nb_life -= 1;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bombs;
}


void player_set_nb_bomb(struct player* player,int bomb)
{
	assert(player);
	player->nb_bombs = bomb;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bombs -= 1;
}

int player_get_nb_range (struct player* player) { //new fct -> donne le nb de vies:
	assert(player);
	return player->nb_range;
}

void player_set_nb_range(struct player* player,int range)
{
	assert(player);
	player->nb_range = range;
}

void player_inc_nb_range(struct player* player) {
	assert(player);
	player->nb_range += 1;
}

void player_dec_nb_range(struct player* player) {
	assert(player);
	player->nb_range -= 1;
}



void player_from_map(struct player* player, struct map* map) {
	assert(player);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
	  for (j = 0; j < map_get_height(map); j++) {	
	    if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
	      player->x = i;
	      player->y = j;
	    }
	  }
	}
}


static int box_move_aux( struct player* player, struct map* map, int x, int y ) {

	if ( !map_is_inside(map, x, y) )
		return 0;

	switch ( map_get_cell_type(map, x, y) ) {
	case CELL_EMPTY:
		return 1;
		break;

	case CELL_BOMB:
		return 0;
		break;

	default:
		return 0;
		break;
	}

	// Box has moved
	return 1;
}

int box_move( struct player* player, struct map* map, int x, int y, unsigned char bonus ) {
	int x_b = x;
	int y_b = y;
	int move = 0;

	switch ( player->current_direction ) {
	case NORTH:
		if ( box_move_aux(player, map, x, y - 1) ) {
			if ( y - 1 < 0 )
				move = 0;
			else {
				y_b--;
				move = 1;
			}
		}
		break;

	case SOUTH:
		if ( box_move_aux(player, map, x, y + 1) ) {
			if ( y + 1 >= map_get_height(map) )
				move = 0;
			else {
				y_b++;
				move = 1;
			}
		}
		break;

	case WEST:
		if ( box_move_aux(player, map, x - 1, y) ) {
			if ( x - 1 < 0 )
				move = 0;
			else {
				x_b--;
				move = 1;
			}

		}
		break;

	case EAST:
		if ( box_move_aux(player, map, x + 1, y) ) {
			if ( x + 1 >= map_get_width(map) )
				move = 0;
			else {
				x_b++;
				move = 1;
			}
		}
		break;
	}

	if ( move ) {
		map_set_cell_type( map, x, y, CELL_EMPTY );
		if ( bonus == BONUS_BOMB_RANGE_DEC ) {
			map_set_cell_type( map, x_b, y_b, CELL_BOX_RANGEDEC );
		}
		else if ( bonus == BONUS_BOMB_RANGE_INC ) {
			map_set_cell_type( map, x_b, y_b, CELL_BOX_RANGEINC );
		}
		else if ( bonus == BONUS_BOMB_NB_DEC ) {
			map_set_cell_type( map, x_b, y_b, CELL_BOX_BOMBDEC );
		}
		else if ( bonus == BONUS_BOMB_NB_INC ) {
			map_set_cell_type( map, x_b, y_b, CELL_BOX_BOMBINC );
		}
		else if ( bonus == BONUS_LIFE ) {
			map_set_cell_type( map, x_b, y_b, CELL_BOX_LIFE );
		}
		else if( bonus == BONUS_MONSTER )
		{
			map_set_cell_type( map, x_b, y_b, CELL_BOX_MONSTER );
		}
		else {
			map_set_cell_type( map, x_b, y_b, CELL_BOX );
		}
	}
	return move;
}


static int player_move_aux(struct game *game, int x, int y) {

	struct player *player = game_get_player(game);
	struct map *map = game_get_current_map(game);

	if (!map_is_inside(map, x, y))
		return 0;

	unsigned char bonus = map_get_bonus_type(map, x, y);
	unsigned char level = map_get_level_type(map, x, y);
	int current = level >> 1;

	printf("try to be in x = %d & y = %d\n", x, y);

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY: //scenery = stone ; tree ; princess
		return 0;
		break;

	case CELL_BOMB:
		return 0;
		break;

	case CELL_BOX:
		return box_move( player, map, x, y, bonus);
		break;

	case CELL_KEY:
		if ( player_get_nb_key(player) < 8 )
			player->nb_keys += 1;
		return 1;
		break;

	case CELL_BONUS:

		switch ( bonus ) {
		case BONUS_BOMB_RANGE_DEC:
			if ( player_get_nb_range(player) > 1 ) {
				player_dec_nb_range( player );
			}
			map_set_cell_type( map, x, y, CELL_EMPTY );
			return 1;
			break;

		case BONUS_BOMB_RANGE_INC:
			if ( player_get_nb_range(player) < 9 ) {
				player_inc_nb_range( player );
			}
			map_set_cell_type( map, x, y, CELL_EMPTY );
			return 1;
			break;

		case BONUS_BOMB_NB_DEC:
			if ( player_get_nb_bomb(player) > 1 ) {
				player_dec_nb_bomb( player );
			}
			map_set_cell_type( map, x, y, CELL_EMPTY );
			return 1;
			break;

		case BONUS_BOMB_NB_INC:
			if ( player_get_nb_bomb(player) < 9 ) {
				player_inc_nb_bomb( player );
			}
			map_set_cell_type( map, x, y, CELL_EMPTY );
			return 1;
			break;

		case BONUS_MONSTER:
			break;

		case BONUS_LIFE:
			if ( player_get_nb_life(player) < 9 ) {
				player_inc_nb_life( player );
			}
			map_set_cell_type( map, x, y, CELL_EMPTY );
			return 1;
			break;

		}

		return 1;
		break;

	case CELL_MONSTER:
		if ( player_get_wounds(player) == 0 )
			player_inc_wounds(player);
		return 0;
		break;

	case CELL_DOOR:
		printf("1\n");
		switch ( map_get_door_type (map, x, y) ) { //porte fermee ou ouverte
		case DOOR_CLOSED:
			printf("1.1\n");
			if ( player_get_nb_key(player) > 0 ) {
				printf("1.1.1\n");
				map_set_cell_type( map, x, y, CELL_EMPTY );
				map_set_cell_type( map, x, y, CELL_DOOR | level | DOOR_OPENED );
				printf("key ? \n");
				player->nb_keys -= 1;
				printf("key pris \n");
			}
			printf("1.2\n");
			return 0;
			break;
		case DOOR_OPENED:
			printf("1.3\n");
			player->hodor = 1;

			printf("change level \n");
			game_set_current( game, current );
			printf("1.4\n");
			map = game_get_current_map(game);
			printf("1.5\n");
			window_create(SIZE_BLOC * map_get_width(map), SIZE_BLOC * map_get_height(map) + BANNER_HEIGHT + LINE_HEIGHT);
			printf("1.6\n");
			return 1;

		default:
			return 0;
			break;
		}

		return 0;
		break;

	case CELL_PRINCESS:
		//game->jeu = GAME_WIN;
		game_set_jeu(game, GAME_WIN);
		return 0;
		break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move( struct game* game ) { //action du player, apres interaction clavier:

	struct player *player = game_get_player(game);
	struct map *map = game_get_current_map(game);


	int x = player->x;
	int y = player->y;
	int move = 0;

	switch ( player->current_direction ) {
	case NORTH:
		if ( player_move_aux(game, x, y - 1) ) {
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if ( player_move_aux(game, x, y + 1) ) {
				player->y++;
				move = 1;
		}
		break;

	case WEST:
		if ( player_move_aux(game, x - 1, y) ) {
				player->x--;
				move = 1;
		}
		break;

	case EAST:
		if ( player_move_aux(game, x + 1, y) ) {
				player->x++;
				move = 1;
		}
		break;
	}

	printf("I'm in x = %d & y = %d\n\n", player->x, player->y);

	if (move) {

		if(map_get_cell_type(map, x, y) != CELL_DOOR && !player->hodor)
		{	//si case initiale != door ET hodor=0
			printf("p1.1\n");
			if(map_get_cell_type(map, x, y) != CELL_BOMB)
			{
				printf("p1.1.1\n");
				map_set_cell_type( map, x, y, CELL_EMPTY );
			}
			map_set_cell_type( map, player->x, player->y, CELL_PLAYER );
		}
		else if(player->hodor)
		{
			printf("p1.2\n");
			player->hodor = 0;
		}
		/*else {
			printf("p2\n");
			map_set_cell_type( map, player->x, player->y, CELL_PLAYER );
		}*/

	}
	return move;
}

void player_display( struct player* player ) {
	assert( player );

	//printf("%d\n", player->wounds);

	//wounds = degats recues par player, allant de 0 a 25.
	// 0 etant player sain, sans dommages
	//arrivee a wounds=25, on remet wounds=0

	if ( player_get_wounds(player) == 0 ) {
		window_display_image( sprite_get_player(player->current_direction),
				player->x * SIZE_BLOC, player->y * SIZE_BLOC );
		//actualise la direction et position de player a position (x;y) indiquee
	}
	else if( player->wounds++ == 1 && player_get_nb_life(player) > 0 ){ //lu donc post-incrementation
		player_dec_nb_life(player);
	}
	else if ( player_get_wounds(player) >= 25 ) {
		player->wounds = 0;
	}
	else if( player->wounds%2 == 0 ){
		window_display_image( sprite_get_player(player->current_direction), player->x * SIZE_BLOC, player->y * SIZE_BLOC );
	}


}

