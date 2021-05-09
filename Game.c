#include "Game.h"
#include <assert.h>
#include <stdlib.h>

struct Game_t
{
	int id; //todo: remove, not neccecery
	int player1_id;
	int player2_id;
	int time;
	Winner winner;
};

Game gameCreate(int id, int player1_id, int player2_id, int time, Winner winner)
{
	if (id <= 0 || player1_id <= 0 || player2_id <= 0 || time <= 0)
	{
		return NULL;
	}

	Game game = malloc(sizeof(struct Game_t));

	if (game == NULL)
	{
		return NULL;
	}

	game->id = id;
	game->player1_id = player1_id;
	game->player2_id = player2_id;
	game->time = time;
	game->winner = winner;

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

	Game copy = gameCreate(game->id, game->player1_id, game->player2_id, game->time, game->winner);

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
		return game->player1_id;
	}
	else
	{
		return game->player2_id;
	}
}

int gameGetId(Game game)
{
	if (game == NULL)
	{
		return -1;
	}

	return game->id;
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

int gameGetTime(Game game)
{
	if (game == NULL)
	{
		return -1;
	}

	return game->time;
}