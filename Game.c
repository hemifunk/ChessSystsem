#include "Game.h"
#include <stdlib.h>
#include <assert.h>

struct Game_t
{
	int id;
	int player1_id;
	int player2_id;
	int time;
	Winner winner;
};

Game gameCreate()
{
	Game new_game = malloc(sizeof(struct Game_t));

	if (new_game == NULL)
		return NULL;

	new_game->id = -1;
	new_game->player1_id = -1;
	new_game->player2_id = -1;
	new_game->time = 0;
	new_game->winner = UNINITIALIZED;

	return new_game;
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

	Game copy = gameCreate();

	copy->id = game->id;
	copy->player1_id = game->player1_id;
	copy->player2_id = game->player2_id;
	copy->time = game->time;
	copy->winner = game->winner;

	return copy;
}

int gameGetPlayerId(Game game, Player_Index player)
{
    if(game == NULL)
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

void gameSetPlayerId(Game game, int id, Player_Index player)
{
	assert(game != NULL);

    if(game == NULL)
    {
		return;
	}

	if (player == PLAYER_1)
    {
		game->player1_id = id;
    }
	else
    {
		game->player2_id = id;
    }
}

int gameGetId(Game game)
{
    if(game == NULL)
    {
		return -1;
	}

	return game->id;
}

void gameSetId(Game game, int id)
{
	assert(game != NULL);
	assert(id >= 0);

	game->id = id;
}

Winner gameGetWinner(Game game)
{
    if(game == NULL)
    {
		return UNINITIALIZED;
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
    if(game == NULL)
    {
		return -1;
	}

	return game->time;
}

void gameSetTime(Game game, int time)
{
	assert(game != NULL);
	assert(time >= 0);

	game->time = time;
}