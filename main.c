#include "chess.h"

int main()
{
	ChessSystem chess = chessCreate();

	ChessResult result = CHESS_SUCCESS;

	result = chessAddGame(chess, 1, 11, 12, DRAW, 60);

	result = chessAddTournament(chess, 1, 10, "Israel");

	result = chessAddGame(chess, 1, 11, 12, DRAW, 60);

	result = chessAddGame(chess, 1, 12, 11, DRAW, 60);

	result = chessAddGame(chess, 2, 11, 12, DRAW, 60);

	chessDestroy(chess);

	printf("result: %d", result);

	return 0;
}