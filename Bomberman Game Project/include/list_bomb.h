#ifndef LIST_BOMB_H_
#define LIST_BOMB_H_

#include <map.h>
#include <constant.h>
#include <player.h>
#include <game.h>

struct llist_bomb ;


struct llist_bomb * llist_put_bomb( struct llist_bomb *llist_bomb, struct bomb *bomb );

struct llist_bomb *llist_bomb_free(struct llist_bomb *llist_bomb, struct bomb *bomb);

struct bomb * select_bomb(struct llist_bomb *llist_bomb , int x, int y);






#endif /* LIST_BOMB_H_ */
