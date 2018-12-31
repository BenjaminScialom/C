#ifndef LIST_MONSTER_H_
#define LIST_MONSTER_H_

#include <map.h>
#include <constant.h>
#include <player.h>
#include <game.h>

struct llist_monster ;


struct llist_monster * llist_put_monster( struct llist_monster *llist_monster, struct monster *monster );

struct llist_monster *llist_monster_free(struct llist_monster *llist_monster, struct monster *monster);

struct monster * select_monster(struct llist_monster *llist_monster , int x, int y);





#endif /* LIST_MONSTER_H_ */
