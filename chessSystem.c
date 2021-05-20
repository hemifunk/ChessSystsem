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

#include "game.h"

#define WINS_WEIGHT 6
#define LOSES_WEIGHT (-10)
#define DRAWS_WEIGHT 2

struct chess_system_t
{
	Map tournaments;
	Map all_players;
};

//TODO: TIME CAN BE 0. FIND ALL time <= 0 occurances.

static void removeTournamentStats(ChessSystem chess, Tournament tournament)
{
	if (chess == NULL || tournament == NULL)
	{
		return;
	}

	Map tournament_players = tournamentGetPlayers(tournament);

	assert(tournament_players != NULL);

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

	if (removed_player != NULL)
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

static void calculateLevels(ChessSystem chess)
{
	if (chess == NULL)
	{
		return;
	}

	MAP_FOREACH(int*, i, chess->all_players)
	{
		Player player = mapGet(chess->all_players, i);

		assert(player != NULL);

		if (playerGetNumGames(player) <= 0)
		{
			playerSetLevel(player, 0);
			genericIntDestroy(i);
			continue;
		}

		const float level = (WINS_WEIGHT * playerGetNumWins(player) +
							 LOSES_WEIGHT * playerGetNumLoses(player) +
							 DRAWS_WEIGHT * playerGetNumDraws(player)) /
							(float)playerGetNumGames(player);

		playerSetLevel(player, level);

		genericIntDestroy(i);
	}
}

//todo: test
static ChessResult savePlayers(ChessSystem chess, FILE* file)
{
	if (chess == NULL || file == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	Player new_top_player = NULL;
	Player old_top_player = NULL;

	do
	{
		MAP_FOREACH(int*, i, chess->all_players)
		{
			Player player = mapGet(chess->all_players, i);

			if (playerGetNumGames(player) <= 0)
			{
				genericIntDestroy(i);
				continue;
			}

			if (old_top_player != NULL &&
				(playerGetLevel(player) > playerGetLevel(old_top_player) ||
				(playerGetLevel(player) == playerGetLevel(old_top_player) && playerGetId(player) >= playerGetId(old_top_player))))
			{
				genericIntDestroy(i);
				continue;
			}

			if (new_top_player == NULL || 
				playerGetLevel(player) > playerGetLevel(new_top_player) ||
				(playerGetLevel(player) == playerGetLevel(new_top_player) && playerGetId(player) < playerGetId(new_top_player)))
			{
				new_top_player = player;
			}

			genericIntDestroy(i);
		}

		if (new_top_player == NULL)
		{
			break;
		}

		if (fprintf(file, "%d %.2f\n", playerGetId(new_top_player), playerGetLevel(new_top_player)) <= 0)
		{
			return CHESS_SAVE_FAILURE;
		}

		old_top_player = new_top_player;
		new_top_player = NULL;

	} while (true);

	return CHESS_SUCCESS;
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
	if (chess == NULL)
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

//todo: who is the winner if we removed both players
//todo: how does it impact stats calculations
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

	if (mapContains(chess->all_players, &player_id) == false)
	{
		return CHESS_PLAYER_NOT_EXIST;
	}

	MAP_FOREACH(int*, i, chess->tournaments)
	{
		Tournament tournament = mapGet(chess->tournaments, i);
		Map tournament_players = tournamentGetPlayers(tournament);

		assert(tournament != NULL);
		assert(tournament_players != NULL);

		if (tournamentHasEnded(tournament) == false && mapContains(tournament_players, &player_id))
		{
			for (int j = 0; j < tournamentGetNumberGames(tournament); j++)
			{
				Game game = listGet(tournamentGetGames(tournament), j);

				removePlayerStats(chess->all_players, game, player_id);
				removePlayerStats(tournament_players, game, player_id);

				gameRemovePlayer(game, player_id);
			}

			mapRemove(tournament_players, &player_id);
		}

		genericIntDestroy(i);
	}

	mapRemove(chess->all_players, &player_id);

	return CHESS_SUCCESS;
}

ChessResult chessEndTournament(ChessSystem chess, int tournament_id)
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

	assert(tournament != NULL);

	if (tournamentHasEnded(tournament))
	{
		return CHESS_TOURNAMENT_ENDED;
	}

	if (tournamentGetNumberGames(tournament) <= 0)
	{
		return CHESS_NO_GAMES;
	}

	tournamentEnd(tournament);

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

	if (*result != CHESS_SUCCESS)
	{
		return -1;
	}

	if (mapContains(chess->all_players, &player_id) == false)
	{
		*result = CHESS_PLAYER_NOT_EXIST;
		return -1;
	}

	Player player = mapGet(chess->all_players, &player_id);

	assert(player != NULL);

	const int time_of_playing = playerGetTotalPlayTime(player);
	const int number_of_games = playerGetNumGames(player);

	if (number_of_games == 0)
	{
		return 0;
	}

	return (double)time_of_playing / number_of_games;
}

ChessResult chessSavePlayersLevels(ChessSystem chess, FILE* file)
{
	if (chess == NULL || file == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	calculateLevels(chess);

	ChessResult result = savePlayers(chess, file);

	return result;
}

ChessResult chessSaveTournamentStatistics(ChessSystem chess, char* path_file)
{
	if (chess == NULL || path_file == NULL)
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
			if (fprintf(file, "%d\n%d\n%.2f\n%s\n%d\n%d\n",
						tournamentGetWinner(tournament),
						tournamentGetLongestGameTime(tournament),
						tournamentGetAvgGameTime(tournament),
						tournamentGetLocation(tournament),
						tournamentGetNumberGames(tournament),
						tournamentGetTotalNumPlayers(tournament)) <= 0)
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
