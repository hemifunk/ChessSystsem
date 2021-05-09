#ifndef _GAME_H
#define _GAME_H

#include "chessSystem.h"

typedef enum
{
	PLAYER_1,
	PLAYER_2
} Player_Index;

typedef struct Game_t* Game;

/** Allocates a new game */
Game gameCreate(int id, int player1_id, int player2_id, int time, Winner winner);

/** Frees an existing game*/
void gameDestroy(Game game);

/**Copies a given game*/
Game gameCopy(Game game);

/** Returns the id of player*/
int gameGetPlayerId(Game game, Player_Index player);

/** Returns the id of game*/
int gameGetId(Game game);

/**Returns the winner */
Winner getWinner(Game game);

int gameGetTime(Game game);

#endif