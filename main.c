#include "chess.h"
#include <assert.h>

//todo: function max len - 50 lines
//todo: line max len: 120 chars
int main()
{
	ChessSystem chess = chessCreate();

	ChessResult result = CHESS_SUCCESS;

	result = chessAddGame(chess, 1, 11, 12, DRAW, 60);

	assert(result == CHESS_TOURNAMENT_NOT_EXIST);

	result = chessAddTournament(chess, 1, 10, "Israel tel aviv");

	assert(result == CHESS_SUCCESS);

	result = chessAddTournament(chess, 2, 10, "test");

	assert(result == CHESS_INVALID_LOCATION);

	result = chessAddGame(chess, 1, 11, 12, DRAW, 60);

	assert(result == CHESS_SUCCESS);

	result = chessAddGame(chess, 1, 11, 11, DRAW, 60);

	assert(result == CHESS_INVALID_ID);

	result = chessAddGame(chess, 1, 12, 11, DRAW, 60);

	assert(result == CHESS_GAME_ALREADY_EXISTS);

	result = chessAddGame(chess, 2, 11, 12, DRAW, 60);

	assert(result == CHESS_TOURNAMENT_NOT_EXIST);

	chessDestroy(chess);

	return 0;
}