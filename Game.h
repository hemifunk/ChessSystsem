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
int getIdPlayer(Game game, PLAYER_INDEX player);

/**Changes the id of player*/
void setIdPlayer(Game game, int id, PLAYER_INDEX player);

/** Returns the id of game*/
int getIdGame(Game game);

/**Changes the id of game */
void setIdGame(Game game, int id);

/**Returns the winner */
Winner getWinner(Game game);

/**Changes the  winner */
void setWinner(Game game, Winner winner);

/**Returns the winner */
int getTimeGame(Game game);

/**Changes the  winner*/
void setTimeGame(Game game, int time);

#endif