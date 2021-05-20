#ifndef TOURNAMENT_H_
#define TOURNAMENT_H_

#include "chessSystem.h"
#include "list.h"
#include "map.h"
#include <stdbool.h>

typedef struct Tournament_t* Tournament;

ChessResult tournamentValidate(int id, int max_games_per_player, const char* location);

Tournament tournamentCreate(int id, int max_games_per_player, const char* location);

void tournamentDestroy(Tournament tournament);

Tournament tournamentCopy(Tournament tournament);

bool tournamentIsLocationValid(const char* location);

List tournamentGetGames(Tournament tournament);

Map tournamentGetPlayers(Tournament tournament);

bool tournamentHasEnded(Tournament tournament);

bool tournamentHasGame(Tournament tournament, int first_player, int second_player);

int tournamentPlayerNumGames(Tournament tournament, int player_id);

int tournamentGetMaxGamesPerPlayer(Tournament tournament);

bool tournamentAddGame(Map all_players_at_chess, Tournament tournament, int first_player, int second_player, Winner winner, int play_time);

void tournamentEnd(Tournament tournament);

Winner tournamentGetWinner(Tournament tournament);

char* tournamentGetLocation(Tournament tournament);

int tournamentGetNumberGames(Tournament tournament);

int tournamentGetNumberPlayers(Tournament tournament);

int tournamentGetLongestGameTime(Tournament tournament);

double tournamentGetAvgGameTime(Tournament tournament);

#endif
