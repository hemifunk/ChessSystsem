#include "Player.h"
#include "Game.h"
#include "chessSystem.h"
#include "list.h"
#include "map.h"
#include <assert.h>
#include <stdlib.h>

struct Player_t
{
	int id;
	int num_wins;
	int num_loses;
	int num_draws;
	int total_play_time;
	int num_games;
	double level;
};

Player playerCreate(int id)
{
	if (id <= 0)
	{
		return NULL;
	}

	Player player = malloc(sizeof(struct Player_t));

	if (player == NULL)
	{
		return NULL;
	}

	player->id = id;
	player->num_draws = 0;
	player->num_games = 0;
	player->num_loses = 0;
	player->num_wins = 0;
	player->total_play_time = 0;
	player->level = 0.0;
	return player;
}

void playerDestroy(Player player)
{
	free(player);
}

Player playerCopy(Player player)
{
	if (player == NULL)
	{
		return NULL;
	}

	Player copy = playerCreate(player->id);

	if (copy == NULL)
	{
		return NULL;
	}

	copy->num_wins = player->num_wins;
	copy->num_loses = player->num_loses;
	copy->num_draws = player->num_draws;
	copy->total_play_time = player->total_play_time;
	copy->num_games = player->num_games;
	copy->level = player->level;
	return copy;
}

int playerGetId(Player player)
{
	if (player == NULL)
	{
		return 0;
	}

	return player->id;
}

int playerGetNumWins(Player player)
{
	if (player == NULL)
	{
		return 0;
	}

	return player->num_wins;
}

void playerSetNumWins(Player player, int num_wins)
{
	assert(player != NULL);
	assert(num_wins >= 0);

	player->num_wins = num_wins;
}

int playerGetNumLoses(Player player)
{
	if (player == NULL)
	{
		return 0;
	}

	return player->num_loses;
}

void playerSetNumLoses(Player player, int num_loses)
{
	assert(player != NULL);
	assert(num_loses >= 0);

	player->num_loses = num_loses;
}

int playerGetNumDraws(Player player)
{
	if (player == NULL)
	{
		return 0;
	}

	return player->num_draws;
}

void playerSetNumDraws(Player player, int num_draws)
{
	assert(player != NULL);
	assert(num_draws >= 0);

	player->num_draws = num_draws;
}

int playerGetTotalPlayTime(Player player)
{
	if (player == NULL)
	{
		return 0;
	}

	return player->total_play_time;
}

void playerSetTotalGameTime(Player player, int time)
{
	assert(player != NULL);
	assert(time >= 0);

	player->total_play_time = time;
}

int playerGetNumGames(Player player)
{
	if (player == NULL)
	{
		return 0;
	}

	return player->num_games;
}

void playerSetNumGames(Player player, int num_games)
{
	assert(player != NULL);
	assert(num_games >= 0);

	player->num_games = num_games;
}

bool playerGetLevel(Player player)
{
	assert(player != NULL);
	return player->level;
}

void playerSetLevel(Player player, double updated_level)
{
	assert(player != NULL);

	player->level = updated_level;
}

PlayerResult getNewWinner(Map players_at_chess, Map players_at_tournament, List games_of_current, int* index_game, int player_id)
{
	Winner new_winner;
	int id_new_winner;
	Game current_game = listGet(games_of_current, *index_game);
	if (current_game != NULL)
	{
		int id_first = gameGetPlayerId(current_game, PLAYER_1);
		int id_second = gameGetPlayerId(current_game, PLAYER_2);
		Winner winner_of_game = gameGetWinner(current_game);

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

		if (!mapContains(players_at_chess, &id_new_winner))
		{
			mapRemove(players_at_chess, &player_id);
			mapRemove(players_at_tournament, &player_id);
			return PLAYER_ALREADY_REMOVED;
		}

		if ((id_first == player_id && (winner_of_game == FIRST_PLAYER || winner_of_game == DRAW)) ||
			(id_second == player_id && (winner_of_game == SECOND_PLAYER || winner_of_game == DRAW)))
		{
			gameSetWinner(current_game, new_winner);

			Player player1 = mapGet(players_at_chess, &id_new_winner);
			playerSetNumWins(player1, playerGetNumWins(player1) + 1);

			Player player2 = mapGet(players_at_tournament, &id_new_winner);
			playerSetNumWins(player2, playerGetNumWins(player2) + 1);
		}
	}
	else
		return PLAYER_OUT_OF_MEMORY;
	return PLAYER_SUCCESS;
}