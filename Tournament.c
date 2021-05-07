#include "Game.h"
#include "Tournament.h"
#include <stdlib.h>
#include <string.h>

int copyKey(void* id)
{
    int *new_id=malloc(sizeof(int));
    if(new_id==NULL)
        return NULL;
    *new_id=*(int*)id;
    return new_id;
}
void freeKey(void* id)
{
    free(id);
}
int compareKeys(void* n1, void* n2)
{
    int x=*(int*)n1;
    int y=*(int*)n2;
    return x-y;
}
Tournament createTournament()
{
    Tournament tournament=malloc(sizeof(Tournament));
    if(tournament==NULL)
        return NULL;
    return tournament;
}

void destroyTournament(Tournament tournament)
{
    free(tournament->location_tournament);
    mapDestroy(tournament->Games);
    mapDestroy(tournament->Players_Results);
    free(tournament);
}

Tournament copyTournament(Tournament tournament)
{
    if(tournament==NULL)
        return NULL;
    Tournament new_tournament=createTournament();
    new_tournament->id_tournament=tournament->id_tournament;
    new_tournament->status_tournament=tournament->status_tournament;
    new_tournament->won_the_tournament=tournament->won_the_tournament;
    new_tournament->max_games_per_player=tournament->max_games_per_player;
    strcpy(new_tournament->location_tournament,tournament->location_tournament);
    new_tournament->Games=mapCopy(tournament->Games);
    if(new_tournament->Games==NULL)
        return NULL;
    new_tournament->Players_Results=mapCopy(tournament->Players_Results);
    if(new_tournament->Players_Results==NULL)
        return NULL;
    return new_tournament;
}