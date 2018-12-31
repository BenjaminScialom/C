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
#include <player.h>

#define CELL(i,j) ( (i) + (j) * map_get_width())


void save_game(struct game* game)
{
    FILE *file;
    file = fopen("data/save.txt", "w"); // chemin relatif du fichier où l'on écrit.

    assert(file != NULL); 				// "file" non vide


    struct map* map = NULL;
    map = game_get_current_map (game);// on va chercher la map correspondant au niveau où l'on est


    fprintf(file, "%d:%d \n", map_get_width(map), map_get_height(map));// on écrit dans le fichier en précisant le numéro de la carte avec sa taille au déut.

    for(int j = 0; j < map_get_height(map); j++)
    {
        for(int i = 0; i < map_get_width(map); i++) // on fixe la ligne puis on la parcours pour chaque ligne
         {
             unsigned char type = map_get_cell(map, i, j); // on récupère le type de la cellule en i,j
             fprintf(file, "%d ", type); // on lécrit dans le fichier
         }
         fprintf(file, "\n");
    }


// attributs du joueur


    fprintf(file, "%d \n", player_get_nb_life(game_get_player(game)));// on récupère le nombre de vie du joueur
    fprintf(file, "%d:%d \n", player_get_x(game_get_player(game)), player_get_y(game_get_player(game)));// on récupère la position du joueur
    fprintf(file, "%d \n", player_get_nb_key(game_get_player(game))); // on récupère le nombre de clefs du joueur.
    fprintf(file, "%d \n", player_get_nb_bomb(game_get_player(game))); // on récupère le nombre de bomb du joueur.
    fprintf( file, "%d \n", player_get_nb_range(game_get_player(game)) );// on récupère le range
    fprintf( file, "%d ",game_get_current(game) );
    fclose(file);

    printf("Game Saved\n");
}





void load_save(struct game* game)
{
	game_set_llist_bomb(game, NULL);
	struct player *player = game_get_player(game);

	FILE *txt_save;
	txt_save = fopen("data/save.txt", "r");
	printf("l1\n");
	if(txt_save != NULL)
	{
		printf("l1.2\n");
/*
		    char loc_map[30];
		    strcpy( loc_map, "data/save" );
		    char *type = ".txt";
		    strcat ( loc_map, type );

		    FILE * txt_save = NULL;
		    txt_save = fopen ( loc_map, "r" );
		    */
		    int width, height;
		    char temp[1];
		    fscanf( txt_save, "%d", &width );
		    fscanf( txt_save,"%1s", temp);
		    fscanf( txt_save, "%d", &height );

		    struct map * map = map_new( width, height );

		    int i,j;
		    for ( j=0 ; j < height ; j++ )   // on recopie chaque cellule du fichier txt pour que la map corresponde...
		        {
		            for ( i=0 ; i < width ; i++ )
		            {
		                int cell_val;
		                fscanf( txt_save, "%d", &cell_val );
		                map_set_grid(map, i, j, cell_val);

		                if(cell_val == CELL_BOMB)
		                {
		                	game_set_bomb( game, bomb_init( game, i, j) );

		                	//Ensuite, le struct bomb sera mis dans la llist_bomb, le bomb sera donc saved dans la llist_bomb.
		                	//game->llist_bomb = llist_put_bomb( game->llist_bomb, game->bomb );

		                	game_set_llist_bomb(game, llist_put_bomb(game_get_llist_bomb(game), game_get_bomb(game)));


		                	game_set_llist_bomb(game, game_get_llist_bomb(game));
		                }
		            }
		        }
		    printf("l1.3\n");

		    char temp2[1];

		    int a,b,d,e,f,g,k;

		    fscanf(txt_save,"%d",&a);
		    player_set_nb_life(player, a);
		    printf("nb life = %d\n", a);

		    fscanf(txt_save,"%d", &b);
		    player_set_x(player, b);
		    printf("player->x = %d\n", b);

		    fscanf(txt_save,"%1s",temp2);

		    fscanf(txt_save,"%d",&d);
		    player_set_y(player,d);
		    printf("player->y = %d\n", d);

		    fscanf(txt_save,"%d",&e);
		    player_set_nb_keys(player,e);
		    printf("nb keys = %d\n", e);

		    fscanf(txt_save,"%d",&f);
		    player_set_nb_bomb(player,f);
		    printf("nb bombs = %d\n", f);

		    fscanf(txt_save,"%d",&g);
		    player_set_nb_range(player,g);
		    printf("nb range = %d\n", g);

		    fscanf(txt_save,"%d",&k);
		    game_set_current(game,k);
		    printf("game->current = %d\n", k);

		    //game_set_llist_bomb(game, NULL);

		    game_set_map(game,map);

		    game_monster_from_maps (game);


		    printf("l1.4\n");

		    fclose( txt_save );  // fermeture du fichier txt
	}
	else
	{

	}

   //return k;  // on renvoie le current
}
