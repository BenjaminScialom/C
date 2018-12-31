#ifndef BOMB_H_
#define BOMB_H_

#include <map.h>
#include <constant.h>
#include <player.h>
#include <game.h>

struct bomb;


struct bomb * bomb_init ( struct game *game, int x, int y);

void bomb_free(struct bomb *bomb);

int bomb_get_x(struct bomb* bomb) ;

int bomb_get_y(struct bomb* bomb) ;

int bomb_get_estate (struct bomb *bomb) ;




void bomb_display ( struct game *game, int x, int y );


#endif /* BOMB_H_ */
