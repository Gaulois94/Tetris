#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "global.h"
#include "menu.h"
#include "game.h"

int init(SDL_Window **window);
void fpsManager(int* timer);
void clear();

#if defined(__WIN32__)
int WinMain()
#else
int main()
#endif
{
	int quit = 0;
	int timer = 0;

	srand(time(NULL));
	Context context = FIRSTMENU;
	SDL_Event event;
	SDL_Window *window = NULL;
	SDL_Surface *windowSurface = NULL;
	SDL_Renderer *windowRenderer = NULL;
	Menu *menu = NULL;
	Game *game = NULL;
	
	/* Init the window or exit if failure. */
	if(!init(&window))
		return EXIT_FAILURE;

	/* Make the window renderer and the window surface */
	windowRenderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_TARGETTEXTURE);
	if(windowRenderer == NULL)
		printf("%s \n", SDL_GetError());

	windowSurface = SDL_GetWindowSurface(window);

	/* Make all context of the game. */
	menu = initMenu(windowRenderer);
	game = initGame(windowRenderer);

	while(!quit)
	{
		/* event loop. */
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				quit = 1;

			else if(event.type == SDL_KEYDOWN)
			{
				const Uint8* keyPressed = SDL_GetKeyboardState(NULL);
				if(context == GAME)
				{
					/* Handle the translation of the blocks */
					if(event.key.keysym.scancode == SDL_SCANCODE_LEFT || event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_DOWN)
					{
						if(event.key.keysym.scancode == SDL_SCANCODE_LEFT)
							moveCurrentBlock(game, LEFT);
						if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
							moveCurrentBlock(game, RIGHT);
						if(event.key.keysym.scancode == SDL_SCANCODE_DOWN)
							moveCurrentBlock(game, DOWN);
					}

					/* Handle the rotation of the blocks */
					else if(event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_x)
					{
						/* Check the direction the user want to move */
						Direction direction = NONE;
						if(keyPressed[SDL_SCANCODE_RIGHT])
							direction = RIGHT;
						else if(keyPressed[SDL_SCANCODE_LEFT])
							direction = LEFT;

						if(event.key.keysym.sym == SDLK_w)
						{
							rotateCurrentBlock(game, 1, direction);
						}
						else
							rotateCurrentBlock(game, -1, direction);
					}

					else if(event.key.keysym.scancode == SDL_SCANCODE_SPACE)
					{
						putDownCurrentBlock(game);
					}
				}
			}
		}

		SDL_RenderClear(windowRenderer);

		/* Call the correct context */
		if(context == FIRSTMENU)
			firstMenu(menu, windowRenderer, &context);
		else if(context == QUIT)
			quit = 1;
		else if(context == GAME)
			updateGame(game);

		SDL_RenderPresent(windowRenderer);
		fpsManager(&timer);
	}

	/* Clear the allocated memory */
	clearMenu(menu);
	clearGame(game);
	SDL_DestroyRenderer(windowRenderer);
	SDL_FreeSurface(windowSurface);
	SDL_DestroyWindow(window);
	clear();

	return 0;
}

int init(SDL_Window** window)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
		return 0;
	}

	*window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(*window == NULL)
	{
		printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
		return 0;
	}

	if(TTF_Init() == -1)
    {
        return 0;    
    }
	return 1;
}

void fpsManager(int* timer)
{
	SDL_Delay(fmax(0, (float) (1.0/FPS) * 1000 - (SDL_GetTicks() - *timer)));
	*timer = SDL_GetTicks();
}

void clear()
{
	TTF_Quit();
	SDL_Quit();
}
