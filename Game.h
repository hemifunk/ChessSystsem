#ifndef GAME_H_
#define GAME_H_

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

/**Changes the id of player*/
void gameSetPlayerId(Game game, int id, Player_Index player);

/** Returns the id of game*/
int gameGetId(Game game);

/**Changes the id of game */
void gameSetId(Game game, int id);

/**Returns the winner */
Winner getWinner(Game game);

/**Changes the  winner */
void gameSetWinner(Game game, Winner winner);

int gameGetTime(Game game);

void gameSetTime(Game game, int time);

#endif