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
Tournament tournamentCreate();
/**Destroys a given tournament*/
void destroyTournament(Tournament tournament);
/**Copies a given tournament*/
Tournament copyTournament(Tournament tournament);
#endif