#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>
#include <bomb.h>

// Abstract data type
struct game;

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);

struct llist_monster * game_monster_from_maps (struct game* game) ;

// Return the player of the current game
struct player* game_get_player(struct game* game);

int game_get_pause(struct game *game);

// Return the current map
struct map* game_get_current_map(struct game* game);

struct bomb *game_get_bomb(struct game *game);
void game_set_bomb (struct game *game , struct bomb *bomb);

struct llist_bomb *game_get_llist_bomb (struct game *game);
void game_set_llist_bomb(struct game *game, struct llist_bomb *llist_bomb);

struct monster *game_get_monster (struct game *game);
void game_set_monster (struct game *game , struct monster *monster);

struct llist_monster *game_get_llist_monster(struct game *game);
void game_set_llist_monster(struct game *game, struct llist_monster *llist_monster);

// Display the game on the screen
void game_display(struct game* game);

// update
int game_update(struct game* game);

//
void game_set_current(struct game* game, int n);

void game_set_jeu(struct game* game, int x);

void game_set_map(struct game* game, struct map* map);

int game_get_current(struct game* game);

static short input_keyboard(struct game* game);

#endif /* GAME_H_ */
