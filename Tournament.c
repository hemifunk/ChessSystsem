#include "Tournament.h"
#include "Game.h"
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

static void* intCopy(void* id)
{
    if(id == NULL)
    {
		return NULL;
	}

	int* copy = malloc(sizeof(int));

	assert(copy != NULL);

	if (copy == NULL)
    {
		return NULL;
    }

	*copy = *(int*)id;

	return copy;
}

static void intFree(void* id)
{
	free(id);
}

static int intCompare(void* left, void* right)
{
	assert(left != NULL && right != NULL);

	if (left == NULL || right == NULL)
	{
		return 0;
	}

	int left_val = *(int*)left;
	int right_val = *(int*)right;

	return left_val - right_val;
}

Tournament tournamentCreate()
{
	Tournament tournament = malloc(sizeof(Tournament));

	if (tournament == NULL)
    {
		return NULL;
    }

	tournament->games = mapCreate(gameCopy, intCopy, gameDestroy, intFree, intCompare);
	tournament->players = mapCreate(playerCopy, intCopy, playerDestroy, intFree, intCompare);

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