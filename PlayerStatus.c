#include "PlayerStatus.h"
#include <stdlib.h>

typedef struct playerStatus_t
{
    int player_id;
    int number_wins;
    int number_loses;
    int number_draws;
    int sum_of_total_time_playing;
    int number_games;
};

PlayerStatus playerStatusCreate()
{
    PlayerStatus player=malloc(sizeof(struct playerStatus_t));
    if(player==NULL)
        return NULL;
    return player;
}

void playerStatusDestroy(PlayerStatus player)
{
    if(player!=NULL)
        free(player);
}

PlayerStatus copyPlayerStatus(PlayerStatus player_status)
{
    if(player_status==NULL)
        return NULL;
    PlayerStatus new_player = playerStatusCreate();
    new_player->player_id=new_player->number_wins;
    new_player->number_wins=player_status->number_wins;
    new_player->number_loses=player_status->number_loses;
    new_player->number_draws=player_status->number_draws;
    new_player->sum_of_total_time_playing=player_status->sum_of_total_time_playing;
    new_player->number_games=player_status->number_games;
    return new_player;
}
int getIdPlayerStatus(PlayerStatus player_status)
{
    return player_status->player_id;
}

void setIdPlayerStatus(PlayerStatus player_status, int id)
{
    player_status->player_id=id;
}

int getNumberWins(PlayerStatus player_status)
{
    return player_status->number_wins;
}

void setNumberWins(PlayerStatus player_status, int number_wins)
{
    player_status->number_wins=number_wins;
}

int getNumberLoses(PlayerStatus player_status)
{
    return player_status->number_loses;
}

void setNumberLoses(PlayerStatus player_status, int number_loses)
{
    player_status->number_loses=number_loses;
}

int getNumberDraws(PlayerStatus player_status)
{
    return player_status->number_draws;
}

void setNumberDraws(PlayerStatus player_status, int number_draws)
{
    player_status->number_draws=number_draws;
}

int getSumOfTotalTimePlaying(PlayerStatus player_status)
{
    return player_status->sum_of_total_time_playing;
}

void setSumOfTotalTimePlaying(PlayerStatus player_status, int time)
{
    player_status->sum_of_total_time_playing=time;
}

int getNumberGames(PlayerStatus player_status)
{
    return player_status->number_games;
}

void setNumberGames(PlayerStatus player_status, int number_games)
{
    player_status->number_games=number_games;
}