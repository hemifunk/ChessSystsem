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
	char* location;
	bool has_finished;
	int max_games_per_player;
	Map games;
	Map players;
};

Tournament tournamentCreate()
{
	Tournament tournament = malloc(sizeof(Tournament));

	if (tournament == NULL)
    {
		return NULL;
    }

	tournament->games = mapCreate(genericGameCopy, genericIntCopy, genericGameDestroy, genericIntDestroy, genericIntCompare);
	tournament->players = mapCreate(genericPlayerCopy, genericIntCopy, genericPlayerDestroy, genericIntDestroy, genericIntCompare);

    if(tournament->games == NULL || tournament->players == NULL)
    {
		tournamentDestroy(tournament);
		return NULL;
	}

	tournament->id = -1;
	tournament->winner_id = -1;
	tournament->has_finished = false;
	tournament->location = NULL;
	tournament->max_games_per_player = 0;

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

void tournamentSetId(Tournament tournament, int id)
{
	assert(tournament != NULL);
	assert(id >= 0);

	tournament->id = id;
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

char* tournamentGetLocation(Tournament tournament)
{
	if (Tournament == NULL)
	{
		return NULL;
	}

	return tournament->location;
}

void tournamentSetLocation(Tournament tournament, char* location)
{
	assert(tournament != NULL);
	assert(location != NULL);

	tournament->location = location;
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

void tournamentSetMaxGamesPerPlayer(Tournament tournament, int max_games_per_player)
{
	assert(tournament != NULL);
	assert(max_games_per_player >= 0);

	tournament->max_games_per_player = max_games_per_player;
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