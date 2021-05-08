#ifndef TOURNAMENT_H_
#define TOURNAMENT_H_
#include "Game.h"
#include "Player.h"
#include <stdbool.h>
#include <string.h>
#include "map.h"

typedef struct Tournament_t* Tournament;

/**Creates new tournament*/
Tournament tournamentCreate(int id, int max_games_per_player, const char* location);

/**Destroys a given tournament*/
void tournamentDestroy(Tournament tournament);

/**Copies a given tournament*/
Tournament tournamentCopy(Tournament tournament);

bool tournamentIsLocationValid(const char* location);

Map tournamentGetGames(Tournament tournament);

bool tournamentGetHasFinished(Tournament tournament);

bool tournamentHasGame(Tournament tournament, int player1_id, int player2_id);

int tournamentGetNumGames(Tournament tournament, int player_id);

int tournamentGetMaxGamesPerPlayer(Tournament tournament);

bool tournamentAddGame(Tournament tournament, int player1_id, int player2_id, Winner winner, int time);

#endif