#include "game.h"

Game* initGame(SDL_Renderer *renderer)
{
	Game* game = (Game*) malloc(sizeof(Game));
	game->windowRenderer = renderer;

	game->numberEntity = 0;
	game->currentBlock = NULL;
	game->blockArray = (Block**) malloc(0*sizeof(Block));

	game->level = 1;
	game->score = 0;
	game->changeScore = 1;
	game->changeLevel = 1;

	/* Set the viewport for each part of the game. */
	game->gameViewport.x = SIZE_X_INFO + 2*SIZE_SPACE;
	game->gameViewport.y = SIZE_SPACE;
	game->gameViewport.w = SIZE_X_GAME;
	game->gameViewport.h = SCREEN_HEIGHT - 2*SIZE_SPACE;

	game->recordViewport.x = SIZE_SPACE;
	game->recordViewport.y = SIZE_SPACE;
	game->recordViewport.w = SIZE_X_INFO;
	game->recordViewport.h = SIZE_Y_INFO;

	game->scoreViewport.x = SIZE_SPACE;
	game->scoreViewport.y = 3*SIZE_SPACE + SIZE_Y_INFO;
	game->scoreViewport.w = SIZE_X_INFO;
	game->scoreViewport.h = SIZE_Y_INFO;

	game->nextViewport.x = 3*SIZE_SPACE + SIZE_X_INFO + SIZE_X_GAME;
	game->nextViewport.y = SIZE_SPACE;
	game->nextViewport.w = SIZE_X_NEXT;
	game->nextViewport.h = SCREEN_HEIGHT - 2*SIZE_SPACE;

	int i=0;
	for(i=0; i < NUMBER_RANDOM_BLOCKS; i++)
	{
		game->nextBlock[i] = getTypeRandomBlock();
		game->rotationBlock[i] = rand()%4;
		game->nextBlockTexture[i] = makeBlock(game, game->nextBlock[i], game->rotationBlock[i]);
	}

	game->font = TTF_OpenFont("DejaVuSansMono.ttf", 15);
	SDL_Color color = {255, 255, 255};
	SDL_Surface* nextSurface = TTF_RenderText_Solid(game->font, "Next", color);
	game->nextLabel = SDL_CreateTextureFromSurface(renderer, nextSurface);

	game->linesDeleted = 0;
	game->scoreLabel = NULL;
	game->levelLabel = NULL;

	getHighScore(game);

	game->timerLevel;

	game->end = 0;
	changeBlock(game);;
	return game;
}

/* Update the Game. */
void updateGame(Game* game)
{
	if(game->end)
		return;

	/* Old renderer's values */
	SDL_Rect oldRenderViewport;
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(game->windowRenderer, &r, &g, &b, &a);
	SDL_RenderGetViewport(game->windowRenderer, &oldRenderViewport);

	/* Set the color for drawing stuff in the renderer only in the game */
	SDL_SetRenderDrawColor(game->windowRenderer, 50, 0, 200, 255);

	/* Update the level */
	updateLevel(game);

	/* Update the modules of the game */
	mainWindow(game);
	scoreWindow(game);
	recordWindow(game);
	nextWindow(game);

	/* Reinit the renderer */
	SDL_SetRenderDrawColor(game->windowRenderer, r, g, b, a);
	SDL_RenderSetViewport(game->windowRenderer, &oldRenderViewport);
}

void mainWindow(Game* game)
{
	/* Set the Viewport for this part of game */
	SDL_RenderSetViewport(game->windowRenderer, &game->gameViewport);

	int i;
	int j;

	/* set a grid in the map */

	/* Old drawColor value */
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(game->windowRenderer, &r, &g, &b, &a);

	SDL_SetRenderDrawColor(game->windowRenderer, 150, 50, 220, 255);
	/* Draw vertical lines */
	for(i=0; i < SIZE_X_GAME / SIZE_BLOCK; i++)
		SDL_RenderDrawLine(game->windowRenderer, i*SIZE_BLOCK, 0,
				i*SIZE_BLOCK, game->gameViewport.h);

	/* Draw horizontal lines */
	for(i=0; i < game->gameViewport.h / SIZE_BLOCK; i++)
		SDL_RenderDrawLine(game->windowRenderer, 0, i*SIZE_BLOCK,
				game->gameViewport.w, i*SIZE_BLOCK);

	/* Reinit the renderDrawColor */
	SDL_SetRenderDrawColor(game->windowRenderer, r, g, b, a);


	/* Draw the block on the window */
	for(i=0; i < game->numberEntity; i++)
	{
		if(game->blockArray[i])
			for(j=0; j < 4; j++)
			{
				if(game->blockArray[i]->texture[j])
					SDL_RenderCopy(game->windowRenderer, game->blockArray[i]->texture[j], NULL, &game->blockArray[i]->rect[j]);
			}
	}

	/* Draw the outline rect of the Render */
	SDL_RenderDrawRect(game->windowRenderer, NULL);	
	return;	
}

void scoreWindow(Game* game)
{
	/* Update the score */
	updateScore(game);

	/* Set the Viewport for this part of game */
	SDL_RenderSetViewport(game->windowRenderer, &game->scoreViewport);

	SDL_Rect scoreRect;
	SDL_Rect levelRect;

	int w,h;
	/* Get the score rect */
	SDL_QueryTexture(game->scoreLabel, NULL, NULL, &w, &h);
	scoreRect.x = (game->scoreViewport.w - w)/2;
	scoreRect.y = (game->scoreViewport.h - 2*h)/2;
	scoreRect.w = w;
	scoreRect.h = h;

	/* Get the level rect */
	SDL_QueryTexture(game->levelLabel, NULL, NULL, &w, &h);
	levelRect.x = (game->scoreViewport.w - w)/2;
	levelRect.y = (game->scoreViewport.h)/2;
	levelRect.w = w;
	levelRect.h = h;

	/* Draw the labels */
	SDL_RenderCopy(game->windowRenderer, game->scoreLabel, NULL, &scoreRect);
	SDL_RenderCopy(game->windowRenderer, game->levelLabel, NULL, &levelRect);

	/* Draw the outline rect of the Render */
	SDL_RenderDrawRect(game->windowRenderer, NULL);	
}

void recordWindow(Game* game)
{
	/* Set the Viewport for this part of game */
	SDL_RenderSetViewport(game->windowRenderer, &game->recordViewport);

	SDL_Rect scoreRect;
	SDL_Rect levelRect;
	SDL_Rect pseudoRect;
	SDL_Rect linesRect;

	int w,h;

	/* Get the rect */
	SDL_QueryTexture(game->highScoreLabel, NULL, NULL, &w, &h);
	scoreRect.x = (game->scoreViewport.w - w)/2;
	scoreRect.y = (game->scoreViewport.h - 4*h)/2;
	scoreRect.w = w;
	scoreRect.h = h;

	SDL_QueryTexture(game->highLevelLabel, NULL, NULL, &w, &h);
	levelRect.x = (game->scoreViewport.w - w)/2;
	levelRect.y = (game->scoreViewport.h - 2*h)/2;
	levelRect.w = w;
	levelRect.h = h;

	SDL_QueryTexture(game->highLinesLabel, NULL, NULL, &w, &h);
	linesRect.x = (game->scoreViewport.w - w)/2;
	linesRect.y = (game->scoreViewport.h)/2;
	linesRect.w = w;
	linesRect.h = h;

	SDL_QueryTexture(game->highPseudoLabel, NULL, NULL, &w, &h);
	pseudoRect.x = (game->scoreViewport.w - w)/2;
	pseudoRect.y = (game->scoreViewport.h + 2*h)/2;
	pseudoRect.w = w;
	pseudoRect.h = h;

	/* Draw the labels */
	SDL_RenderCopy(game->windowRenderer, game->highScoreLabel, NULL, &scoreRect);
	SDL_RenderCopy(game->windowRenderer, game->highLevelLabel, NULL, &levelRect);
	SDL_RenderCopy(game->windowRenderer, game->highLinesLabel, NULL, &linesRect);
	SDL_RenderCopy(game->windowRenderer, game->highPseudoLabel, NULL, &pseudoRect);

	/* Draw the outline rect of the Render */
	SDL_RenderDrawRect(game->windowRenderer, NULL);	
}

void nextWindow(Game* game)
{
	SDL_RenderSetViewport(game->windowRenderer, &game->nextViewport);

	/* Get the outline rect for the next one block */
	const int spaceOutlineBlock = 2;
	SDL_Rect nextOneOutline;
	nextOneOutline.y = 5 - spaceOutlineBlock;
	nextOneOutline.x = (SIZE_X_NEXT - 4*SIZE_BLOCK)/2 - spaceOutlineBlock;
	nextOneOutline.w = 4*SIZE_BLOCK + 2*spaceOutlineBlock;
	nextOneOutline.h = 4*SIZE_BLOCK + 2*spaceOutlineBlock;

	/* Put the Block in the correct place on the screen */
	int i;
	for(i=0; i < NUMBER_RANDOM_BLOCKS; i++)
	{
		int x, y;
		SDL_Rect blockRect = getRectBlock(game->nextBlockTexture[i]);
		/* If the block we move is the current block */
		if(i==0)
		{
			x = nextOneOutline.x + (nextOneOutline.w - blockRect.w)/2;
			y = nextOneOutline.y + (nextOneOutline.h - blockRect.h)/2;
		}

		else if(i%2 == 1)
		{
			x = (SIZE_X_NEXT - 5*SIZE_SPACE)/2;
			y = ((i+1)/2)*(4*SIZE_BLOCK + 5) + SCREEN_HEIGHT/2 - 6*SIZE_SPACE;
		}

		else
		{
			x = (SIZE_X_NEXT + 2*SIZE_SPACE + 5)/2;
			y = ((i+1)/2)*(4*SIZE_BLOCK + 5) + SCREEN_HEIGHT/2 - 6*SIZE_SPACE;
		}

		/* We move the sub block */
		int j;
		for(j=0; j < NUMBER_BLOCK_PER_ENTITY; j++)
		{
			game->nextBlockTexture[i]->rect[j].x -= (blockRect.x - x);
			game->nextBlockTexture[i]->rect[j].y -= (blockRect.y - y);
		}
	}

	for(i=0; i < NUMBER_RANDOM_BLOCKS; i++)
	{
		int j;
		for(j=0; j < NUMBER_BLOCK_PER_ENTITY; j++)
			SDL_RenderCopy(game->windowRenderer, game->nextBlockTexture[i]->texture[j], NULL, &(game->nextBlockTexture[i]->rect[j]));
	}

	SDL_RenderDrawRect(game->windowRenderer, NULL);	
}

void updateLevel(Game* game)
{
	if(game->changeLevel)
	{
		/* Update the speed */
		game->level = fmin(10, game->linesDeleted / PASS_LEVEL + 1);
		SDL_RemoveTimer(game->timerLevel);
		game->timerLevel = SDL_AddTimer(1000/game->level, moveCurrentBlockCallback, game);

		/* Update the label */
		char level[12];

		/* Convert int to char */
		sprintf(level, "Level : %d", game->level);

		/* Free the old texture */
		if(game->levelLabel)
			SDL_DestroyTexture(game->levelLabel);

		SDL_Color color = {255, 255, 255};

		SDL_Surface* levelSurface = TTF_RenderText_Solid(game->font, level, color);
		game->levelLabel = SDL_CreateTextureFromSurface(game->windowRenderer, levelSurface);

		SDL_FreeSurface(levelSurface);

		game->changeLevel = 0;
	}
}

void updateScore(Game* game)
{
	if(game->changeScore)
	{
		/* Update the label */
		char score[20];

		/* Convert int to char */
		sprintf(score, "Score : %d", game->score);

		/* Free the old texture */
		if(game->scoreLabel)
			SDL_DestroyTexture(game->scoreLabel);

		/* Make the new texture */
		SDL_Color color = {255, 255, 255};
		SDL_Surface* scoreSurface = TTF_RenderText_Solid(game->font, score, color);
		game->scoreLabel = SDL_CreateTextureFromSurface(game->windowRenderer, scoreSurface);
		SDL_FreeSurface(scoreSurface);

		game->changeScore = 0;
	}
}

void changeBlock(Game* game)
{
	checkEndGame(game);
	deleteLines(game);
	game->currentBlock = NULL;
	putNewBlock(game);
}

void deleteLines(Game* game)
{
	/* Get the bock position in the map */
	int position[SIZE_X_GAME/SIZE_BLOCK][(SCREEN_HEIGHT-2*SIZE_SPACE)/SIZE_BLOCK];
	Block* matchBlock[SIZE_X_GAME/SIZE_BLOCK][(SCREEN_HEIGHT-2*SIZE_SPACE)/SIZE_BLOCK];
	int indexSubBlock[SIZE_X_GAME/SIZE_BLOCK][(SCREEN_HEIGHT-2*SIZE_SPACE)/SIZE_BLOCK];

	int numberLinesToDel = 0;
	int lineMoved[(SCREEN_HEIGHT-2*SIZE_SPACE)/SIZE_BLOCK];

	/* Initialisation */
	int i=0;
	int j=0;

	for(j=0; j < game->gameViewport.h/SIZE_BLOCK; j++)
	{
		for(i=0; i < game->gameViewport.w/SIZE_BLOCK; i++)
		{
			/* 0 means "not found" and 1 means "found" */
			position[i][j] = 0;
			matchBlock[i][j] = NULL;
			indexSubBlock[i][j] = -1;
		}
		lineMoved[j] = 0;
	}

	for(i=0; i < game->numberEntity; i++)
	{
		for(j=0; j < NUMBER_BLOCK_PER_ENTITY; j++)
		{
			int xIndex = game->blockArray[i]->rect[j].x / SIZE_BLOCK;
			int	yIndex=game->blockArray[i]->rect[j].y / SIZE_BLOCK;
			if(game->blockArray[i]->texture[j] != NULL)
			{
				position[xIndex][yIndex] = 1;
				matchBlock[xIndex][yIndex] = game->blockArray[i];
				indexSubBlock[xIndex][yIndex] = j;
			}
		}
	}

	/* Check if there are full lines and delete them */
	for(j=0; j < game->gameViewport.h/SIZE_BLOCK; j++)
	{
		int lineComplete = 1;
		{
			for(i=0; i < game->gameViewport.w/SIZE_BLOCK; i++)
			if(position[i][j] == 0)
			{
				lineComplete = 0;
				break;
			}	
		}
		if(lineComplete)
		{
			lineMoved[j]=1;
			numberLinesToDel++;
			for(i=0; i  < game->gameViewport.w/SIZE_BLOCK; i++)
			{
				SDL_DestroyTexture(matchBlock[i][j]->texture[indexSubBlock[i][j]]);
				matchBlock[i][j]->texture[indexSubBlock[i][j]] = NULL;
			}
		}
	}

	if(numberLinesToDel == 0)
		return;

	/* Test if every block are available */
	for(i=0; i < game->numberEntity; i++)
	{
		int available = 0;
		int j=0;
		for(j=0; j < NUMBER_BLOCK_PER_ENTITY; j++)
		{
			if(game->blockArray[i]->texture[j] != NULL)
			{
				available = 1;
				break;
			}
		}

		if(!available)
			removeBlock(game, i);
	}

	/* Move the lines to the bottom */
	int l;
	for(l=game->gameViewport.h/SIZE_BLOCK - 1; l >=0; l--)
	{
		if(lineMoved[l])
		{
			/* We move each block above the line number l */
			for(j=l-1; j >= 0; j--)
			{
				for(i=0; i < game->gameViewport.w/SIZE_BLOCK; i++)
				{
					if(matchBlock[i][j] != NULL)
					{
						matchBlock[i][j]->rect[indexSubBlock[i][j]].y += SIZE_BLOCK;
					}
				}
			}
		}
	}

	/* We change the score and the level */
	int oldLevel = game->level;

	/* If we made a tetris */
	if(numberLinesToDel == 4)
		game->score += 400;
	game->score += numberLinesToDel * 100;
	game->level = (int)(game->score / (PASS_LEVEL*100)) + 1;

	if(oldLevel != game->level)
		game->changeLevel = 1;
	game->changeScore = 1;

	game->linesDeleted += numberLinesToDel;
}

/* param is Game* struct */
Uint32 moveCurrentBlockCallback(Uint32 interval, void* param)
{
	const Uint8* keyPressed = SDL_GetKeyboardState(NULL);
	if(keyPressed[SDL_SCANCODE_DOWN])
		return interval;

	Game* game = (Game*) param;

	/* Get the direction of the block */
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if(game->currentBlock != NULL)
	{
		if(moveBlock(game, game->currentBlock, 1, DOWN))
			changeBlock(game);
	}

	return interval;
}

void moveCurrentBlock(Game* game, Direction direction)
{
	if(game->currentBlock != NULL && moveBlock(game, game->currentBlock, 1, direction))
		changeBlock(game);
}

void putDownCurrentBlock(Game* game)
{
	Block* currentBlock = game->currentBlock;
	while(currentBlock == game->currentBlock)
		moveCurrentBlock(game, DOWN);
}

int moveBlock(Game* game, Block* block, int nbCase, Direction direction)
{
	if(block == NULL)
		return;

	int xMod = 0;
	int yMod = 0;

	SDL_Rect rectBlock = getRectBlock(block);

	switch(direction)
	{
		case LEFT:
			xMod = -1;
			break;
		case RIGHT:
			xMod = 1;
			break;
		case DOWN:
			yMod = 1;
			break;
		case UP:
			yMod = -1;
			break;
	}

	int j = 0;
	int i;
	int k;
	int collision = 0;

	for(j=1; j <= nbCase; j++)
	{
		rectBlock.x += xMod*SIZE_BLOCK;
		rectBlock.y += yMod*SIZE_BLOCK;

		if(rectBlock.x < 0 || rectBlock.x + rectBlock.w > game->gameViewport.w ||
			   	direction == UP && rectBlock.y < 0 || rectBlock.y + rectBlock.h > game->gameViewport.h)
		{
			collision = 1;
			goto endMoveBlock;
		}

		for(k=0; k < game->numberEntity-1; k++)
		{
			/* If 2 Blocks are in collisions */
			if(rectCollision(getRectBlock(game->blockArray[k]), rectBlock))
			{
				/* We test for each subBlock available if there is collision */
				int b1 = 0;
				for(b1=0; b1 < NUMBER_BLOCK_PER_ENTITY; b1++)
				{
					if(game->blockArray[k]->texture[b1])
					{
						int b2=0;
						for(b2=0; b2 < NUMBER_BLOCK_PER_ENTITY; b2++)
						{
							if(block->texture[b2])
							{
								/* We need to test the collision if the block have moved */
								SDL_Rect subBlockRect = block->rect[b2];
								subBlockRect.x += xMod * SIZE_BLOCK;
								subBlockRect.y += yMod * SIZE_BLOCK;
								if(rectCollision(game->blockArray[k]->rect[b1], subBlockRect))
								{
									collision = 1;
									goto endMoveBlock;
								}
							}
						}
					}
				}
			}
		}

		for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
		{
			block->rect[i].x += xMod*SIZE_BLOCK;
			block->rect[i].y += yMod*SIZE_BLOCK;
		}
	}

	endMoveBlock:
	if(collision)
	{
		if(direction == DOWN)
			return 1;
	}

	return 0;
}

Block* makeBlock(Game* game, TypeBlock typeBlock, int rotation) 
{
	Block* block;
	block = (Block*) malloc(sizeof(Block));

	SDL_Surface* surface = SDL_CreateRGBSurface(0, SIZE_BLOCK, SIZE_BLOCK, 24, 0, 0, 0, 0);
	Uint32 color;

	/* set coordinates for each blocks */
	switch(typeBlock)
	{
		case L:
			color = SDL_MapRGB(surface->format, 0, 0, 0xff);

			block->rect[0].x = 0;
			block->rect[0].y = 0;

			block->rect[1].x = 0;
			block->rect[1].y = SIZE_BLOCK;

			block->rect[2].x = 0;
			block->rect[2].y = 2*SIZE_BLOCK;

			block->rect[3].x = SIZE_BLOCK;
			block->rect[3].y = 2*SIZE_BLOCK;

			break;

		case J:
			color = SDL_MapRGB(surface->format, 0, 0xff, 0xff);

			block->rect[0].x = SIZE_BLOCK;
			block->rect[0].y = 0;

			block->rect[1].x = SIZE_BLOCK;
			block->rect[1].y = SIZE_BLOCK;

			block->rect[2].x = SIZE_BLOCK;
			block->rect[2].y = 2*SIZE_BLOCK;

			block->rect[3].x = 0;
			block->rect[3].y = 2*SIZE_BLOCK;
			break;
		case Z:
			color = SDL_MapRGB(surface->format, 0xff, 0, 0);

			block->rect[0].x = 0;
			block->rect[0].y = 0;

			block->rect[1].x = SIZE_BLOCK;
			block->rect[1].y = 0;

			block->rect[2].x = SIZE_BLOCK;
			block->rect[2].y = SIZE_BLOCK;

			block->rect[3].x = 2*SIZE_BLOCK;
			block->rect[3].y = SIZE_BLOCK;

			break;
		case S:
			color = SDL_MapRGB(surface->format, 0, 0xff, 0);

			block->rect[0].x = SIZE_BLOCK;
			block->rect[0].y = 0;

			block->rect[1].x = 2*SIZE_BLOCK;
			block->rect[1].y = 0;

			block->rect[2].x = 0;
			block->rect[2].y = SIZE_BLOCK;

			block->rect[3].x = SIZE_BLOCK;
			block->rect[3].y = SIZE_BLOCK;

			break;
		case T:
			color = SDL_MapRGB(surface->format, 0xff, 0, 0xff);

			block->rect[0].x = SIZE_BLOCK;
			block->rect[0].y = 0;

			block->rect[1].x = 0;
			block->rect[1].y = SIZE_BLOCK;

			block->rect[2].x = SIZE_BLOCK;
			block->rect[2].y = SIZE_BLOCK;

			block->rect[3].x = 2*SIZE_BLOCK;
			block->rect[3].y = SIZE_BLOCK;
			break;
		case I:
			color = SDL_MapRGB(surface->format, 0xff, 0xff, 0xff);

			block->rect[0].x = 0;
			block->rect[0].y = 0;

			block->rect[1].x = 0;
			block->rect[1].y = SIZE_BLOCK;

			block->rect[2].x = 0;
			block->rect[2].y = 2*SIZE_BLOCK;

			block->rect[3].x = 0;
			block->rect[3].y = 3*SIZE_BLOCK;
			break;
		case O:
			color = SDL_MapRGB(surface->format, 0xff, 0xff, 0);

			block->rect[0].x = 0;
			block->rect[0].y = 0;

			block->rect[1].x = SIZE_BLOCK;
			block->rect[1].y = 0;

			block->rect[2].x = 0;
			block->rect[2].y = SIZE_BLOCK;

			block->rect[3].x = SIZE_BLOCK;
			block->rect[3].y = SIZE_BLOCK;
	}

	/* Set the color of the surface */
	SDL_FillRect(surface, NULL, color);

	/* Create Texture with this surface and set the size of the blocks */
	int i = 0;
	for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
	{
		block->texture[i] = SDL_CreateTextureFromSurface(game->windowRenderer, surface);
		block->rect[i].w = SIZE_BLOCK;
		block->rect[i].h = SIZE_BLOCK;
	}

	SDL_FreeSurface(surface);

	/* Turn this block */
	makeRotation(game, block, rotation, LEFT);

	return block;
}

void rotateCurrentBlock(Game* game, int rotation, Direction direction)
{
	if(game->currentBlock)
		makeRotation(game, game->currentBlock, rotation, direction);
}

/* position : 1 or -1       direction : LEFT or RIGHT or NONE. */
void makeRotation(Game* game, Block* block, int rotation, Direction direction)
{
	int i=0;
	SDL_Rect initRect = getRectBlock(block);

	/* Save the old rect */
	SDL_Rect oldRect[4];
	for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
		oldRect[i] = block->rect[i];

	/* The center of the Block */
	int cx;
	int cy;
	getCenterBlock(block, &cx, &cy);

	/* While the user want a rotation */
	i=0;
	while(i < abs(rotation))
	{
		int j;
		/* We make a rotation for each block */
		for(j=0; j < NUMBER_BLOCK_PER_ENTITY; j++)
		{
			/* We save the location before modification */
			int x = block->rect[j].x;
			int y = block->rect[j].y;

			if(signbit(rotation))
			{
				block->rect[j].x = (cx + cy - y - SIZE_BLOCK);	
				block->rect[j].y = x + cy - cx;
			}

			else
			{
				block->rect[j].x = y + cx - cy;
				block->rect[j].y = cx + cy - x - SIZE_BLOCK;
			}
		}
		i+=1;
	}

	/* If the new position is not position correctly, we move him */
	int signe = 0;
	if(direction == RIGHT)
		signe = 1;
	else if(direction == LEFT)
		signe = -1;

	SDL_Rect blockRect = getRectBlock(block);
	for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
	{
		block->rect[i].x += -blockRect.x + initRect.x + signe*SIZE_BLOCK;
		block->rect[i].y += -blockRect.y + initRect.y;
	}


	/* We check the collision with the border of the screen */
	blockRect = getRectBlock(block);
	if(blockRect.x < 0)
		for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
			block->rect[i].x -= blockRect.x;

	else if(blockRect.x + blockRect.w > game->gameViewport.w)
		for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
			block->rect[i].x -= blockRect.x + blockRect.w - game->gameViewport.w;

	else if(blockRect.y + blockRect.h > game->gameViewport.h)
		for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
			block->rect[i].y -= blockRect.y + blockRect.h - game->gameViewport.h;
	

	/* We check if there aren't collisions between blocks*/
	int j;
	int reboot = 0;
	int test = 0;

	/* Get the direction to move the block for escape the collision */
	Direction direCollision = NONE;

	while(test < 2 && reboot == 0)
	{
		int haveCollision = 0;
		for(i=0; i < game->numberEntity-1; i++)
		{
			int collision = 0;
			SDL_Rect selectRect = getRectBlock(game->blockArray[i]);
			blockRect = getRectBlock(block);

			if(getBlockCollision(game->blockArray[i], block))
				collision = 1;

			if(collision)
			{
				signe = 1;
				if(initRect.x <= selectRect.x)
					signe = -1;

				/* If the block is in collision between two blocks */
				if(direCollision == RIGHT && signe == -1 || direCollision == LEFT && signe == 1)
					reboot = 1;

				direCollision = LEFT;
				if(signe == 1)
					direCollision = RIGHT;

				/* Move the block until the end of the collision */
				while(getBlockCollision(game->blockArray[i], block))
				{
					for(j=0; j < NUMBER_BLOCK_PER_ENTITY; j++)
						block->rect[j].x += signe*SIZE_BLOCK;
				}

				/* Or need to move but there is the limit */
				if(direCollision == LEFT && blockRect.x < 0 || direCollision == RIGHT && blockRect.x + blockRect.w > game->gameViewport.w)
					reboot = 1;

				/* Then we reboot */
				if(reboot)
					break;

			}

			haveCollision = collision | haveCollision;
		}
		
		test += 1;
		if(!haveCollision || reboot)
			break;
	}

	/* If the rotation is canceled */
	if(reboot)
	{
		for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
			block->rect[i] = oldRect[i];
		return;
	}
}

TypeBlock getTypeRandomBlock()
{
	return rand() % (NUMBER_BLOCK_EXIST);
}

/* Get the center of the Block */
void getCenterBlock(Block* block, int* x, int* y)
{
	/* Get the min and max location of the block */
	int xMin,yMin,xMax,yMax;
	getMaxMinLocation(block, &xMin, &yMin, &xMax, &yMax);

	*x = (xMin + xMax)/2;
	*y = (yMin + yMax)/2;
}

/* Get the min and max location of blocks */
void getMaxMinLocation(Block* block, int* xMin, int* yMin, int* xMax, int* yMax)
{
	SDL_Rect enableRect[NUMBER_BLOCK_PER_ENTITY];

	int i;
	int j=0;
	for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
	{
		if(block->texture[i])
		{
			enableRect[j] = block->rect[i];
			j++;
		}
	}

	if(j==1)
		enableRect[1] = enableRect[0];

	*xMin=fmin(enableRect[0].x, enableRect[1].x);
	*xMax=fmax(enableRect[0].x + enableRect[0].w, enableRect[1].x + enableRect[1].w);

	*yMin=fmin(enableRect[0].y, enableRect[1].y);
	*yMax=fmax(enableRect[0].y + enableRect[0].h, enableRect[1].y + enableRect[1].h);

	for(i=2; i < j; i++)
	{
		*xMin=fmin(*xMin, enableRect[i].x);
		*xMax=fmax(*xMax, enableRect[i].x + enableRect[i].w);

		*yMin=fmin(*yMin, enableRect[i].y);
		*yMax=fmax(*yMax, enableRect[i].y + enableRect[i].h);
	}
}

/* Get the Rect of the Block */
SDL_Rect getRectBlock(Block* block)
{
	int xMin,yMin,xMax,yMax;
	getMaxMinLocation(block, &xMin, &yMin, &xMax, &yMax);

	SDL_Rect rect;
	rect.x = xMin;
	rect.y = yMin;
	rect.w = xMax - xMin;
	rect.h = yMax - yMin;

	return rect;
}

int getBlockEntityEnable(Block* block)
{
	int i =0;
	int nbEnable = 0;

	for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
	{
		if(block->texture[i] != NULL)
			nbEnable++;
	}

	return nbEnable;
}

/* Put a new Block on the Game */
void putNewBlock(Game* game)
{
	/* Delete the old Block */
	TypeBlock typeBlock = game->nextBlock[0];
	int rotationBlock = game->rotationBlock[0];

	clearBlock(game->nextBlockTexture[0]);

	/* Move element in the table. */
	int i;
	for(i=1; i < NUMBER_RANDOM_BLOCKS; i++)
	{
		game->nextBlock[i-1] = game->nextBlock[i];
		game->rotationBlock[i-1] = game->rotationBlock[i];
		game->nextBlockTexture[i-1] = game->nextBlockTexture[i];
	}


	/* Create a new random element */
	game->nextBlock[NUMBER_RANDOM_BLOCKS-1] = getTypeRandomBlock();
	game->rotationBlock[NUMBER_RANDOM_BLOCKS-1] = rand()%5;
	game->nextBlockTexture[i-1] = makeBlock(game, game->nextBlock[NUMBER_RANDOM_BLOCKS-1], game->rotationBlock[NUMBER_RANDOM_BLOCKS-1]);

	/* Create a entity */
	game->numberEntity += 1;
	game->blockArray = (Block**) realloc(game->blockArray, (game->numberEntity) * sizeof(Block));
	game->blockArray[game->numberEntity-1] = makeBlock(game, typeBlock,rotationBlock);

	game->currentBlock = game->blockArray[game->numberEntity -1];
	SDL_Rect blockRect = getRectBlock(game->currentBlock);
	for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
	{
		game->currentBlock->rect[i].x -= (blockRect.x - SIZE_X_GAME/2 + ((blockRect.w / SIZE_BLOCK) / 2) * SIZE_BLOCK);
		game->currentBlock->rect[i].y -= (blockRect.y + blockRect.h);
	}
}

int getBlockCollision(Block* block1, Block* block2)
{
	int i,j;

	for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
	{
		for(j=0; j < NUMBER_BLOCK_PER_ENTITY; j++)
		{
			if(rectCollision(block1->rect[i], block2->rect[j]))
				return 1;
		}
	}

	return 0;
}

void getHighScore(Game* game)
{
	int highLevel, highScore, highLines;
	char highPseudo[20]; 

	getRecords(&highScore, &highLevel, &highLines, highPseudo);	

	SDL_Color color = {255, 255, 255};

	char highScoreString[30];
	sprintf(highScoreString, "Score : %d", highScore);
	SDL_Surface* scoreSurface = TTF_RenderText_Solid(game->font, highScoreString, color);
	game->highScoreLabel = SDL_CreateTextureFromSurface(game->windowRenderer, scoreSurface);
	SDL_FreeSurface(scoreSurface);

	char highLevelString[30];
	sprintf(highLevelString, "Level : %d", highLevel);
	SDL_Surface* levelSurface = TTF_RenderText_Solid(game->font, highLevelString, color);
	game->highLevelLabel = SDL_CreateTextureFromSurface(game->windowRenderer, levelSurface);
	SDL_FreeSurface(levelSurface);

	char highLinesString[30];
	sprintf(highLinesString, "Lines : %d", highLines);
	SDL_Surface* linesSurface = TTF_RenderText_Solid(game->font, highLinesString, color);
	game->highLinesLabel = SDL_CreateTextureFromSurface(game->windowRenderer, linesSurface);
	SDL_FreeSurface(linesSurface);

	char highPseudoString[55];
	sprintf(highPseudoString, "Pseudo : %s", highPseudo);
	SDL_Surface* pseudoSurface = TTF_RenderText_Solid(game->font, highPseudoString, color);
	game->highPseudoLabel = SDL_CreateTextureFromSurface(game->windowRenderer, pseudoSurface);
	SDL_FreeSurface(pseudoSurface);

	printf("score : %s   lines : %s     level : %s      pseudo : %s \n", highScoreString, highLinesString, highLevelString, highPseudoString);
}

void setHighScore(Game* game)
{
	int highScore;
	getRecords(&highScore, NULL, NULL, NULL);

	if(game->score > highScore)
		setRecords(game->score, game->level, game->linesDeleted, "");
}

/* Remove a block in the Game. */
void removeBlock(Game* game, int index)
{
	if(index < 0 || index >= game->numberEntity)
		return;

	/* Destroy the block */
	clearBlock(game->blockArray[index]);

	/* Move the blocks in the table */
	int i=0;
	for(i=index; i<game->numberEntity-1; i++)
		game->blockArray[i] = game->blockArray[i+1];

	/* We realloc the good size */
	game->numberEntity -= 1;
	game->blockArray = (Block**) realloc(game->blockArray, (game->numberEntity) * sizeof(Block));
}

/* Check if the game is over */
int checkEndGame(Game* game)
{
	if(game->currentBlock == NULL)
		return game->end;
	SDL_Rect currentRect = getRectBlock(game->currentBlock);
	if(currentRect.y < 0)
	{
		game->end = 1;
		setHighScore(game);
	}
	return game->end;
}

/* Clear all the allocated memory */
void clearGame(Game* game)
{
	game->currentBlock = NULL;
	/* Clear the blocks */
	int i=0;
	for(i=0; i < game->numberEntity; i++)
		clearBlock(game->blockArray[i]);
	for(i=0; i < NUMBER_RANDOM_BLOCKS; i++)
		clearBlock(game->nextBlockTexture[i]);

	/* Clear textures */
	SDL_DestroyTexture(game->nextLabel);
	SDL_DestroyTexture(game->scoreLabel);
	SDL_DestroyTexture(game->levelLabel);
	SDL_DestroyTexture(game->highScoreLabel);
	SDL_DestroyTexture(game->highLevelLabel);
	SDL_DestroyTexture(game->highLinesLabel);
	SDL_DestroyTexture(game->highPseudoLabel);

	TTF_CloseFont(game->font);
	SDL_RemoveTimer(game->timerLevel);

	free(game->blockArray);
	free(game);
}

/* Clear the texture of the block */
void clearBlock(Block* block)
{
	int i;
	if(block != NULL)
	{
		for(i=0; i < NUMBER_BLOCK_PER_ENTITY; i++)
		{
			if(block->texture[i] != NULL)
				SDL_DestroyTexture(block->texture[i]);
		}

		free(block);
	}
}
