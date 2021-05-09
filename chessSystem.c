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

ChessSystem chessCreate()
{
	ChessSystem chess = malloc(sizeof(struct Chess_System_t));

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

	if(tournament_id <= 0)
	{
		return CHESS_INVALID_ID;
	}

	if(max_games_per_player <= 0)
	{
		return CHESS_INVALID_MAX_GAMES;
	}

	if (tournament_location == NULL)
	{
		return CHESS_INVALID_LOCATION;
	}

	if (mapContains(chess->tournaments, &tournament_id))
	{
		return CHESS_TOURNAMENT_ALREADY_EXISTS;
	}

	Tournament tournament = tournamentCreate(tournament_id, max_games_per_player, tournament_location);

	if(tournament == NULL)
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
	if(chess == NULL)
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

	if (mapRemove(chess->tournaments, &tournament_id) != MAP_SUCCESS)
	{
		return CHESS_OUT_OF_MEMORY;
	}

	return CHESS_SUCCESS;
}

//todo: review
ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player, int second_player, Winner winner, int play_time)
{
	if (chess == NULL)
	{
		return CHESS_NULL_ARGUMENT;
	}

	if (tournament_id <= 0 || first_player <= 0 || second_player <= 0)
	{
		return CHESS_INVALID_ID;
	}

	if (play_time <= 0)
	{
		return CHESS_INVALID_PLAY_TIME;
	}

	if (mapContains(chess->tournaments, &tournament_id) == false)
	{
		return CHESS_TOURNAMENT_NOT_EXIST;
	}

	Tournament tournament = (Tournament)mapGet(chess->tournaments, &tournament_id);

	if(tournament == NULL)
	{
		return CHESS_OUT_OF_MEMORY;
	}

	if(tournamentGetHasFinished(tournament))
	{
		return CHESS_TOURNAMENT_ENDED;
	}

	if (tournamentHasGame(tournament, first_player, second_player))
	{
		return CHESS_GAME_ALREADY_EXISTS;
	}

	if(tournamentGetNumGames(tournament, first_player) >= tournamentGetMaxGamesPerPlayer(tournament))
	{
		return CHESS_EXCEEDED_GAMES;
	}

	if(tournamentGetNumGames(tournament, second_player) >= tournamentGetMaxGamesPerPlayer(tournament))
	{
		return CHESS_EXCEEDED_GAMES;
	}

	if(tournamentAddGame(tournament, first_player, second_player, winner, play_time) == false)
	{
		return CHESS_OUT_OF_MEMORY;
	}

	return CHESS_SUCCESS;
}