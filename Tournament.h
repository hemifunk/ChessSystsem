#ifndef TOURNAMENT_H_
#define TOURNAMENT_H_
#include "Game.h"
#include "chessSystem.h"
#include "Player.h"
#include <stdbool.h>
#include <string.h>
#include "map.h"

typedef struct Tournament_t* Tournament;

/**Creates new tournament*/
Tournament tournamentCreate(int id, int max_games_per_player, const char* location, ChessResult* result);
/**Destroys a given tournament*/
void tournamentDestroy(Tournament tournament);
/**Copies a given tournament*/
Tournament tournamentCopy(Tournament tournament);
#endif