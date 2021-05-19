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

//todo: use this func to remove player from a game
void gameRemovePlayer(Game game, int player)
{
	if (game == NULL || player <= 0)
	{
		return;
	}

	if (player == game->first_player)
	{
		game->winner = SECOND_PLAYER;
		game->player_removed = true;
	}
	else if (player == game->second_player)
	{
		game->winner = FIRST_PLAYER;
		game->player_removed = true;
	}
}

PlayerResult gameGetNewWinner(Map chess_players, Map tournament_players, Game game, int player_id)
{
	assert(chess_players != NULL);
	assert(tournament_players != NULL);

	if (game == NULL)
	{
		return PLAYER_OUT_OF_MEMORY;
	}

	int id_first = gameGetPlayerId(game, PLAYER_1);
	int id_second = gameGetPlayerId(game, PLAYER_2);

	Winner winner_of_game = gameGetWinner(game);

	Winner new_winner;
	int id_new_winner;

	if ((winner_of_game == FIRST_PLAYER || (winner_of_game == DRAW)) && player_id == id_first)
	{
		new_winner = SECOND_PLAYER;
		id_new_winner = id_second;
	}
	else if ((winner_of_game == SECOND_PLAYER || (winner_of_game == DRAW)) && player_id == id_second)
	{
		new_winner = FIRST_PLAYER;
		id_new_winner = id_first;
	}

	if (mapContains(chess_players, &id_new_winner) == false)
	{
		mapRemove(chess_players, &player_id);
		mapRemove(tournament_players, &player_id);

		return PLAYER_ALREADY_REMOVED;
	}

	if ((id_first == player_id && (winner_of_game == FIRST_PLAYER || winner_of_game == DRAW)) ||
		(id_second == player_id && (winner_of_game == SECOND_PLAYER || winner_of_game == DRAW)))
	{
		gameSetWinner(game, new_winner);

		Player player1 = mapGet(chess_players, &id_new_winner);
		playerSetNumWins(player1, playerGetNumWins(player1) + 1);

		Player player2 = mapGet(tournament_players, &id_new_winner);
		playerSetNumWins(player2, playerGetNumWins(player2) + 1);
	}

	return PLAYER_SUCCESS;
}