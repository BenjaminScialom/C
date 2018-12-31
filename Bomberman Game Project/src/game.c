#include <assert.h>
#include <time.h>


#include <game.h>
#include <constant.h>
#include <map.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <list_bomb.h>
#include <list_monster.h>
#include <monster.h>
#include <bomb.h>
#include <save.h>

struct game {
	struct map** maps;       // the game's map
	short levels;            // nb maps of the game
	short current;	//donne le level actuel (commencera au level 0)
	struct player* player;
	struct bomb *bomb;
	struct llist_bomb *llist_bomb;
	struct llist_monster *llist_monster;
	int pause;
	enum jeu *jeu;
};



struct llist_monster * game_monster_from_maps (struct game* game) {
	assert(game);

	game->llist_monster = NULL;
	//int id = 1;

	for(int k = 0; k <= 7 ; k++)
	{
		struct map * map_k = game->maps[k];
		assert(map_k != NULL);
		assert(map_get_height(map_k) > 0 && map_get_width(map_k) > 0);

		//int x, y;
		for (int i = 0; i < map_get_width(map_k); i++) {
		  for (int j = 0; j < map_get_height(map_k); j++) {
		    //x = i * SIZE_BLOC;
		    //y = j * SIZE_BLOC;

		    unsigned char type = map_get_grid(map_k, i, j);

		    switch (type & 0xf0)
		    {
		    case CELL_MONSTER: //on ajoute a la liste monster
		    	game->llist_monster = llist_put_monster( game->llist_monster, monster_init(i, j, k) );
		    	//id++;
		    	break;

		    case CELL_BOX:
		    	switch (type & 0x0f)
		    	{
		    	case BONUS_MONSTER: //on ajoute a la liste monster
		    		game->llist_monster = llist_put_monster( game->llist_monster, monster_init(i, j, k) );
		    		//id++;
		    		break;

		    	default:
		    		break;
		    	}
		    	break;

		    default:
		   		break;
		    }

		  }
		}
	}
	return game->llist_monster;

}


struct game* game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->maps = malloc(sizeof(struct game));
	//game->maps[0] = map_get_default();

	game->levels = NUMERO_LEVELS;
	game->current = NUMERO_CURRENT;

	for (int i = 0 ; i <= game->levels ; i++ )
	{
		game->maps[i] = fill_map(i);
	}

	game->llist_bomb = NULL;
	game->llist_monster = game_monster_from_maps(game);

	game->player = player_init(NB_BOMB); //les variables du player sont initialises.
	player_from_map(game->player, game->maps[0]); // get x,y of the player on the first map

	game->pause = 0;
	game->jeu = MENU;

	return game;
}



void game_free( struct game* game ) {
	assert( game );

	player_free( game->player );

	for ( int i = 0; i < game->levels; i++ )
		map_free( game->maps[i] );

	//monster_free( game->monster );
	//free_all_bombs ( game->bomb );

}


int game_get_pause(struct game *game) {
	assert(game);
	return game->pause;
}

void game_set_jeu(struct game* game, int x) {
	assert(game);
	game->jeu = x;
}

void game_set_map(struct game* game, struct map* map)
{
	assert(game);
	game->maps[game->current] = map;
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->current];
}


int game_get_current(struct game* game)
{
	assert(game);
	return game ->current;
}

void game_set_current (struct game *game , int n) {
	assert(game);
	game->current = n;
}

struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

struct bomb *game_get_bomb(struct game *game) {
	assert(game);
	return game->bomb;
}

void game_set_bomb (struct game *game , struct bomb *bomb) {
	assert(game);
	game->bomb = bomb;
}

struct llist_bomb *game_get_llist_bomb(struct game *game) { //New function:
	assert(game);
	return game->llist_bomb;
}

void game_set_llist_bomb(struct game *game, struct llist_bomb *llist_bomb) { //New function:
	assert(game);
	game->llist_bomb = llist_bomb;
}

struct llist_monster *game_get_llist_monster(struct game *game) { //New function:
	assert(game);
	return game->llist_monster;
}

void game_set_llist_monster(struct game *game, struct llist_monster *llist_monster) { //New function:
	assert(game);
	game->llist_monster = llist_monster;
}


void game_banner_display( struct game* game ) { //Banniere du jeu, rafrachit par la boucle infinie:
	assert(game);

	struct map* map = game_get_current_map(game); //Prend la map du level donnee dans le current de game.

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y); //colle la ligne du banner.

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - (map_get_width(map) - 1) * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y); //colle  symbole de vie = coeur.

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_life(game_get_player(game))), x, y); //colle le nbre de vie initial.

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image( sprite_get_banner_bomb(), x, y ); //colle le symbole bombe a disposition.

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number( player_get_nb_bomb( game_get_player(game) ) ), x, y ); //colle le nbre de bombes restant.

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image( sprite_get_banner_range(), x, y ); //colle le symbole de propagation bomb.

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(
			sprite_get_number( player_get_nb_range( game_get_player(game) ) ), x, y );
	//colle le nbre de propagation initial bomb.

	if (player_get_nb_key(game_get_player(game)) > 0) { //Affiche les cles dans l'inventaire si on en a:
		x = 5 * white_bloc + 6 * SIZE_BLOC;
		window_display_image(sprite_get_key(), x, y); //colle le symbole cle a disposition.

		x = 5 * white_bloc + 7 * SIZE_BLOC;
		window_display_image(sprite_get_number(player_get_nb_key(game_get_player(game))), x, y); //colle le nbre de cles restant.
	}

	x = ( map_get_width(map) - 2 ) * SIZE_BLOC ;
	window_display_image( sprite_get_number(game->current), x, y ); //colle le level map actuel.

	x = ( map_get_width(map) - 1 ) * SIZE_BLOC ;
	window_display_image ( sprite_get_banner_door(), x, y ); //colle le symbole door level.

}

void game_display(struct game* game) { //Affichage du jeu (dans la boucle infinie) :
	assert(game);

	if(player_get_nb_life(game_get_player(game)) == 0)
	{
		game->jeu = GAME_OVER;
	}
	window_clear(); //remet vierge la fenetre.

	game_banner_display(game); //affiche la banner du jeu.
	map_display( game_get_current_map(game), game ); //affiche la map au niveau donne par le current du player.

	player_display(game->player); //puis par-dessus on affiche le player

	window_refresh(); //remet a jour.
}

static short input_keyboard(struct game* game) { //Interaction clavier:
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);


	while ( SDL_PollEvent(&event) ) {
		switch ( event.type ) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN: //Si touche pressee, alors si...
			switch ( event.key.keysym.sym ) {
			case SDLK_ESCAPE: //touche echap pr quitter le jeu
				return 1;
			case SDLK_UP: //touche fleche haut
				if(!game->pause)
				{
					player_set_current_way(player, NORTH);
					player_move ( game );
				}
				break;
			case SDLK_DOWN: //touche fleche bas
				if(!game->pause)
				{
					player_set_current_way(player, SOUTH);
					player_move ( game );
				}
				break;
			case SDLK_RIGHT: //touche fleche droite
				if(!game->pause)
				{
					player_set_current_way(player, EAST);
					player_move ( game );
				}
				break;
			case SDLK_LEFT: //touche fleche gauche
				if(!game->pause)
				{
					player_set_current_way(player, WEST);
					player_move ( game );
				}
				break;
			case SDLK_SPACE: //touche espace (bombe)
				if ( !game->pause  &&  player_get_nb_bomb(player) > 0  &&  player_get_wounds(player) == 0 ) {
					if( map_get_cell_type( map, player_get_x(player), player_get_y(player) ) == CELL_PLAYER ) {

						//On initialise la bombe posee, en remplissant son struct bomb.
						game->bomb = bomb_init( game, player_get_x(player), player_get_y(player) );
						printf("Bomb initalized \n");

						//Ensuite, le struct bomb sera mis dans la llist_bomb, le bomb sera donc saved dans la llist_bomb.
						game->llist_bomb = llist_put_bomb( game->llist_bomb, game->bomb );
						printf("Bomb put into the llist. \n");


						//On reserve la case pour la bomb
						map_set_cell_type ( map, player_get_x(player), player_get_y(player), CELL_BOMB );
						printf("Map reserved for a bomb \n");

						//Et on diminue le nbre de bomb dans l'inventaire
						player_dec_nb_bomb ( game_get_player(game) );
						printf("-1 bomb \n");
					}
				}
				break;

			case SDLK_p:
				game->pause = !game->pause;
				break;

			case SDLK_s:
				save_game(game);
				break;

			case SDLK_l:
				load_save(game);
				break;

			default:
				break;
			}
			break;
		}
	}
	return 0;
}

int game_update(struct game* game) {

	int continuer = 1;
    SDL_Event event;
    struct map* map = game_get_current_map(game);

	if (input_keyboard(game))
		return 1; // exit game

	if( game->jeu == MENU)
	{
		window_create(1280,720);
		window_clear();
		window_display_image(sprite_get_menu(), 0, 0);
		window_refresh();
		while (continuer)
	    {
	        SDL_WaitEvent(&event);
	        switch(event.type)
	        {
	            case SDL_QUIT:
	                continuer = 0;
	                return 1;
	                break;
	            case SDL_KEYDOWN:
	                switch (event.key.keysym.sym)
	                {
	                    case SDLK_TAB: /* Appui sur la touche Echap, on arrête le programme */
	                        continuer = 0;
	                        game->jeu = PLAY;
	                		window_create(SIZE_BLOC * map_get_width(map), SIZE_BLOC * map_get_height(map) + BANNER_HEIGHT + LINE_HEIGHT);
	                        return 0;
	                        break;
	                    default:
	                    	break;
	                }
	                break;

	            default:
	            	break;
	        }
	    }
	}
	else if(game->jeu == PLAY)
	{

		//window_create(SIZE_BLOC * MAP_WIDTH,
		//SIZE_BLOC * MAP_HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);
		return 0;
	}
	else if(game->jeu == GAME_WIN)
	{
		window_create(1280,720);
		window_clear();
		window_display_image(sprite_get_win(), 0, 0);
		window_refresh();
		while (continuer)
	    {
	        SDL_WaitEvent(&event);
	        switch(event.type)
	        {
	            case SDL_QUIT:
	                continuer = 0;
	                return 1;
	                break;
	            case SDL_KEYDOWN:
	                switch (event.key.keysym.sym)
	                {
	                    case SDLK_TAB: /* Appui sur la touche Echap, on arrête le programme */
	                        continuer = 0;

	                    	game->levels = NUMERO_LEVELS;
	                    	game->current = NUMERO_CURRENT;

	                    	for (int i = 0 ; i <= game->levels ; i++ )
	                    	{
	                    		game->maps[i] = fill_map(i);
	                    	}

	                    	game->llist_bomb = NULL;
	                    	game->llist_monster = game_monster_from_maps(game);

	                    	game->player = player_init(NB_BOMB); //les variables du player sont initialises.
	                    	player_from_map(game->player, game->maps[0]); // get x,y of the player on the first map

	                    	game->pause = 0;
	                    	game->jeu = MENU;
	                        return 0;
	                        break;
	                    default:
	                    	break;
	                }
	                break;

	            default:
	            	break;
	        }
	    }
	}

	else if(game->jeu == GAME_OVER)
	{
		window_create(1025,516);
		window_clear();
		window_display_image(sprite_get_over(), 0, 0);
		window_refresh();
		while (continuer)
	    {
	        SDL_WaitEvent(&event);
	        switch(event.type)
	        {
	            case SDL_QUIT:
	                continuer = 0;
	                return 1;
	                break;
	            case SDL_KEYDOWN:
	                switch (event.key.keysym.sym)
	                {
	                    case SDLK_TAB: /* Appui sur la touche Echap, on arrête le programme */
	                        continuer = 0;

	                    	game->levels = NUMERO_LEVELS;
	                    	game->current = NUMERO_CURRENT;

	                    	for (int i = 0 ; i <= game->levels ; i++ )
	                    	{
	                    		game->maps[i] = fill_map(i);
	                    	}

	                    	game->llist_bomb = NULL;
	                    	game->llist_monster = game_monster_from_maps(game);

	                    	game->player = player_init(NB_BOMB); //les variables du player sont initialises.
	                    	player_from_map(game->player, game->maps[0]); // get x,y of the player on the first map

	                    	game->pause = 0;
	                    	game->jeu = MENU;

	                        return 0;
	                        break;
	                    default:
	                    	break;
	                }
	                break;

	            default:
	            	break;
	        }
	    }
	}

	return 0;
}


