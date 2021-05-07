#include "Game.h"
#include <stdlib.h>

typedef struct game_t
{
    int player1_id;
    int player2_id;
    int game_id;
    Winner won_the_game;
    int time_of_game;
};

Game gameCreate()
{
    Game new_game=malloc(sizeof(struct game_t));
    if(new_game==NULL) return NULL;
    return new_game;
}

void gameDestroy(Game game)
{
    if(game!=NULL)
        free(game);
}

Game gameCopy(Game game)
{
    if(game==NULL)
        return NULL;
    Game new_game=gameCreate();
    new_game->game_id=game->game_id;
    new_game->player1_id=game->player1_id;
    new_game->player2_id=game->player2_id;
    new_game->time_of_game=game->time_of_game;
    new_game->won_the_game=game->won_the_game;
    return new_game;
}

int getIdPlayer(Game game, PLAYER_INDEX player)
{
    if(player==PLAYER_1)
        return game->player1_id;
    else
        return game->player2_id;
}

void setIdPlayer(Game game, int id, PLAYER_INDEX player)
{
    if(player==PLAYER_1)
        game->player1_id=id;
    else
        game->player2_id=id;
}

int getIdGame(Game game)
{
    return game->game_id;
}

void setIdGame(Game game, int id)
{
    game->game_id=id;
}

void setWinner(Game game, Winner winner)
{
   game->won_the_game=winner;
}

int getTimeGame(Game game)
{
    return game->time_of_game;
}

void setTimeGame(Game game, int time)
{
    game->time_of_game=time;
}