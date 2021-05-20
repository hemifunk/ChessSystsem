#include "chessSystem.h"
#include "generics.h"
#include "map.h"
#include "player.h"
#include "tournament.h"
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINS_WEIGHT 6
#define LOSES_WEIGHT -10
#define DRAWS_WEIGHT 2

struct chess_system_t
{
	Map tournaments;
	Map all_players;
};

static bool hasNum(int* arr, int size, int number)
{
	for (int i = 0; i < size; i++)
	{
		if (number == arr[i])
		{
			return true;
		}
	}

	return false;
}

static double findMinLevel(Map players, int* unallowed_ids, int index, int* id_minimal)
{
	double min = INT_MAX;
	int id_min;

	MAP_FOREACH(int*, i, players)
	{
		Player player = mapGet(players, i);

		if (min > playerGetLevel(player) && hasNum(unallowed_ids, mapGetSize(players), playerGetId(player) == false))
		{
			min = playerGetLevel(player);
			id_min = playerGetId(player);
		}

		genericIntDestroy(i);
	}

	unallowed_ids[index] = id_min;
	*id_minimal = id_min;

	return min;
}

static void removeTournamentStats(ChessSystem chess, Tournament tournament)
{
	if (chess == NULL || tournament == NULL)
	{
		return;
	}

	Map tournament_players = tournamentGetPlayers(tournament);

	MAP_FOREACH(int*, id, tournament_players)
	{
		Player local_data = mapGet(tournament_players, id);
		Player global_data = mapGet(chess->all_players, id);

		assert(local_data != NULL && global_data != NULL);

		playerSetNumGames(global_data, playerGetNumGames(global_data) - playerGetNumGames(local_data));

		playerSetNumWins(global_data, playerGetNumWins(global_data) - playerGetNumWins(local_data));

		playerSetNumLoses(global_data, playerGetNumLoses(global_data) - playerGetNumLoses(local_data));

		playerSetNumDraws(global_data, playerGetNumDraws(global_data) - playerGetNumDraws(local_data));

		genericIntDestroy(id);
	}
}

static void removePlayerStats(Map players, Game game, int removed_player_id)
{
	if (players == NULL || game == NULL || removed_player_id <= 0)
	{
		return;
	}

	int first_player_id = gameGetPlayerId(game, PLAYER_1);
	int second_player_id = gameGetPlayerId(game, PLAYER_2);

	if (first_player_id != removed_player_id && second_player_id != removed_player_id)
	{
		return;
	}

	Player removed_player = mapGet(players, &removed_player_id);
	Player other_player = mapGet(players, first_player_id != removed_player_id ? &first_player_id : &second_player_id);

	if (removed_player != NULL) //todo: is it right?
	{
		playerSetNumWins(removed_player, 0);
		playerSetNumDraws(removed_player, 0);
		playerSetNumLoses(removed_player, 0);
		playerSetNumGames(removed_player, 0);
		playerSetTotalGameTime(removed_player, 0);
		playerSetLevel(removed_player, 0);
	}

	if (other_player == NULL) // other player was also removed
	{
		return;
	}

	if (gameGetWinner(game) == DRAW)
	{
		playerSetNumDraws(other_player, playerGetNumDraws(other_player) - 1);
		playerSetNumWins(other_player, playerGetNumWins(other_player) + 1);
	}

	else if ((gameGetWinner(game) == FIRST_PLAYER && first_player_id == removed_player_id) ||
			 (gameGetWinner(game) == SECOND_PLAYER && second_player_id == removed_player_id))
	{
		playerSetNumLoses(other_player, playerGetNumLoses(other_player) - 1);
		playerSetNumWins(other_player, playerGetNumWins(other_player) + 1);
	}
}

ChessSystem chessCreate()
{
	ChessSystem chess = malloc(sizeof(struct chess_system_t));

	if (chess == NULL)
	{
		return NULL;
	}

	chess->tournaments = mapCreate(genericTournamentCopy, genericIntCopy, genericTournamentDestroy, genericIntDestroy, genericIntCompare);
	chess->all_players = mapCreate(genericPlayerCopy, genericIntCopy, genericPlayerDestroy, genericIntDestroy, genericIntCompare);

	if (chess->all_players == NULL || chess->tournaments == NULL)
	{
		chessDestroy(chess);
		return NULL;
	}

	return chess;
}

void chessDestroy(ChessSystem chess)
{
	if (chess == NULL)
	{
		return;
	}

	mapDestroy(chess->tournaments);
	mapDestroy(chess->all_players);
	free(chess);
}

ChessResult chessAddTournament(ChessSystem chess, int tournament_id, int max_games_per_player, const char* tournament_location)
{
	if (chess == NULL || chess->tournaments == NULL || chess->all_players == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	ChessResult result = tournamentValidate(tournament_id, max_games_per_player, tournament_location);

	if (result != CHESS_SUCCESS)
	{
		return result;
	}

	if (mapContains(chess->tournaments, &tournament_id))
	{
		return CHESS_TOURNAMENT_ALREADY_EXISTS;
	}

	Tournament tournament = tournamentCreate(tournament_id, max_games_per_player, tournament_location);

	if (tournament == NULL)
	{
		return CHESS_OUT_OF_MEMORY;
	}

	if (mapPut(chess->tournaments, &tournament_id, tournament) != MAP_SUCCESS)
	{
		tournamentDestroy(tournament);
		return CHESS_OUT_OF_MEMORY;
	}

	tournamentDestroy(tournament);
	return CHESS_SUCCESS;
}

ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id)
{
	if (chess == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	if (tournament_id <= 0)
	{
		return CHESS_INVALID_ID;
	}

	if (mapContains(chess->tournaments, &tournament_id) == false)
	{
		return CHESS_TOURNAMENT_NOT_EXIST;
	}

	Tournament tournament = mapGet(chess->tournaments, &tournament_id);

	removeTournamentStats(chess, tournament);

	mapRemove(chess->tournaments, &tournament_id);

	return CHESS_SUCCESS;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player, int second_player, Winner winner, int play_time)
{
	if (chess == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	if (tournament_id <= 0)
	{
		return CHESS_INVALID_ID;
	}

	ChessResult result = gameValidate(first_player, second_player, play_time);

	if (result != CHESS_SUCCESS)
	{
		return result;
	}

	if (mapContains(chess->tournaments, &tournament_id) == false)
	{
		return CHESS_TOURNAMENT_NOT_EXIST;
	}

	Tournament tournament = mapGet(chess->tournaments, &tournament_id);

	assert(tournament != NULL);

	if (tournamentHasEnded(tournament))
	{
		return CHESS_TOURNAMENT_ENDED;
	}

	if (tournamentHasGame(tournament, first_player, second_player))
	{
		return CHESS_GAME_ALREADY_EXISTS;
	}

	if (tournamentPlayerNumGames(tournament, first_player) >= tournamentGetMaxGamesPerPlayer(tournament) ||
		tournamentPlayerNumGames(tournament, second_player) >= tournamentGetMaxGamesPerPlayer(tournament))
	{
		return CHESS_EXCEEDED_GAMES;
	}

	if (tournamentAddGame(chess->all_players, tournament, first_player, second_player, winner, play_time) == false)
	{
		return CHESS_OUT_OF_MEMORY;
	}

	return CHESS_SUCCESS;
}

ChessResult chessRemovePlayer(ChessSystem chess, int player)
{
	if (chess == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	if (player <= 0)
	{
		return CHESS_INVALID_ID;
	}

	if (mapContains(chess->all_players, &player) == false)
	{
		return CHESS_PLAYER_NOT_EXIST;
	}

	MAP_FOREACH(int*, i, chess->tournaments)
	{
		Tournament tournament = mapGet(chess->tournaments, i);
		Map tournament_players = tournamentGetPlayers(tournament);

		if (tournamentHasEnded(tournament) == false && mapContains(tournament_players, &player))
		{
			for (int j = 0; j < tournamentGetNumberGames(tournament); j++)
			{
				Game game = listGet(tournamentGetGames(tournament), j);

				removePlayerStats(chess->all_players, game, player);
				removePlayerStats(tournament_players, game, player);

				gameRemovePlayer(game, player);
			}

			mapRemove(tournament_players, &player);
		}

		genericIntDestroy(i);
	}

	mapRemove(chess->all_players, &player);

	return CHESS_SUCCESS;
}

//todo: review
ChessResult chessEndTournament(ChessSystem chess, int tournament_id)
{
	if (chess == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	if (mapContains(chess->tournaments, &tournament_id) == false)
	{
		return CHESS_TOURNAMENT_NOT_EXIST;
	}

	Tournament tournament_to_end = mapGet(chess->tournaments, &tournament_id);

	assert(tournament_to_end != NULL);

	if (tournamentHasEnded(tournament_to_end))
	{
		return CHESS_TOURNAMENT_ENDED;
	}

	tournamentEnd(tournament_to_end);

	return CHESS_SUCCESS;
}

double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult* result)
{
	assert(result != NULL);

	*result = CHESS_SUCCESS;

	if (chess == NULL)
	{
		*result = CHESS_NULL_ARGUMENT;
	}

	if (player_id <= 0)
	{
		*result = CHESS_INVALID_ID;
	}

	if (mapContains(chess->all_players, &player_id) == false)
	{
		*result = CHESS_PLAYER_NOT_EXIST;
	}

	if (*result != CHESS_SUCCESS)
	{
		return -1;
	}

	Player player = mapGet(chess->all_players, &player_id);

	assert(player != NULL);

	int time_of_playing = playerGetTotalPlayTime(player);
	int number_of_games = playerGetNumGames(player);

	if (number_of_games == 0)
	{
		return 0;
	}

	return ((double)time_of_playing) / number_of_games;
}

//todo: review
//todo: shorten
ChessResult chessSavePlayersLevels(ChessSystem chess, FILE* file)
{
	//todo: empty file if no players.
	if (chess == NULL || file == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	int num_games = 0;

	MAP_FOREACH(int*, i, chess->tournaments)
	{
		num_games += tournamentGetNumberGames(mapGet(chess->tournaments, i));

		genericIntDestroy(i);
	}

	if (num_games <= 0)
	{
		return CHESS_SUCCESS;
	}

	Player old_top_player = NULL;
	Player new_top_player = NULL;

	MAP_FOREACH(int*, i, chess->all_players)
	{
		Player player = mapGet(chess->all_players, i);

		if (playerGetNumGames(player) <= 0)
		{
			genericIntDestroy(i);

			continue;
		}

		assert(player != NULL);

		double level = (WINS_WEIGHT * playerGetNumWins(player) +
						LOSES_WEIGHT * playerGetNumLoses(player) +
						DRAWS_WEIGHT * playerGetNumDraws(player)) /
					   (double)num_games;

		playerSetLevel(player, level);

		if (playerGetLevel(player) > new_top_player)
		{
			new_top_player = player;
		}
		else if (playerGetLevel(player) == new_top_player && playerGetId(player) < playerGetId(new_top_player))
		{
			new_top_player = player;
		}

		genericIntDestroy(i);
	}

	// todo: SO UGLY AAAAAAAAAAAAAA
	while (new_top_player != NULL)
	{
		if (fprintf(file, "%d %d\n", playerGetId(new_top_player), playerGetLevel(new_top_player)) <= 0)
		{
			return CHESS_SAVE_FAILURE;
		}

		old_top_player = new_top_player;
		new_top_player = NULL;

		MAP_FOREACH(int*, i, chess->all_players)
		{
			Player player = mapGet(chess->all_players, i);

			if (playerGetLevel(player) > playerGetLevel(old_top_player) || playerGetNumGames(player) == 0)
			{
				genericIntDestroy(i);
				continue;
			}

			if (playerGetLevel(player) == playerGetLevel(old_top_player) && playerGetId(player) >= playerGetId(old_top_player))
			{
				genericIntDestroy(i);
				continue;
			}

			if (new_top_player == NULL)
			{
				new_top_player = player;
				genericIntDestroy(i);
				continue;
			}

			if (playerGetLevel(player) > new_top_player)
			{
				new_top_player = player;
			}
			else if (playerGetLevel(player) == new_top_player && playerGetId(player) < playerGetId(new_top_player))
			{
				new_top_player = player;
			}

			genericIntDestroy(i);
		}
	}

	return CHESS_SUCCESS;
}

ChessResult chessSaveTournamentStatistics(ChessSystem chess, char* path_file)
{
	if (chess = NULL || path_file == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	FILE* file = fopen(path_file, "w");

	if (file == NULL)
	{
		return CHESS_SAVE_FAILURE;
	}

	bool has_ended_tournaments = false;

	MAP_FOREACH(int*, i, chess->tournaments)
	{
		Tournament tournament = mapGet(chess->tournaments, i);

		assert(tournament != NULL);

		if (tournamentHasEnded(tournament))
		{
			if (fprintf(file, "%d\n%d\n%f\n%s\n%d\n%d\n",
						tournamentGetWinner(tournament),
						tournamentGetLongestGameTime(tournament),
						tournamentGetAvgGameTime(tournament),
						tournamentGetLocation(tournament),
						tournamentGetNumberGames(tournament),
						tournamentGetNumberPlayers(tournament)) <= 0)
			{
				fclose(file);
				genericIntDestroy(i);
				return CHESS_SAVE_FAILURE;
			}

			has_ended_tournaments = true;
		}

		genericIntDestroy(i);
	}

	fclose(file);

	if (has_ended_tournaments == false)
	{
		return CHESS_NO_TOURNAMENTS_ENDED;
	}

	return CHESS_SUCCESS;
}