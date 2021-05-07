#ifndef TOURNAMENT_H_
#define TOURNAMENT_H
#include "Game.h"
#include "chessSystem.h"
#include "PlayerStatus.h"
#include <stdbool.h>
#include <string.h>
#include "map.h"

typedef struct tournament_t
{
    int id_tournament;
    bool status_tournament;
    char *location_tournament;
    Winner won_the_tournament;
    int max_games_per_player;
    Map Games;
    Map Players_Results;
} *Tournament;

/**Given an integer, the function copies it*/
int copyKey(void* id);
/**Given an integer, the function frees it*/
void freeKey(void* id);
/**Given 2 integers, the function compares them*/
int compareKeys(void* n1, void* n2);
/**Creates new tournament*/
Tournament createTournament();
/**Destroys a given tournament*/
void destroyTournament(Tournament tournament);
/**Copies a given tournament*/
Tournament copyTournament(Tournament tournament);
#endif