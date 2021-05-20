#include "Tournament.h"
#include "chessSystem.h"
#include <assert.h>

//TODO: REVIEW ALL CHANGES
//todo: update chessSystsem.h to newest file
//todo: check updated specifications
//todo: go over piazza questions, a lot of weird edge cases presented there
//todo: write tests

//todo: function max len - 50 lines
//todo: line max len: 120 chars
int main()
{
	ChessSystem chess = chessCreate();

	ChessResult result = CHESS_SUCCESS;

	result = chessAddTournament(chess, 1, 2, "Israel");

	assert(result == CHESS_SUCCESS);

	result = chessAddTournament(chess, 2, 2, "Paris");

	assert(result == CHESS_SUCCESS);

	result = chessAddGame(chess, 1, 123, 456, FIRST_PLAYER, 60);

	assert(result == CHESS_SUCCESS);

	result = chessAddGame(chess, 1, 11, 11, DRAW, 60);

	assert(result == CHESS_INVALID_ID);

	result = chessAddGame(chess, 2, 789, 371, SECOND_PLAYER, 70);

	assert(result == CHESS_SUCCESS);

	result = chessAddGame(chess, 1, 123, 456, DRAW, 80);

	assert(result == CHESS_GAME_ALREADY_EXISTS);

	result = chessAddGame(chess, 1, 456, 371, SECOND_PLAYER, 84);

	assert(result == CHESS_SUCCESS);

	result = chessAddGame(chess, 2, 123, 478, FIRST_PLAYER, 32);

	assert(result == CHESS_SUCCESS);

	result = chessRemovePlayer(chess, 123);

	assert(result == CHESS_SUCCESS);

	result = chessRemovePlayer(chess, 478);

	assert(result == CHESS_SUCCESS);

	result = chessEndTournament(chess, 1);

	assert(result == CHESS_SUCCESS);

	ChessResult* Result;
	ChessResult r = CHESS_SUCCESS;
	Result = &r;
	double avg = chessCalculateAveragePlayTime(chess, 123, Result);

	assert(*Result == CHESS_PLAYER_NOT_EXIST);

	avg = chessCalculateAveragePlayTime(chess, 371, Result);

	assert(*Result == CHESS_SUCCESS);

	printf("%lf\n", avg);

	result = chessRemoveTournament(chess, 2);

	chessDestroy(chess);

	return 0;
}