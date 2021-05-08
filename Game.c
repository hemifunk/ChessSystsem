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

static void setResult(ChessResult* result, ChessResult value)
{
	if (result == NULL)
	{
		return;
	}

	*result = value;
}

Game gameCreate(int id, int player1_id, int player2_id, int time, Winner winner, ChessResult* result)	
{
	if (id <= 0 || player1_id <= 0 || player2_id <= 0)
	{
		setResult(result, CHESS_INVALID_ID);
		return NULL;
	}

	if(time <= 0)
	{
		setResult(result, CHESS_INVALID_PLAY_TIME);
		return NULL;
	}

	Game game = malloc(sizeof(struct Game_t));

	if (game == NULL)
	{
		setResult(result, CHESS_OUT_OF_MEMORY);
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

int gameGetId(Game game)
{
    if(game == NULL)
    {
		return -1;
	}

	return game->id;
}

Winner gameGetWinner(Game game)
{
    if(game == NULL)
    {
		return UNINITIALIZED;
	}

	return game->winner;
}

int gameGetTime(Game game)
{
    if(game == NULL)
    {
		return -1;
	}

	return game->time;
}