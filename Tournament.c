#include "tournament.h"
#include "generics.h"
#include "game.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Tournament_t
{
	int id;
	char* location;
	Winner winner;
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
	tournament->has_finished = false;
	tournament->location = NULL;
	tournament->max_games_per_player = 0;

	tournament->winner = UNINITIALIZED;

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
	copy->has_finished = tournament->has_finished;
	copy->winner = tournament->winner;
	copy->max_games_per_player = tournament->max_games_per_player;

	return copy;
}