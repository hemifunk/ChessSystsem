#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include "Game.h"
#include "Player.h"
#include <stdbool.h>
#include <string.h>
#include "map.h"

typedef struct Tournament_t* Tournament;

Tournament tournamentCreate(int id, int max_games_per_player, const char* location);

void tournamentDestroy(Tournament tournament);

Tournament tournamentCopy(Tournament tournament);

bool tournamentIsLocationValid(const char* location);

Map tournamentGetGames(Tournament tournament);

bool tournamentHasEnded(Tournament tournament);

bool tournamentHasGame(Tournament tournament, int player1_id, int player2_id);

int tournamentGetNumGames(Tournament tournament, int player_id);

int tournamentGetMaxGamesPerPlayer(Tournament tournament);

bool tournamentAddGame(Tournament tournament, int player1_id, int player2_id, Winner winner, int time);

void tournamentEnd(Tournament tournament);

#endif