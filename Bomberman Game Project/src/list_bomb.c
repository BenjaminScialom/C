#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <list_bomb.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <player.h>


struct llist_bomb {
	struct bomb *bomb;
	struct llist_bomb *next_llist;
	//struct llist_bomb *prev_llist;
};



struct llist_bomb * llist_put_bomb( struct llist_bomb *llist_bomb, struct bomb *bomb ) { //New fct
	//charge une nouvelle bomb a la suite d'autres bomb, ie en fin de liste.

	//nouvelle Allocation memoire de la new bomb:
	struct llist_bomb *new_elt = malloc( sizeof(struct llist_bomb) );

	if(new_elt != NULL)
	{
		new_elt->bomb = bomb;
		new_elt->next_llist = NULL;

		//On l'ajoute ensuite en fin de liste
		if ( llist_bomb == NULL )
		{
			//new_elt->prev_llist = NULL;
			return new_elt;
		}
		else
		{
			struct llist_bomb *post = llist_bomb;


			while ( post->next_llist != NULL )
			{
				post = post->next_llist;
			}
			post->next_llist = new_elt;
			//new_elt->prev_llist = post;

		}
	}
	return llist_bomb;
}




struct bomb * select_bomb( struct llist_bomb *llist_bomb , int x, int y ) {
	assert(llist_bomb);
	//struct llist_bomb *llist_bomb = game_get_llist_bomb(game);
	struct llist_bomb *ptr_llist = llist_bomb;
	struct bomb *bomb = ptr_llist->bomb;

	while ( bomb_get_x(bomb) != x  ||  bomb_get_y(bomb) != y ) {
		ptr_llist = ptr_llist->next_llist;
		bomb = ptr_llist->bomb;
	}
	return bomb;
}

struct llist_bomb * llist_bomb_free( struct llist_bomb *llist_bomb, struct bomb *bomb) {

	if( llist_bomb != NULL )
	{
		struct llist_bomb *renvoyer = llist_bomb->next_llist;
		free(llist_bomb);
		return renvoyer;
	}
	else
	{
		return NULL;
	}
}
