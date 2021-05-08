#include "tournament.h"
#include "chessSystem.h"
#include "map.h"
#include "generics.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Chess_System_t
{
	Map tournaments;
	Map all_players;
};

static void initializePlayer(Player player, int player_id, int time)
{
	playerSetId(player, player_id);
	playerSetNumGames(player, 1);
	playerSetTotalGameTime(player, time);
}

static ChessResult setPlayerResults(Map Players, Tournament tournament, int id, int time, int id_winner)
{
	Player player_result = playerCreate();

	if (player_result == NULL)
	{
		int index = mapGetSize(tournament->games) - 1;
		int* ptr = &index;
		if (mapRemove(tournament->games, ptr) == MAP_SUCCESS)
			return CHESS_OUT_OF_MEMORY;
		else
			return CHESS_OUT_OF_MEMORY;
	}

	int* ptr_id = &id;

	if (!mapContains(Players, ptr_id))
	{
		initializePlayer(player_result, id, time);
	}

	if (id_winner == id)
	{
		playerSetNumWins(player_result, 1 + playerGetNumWins(player_result));
	}
	else if (id_winner == 0)
	{
		playerSetNumDraws(player_result, 1 + playerGetNumDraws(player_result));
	}
	else
	{
		playerSetNumLoses(player_result, 1 + playerGetNumLoses(player_result));
	}
	
	if (mapPut(Players, ptr_id, player_result) == MAP_SUCCESS)
		return CHESS_SUCCESS;
	else
	{
		playerDestroy(player_result);
		int index = mapGetSize(tournament->games) - 1;
		int* ptr = &index;
		if (mapRemove(tournament->games, ptr) == MAP_SUCCESS)
			return CHESS_OUT_OF_MEMORY;
		else
			return CHESS_OUT_OF_MEMORY;
	}
}

static bool isGameExists(Tournament tournament, int player_id1, int player_id2)
{
	//Should I come back to mapGetFirst?
	//Should I use iterator Macro?
	//Does requested_game needs to be free?
	int size = mapGetSize(tournament->games);
	for (int i = 0; i < size; i++)
	{
		int* ptr = &i;
		Game requested_game = mapGet(tournament->games, ptr);
		if (gameGetPlayerId(requested_game, PLAYER_1) == player_id1 &&
			gameGetPlayerId(requested_game, PLAYER_2) == player_id2)
		{
			return true;
		}
	}
	return false;
}

static bool isPlayedMaxGames(Tournament tournament, int player_id1, int player_id2)
{
	int counter1 = 0, counter2 = 0;
	int size = mapGetSize(tournament->games);
	for (int i = 0; i < size; i++)
	{
		int* ptr = &i;
		Game requested_game = mapGet(tournament->games, ptr);
		int p1 = gameGetPlayerId(requested_game, PLAYER_1);
		int p2 = gameGetPlayerId(requested_game, PLAYER_2);
		if (p1 == player_id1 || p2 == player_id1)
		{
			counter1++;
		}
		int p3 = gameGetPlayerId(requested_game, PLAYER_1);
		int p4 = gameGetPlayerId(requested_game, PLAYER_2);
		if (p3 == player_id2 || p4 == player_id2)
		{
			counter2++;
		}
	}
	//what to do with requested_game? should be free?
	return counter1 == tournament->max_games_per_player ||
		   counter2 == tournament->max_games_per_player;
}

ChessSystem chessCreate()
{
	ChessSystem chess = malloc(sizeof(struct Chess_System_t));
	
	if (chess == NULL)
	{
		return NULL;
	}

	chess->tournaments = mapCreate(genericTournamentCopy, genericIntCopy, genericTournamentDestroy, genericIntDestroy, genericIntCompare);
	chess->all_players = mapCreate(genericPlayerCopy, genericIntCopy, genericPlayerDestroy, genericIntDestroy, genericIntCompare);

	if(chess->all_players == NULL || chess->tournaments == NULL)
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

	if (mapContains(chess->tournaments, tournament_id))
	{
		return CHESS_TOURNAMENT_ALREADY_EXISTS;
	}

	ChessResult result = CHESS_SUCCESS;

	Tournament tournament = tournamentCreate(tournament_id, max_games_per_player, tournament_location, &result);

	if(tournament == NULL)
	{
		return result;
	}	

	if (mapPut(chess->tournaments, tournament_id, tournament) != MAP_SUCCESS)
	{
		tournamentDestroy(tournament);
		return CHESS_OUT_OF_MEMORY;
	}

	tournamentDestroy(tournament);
	return CHESS_SUCCESS;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player, int second_player, GameWinner winner, int play_time)
{
	if (chess == NULL)
		return CHESS_NULL_ARGUMENT;

	if (tournament_id <= 0 || first_player <= 0 || second_player <= 0)
		return CHESS_INVALID_ID;

	if (play_time < 0)
		return CHESS_INVALID_PLAY_TIME;

	int* ptr_id;
	ptr_id = &tournament_id;

	if (!mapContains(chess->tournaments, ptr_id))
		return CHESS_TOURNAMENT_NOT_EXIST;

	Tournament requested_tour = tournamentCreate();

	if (requested_tour == NULL)
		return CHESS_OUT_OF_MEMORY;

	requested_tour = mapGet(chess->tournaments, ptr_id);

	if (requested_tour->has_finished)
	{
		return CHESS_TOURNAMENT_ENDED;
	}

	if (isGameExists(requested_tour, first_player, second_player))
	{
		return CHESS_GAME_ALREADY_EXISTS;
	}

	if (isPlayedMaxGames(requested_tour, first_player, second_player))
	{
		return CHESS_EXCEEDED_GAMES;
	}
	
	Game new_game = gameCreate();

	if (new_game == NULL)
		return CHESS_OUT_OF_MEMORY;

	gameSetId(new_game, mapGetSize(requested_tour->games));
	gameSetPlayerId(new_game, first_player, PLAYER_1);
	gameSetPlayerId(new_game, second_player, PLAYER_2);
	gameSetTime(new_game, play_time);
	gameSetWinner(new_game, winner);

	int id_game = gameGetId(new_game);
	int* ptr_game_id = &id_game;

	if (mapPut(requested_tour->games, ptr_game_id, new_game) != MAP_SUCCESS)
	{
		gameDestroy(new_game);
		return CHESS_OUT_OF_MEMORY;
	}

	int id_winner = 0;

	if (winner == FIRST_PLAYER)
		id_winner = first_player;
	else if (winner == SECOND_PLAYER)
		id_winner = second_player;

	if (setPlayerResults(chess->all_players, requested_tour, first_player, play_time, id_winner) != CHESS_SUCCESS)
		return CHESS_OUT_OF_MEMORY;

	if (setPlayerResults(chess->all_players, requested_tour, second_player, play_time, id_winner) != CHESS_SUCCESS)
		return CHESS_OUT_OF_MEMORY;

	if (setPlayerResults(requested_tour->players, requested_tour, first_player, play_time, id_winner) != CHESS_SUCCESS)
		return CHESS_OUT_OF_MEMORY;

	if (setPlayerResults(requested_tour->players, requested_tour, second_player, play_time, id_winner) != CHESS_SUCCESS)
		return CHESS_OUT_OF_MEMORY;

	return CHESS_SUCCESS;
}

ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id)
{
	int* ptr_id = &tournament_id;

	if (tournament_id <= 0)
		return CHESS_INVALID_ID;

	if (!mapContains(chess->tournaments, ptr_id))
		return CHESS_TOURNAMENT_NOT_EXIST;

	if (mapRemove(chess->tournaments, ptr_id) != MAP_SUCCESS)
		return CHESS_OUT_OF_MEMORY;
	else
		return CHESS_SUCCESS;
}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
	if (player_id <= 0)
		return CHESS_INVALID_ID;
}