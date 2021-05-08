#include <stdio.h>
#include "player.h"
#include "game.h"
#include "tournament.h"

void* genericIntCopy(void* value)
{
	if (value == NULL)
	{
		return NULL;
	}

	int* copy = malloc(sizeof(int));

	assert(copy != NULL);

	if (copy == NULL)
	{
		return NULL;
	}

	*copy = *(int*)value;

	return copy;
}

void* genericPlayerCopy(void* player)
{
	if (player == NULL)
	{
		return NULL;
	}

	Player copy = playerCopy(player);

	assert(copy != NULL);

	return copy;
}

void* genericGameCopy(void* game)
{
	if (game == NULL)
	{
		return NULL;
	}

	Player copy = gameCopy(game);

	assert(copy != NULL);

	return copy;
}

void* genericTournamentCopy(void* tournament)
{
	if (tournament == NULL)
	{
		return NULL;
	}

	Player copy = tournamentCopy(tournament);

	assert(copy != NULL);

	return copy;
}

void genericIntDestroy(void* value)
{
	free(value);
}

void genericPlayerDestroy(void* player)
{
	playerDestroy(player);
}

void genericGameDestroy(void* game)
{
	gameDestroy(game);
}

void genericTournamentDestroy(void* tournament)
{
	tournamentDestroy(tournament);
}

int genericIntCompare(void* left, void* right)
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