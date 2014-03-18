#include "menu.h"

Menu* initMenu(SDL_Renderer *windowRenderer)
{
	Menu* menu;
	menu = (Menu*) malloc(sizeof(Menu));

	SDL_Color color = {200, 200, 200};
	menu->quitColor = color;
	menu->startColor = color;

	/* Open the font */
	menu->font = TTF_OpenFont("DejaVuSansMono.ttf", 24);
	if(menu->font == NULL)
		printf("An error happened when open DejaVuSansMono.ttf \n");

	/* Make the label */
	SDL_Surface* quitSurface = TTF_RenderText_Solid(menu->font, "Quit", color);
	SDL_Surface* startSurface = TTF_RenderText_Solid(menu->font, "Start", color);
	menu->quitLabel = SDL_CreateTextureFromSurface(windowRenderer, quitSurface);
	menu->startLabel = SDL_CreateTextureFromSurface(windowRenderer, startSurface);

	/* Make the rect of the label. */
	menu->quitRect.x = (SCREEN_WIDTH - quitSurface->w)/2;
	menu->quitRect.y = (SCREEN_HEIGHT + quitSurface->h)/2;
	menu->quitRect.w = quitSurface->w;
	menu->quitRect.h = quitSurface->h;

	menu->startRect.x = (SCREEN_WIDTH - startSurface->w)/2;
	menu->startRect.y = (SCREEN_HEIGHT - 3*startSurface->h)/2;
	menu->startRect.w = startSurface->w;
	menu->startRect.h = startSurface->h;

	/* Free the surface used for creating the texture label. */
	SDL_FreeSurface(quitSurface);
	SDL_FreeSurface(startSurface);

	return menu;
}

void firstMenu(Menu *menu, SDL_Renderer *windowRenderer, Context *context)
{
	/* Get the mouse position */
	SDL_Rect mouseRect;
	mouseRect.w = 0;
	mouseRect.h = 0;
	Uint32 mouseState = SDL_GetMouseState(&mouseRect.x, &mouseRect.y);

	/* Update the color of the labels */
	updateLabelColor(menu, windowRenderer, &mouseRect);

	/* Draw the label on the renderer */
	SDL_RenderCopy(windowRenderer, menu->quitLabel, NULL, &menu->quitRect);
	SDL_RenderCopy(windowRenderer, menu->startLabel, NULL, &menu->startRect);

	/* Test the click events. */
	if(rectCollision(mouseRect, menu->quitRect) && (mouseState & SDL_BUTTON(1)))
		*context = QUIT;
	else if(rectCollision(mouseRect, menu->startRect) && (mouseState & SDL_BUTTON(1)))
		*context = GAME;
}

int pauseMenu(Menu *menu, SDL_Window *window)
{

}

void clearMenu(Menu *menu)
{
	SDL_DestroyTexture(menu->quitLabel);
	SDL_DestroyTexture(menu->startLabel);
	TTF_CloseFont(menu->font);
	free(menu);
}

void updateLabelColor(Menu *menu, SDL_Renderer *windowRenderer, SDL_Rect *mouseRect)
{
	/* The color for the labels. */
	SDL_Color inColor = {255, 255, 255};
	SDL_Color outColor = {200, 200, 200};

	if(rectCollision(*mouseRect, menu->quitRect))
	{
		if(menu->quitColor.r != 255)
		{
			menu->quitColor = inColor;
			SDL_DestroyTexture(menu->quitLabel);
			SDL_Surface* quitSurface = TTF_RenderText_Solid(menu->font, "Quit", menu->quitColor);
			menu->quitLabel = SDL_CreateTextureFromSurface(windowRenderer, quitSurface);
			SDL_FreeSurface(quitSurface);
		}
	}

	else
	{
		if(menu->quitColor.r != 200)
		{
			menu->quitColor = outColor;
			SDL_DestroyTexture(menu->quitLabel);
			SDL_Surface* quitSurface = TTF_RenderText_Solid(menu->font, "Quit", menu->quitColor);
			menu->quitLabel = SDL_CreateTextureFromSurface(windowRenderer, quitSurface);
			SDL_FreeSurface(quitSurface);
		}
	}

	if(rectCollision(*mouseRect, menu->startRect))
	{
		if(menu->startColor.r != 255)
		{
			menu->startColor = inColor;
			SDL_DestroyTexture(menu->startLabel);
			SDL_Surface *startSurface = TTF_RenderText_Solid(menu->font, "Start", menu->startColor);
			menu->startLabel = SDL_CreateTextureFromSurface(windowRenderer, startSurface);
			SDL_FreeSurface(startSurface);
		}
	}

	else
	{
		if(menu->startColor.r != 200)
		{
			menu->startColor = outColor;
			SDL_DestroyTexture(menu->startLabel);
			SDL_Surface *startSurface = TTF_RenderText_Solid(menu->font, "Start", menu->startColor);
			menu->startLabel = SDL_CreateTextureFromSurface(windowRenderer, startSurface);
			SDL_FreeSurface(startSurface);
		}
	}
}
