#ifndef MONSTER_H_
#define MONSTER_H_

#include <map.h>
#include <constant.h>
#include <player.h>
#include <game.h>

struct monster;

struct monster *monster_init (int x, int y, int id);

void monster_free(struct monster* monster);

int monster_get_x(struct monster* monster);

int monster_get_y(struct monster* monster);

void monster_set_life (struct monster* monster, int killed);

int monster_get_life (struct monster* monster);

void monster_display ( struct game *game, struct map *map, int x, int y );

#endif /* MONSTER_H_ */
