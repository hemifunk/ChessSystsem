#include "Game.h"
#include <assert.h>
#include <stdlib.h>

struct Game_t
{
	int first_player;
	int second_player;
	int play_time;
	Winner winner;
};

Game gameCreate(int first_player, int second_player, int time, Winner winner)
{
	if (first_player <= 0 || second_player <= 0 || time <= 0 || first_player == second_player)
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

int gameGetTime(Game game)
{
	if (game == NULL)
	{
		return -1;
	}

	return game->play_time;
}