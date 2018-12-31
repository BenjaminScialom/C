#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "[PG110] Bombeirb 2016-2017"

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       40

// Size (# of pixels) of banner
#define LINE_HEIGHT	4
#define BANNER_HEIGHT	40

// Time management
#define DEFAULT_GAME_FPS 30

// map size
#define MAP_WIDTH  12
#define MAP_HEIGHT 12

//Initialisation game
#define NB_BOMB  3
#define NUMERO_CURRENT  0
#define NUMERO_LEVELS   7

//Initialisation player
#define NB_KEYS  0
#define NB_HEART 2

//Initialisation bomb
#define NB_RANGE  1

enum direction {
	NORTH = 0,
	SOUTH, // 1
	WEST, // 2
	EAST //3
};

enum explosion {
	WAIT = 0,
	CLOCK,
	BOOM
};

enum jeu {
	MENU = 0,
	PLAY,
	GAME_OVER,
	GAME_WIN

};

#endif /* CONSTANT */
