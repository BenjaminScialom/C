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

struct llist_monster {
	struct monster *monster;
	struct llist_monster *next_llist;
};



struct llist_monster * llist_put_monster( struct llist_monster *llist_monster, struct monster *monster ) { //New fct
	//charge une nouvelle monster a la suite d'autres monster, ie en fin de liste.

	//nouvelle Allocation memoire de la new monster:
	struct llist_monster *new_elt = malloc( sizeof(struct llist_monster) );

	if(new_elt != NULL) //on verifie que l'espace est bien allouee
	{
		new_elt->monster = monster;
		new_elt->next_llist = NULL;

		//On l'ajoute ensuite en fin de liste
		if ( llist_monster == NULL )
		{	//si list nul
			return new_elt;
		}
		else
		{	//si list non nul, on parcourt jusqu'a la fin pr poser le monster
			struct llist_monster *post = llist_monster;

			while ( post->next_llist != NULL )
			{
				post = post->next_llist;
			}
			post->next_llist = new_elt;
		}
	}
	return llist_monster;
}




struct monster * select_monster( struct llist_monster *llist_monster , int x, int y ) {
	assert(llist_monster);

	struct llist_monster *ptr_llist = llist_monster; //pointeur de llist
	struct monster *monster = ptr_llist->monster; //pointeur du monster de la llist pointee.

	while ( monster_get_x(monster) != x  ||  monster_get_y(monster) != y )
	{	//parcours tant que non trouvee
		if(ptr_llist->next_llist != NULL)
		{	//si le suivant existe
			ptr_llist = ptr_llist->next_llist;
			printf("W1 \n");
			monster = ptr_llist->monster;
			printf("W1.1 \n");
		}
		/*else
		{	//sinon, on l'ajoute en fin
			ptr_llist = llist_put_monster( ptr_llist, monster_init(x,y) );
			monster = ptr_llist->monster;
		}*/
		else
		{
			//ptr_llist = llist_monster;
			break;
		}

	}
	return monster;
}

struct llist_monster * llist_monster_free( struct llist_monster *llist_monster, struct monster *monster) {

	struct llist_monster *renvoyer = llist_monster;
	struct llist_monster *preced = llist_monster;

	while( renvoyer->monster != monster )
	{
		printf("f10\n");
		preced = renvoyer;
		printf("f11\n");

		if(renvoyer->next_llist != NULL)
		{
			renvoyer = renvoyer->next_llist;
			printf("f12\n");
		}
		else {
			renvoyer = llist_monster;
		}
	}

	//ok on a le monster dans llist, mtn on raccorde la llist
	if (renvoyer->next_llist != NULL)
	{
		preced->next_llist = renvoyer->next_llist;
		printf("f2\n");
	}
	else
	{
		preced->next_llist = NULL;
		printf("f3\n");
	}

	return llist_monster;
}








