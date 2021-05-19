#include "chessSystem.h"
#include "Player.h"
#include "Tournament.h"
#include "generics.h"
#include "map.h"
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
	int number_games;
};

static void clearPlayersWithZeroGames(ChessSystem chess)
{
	Map all_players_at_chess = chess->all_players;

	assert(all_players_at_chess != NULL);

	MAP_FOREACH(int*, i, all_players_at_chess)
	{
		Player current_player = mapGet(all_players_at_chess, i);
		assert(current_player != NULL);

		if (playerGetNumGames(current_player) == 0)
		{
			MAP_FOREACH(int*, j, chess->tournaments)
			{
				Tournament current_tournament = mapGet(chess->tournaments, j);
				assert(current_tournament != NULL);

				Map players_at_current_tournament = tournamentGetPlayers(current_tournament);
				assert(players_at_current_tournament != NULL);

				if (mapContains(players_at_current_tournament, i))
				{
					mapRemove(players_at_current_tournament, i);
				}

				genericIntDestroy(j);
			}
			mapRemove(all_players_at_chess, i);
		}
		genericIntDestroy(i);
	}
}

static bool isContained(int* arr, int size, int number)
{
	for (int i = 0; i < size; i++)
	{
		if (number == arr[i])
			return true;
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
		if (min > playerGetLevel(player) && !isContained(unallowed_ids, mapGetSize(players), playerGetId(player)))
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
static void updateChessPlayers(Map players, Tournament tournament)
{
	assert(tournament != NULL);
	assert(players != NULL);

	MAP_FOREACH(int*, i, players)
	{
		if (mapContains(tournamentGetPlayers(tournament), i))
		{
			Player player_to_remove_from = mapGet(players, i);
			Player player_to_remove_info = mapGet(tournamentGetPlayers(tournament), i);

			playerSetNumGames(player_to_remove_from, playerGetNumGames(player_to_remove_from) - playerGetNumGames(player_to_remove_info));

			playerSetNumWins(player_to_remove_from, playerGetNumWins(player_to_remove_from) - playerGetNumWins(player_to_remove_info));

			playerSetNumLoses(player_to_remove_from, playerGetNumLoses(player_to_remove_from) - playerGetNumLoses(player_to_remove_info));

			playerSetNumDraws(player_to_remove_from, playerGetNumDraws(player_to_remove_from) - playerGetNumDraws(player_to_remove_info));
		}
		genericIntDestroy(i);
		//todo: maybe call a diff func.
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
	chess->number_games = 0;
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

	updateChessPlayers(chess->all_players, tournament);

	clearPlayersWithZeroGames(chess);

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

	Tournament tournament = (Tournament)mapGet(chess->tournaments, &tournament_id);

	assert(tournament != NULL);

	if (tournamentHasEnded(tournament))
	{
		return CHESS_TOURNAMENT_ENDED;
	}

	if (tournamentHasGame(tournament, first_player, second_player) &&
		mapContains(tournamentGetPlayers(tournament), &first_player) &&
		mapContains(tournamentGetPlayers(tournament), &second_player))
	{
		return CHESS_GAME_ALREADY_EXISTS;
	}
	if (playerNumGames(tournament, first_player) >= tournamentGetMaxGamesPerPlayer(tournament) ||
		playerNumGames(tournament, second_player) >= tournamentGetMaxGamesPerPlayer(tournament))
	{
		return CHESS_EXCEEDED_GAMES;
	}

	if (tournamentAddGame(chess->all_players, tournament, first_player, second_player, winner, play_time) == false)
	{
		return CHESS_OUT_OF_MEMORY;
	}

	chess->number_games = chess->number_games + 1;
	return CHESS_SUCCESS;
}

//todo: consult with Gilad about check-ups for NULL_ARGAUMENTS
ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
	if (chess == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	if (player_id <= 0)
	{
		return CHESS_INVALID_ID;
	}

	if (!mapContains(chess->all_players, &player_id))
	{
		return CHESS_PLAYER_NOT_EXIST;
	}

	MAP_FOREACH(int*, i, chess->tournaments)
	{
		Tournament current = mapGet(chess->tournaments, i);

		assert(current != NULL);

		if (!tournamentHasEnded(current))
		{
			List games_of_current = tournamentGetGames(current);
			Map Players = tournamentGetPlayers(current);

			if (Players == NULL)
			{
				genericIntDestroy(i);
				return CHESS_OUT_OF_MEMORY;
			}

			if (games_of_current != NULL)
			{
				for (int j = 0; j < listSize(games_of_current); j++)
				{
					PlayerResult result = getNewWinner(chess->all_players, Players, games_of_current, &j, player_id);

					if (result != PLAYER_SUCCESS && result != PLAYER_ALREADY_REMOVED)
					{
						genericIntDestroy(i);
						return CHESS_OUT_OF_MEMORY;
					}
				}
			}
			mapRemove(Players, &player_id);
		}
		genericIntDestroy(i);
	}

	clearPlayersWithZeroGames(chess);

	mapRemove(chess->all_players, &player_id);

	return CHESS_SUCCESS;
}
ChessResult chessEndTournament(ChessSystem chess, int tournament_id)
{
	if (chess == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	if (!mapContains(chess->tournaments, &tournament_id))
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

double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult* chess_result)
{
	if (chess == NULL)
	{
		*chess_result = CHESS_NULL_ARGUMENT;
	}
	if (player_id <= 0)
	{
		*chess_result = CHESS_INVALID_ID;
	}
	if (!mapContains(chess->all_players, &player_id))
	{
		*chess_result = CHESS_PLAYER_NOT_EXIST;
	}
	else
	{
		Player player = mapGet(chess->all_players, &player_id);
		if (player == NULL)
		{
			*chess_result = CHESS_OUT_OF_MEMORY;
		}
		else
		{
			double time_of_playing = playerGetTotalPlayTime(player);
			int number_of_games = playerGetNumGames(player);

			assert(playerGetNumGames(player) != 0);

			*chess_result = CHESS_SUCCESS;
			return time_of_playing / number_of_games;
		}
	}
	return -1;
}
//todo: Check about write, append
//todo: Consult with Gilad. getting file and not path. should I open it?
ChessResult chessSavePlayersLevels(ChessSystem chess, FILE* file)
{
	//todo: empty file if no players.
	if (chess == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	if (file == NULL)
	{
		return CHESS_SAVE_FAILURE;
	}

	int number_of_games_at_system = chess->number_games;
	int* unallowed_ids = malloc(mapGetSize(chess->all_players) * sizeof(int));

	if (unallowed_ids == NULL)
	{
		return CHESS_OUT_OF_MEMORY;
	}

	for (int i = 0; i < mapGetSize(chess->all_players); i++)
	{
		unallowed_ids[i] = 0;
	}

	MAP_FOREACH(int*, i, chess->all_players)
	{
		Player player = mapGet(chess->all_players, i);

		if (player == NULL)
		{
			genericIntDestroy(i);
			return CHESS_OUT_OF_MEMORY;
		}

		double level = (WINS_WEIGHT * playerGetNumWins(player) + LOSES_WEIGHT * playerGetNumLoses(player) +
						DRAWS_WEIGHT * playerGetNumDraws(player)) /
					   number_of_games_at_system;

		level = (int)(level * 100);
		level = (double)level / 100;

		playerSetLevel(player, level);
		genericIntDestroy(i);
	}

	int* minimal_id = 0;
	int min;

	for (int i = 0; i < mapGetSize(chess->all_players); i++)
	{
		min = findMinLevel(chess->all_players, unallowed_ids, i, minimal_id);
		fprintf(file, "%d %d\n", *minimal_id, min);
	}

	free(unallowed_ids);

	return CHESS_SUCCESS;
}

ChessResult chessSaveTournamentStatistics(ChessSystem chess, char* path_file)
{
	FILE* file = fopen(path_file, "w");

	if (file == NULL)
	{
		fclose(file);
		return CHESS_SAVE_FAILURE;
	}

	int count_ended_tournaments = 0;
	MAP_FOREACH(int*, i, chess->tournaments)
	{
		Tournament tournament = mapGet(chess->tournaments, i);

		if (tournament == NULL)
		{
			genericIntDestroy(i);
			//todo: clean file after error. how?
			fclose(file);
			return CHESS_OUT_OF_MEMORY;
		}
		if (tournamentHasEnded(tournament))
		{
			fprintf(file, "%d\n%d\n%f\n%s\n%d\n%d\n", tournamentGetWinner(tournament), tournamentGetLongestGameTime(tournament),
					tournamentGetAvgGameTime(tournament), tournamentGetLocation(tournament), tournamentGetNumberGames(tournament),
					tournamentGetNumberPlayers(tournament));

			count_ended_tournaments++;
		}
		genericIntDestroy(i);
	}

	fclose(file);

	if (count_ended_tournaments == 0)
		return CHESS_NO_TOURNAMENTS_ENDED;
	return CHESS_SUCCESS;
}