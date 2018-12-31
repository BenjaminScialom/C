#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>
#include <bomb.h>
#include <game.h>

struct player;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number);
void   player_free(struct player* player);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

void player_set_x(struct player* player,int x);
void player_set_y(struct player* player,int y);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Give the number of key that player can have.
int player_get_nb_key(struct player* player);

void player_set_nb_keys(struct player* player,int key);

// Set, Increase, Decrease the number of life that player can have.
int player_get_nb_life(struct player* player);
void player_inc_nb_life(struct player* player) ;
void player_dec_nb_life(struct player* player) ;

void player_set_nb_life(struct player * player, int life) ;

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

void player_set_nb_bomb(struct player* player,int bomb);

// Set, Increase, Decrease the number of range that player can have.
int  player_get_nb_range(struct player * player);
void player_inc_nb_range(struct player * player);
void player_dec_nb_range(struct player * player);

void player_set_nb_range(struct player* player,int range);

// Set, Get, Increase, Decrease the wound of the player
void player_set_wounds(struct player * player, int zeros);
int  player_get_wounds(struct player * player);
void player_inc_wounds(struct player * player);
void player_dec_wounds(struct player * player);

// Load the player position from the map
void player_from_map(struct player* player, struct map* map);


// Move the player according to the current direction
int player_move( struct game* game ) ;

// Display the player on the screen
void player_display(struct player* player);

#endif /* PLAYER_H_ */
