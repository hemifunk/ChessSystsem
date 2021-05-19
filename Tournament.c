#include "Tournament.h"
#include "Game.h"
#include "Player.h"
#include "generics.h"
#include "list.h"
#include "map.h"
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
	List games;
	Map players;
};

static bool updatePlayers(Map players, int first_player, int second_player, Winner winner, int play_time)
{
	if (first_player <= 0 || second_player <= 0 || play_time <= 0)
	{
		return false;
	}

	Player first = mapGet(players, &first_player);
	Player second = mapGet(players, &second_player);

	if (first == NULL || second == NULL)
	{
		return false;
	}

	playerSetTotalGameTime(first, playerGetTotalPlayTime(first) + play_time);
	playerSetTotalGameTime(second, playerGetTotalPlayTime(second) + play_time);

	playerSetNumGames(first, playerGetNumGames(first) + 1);
	playerSetNumGames(second, playerGetNumGames(second) + 1);

	if (winner == FIRST_PLAYER)
	{
		playerSetNumWins(first, playerGetNumWins(first) + 1);
		playerSetNumLoses(second, playerGetNumLoses(second) + 1);
	}
	else if (winner == SECOND_PLAYER)
	{
		playerSetNumLoses(first, playerGetNumLoses(first) + 1);
		playerSetNumWins(second, playerGetNumWins(second) + 1);
	}
	else
	{
		playerSetNumDraws(first, playerGetNumDraws(first) + 1);
		playerSetNumDraws(second, playerGetNumDraws(second) + 1);
	}

	return true;
}

static bool tournamentAddPlayer(Map players, int id)
{
	if (mapContains(players, &id))
	{
		return true;
	}

	Player player = playerCreate(id);

	if (player == NULL)
	{
		return false;
	}

	if (mapPut(players, &id, player) != MAP_SUCCESS)
	{
		playerDestroy(player);
		return false;
	}

	playerDestroy(player);

	return true;
}

ChessResult tournamentValidate(int id, int max_games_per_player, const char* location)
{
	if (id <= 0)
	{
		return CHESS_INVALID_ID;
	}

	if (max_games_per_player <= 0)
	{
		return CHESS_INVALID_MAX_GAMES;
	}

	if (tournamentIsLocationValid(location) == false)
	{
		return CHESS_INVALID_LOCATION;
	}

	return CHESS_SUCCESS;
}

Tournament tournamentCreate(int id, int max_games_per_player, const char* location)
{
	if (tournamentValidate(id, max_games_per_player, location) != CHESS_SUCCESS)
	{
		return NULL;
	}

	Tournament tournament = malloc(sizeof(struct Tournament_t));

	if (tournament == NULL)
	{
		return NULL;
	}
	tournament->location = malloc(strlen(location) + 1);
	tournament->games = listCreate(genericGameCopy, genericGameDestroy);
	tournament->players = mapCreate(genericPlayerCopy, genericIntCopy, genericPlayerDestroy, genericIntDestroy, genericIntCompare);
	if (tournament->games == NULL || tournament->players == NULL || tournament->location == NULL)
	{
		tournamentDestroy(tournament);
		return NULL;
	}
	strcpy(tournament->location, location);
	tournament->id = id;
	tournament->winner_id = 0;
	tournament->has_finished = false;
	tournament->max_games_per_player = max_games_per_player;

	return tournament;
}

void tournamentDestroy(Tournament tournament)
{
	free(tournament->location);
	listDestroy(tournament->games);
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

	listDestroy(copy->games);
	mapDestroy(copy->players);

	copy->games = listCopy(tournament->games);
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
		return 0;
	}

	return tournament->max_games_per_player;
}

List tournamentGetGames(Tournament tournament)
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

		assert(current != NULL);

		int score = playerGetNumWins(current) * 2 + playerGetNumDraws(current);

		if (score >= max_score)
		{
			max_score = score;

			tournament->winner_id = playerGetId(current);
		}

		genericIntDestroy(i);
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

	//todo: add defines
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

//todo: add param validation
bool tournamentAddGame(Map all_players_at_chess, Tournament tournament, int first_player, int second_player, Winner winner, int play_time)
{
	if (tournament == NULL)
	{
		return false;
	}

	if (tournamentHasGame(tournament, first_player, second_player))
	{
		return false;
	}

	if (playerNumGames(tournament, first_player) >= tournament->max_games_per_player)
	{
		return false;
	}

	if (playerNumGames(tournament, second_player) >= tournament->max_games_per_player)
	{
		return false;
	}

	//what happens if one succeeds and the other doesn't? need to earse info?
	if (tournamentAddPlayer(tournament->players, first_player) == false || tournamentAddPlayer(tournament->players, second_player) == false || tournamentAddPlayer(all_players_at_chess, first_player) == false || tournamentAddPlayer(all_players_at_chess, second_player) == false)
	{
		return false;
	}

	Game game = gameCreate(first_player, second_player, play_time, winner);

	if (game == NULL)
	{
		return false;
	}

	if (listAdd(tournament->games, game) == false)
	{
		gameDestroy(game);
		return false;
	}

	updatePlayers(tournament->players, first_player, second_player, winner, play_time);
	updatePlayers(all_players_at_chess, first_player, second_player, winner, play_time);

	gameDestroy(game);
	return true;
}

bool tournamentHasGame(Tournament tournament, int first_player, int second_player)
{

	for (int i = 0; i < listSize(tournament->games); i++)
	{
		Game current = listGet(tournament->games, i);
		if (gameGetPlayerId(current, FIRST_PLAYER) == first_player && gameGetPlayerId(current, SECOND_PLAYER) == second_player)
		{
			return true;
		}

		if (gameGetPlayerId(current, FIRST_PLAYER) == second_player && gameGetPlayerId(current, SECOND_PLAYER) == first_player)
		{
			return true;
		}
	}
	return false;
}

int playerNumGames(Tournament tournament, int player)
{
	if (tournament == NULL || player <= 0)
	{
		return 0;
	}

	if (mapContains(tournament->players, &player) == false)
	{
		return 0;
	}

	return playerGetNumGames(mapGet(tournament->players, &player));
}

Winner tournamentGetWinner(Tournament tournament)
{
	assert(tournament != NULL);

	return tournament->winner_id;
}

char* tournamentGetLocation(Tournament tournament)
{
	assert(tournament != NULL);

	return tournament->location;
}

int tournamentGetNumberGames(Tournament tournament)
{
	assert(tournament != NULL);

	return listSize(tournament->games);
}

int tournamentGetNumberPlayers(Tournament tournament)
{
	assert(tournament != NULL);

	return mapGetSize(tournament->players);
}

int tournamentGetLongestGameTime(Tournament tournament)
{
	assert(tournament != NULL);

	List Games = tournamentGetGames(tournament);

	if (Games == NULL)
	{
		return 0;
	}

	int longest_game_time = 0;

	for (int i = 0; i < tournamentGetNumberGames(tournament); i++)
	{
		Game current = listGet(Games, i);

		if (current == NULL)
		{
			return 0;
		}

		if (longest_game_time < gameGetTime(current))
		{
			longest_game_time = gameGetTime(current);
		}
	}

	return longest_game_time;
}

double tournamentGetAvgGameTime(Tournament tournament)
{
	assert(tournament != NULL);

	List Games = tournamentGetGames(tournament);

	if (Games == NULL)
	{
		return 0;
	}

	double sum_game_time = 0;

	for (int i = 0; i < tournamentGetNumberGames(tournament); i++)
	{
		Game current = listGet(Games, i);

		if (current == NULL)
		{
			return 0;
		}

		sum_game_time += gameGetTime(current);
	}

	double avg_game_time = sum_game_time / tournamentGetNumberGames(tournament);

	avg_game_time = (int)avg_game_time * 100;
	avg_game_time = (double)avg_game_time / 100;

	return avg_game_time;
}