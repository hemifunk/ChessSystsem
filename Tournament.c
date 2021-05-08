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
	Map games;
	Map players;
};

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

int tournamentGetId(Tournament tournament)
{
	if (tournament == NULL)
	{
		return -1;
	}

	return tournament->id;
}

int tournamentGetWinnerId(Tournament tournament)
{
	if (tournament == NULL)
	{
		return -1;
	}

	return tournament->winner_id;
}

const char* tournamentGetLocation(Tournament tournament)
{
	if (tournament == NULL)
	{
		return NULL;
	}

	return tournament->location;
}

bool tournamentGetHasFinished(Tournament tournament)
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

Map tournamentGetPlayers(Tournament tournament)
{
	if (tournament == NULL)
	{
		return NULL;
	}

	return tournament->players;
}

void tournamentFinish(Tournament tournament)
{
	if (tournament == NULL)
	{
		return;
	}

	if (tournament->has_finished)
	{
		return;
	}

	//todo: find winner
	//todo: set winner

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

bool tournamentAddGame(Tournament tournament, int player1_id, int player2_id, Winner winner, int time)
{
	if(tournamentHasGame(tournament, player1_id, player2_id))
	{
		return false;
	}

	Game game = gameCreate(game_id, player1_id, player2_id, time, winner);

	if(game == NULL)
	{
		return false;
	}

	int game_id = mapGetSize(tournament->games) + 1;

	if (mapPut(tournament->games, &game_id, game) != MAP_SUCCESS)
	{
		gameDestroy(game);
		return false;
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
	int count = 0;

	MAP_FOREACH(int*, i, tournament->games)
	{
		Game current = mapGet(tournament->games, i);

		if (gameGetPlayerId(current, PLAYER_1) == player_id || gameGetPlayerId(current, PLAYER_2) == player_id)
		{
			count += 1;
		}
	}

	return count;
}