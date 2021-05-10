#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include "chess.h"
#include "game.h"
#include "list.h"
#include "map.h"
#include "player.h"
#include <stdbool.h>
#include <string.h>

typedef struct Tournament_t* Tournament;

ChessResult tournamentValidate(int id, int max_games_per_player, const char* location);

Tournament tournamentCreate(int id, int max_games_per_player, const char* location);

void tournamentDestroy(Tournament tournament);

Tournament tournamentCopy(Tournament tournament);

bool tournamentIsLocationValid(const char* location);

List tournamentGetGames(Tournament tournament);

bool tournamentHasEnded(Tournament tournament);

bool tournamentHasGame(Tournament tournament, int player1_id, int player2_id);

int playerNumGames(Tournament tournament, int player_id);

int tournamentGetMaxGamesPerPlayer(Tournament tournament);

bool tournamentAddGame(Tournament tournament, int player1_id, int player2_id, Winner winner, int time);

void tournamentEnd(Tournament tournament);

#endif