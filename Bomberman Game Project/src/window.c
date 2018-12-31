#include <SDL/SDL_image.h> // IMG_Load
#include <assert.h>

#include <constant.h>
#include <misc.h>
#include <sprite.h>

SDL_Surface* window;

void window_create(int width, int height) {
	assert(width > 0 && height > 0);

	SDL_WM_SetCaption(WINDOW_NAME, NULL );
	window = SDL_SetVideoMode(width, height, 0, // If bits-per-pixel is 0, it is treated as the current display bits per pixel.
			SDL_HWSURFACE);

	if (window == NULL ) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

void window_display_image(SDL_Surface* sprite, int x, int y) {
	assert(window);
	assert(sprite);

	SDL_Rect place;
	place.x = x;
	place.y = y;
	if(sprite == sprite_get_pause()) {
		SDL_SetAlpha(sprite, SDL_SRCALPHA, 128);
	}
	SDL_BlitSurface(sprite, NULL, window, &place); //Collage de l'image "sprite" a l'endroit indiqu� par place.
}

void window_clear() { //Rafraichit/Nettoie fenetre (blanche):
	assert(window);
	SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));
}

void window_refresh() { //remet a jour aux yeux de l'utilisateur:
	assert(window);
	SDL_Flip(window);
}
