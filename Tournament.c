#include "tournament.h"
#include "game.h"
#include "generics.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Tournament_t
{
	int id;
	int winner_id;
	char* location;
	bool has_finished;
	int max_games_per_player;
	Map games; //todo: replace with list?
	Map players; //todo: replace with list?
};

static Player tournamentAddPlayer(Tournament tournament, int id)
{
	if (mapContains(tournament->players, &id))
	{
		return mapGet(tournament->players, &id);
	}

	Player player = playerCreate(id);

	if (player == NULL)
	{
		return NULL;
	}

	if (mapPut(tournament->players, &id, player) != MAP_SUCCESS)
	{
		playerDestroy(player);
		return NULL;
	}

	playerDestroy(player);

	return mapGet(tournament->players, &id);
}

Tournament tournamentCreate(int id, int max_games_per_player, const char* location)
{
	if (id <= 0 || max_games_per_player <= 0 || location == NULL || tournamentIsLocationValid(location) == false)
	{
		return NULL;
	}

	Tournament tournament = malloc(sizeof(struct Tournament_t));

	if (tournament == NULL)
	{
		return NULL;
	}

	tournament->games = mapCreate(genericGameCopy, genericIntCopy, genericGameDestroy, genericIntDestroy, genericIntCompare);
	tournament->players = mapCreate(genericPlayerCopy, genericIntCopy, genericPlayerDestroy, genericIntDestroy, genericIntCompare);
	tournament->location = strdup(location);

	if (tournament->games == NULL || tournament->players == NULL || tournament->location == NULL)
	{
		tournamentDestroy(tournament);
		return NULL;
	}

	tournament->id = id;
	tournament->winner_id = 0;
	tournament->has_finished = false;
	tournament->max_games_per_player = max_games_per_player;

	return tournament;
}

void tournamentDestroy(Tournament tournament)
{
	free(tournament->location);
	mapDestroy(tournament->games);
	mapDestroy(tournament->players);
	free(tournament);
}

Tournament tournamentCopy(Tournament tournament)
{
	if (tournament == NULL)
	{
		return NULL;
	}

	Tournament copy = tournamentCreate(tournament->id, tournament->max_games_per_player, tournament->location);

	if (copy == NULL)
	{
		return NULL;
	}

	mapDestroy(copy->games);
	mapDestroy(copy->players);

	copy->games = mapCopy(tournament->games);
	copy->players = mapCopy(tournament->players);

	if (copy->games == NULL || copy->players == NULL)
	{
		tournamentDestroy(copy);
		return NULL;
	}

	copy->has_finished = tournament->has_finished;
	copy->winner_id = tournament->winner_id;

	return copy;
}

bool tournamentHasEnded(Tournament tournament)
{
	assert(tournament != NULL);

	if (tournament == NULL)
	{
		return false;
	}

	return tournament->has_finished;
}

int tournamentGetMaxGamesPerPlayer(Tournament tournament)
{
	if (tournament == NULL)
	{
		return -1;
	}

	return tournament->max_games_per_player;
}

Map tournamentGetGames(Tournament tournament)
{
	if (tournament == NULL)
	{
		return NULL;
	}

	return tournament->games;
}

void tournamentEnd(Tournament tournament)
{
	if (tournament == NULL)
	{
		return;
	}

	if (tournament->has_finished)
	{
		return;
	}

	int max_score = 0;

	MAP_FOREACH(int*, i, tournament->players)
	{
		Player current = mapGet(tournament->players, i);

		int score = playerGetNumWins(current) * 2 + playerGetNumDraws(current);

		if (score >= max_score)
		{
			max_score = score;

			tournament->winner_id = playerGetId(current);
		}
	}

	tournament->has_finished = true;
}

bool tournamentIsLocationValid(const char* location)
{
	if (location == NULL)
	{
		return false;
	}

	char current = location[0];

	if ((current >= 'A' && current <= 'Z') == false)
	{
		return false;
	}

	int i = 1;

	while (location[i] != '\0')
	{
		current = location[i];

		if (((current >= 'a' && current <= 'z') || current == ' ') == false)
		{
			return false;
		}

		i++;
	}

	return true;
}

//todo: review
bool tournamentAddGame(Tournament tournament, int first_player, int second_player, Winner winner, int play_time)
{
	if (tournamentHasGame(tournament, first_player, second_player))
	{
		return false;
	}

	if (tournamentGetNumGames(tournament, first_player) >= tournamentGetMaxGamesPerPlayer(tournament) ||
		tournamentGetNumGames(tournament, second_player) >= tournamentGetMaxGamesPerPlayer(tournament))
	{
		return false;
	}

	Player first_ptr = tournamentAddPlayer(tournament, first_player);
	Player second_ptr = tournamentAddPlayer(tournament, second_player);

	if (first_ptr == NULL || second_ptr == NULL)
	{
		return false;
	}

	//todo: fix weirdness
	int last_index = mapGetSize(tournament->games) - 1;

	Game last_game = mapGet(tournament->games, &last_index);

	int game_id = 0;

	if (last_game == NULL)
	{
		game_id = 1;
	}
	else
	{
		game_id = gameGetId(last_game) + 1;
	}

	Game game = gameCreate(game_id, first_player, second_player, play_time, winner);

	if (game == NULL)
	{
		return false;
	}

	if (mapPut(tournament->games, &game_id, game) != MAP_SUCCESS)
	{
		gameDestroy(game);
		return false;
	}

	if (winner == FIRST_PLAYER)
	{
		playerSetNumWins(first_ptr, playerGetNumWins(first_ptr) + 1);
		playerSetNumLoses(second_ptr, playerGetNumLoses(second_ptr) + 1);
	}
	else if (winner == SECOND_PLAYER)
	{
		playerSetNumLoses(first_ptr, playerGetNumLoses(first_ptr) + 1);
		playerSetNumWins(second_ptr, playerGetNumWins(second_ptr) + 1);
	}
	else
	{
		playerSetNumDraws(first_ptr, playerGetNumDraws(first_ptr) + 1);
		playerSetNumDraws(second_ptr, playerGetNumDraws(second_ptr) + 1);
	}

	gameDestroy(game);
	return true;
}

bool tournamentHasGame(Tournament tournament, int player1_id, int player2_id)
{
	MAP_FOREACH(int*, i, tournament->games)
	{
		Game current = mapGet(tournament->games, i);

		if (gameGetPlayerId(current, PLAYER_1) == player1_id && gameGetPlayerId(current, PLAYER_2) == player2_id)
		{
			return true;
		}

		if (gameGetPlayerId(current, PLAYER_1) == player2_id && gameGetPlayerId(current, PLAYER_2) == player1_id)
		{
			return true;
		}
	}

	return false;
}

int tournamentGetNumGames(Tournament tournament, int player_id)
{	
	MAP_FOREACH(int*, i, tournament->players)
	{
		Player current = mapGet(tournament->games, i);

		if (playerGetId(current) == player_id)
		{
			return playerGetNumGames(current);
		}
	}

	return 0;
}