#include <SDL/SDL_image.h>
#include <assert.h>

#include <sprite.h>
#include <misc.h>

// Sprites general
#define MAP_CASE        "sprite/wood_box.png"
#define MAP_KEY			"sprite/key.png"
#define MAP_DOOR_OPENED	"sprite/door_opened.png"
#define MAP_DOOR_CLOSED	"sprite/door_closed.png"

// Scenery elements
#define MAP_STONE		"sprite/stone.png"
#define MAP_TREE        "sprite/tree.png"

// Sprites of Banner
#define BANNER_LINE		"sprite/banner_line.png"
#define BANNER_LIFE		"sprite/banner_life.png"
#define BANNER_BOMB		"sprite/bomb3.png"
#define BANNER_RANGE	"sprite/banner_range.png"
#define BANNER_0		"sprite/banner_0.jpg"
#define BANNER_1		"sprite/banner_1.jpg"
#define BANNER_2		"sprite/banner_2.jpg"
#define BANNER_3		"sprite/banner_3.jpg"
#define BANNER_4		"sprite/banner_4.jpg"
#define BANNER_5		"sprite/banner_5.jpg"
#define BANNER_6		"sprite/banner_6.jpg"
#define BANNER_7		"sprite/banner_7.jpg"
#define BANNER_8		"sprite/banner_8.jpg"
#define BANNER_9		"sprite/banner_9.jpg"
#define BANNER_DOOR		"sprite/door.png"


// Sprites of Bombs
#define BOMB_TTL1       "sprite/bomb1.png"
#define BOMB_TTL2       "sprite/bomb2.png"
#define BOMB_TTL3       "sprite/bomb3.png"
#define BOMB_TTL4       "sprite/bomb4.png"

// Sprites explosion
#define EXPLOSION "sprite/explosion.png"

// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"
#define IMG_BONUS_LIFE			  "sprite/banner_life.png"

// Sprites of Players
#define PLAYER_LEFT     "sprite/player_left.png"
#define PLAYER_UP       "sprite/player_up.png"
#define PLAYER_RIGHT    "sprite/player_right.png"
#define PLAYER_DOWN     "sprite/player_down.png"

// Sprites of Monsters *Graou*
#define MONSTER_LEFT     "sprite/monster_left.png"
#define MONSTER_UP       "sprite/monster_up.png"
#define MONSTER_RIGHT    "sprite/monster_right.png"
#define MONSTER_DOWN     "sprite/monster_down.png"

// Sprite of Princess *kawaïï*
#define PRINCESS		"sprite/bomberwoman.png"

// Images illustratifs
#define PAUSE			"sprite/pause.png"
#define WIN				"sprite/win.jpg"
#define MENU			"sprite/menu.jpg"
#define GAME_OVER			"sprite/game_over.png"

// banner
SDL_Surface* numbers[10];
SDL_Surface* banner_life;
SDL_Surface* banner_bomb;
SDL_Surface* banner_range;
SDL_Surface* banner_line;
SDL_Surface *banner_door;

// map
SDL_Surface* box;
SDL_Surface* goal;
SDL_Surface* key;
SDL_Surface* door_opened;
SDL_Surface* door_closed;
SDL_Surface* stone;
SDL_Surface* tree;

// bonus
#define NB_BONUS 6
SDL_Surface* bonus[NB_BONUS + 1];

// player
SDL_Surface* player_img[5];

//Le monster
SDL_Surface* monster_img[5];

//La bomb
SDL_Surface* bomb_img[4];

//L'explosion
SDL_Surface* explosion_img[1];

// Princess
SDL_Surface* princess;

// Images
SDL_Surface* pause;
SDL_Surface* win;
SDL_Surface* menu;
SDL_Surface* game_over;


static void banner_load() { //Fct qui charge les images du banner:
	// numbers imgs
	numbers[0] = load_image(BANNER_0);
	numbers[1] = load_image(BANNER_1);
	numbers[2] = load_image(BANNER_2);
	numbers[3] = load_image(BANNER_3);
	numbers[4] = load_image(BANNER_4);
	numbers[5] = load_image(BANNER_5);
	numbers[6] = load_image(BANNER_6);
	numbers[7] = load_image(BANNER_7);
	numbers[8] = load_image(BANNER_8);
	numbers[9] = load_image(BANNER_9);

	// other banner sprites
	banner_life = load_image(BANNER_LIFE);
	banner_bomb = load_image(BANNER_BOMB);
	banner_range = load_image(BANNER_RANGE);
	banner_line = load_image(BANNER_LINE);
	banner_door = load_image( BANNER_DOOR );

}

static void banner_unload() { //Fct qui lib�re l'espace allou�e aux images du banner pr�c�demment charg�es:
	// numbers imgs
	for (int i = 0; i < 10; i++) {
		SDL_FreeSurface(numbers[i]);
	}

	// other banner sprites
	SDL_FreeSurface(banner_life);
	SDL_FreeSurface(banner_bomb);
	SDL_FreeSurface(banner_range);
	SDL_FreeSurface(banner_life);

}

static void map_load() { //Fct qui charge les images du jeu:
	// Sprite loading
	tree = load_image(MAP_TREE);
	box = load_image(MAP_CASE);
	key = load_image(MAP_KEY);
	stone = load_image(MAP_STONE);
	door_opened = load_image(MAP_DOOR_OPENED);
	door_closed = load_image(MAP_DOOR_CLOSED);
}

static void map_unload() { //Fct qui lib�re l'espace allou�e aux images du jeu pr�c. charg�es:
	SDL_FreeSurface(tree);
	SDL_FreeSurface(box);
	SDL_FreeSurface(goal);
	SDL_FreeSurface(key);
	SDL_FreeSurface(stone);
	SDL_FreeSurface(door_opened);
	SDL_FreeSurface(door_closed);
}

static void bonus_load() { //Fct qui charge les images bonus:
	bonus [0] = NULL;
	bonus [BONUS_BOMB_RANGE_INC] = load_image(IMG_BONUS_BOMB_RANGE_INC);
	bonus [BONUS_BOMB_RANGE_DEC] = load_image(IMG_BONUS_BOMB_RANGE_DEC);
	bonus [BONUS_BOMB_NB_INC] = load_image(IMG_BONUS_BOMB_NB_INC);
	bonus [BONUS_BOMB_NB_DEC] = load_image(IMG_BONUS_BOMB_NB_DEC);
	bonus [BONUS_LIFE] = load_image(IMG_BONUS_LIFE);
}

static void bonus_unload() { //Fct qui lib�re l'espace allou�e aux images bonus pr�c. charg�es:
	for (int i = 0; i < NB_BONUS; i++)
		if(bonus[i])
			SDL_FreeSurface(bonus[i]);
}


static void bomb_load () { //New function ! -> pr chargement des bombes avec meches differentes
	bomb_img[0] = load_image(BOMB_TTL1);
	bomb_img[1] = load_image(BOMB_TTL2);
	bomb_img[2] = load_image(BOMB_TTL3);
	bomb_img[3] = load_image(BOMB_TTL4);

}

static void explosion_load(){
	explosion_img[0]= load_image(EXPLOSION);
}

static void bomb_unload() { //Fct qui libere l'espace allouee aux images bombes pr�c. charg�es:
	for (int i = 0; i < 4; i++)
		if(bomb_img[i])
			SDL_FreeSurface(bomb_img[i]);
}

static void explosion_unload(){
	SDL_FreeSurface(explosion_img[0]);
}

static void player_load() {
	player_img[WEST] = load_image(PLAYER_LEFT);
	player_img[EAST] = load_image(PLAYER_RIGHT);
	player_img[NORTH] = load_image(PLAYER_UP);
	player_img[SOUTH] = load_image(PLAYER_DOWN);
}

static void player_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(player_img[i]);
}

static void monster_load() {
	monster_img[WEST] = load_image(MONSTER_LEFT);
	monster_img[EAST] = load_image(MONSTER_RIGHT);
	monster_img[NORTH] = load_image(MONSTER_UP);
	monster_img[SOUTH] = load_image(MONSTER_DOWN);
}

static void monster_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(monster_img[i]);
}

static void princess_load() {
	princess = load_image(PRINCESS);
}

static void princess_unload() {
	SDL_FreeSurface(princess);
}

static void images_load() {
	pause = load_image(PAUSE);
	win = load_image(WIN);
	menu = load_image(MENU);
	game_over = load_image(GAME_OVER);
}

static void images_unload() {
	SDL_FreeSurface(pause);
	SDL_FreeSurface(win);
	SDL_FreeSurface(menu);
	SDL_FreeSurface(game_over);
}

void sprite_load() { //Fonction qui charge tout le jeu � l'aide des mini-fct pr chaque cas:
	map_load();
	bonus_load();
	banner_load();
	player_load();
	bomb_load(); //rajout
	monster_load(); //""
	princess_load();//""
	images_load();//""
	explosion_load();
}

void sprite_free() {
	map_unload();
	bonus_unload();
	banner_unload();
	player_unload();
	bomb_unload(); //rajout
	monster_unload(); //""
	princess_unload();//""
	images_unload();//""
	explosion_unload();
}

SDL_Surface* sprite_get_number(short number) {
	assert(number >= 0 && number <= 9);
	return numbers[number]; //retourne l'image correspondant au nbre de l'item restant.
}

SDL_Surface* sprite_get_player(enum direction direction) {
	assert(player_img[direction]);
	return player_img[direction]; //retourne l'image de la direction prise par player.
}

SDL_Surface* sprite_get_monster( enum direction direction ) {
	assert(monster_img[direction]);
	return monster_img[direction]; //retourne l'image de la direction prise par player.
}

SDL_Surface* sprite_get_banner_life() {
	assert(banner_life);
	return banner_life; //retourne l'image symbole de vie =coeur.
}

SDL_Surface* sprite_get_banner_bomb() {
	assert(banner_bomb);
	return banner_bomb; //retourne l'image bombe.
}

SDL_Surface* sprite_get_banner_line() {
	assert(banner_line);
	return banner_line;
}

SDL_Surface* sprite_get_banner_range() {
	assert(banner_range);
	return banner_range;
}

SDL_Surface* sprite_get_banner_door() { //New function for banner level:
	assert(banner_door);
	return banner_door;
}

SDL_Surface* sprite_get_bonus(enum bonus_type bonus_type) {
	assert(bonus[bonus_type]);
	return bonus[bonus_type]; //retourne image du bonus correspondant.
}


SDL_Surface* sprite_get_princess() {
	assert(princess);
	return princess;
}

SDL_Surface* sprite_get_tree() {
	assert(tree);
	return tree;
}

SDL_Surface* sprite_get_box() {
	assert(box);
	return box;
}

SDL_Surface* sprite_get_key() {
	assert(key);
	return key;
}

SDL_Surface* sprite_get_stone() {
	assert(stone);
	return stone;
}

SDL_Surface* sprite_get_door_opened() {
	assert(door_opened);
	return door_opened;
}

SDL_Surface* sprite_get_door_closed() {
	assert(door_closed);
	return door_closed;
}


SDL_Surface* sprite_get_timer_bomb (int i) { //New fct -> retourne l'image de la bombe avec longueur mèche indiqué avec l'indice:
	assert(bomb_img[i]);
	return bomb_img[i];
}

SDL_Surface* sprite_get_explosion(){
	assert(explosion_img[0]);
	return explosion_img[0];
}


SDL_Surface* sprite_get_pause() {
	assert(pause);
	return pause;
}

SDL_Surface* sprite_get_win() {
	assert(win);
	return win;
}

SDL_Surface* sprite_get_menu() {
	assert(menu);
	return menu;
}

SDL_Surface* sprite_get_over() {
	assert(game_over);
	return game_over;
}


