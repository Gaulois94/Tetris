#ifndef GAME_INCLUDE
#define GAME_INCLUDE

#include <SDL2/SDL.h>
#include <stdio.h>
#include <global.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>
#include "records.h"

typedef struct Block{
	SDL_Texture *texture[4];
	SDL_Rect rect[4];
	TypeBlock type;
}Block;

typedef struct Game{
	Block** blockArray;
	Block* currentBlock;
	int numberEntity;

	TypeBlock nextBlock[5];
	int rotationBlock[5];

	SDL_Rect gameViewport;
	SDL_Rect scoreViewport;
	SDL_Rect nextViewport;
	SDL_Rect recordViewport;

	TTF_Font *font;
	SDL_Texture *nextLabel;
	SDL_Texture *scoreLabel;
	SDL_Texture *levelLabel;

	SDL_Texture *highScoreLabel;
	SDL_Texture *highLevelLabel;
	SDL_Texture *highLinesLabel;
	SDL_Texture *highPseudoLabel;


	SDL_TimerID timerLevel;
	SDL_Renderer *windowRenderer;
	int level;
	int score;
	int linesDeleted;
	int changeScore;
	int changeLevel;

	int end;
}Game;

Game* initGame(SDL_Renderer *renderer);
void updateGame(Game* game);
void mainWindow(Game* game);
void scoreWindow(Game* game);
void recordWindow(Game* game);

void updateLevel(Game* game);
void updateScore(Game* game);

void changeBlock(Game* game);
void deleteLines(Game* game);

Uint32 moveCurrentBlockCallback(Uint32 interval, void* param);
void moveCurrentBlock(Game* game, Direction direction);
int moveBlock(Game* game, Block* bloc, int value, Direction direction);

Block* makeBlock(Game* game, TypeBlock typeBlock, int rotation);
void makeRotation(Game* game, Block* block, int rotation, Direction direction);
void rotateCurrentBlock(Game* game, int rotation, Direction direction);

TypeBlock getTypeRandomBlock();
void getCenterBlock(Block* block, int* x, int* y);
void getMaxMinLocation(Block* bloc, int* xMin, int* yMin, int* xMax, int* yMax);
SDL_Rect getRectBlock(Block* block);
int getBlockEntityEnable(Block* block);

int getBlockCollision(Block* block1, Block* block2);
void getHighScore(Game* game);
void setHighScore(Game* game);

void putNewBlock(Game* game);
void removeBlock(Game* game, int index);
int checkEndGame(Game* game);
void clearGame(Game* game);
void clearBlock(Block* block);

#endif
