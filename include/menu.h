#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "global.h"

typedef struct Menu{
	SDL_Texture *quitLabel;
	SDL_Texture *startLabel;
	SDL_Color quitColor;
	SDL_Color startColor;
	SDL_Rect quitRect;
	SDL_Rect startRect;
	TTF_Font *font;
}Menu;

Menu* initMenu(SDL_Renderer *windowRenderer);
void firstMenu(Menu *menu, SDL_Renderer *windowRenderer, Context *context);
int pauseMenu(Menu *menu, SDL_Window *window);
void clearMenu(Menu *menu);

void updateLabelColor(Menu *menu, SDL_Renderer *windowRenderer, SDL_Rect *mouseRect);
