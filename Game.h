#ifndef GAME_H_
#define GAME_H
#include "chessSystem.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct game_t* Game;

typedef enum {
    PLAYER_1, PLAYER_2
} PLAYER_INDEX;

/** Allocates a new game */
Game gameCreate();

/** Frees an existing game*/
void gameDestroy(Game game);

/**Copies a given game*/
Game gameCopy(Game game);

/** Returns the id of player*/
int gameGetPlayerId(Game game, PLAYER_INDEX player);

/**Changes the id of player*/
void gameSetPlayerId(Game game, int id, PLAYER_INDEX player);

/** Returns the id of game*/
int gameGetId(Game game);

/**Changes the id of game */
void gameSetId(Game game, int id);

/**Returns the winner */
Winner getWinner(Game game);

/**Changes the  winner */
void gameSetWinner(Game game, Winner winner);

/**Returns the winner */
int gameGetTime(Game game);

/**Changes the  winner*/
void gameSetTime(Game game, int time);

#endif