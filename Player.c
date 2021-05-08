#include "Player.h"
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
};

Player playerCreate()
{
	Player player = malloc(sizeof(struct Player_t));

	if(player==NULL)
    {
        return NULL;
    }

	player->id = -1;
	player->num_draws = 0;
	player->num_games = 0;
	player->num_loses = 0;
	player->num_wins = 0;
	player->total_play_time = 0;

	return player;
}

void playerDestroy(Player player)
{
    free(player);
}

Player playerCopy(Player player)
{
    if(player==NULL)
    {
        return NULL;
    }

    Player copy = playerCreate();

    if(copy == NULL)
    {
		return NULL;
	}

    copy->id=copy->id;
    copy->num_wins=player->num_wins;
    copy->num_loses=player->num_loses;
    copy->num_draws=player->num_draws;
    copy->total_play_time=player->total_play_time;
    copy->num_games=player->num_games;

    return copy;
}

int playerGetId(Player player)
{
    if(player == NULL)
    {
		return -1;
	}

    return player->id;
}

void playerSetId(Player player, int id)
{
	assert(player != NULL);
	assert(id >= 0);

	player->id=id;
}

int playerGetNumWins(Player player)
{
    if(player == NULL)
    {
		return -1;
	}

    return player->num_wins;
}

void playerSetNumWins(Player player, int num_wins)
{
	assert(player != NULL);
	assert(num_wins >= 0);

	player->num_wins=num_wins;
}

int playerGetNumLoses(Player player)
{
	if (player == NULL)
	{
		return -1;
	}

	return player->num_loses;
}

void playerSetNumLoses(Player player, int num_loses)
{
	assert(player != NULL);
	assert(num_loses >= 0);

	player->num_loses=num_loses;
}

int playerGetNumDraws(Player player)
{
	if (player == NULL)
	{
		return -1;
	}

	return player->num_draws;
}

void playerSetNumDraws(Player player, int num_draws)
{
	assert(player != NULL);
	assert(num_draws >= 0);

	player->num_draws=num_draws;
}

int playerGetTotalPlayTime(Player player)
{
	if (player == NULL)
	{
		return -1;
	}

	return player->total_play_time;
}

void playerSetTotalGameTime(Player player, int time)
{
	assert(player != NULL);
	assert(time >= 0);

	player->total_play_time=time;
}

int playerGetNumGames(Player player)
{
	if (player == NULL)
	{
		return -1;
	}

	return player->num_games;
}

void playerSetNumGames(Player player, int num_games)
{
	assert(player != NULL);
	assert(num_games >= 0);

	player->num_games=num_games;
}