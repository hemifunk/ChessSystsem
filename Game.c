#include "game.h"
#include "chessSystem.h"
#include "list.h"
#include "map.h"
#include "player.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

struct Game_t
{
	int first_player;
	int second_player;
	int play_time;
	Winner winner;
	bool player_removed;
};

ChessResult gameValidate(int first_player, int second_player, int time)
{
	if (first_player <= 0 || second_player <= 0 || first_player == second_player)
	{
		return CHESS_INVALID_ID;
	}

	if (time <= 0)
	{
		return CHESS_INVALID_PLAY_TIME;
	}

	return CHESS_SUCCESS;
}

Game gameCreate(int first_player, int second_player, int time, Winner winner)
{
	if (gameValidate(first_player, second_player, time) != CHESS_SUCCESS)
	{
		return NULL;
	}

	Game game = malloc(sizeof(struct Game_t));

	if (game == NULL)
	{
		return NULL;
	}

	game->first_player = first_player;
	game->second_player = second_player;
	game->play_time = time;
	game->winner = winner;
	game->player_removed = false;

	return game;
}

void gameDestroy(Game game)
{
	free(game);
}

Game gameCopy(Game game)
{
	if (game == NULL)
	{
		return NULL;
	}

	Game copy = gameCreate(game->first_player, game->second_player, game->play_time, game->winner);
	copy->player_removed = game->player_removed;

	return copy;
}

int gameGetPlayerId(Game game, Player_Index player)
{
	if (game == NULL)
	{
		return -1;
	}

	if (player == PLAYER_1)
	{
		return game->first_player;
	}
	else
	{
		return game->second_player;
	}
}

Winner gameGetWinner(Game game)
{
	assert(game != NULL);

	if (game == NULL)
	{
		return DRAW;
	}

	return game->winner;
}

void gameSetWinner(Game game, Winner winner)
{
	assert(game != NULL);

	game->winner = winner;
}

int gameGetTime(Game game)
{
	if (game == NULL)
	{
		return -1;
	}

	return game->play_time;
}

bool gameHasPlayerRemoved(Game game)
{
	assert(game != NULL);

	return game->player_removed;
}