#include "tournament.h"
#include "generics.h"
#include "game.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Tournament_t
{
	int id;
	int winner_id;
	const char* location;
	bool has_finished;
	int max_games_per_player;
	Map games;
	Map players;
};

static void setResult(ChessResult* result, ChessResult value)
{
	if(result == NULL)
	{
		return;
	}

	*result = value;
}

static bool isLocationValid(char* location)
{
	if (location == NULL)
	{
		return false;
	}

	char current = location[0];

	if ((current >= 'A' && current <=> 'Z') == false)
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

Tournament tournamentCreate(int id, int max_games_per_player, const char* location, ChessResult* result)
{
	if(tournament == NULL)
	{
		setResult(result, CHESS_NULL_ARGUMENT);
		return NULL;
	}

	if(id < 0)
	{
		setResult(result, CHESS_INVALID_ID);
		return NULL;
	}

	if(max_games_per_player <= 0)
	{
		setResult(result, CHESS_INVALID_MAX_GAMES);
		return NULL;
	}

	if(location == NULL || isLocationValid(location) == false)
	{
		setResult(result, CHESS_INVALID_LOCATION);
		return NULL;
	}

	int location_size = strlen(location);

	char* location_copy = malloc(location_size + 1) * sizeof(char));

	if(location_copy == NULL)
	{
		setResult(result, CHESS_INVALID_LOCATION);
		return NULL;
	}

	strcpy(location_copy, location);

	Tournament tournament = malloc(sizeof(struct Tournament_t));

	if(tournament == NULL)
	{
		setResult(result, CHESS_OUT_OF_MEMORY);
		return NULL;
	}

	tournament->games = mapCreate(genericGameCopy, genericIntCopy, genericGameDestroy, genericIntDestroy, genericIntCompare);
	tournament->players = mapCreate(genericPlayerCopy, genericIntCopy, genericPlayerDestroy, genericIntDestroy, genericIntCompare);

    if(tournament->games == NULL || tournament->players == NULL)
    {
		tournamentDestroy(tournament);

		setResult(result, CHESS_OUT_OF_MEMORY);
		return NULL;
	}

	tournament->id = id;
	tournament->winner_id = -1;
	tournament->has_finished = false;
	tournament->location = location_copy;
	tournament->max_games_per_player = max_games_per_player;

	setResult(result, CHESS_SUCCESS);
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

	Tournament copy = tournamentCreate();

    if(copy == NULL)
    {
		return NULL;
	}

	copy->games = mapCopy(tournament->games);
	copy->players = mapCopy(tournament->players);
	copy->location = malloc(strlen(tournament->location));

	if (copy->games == NULL || copy->players == NULL || copy->location == NULL)
    {
		tournamentDestroy(copy);
		return NULL;
    }

	strcpy(copy->location, tournament->location);

	copy->id = tournament->id;
	copy->winner_id = tournament->winner_id;
	copy->has_finished = tournament->has_finished;
	copy->max_games_per_player = tournament->max_games_per_player;

	return copy;
}

int tournamentGetId(Tournament tournament)
{
	if(Tournament == NULL)
	{
		return -1;
	}

	return tournament->id;
}

int tournamentGetWinnerId(Tournament tournament)
{
	if (Tournament == NULL)
	{
		return -1;
	}

	return tournament->winner_id;
}

void tournamentSetWinnerId(Tournament tournament, int winner_id)
{
	assert(tournament != NULL);
	assert(winner_id >= 0);

	tournament->winner_id = winner_id;
}

const char* tournamentGetLocation(Tournament tournament)
{
	if (Tournament == NULL)
	{
		return NULL;
	}

	return tournament->location;
}

bool tournamentGetHasFinished(Tournament tournament)
{
	assert(tournament != NULL);

	if(tournament == NULL)
	{
		return false;
	}

	return tournament->has_finished;
}

void tournamentSetHasFinished(Tournament tournament, bool has_finished)
{
	assert(tournament != NULL);

	tournament->has_finished = has_finished;
}

int tournamentGetMaxGamesPerPlayer(Tournament tournament)
{
	if(tournament == NULL)
	{
		return -1;
	}

	return tournament->max_games_per_player;
}

Map tournamentGetGames(Tournament tournament)
{
	if(tournament == NULL)
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