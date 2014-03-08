#ifndef GLOBAL_INCLUDE
#define GLOBAL_INCLUDE

#define NUMBER_RANDOM_BLOCKS 5
#define NUMBER_BLOCK_PER_ENTITY 4
#define NUMBER_BLOCK_EXIST 7
#define FPS 60
#define PASS_LEVEL 12

/* Define Sizes */
#define SCREEN_WIDTH 515
#define SCREEN_HEIGHT 450
#define SIZE_X_INFO 200
#define SIZE_X_GAME 240
#define SIZE_X_NEXT 200
#define SIZE_SPACE 25
#define SIZE_Y_INFO 175
#define SIZE_BLOCK 20

typedef enum Context
{
	FIRSTMENU,
	GAME,
	QUIT
}Context;

typedef enum TypeBlock{
	L,
	J,
	Z,
	S,
	T,
	I,
	O
}TypeBlock;

typedef enum Direction{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE
}Direction;

#endif
