#ifndef _GAME_H
#define _GAME_H

#include "chessSystem.h"
#include "map.h"

typedef enum
{
	PLAYER_OUT_OF_MEMORY,
	PLAYER_NULL_ARGUMENT,
	PLAYER_ALREADY_REMOVED,
	PLAYER_SUCCESS
} PlayerResult;

typedef enum
{
	PLAYER_1,
	PLAYER_2
} Player_Index;

typedef struct Game_t* Game;

ChessResult gameValidate(int first_player, int second_player, int time);

/** Allocates a new game */
Game gameCreate(int first_player, int second_player, int time, Winner winner);

/** Frees an existing game*/
void gameDestroy(Game game);

/**Copies a given game*/
Game gameCopy(Game game);

/** Returns the id of player*/
int gameGetPlayerId(Game game, Player_Index player);

/**Returns the winner */
Winner gameGetWinner(Game game);

void gameSetWinner(Game game, Winner winner);

int gameGetTime(Game game);

PlayerResult gameGetNewWinner(Map chess_players, Map tournament_players, Game game, int player_id);

void gameRemovePlayer(Game game, int player);

bool gameHasPlayerRemoved(Game game);

#endif