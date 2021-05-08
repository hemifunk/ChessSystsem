#include <stdio.h>
#include <assert.h>
#include "chessSystem.h"
#include "map.h"
#include "Tournament.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define FIRST_LETTER_UPPER_CASE 'A'
#define LAST_LETTER_UPPER_CASE 'Z'
#define FIRST_LETTER_LOWER_CASE 'a'
#define LAST_LETTER_LOWER_CASE 'z'
#define SPACE_CHAR ' '

struct chess_system_t
{
    Map tournaments;
    Map player_results;
};

static void emptyPlayerResult(PlayerStatus player, int id_player, int time)
{
    setIdPlayerStatus(player,id_player);
    setNumberWins(player,0);
    setNumberDraws(player,0);
    setNumberLoses(player,0);
    setNumberGames(player,1);
    setSumOfTotalTimePlaying(player,time);
}

static ChessResult setPlayerResults(Map Players,Tournament tournament, int id, int time,int id_winner)
{
    PlayerStatus player_result=playerStatusCreate();
    if(player_result==NULL)
    {
        int index=mapGetSize(tournament->Games)-1;
        int *ptr=&index;
        if(mapRemove(tournament->Games,ptr)==MAP_SUCCESS)
        return CHESS_OUT_OF_MEMORY;
        else return CHESS_OUT_OF_MEMORY;
    }
    int *ptr_id=&id;
    if(!mapContains(Players,ptr_id))
    {
        emptyPlayerResult(player_result,id,time);
    }
    if(id_winner==id)
    {
        setNumberWins(player_result,1+getNumberWins(player_result));
    }
    else if(id_winner==0)
    {
        setNumberDraws(player_result,1+getNumberDraws(player_result));
    }
    else
    {
        setNumberLoses(player_result,1+getNumberLoses(player_result));
    }
    if(mapPut(Players,ptr_id,player_result)==MAP_SUCCESS)
    return CHESS_SUCCESS;
    else 
    {
        playerStatusDestroy(player_result);
        int index=mapGetSize(tournament->Games)-1;
        int *ptr=&index;
        if(mapRemove(tournament->Games,ptr)==MAP_SUCCESS)
        return CHESS_OUT_OF_MEMORY;
        else return CHESS_OUT_OF_MEMORY;
    }   
}

static bool isStringValid (const char* str)
{
    int index=0;
    if(!(str[index]>=FIRST_LETTER_UPPER_CASE&&str[index]<=LAST_LETTER_UPPER_CASE))
    return false;
    while(str[index]!=0)
    {
        if (!(((str[index]>=FIRST_LETTER_LOWER_CASE) && (str[index]<=LAST_LETTER_LOWER_CASE)) || (str[index]==SPACE_CHAR)))
        return false;
        index++;
    }
    return true;
}

static bool isGameExists(Tournament tournament, int player_id1, int player_id2)
{
    //Should I come back to mapGetFirst?
    //Should I use iterator Macro?
    //Does requested_game needs to be free?
    int size=mapGetSize(tournament->Games);
    for(int i=0; i<size; i++)
    {
        int *ptr=&i;
        Game requested_game=mapGet(tournament->Games,ptr);
        if(gameGetPlayerId(requested_game,PLAYER_1)==player_id1&&
        gameGetPlayerId(requested_game,PLAYER_2)==player_id2)
        {
            return true;
        }
    }
    return false;
}
static bool isPlayedMaxGames(Tournament tournament, int player_id1, int player_id2)
{
    int counter1=0, counter2=0;
    int size=mapGetSize(tournament->Games);
    for(int i=0; i<size; i++)
    {
        int *ptr=&i;
        Game requested_game=mapGet(tournament->Games,ptr);
        int p1=gameGetPlayerId(requested_game,PLAYER_1);
        int p2=gameGetPlayerId(requested_game,PLAYER_2);
        if(p1==player_id1||p2==player_id1)
        {
            counter1++;
        }
        int p3=gameGetPlayerId(requested_game,PLAYER_1);
        int p4=gameGetPlayerId(requested_game,PLAYER_2);
        if(p3==player_id2||p4==player_id2)
        {
            counter2++;
        }
    }
    //what to do with requested_game? should be free?
    return counter1==tournament->max_games_per_player || 
    counter2==tournament->max_games_per_player;
}
ChessSystem chessCreate()
{
    ChessSystem chess = malloc(sizeof(struct chess_system_t));
    if(chess==NULL)
        return NULL;
    chess->tournaments=mapCreate((copyMapDataElements)copyTournament, (copyMapKeyElements)copyKey, 
    (freeMapDataElements)destroyTournament, (freeMapKeyElements)freeKey, (compareMapKeyElements)compareKeys);
    chess->player_results=mapCreate((copyMapDataElements)copyPlayerStatus, (copyMapKeyElements)copyKey, 
    (freeMapDataElements)playerStatusDestroy, (freeMapKeyElements)freeKey, (compareMapKeyElements)compareKeys);
    return chess;
}
void chessDestroy(ChessSystem chess)
{
    if(chess==NULL)
        return;
    mapDestroy(chess->tournaments);
    mapDestroy(chess->player_results);
    free(chess);
}

ChessResult chessAddTournament(ChessSystem chess, int tournament_id, int max_games_per_player, const char* tournament_location)
{
    if(chess==NULL) return CHESS_NULL_ARGUMENT;
    if(tournament_location==NULL) return CHESS_NULL_ARGUMENT;
    if(tournament_id<=0) return CHESS_INVALID_ID;
    int *ptr_id;
    ptr_id=&tournament_id;
    if(mapContains(chess->tournaments,ptr_id)) return CHESS_TOURNAMENT_ALREADY_EXISTS;
    if(!isStringValid(tournament_location)) return CHESS_INVALID_LOCATION;
    if(max_games_per_player<=0) return CHESS_INVALID_MAX_GAMES;
    Tournament tournament=createTournament();
        if(tournament==NULL) return CHESS_OUT_OF_MEMORY;
    tournament->id_tournament=tournament_id;
    tournament->max_games_per_player=max_games_per_player;
    tournament->status_tournament=false;
    tournament->location_tournament=malloc(strlen(tournament_location)+1);
    if(tournament_location==NULL)
    {
        destroyTournament(tournament);
        return CHESS_OUT_OF_MEMORY;
    }
    strcpy(tournament->location_tournament,tournament_location);
    Map Games=mapCreate((copyMapDataElements)gameCopy,(copyMapKeyElements)copyKey,
    (freeMapDataElements)gameDestroy,(freeMapKeyElements)freeKey,(compareMapKeyElements)compareKeys);
    if(Games==NULL)
    {
        free(tournament->location_tournament);
        destroyTournament(tournament);
        return CHESS_OUT_OF_MEMORY;
    }
    Map PlayerStatuses=mapCreate((copyMapDataElements)copyPlayerStatus,(copyMapKeyElements)copyKey,
    (freeMapDataElements)playerStatusDestroy,(freeMapKeyElements)freeKey,
    (compareMapKeyElements)compareKeys);
    if(PlayerStatuses==NULL)
    {
        free(tournament->location_tournament);
        mapDestroy(Games);
        destroyTournament(tournament);
        return CHESS_OUT_OF_MEMORY;  
    }
    if(mapPut(chess->tournaments,ptr_id, tournament)!=MAP_SUCCESS)
    {
        free(tournament->location_tournament);
        mapDestroy(Games);
        mapDestroy(PlayerStatuses);
        destroyTournament(tournament);
        return CHESS_OUT_OF_MEMORY; 
    }
    //Should be done?
    destroyTournament(tournament);   
    return CHESS_SUCCESS;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player, int second_player, Winner winner, int play_time)
{
    if(chess==NULL) return CHESS_NULL_ARGUMENT;
    if(tournament_id<=0 || first_player <=0 || second_player<=0) return CHESS_INVALID_ID;
    if(play_time<0) return CHESS_INVALID_PLAY_TIME;
    int *ptr_id;
    ptr_id=&tournament_id;
    if(!mapContains(chess->tournaments,ptr_id)) return CHESS_TOURNAMENT_NOT_EXIST;
    Tournament requested_tour=createTournament();
    if(requested_tour==NULL) return CHESS_OUT_OF_MEMORY;
    requested_tour=mapGet(chess->tournaments,ptr_id);
    if(requested_tour->status_tournament)
    {
        return CHESS_TOURNAMENT_ENDED;
    } 
    if(isGameExists(requested_tour,first_player,second_player))
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }
    if(isPlayedMaxGames(requested_tour,first_player,second_player))
    {
        return CHESS_EXCEEDED_GAMES;
    }
    Game new_game=gameCreate();
    if(new_game==NULL)
        return CHESS_OUT_OF_MEMORY;
    gameSetId(new_game,mapGetSize(requested_tour->Games));
    gameSetPlayerId(new_game,first_player,PLAYER_1); 
    gameSetPlayerId(new_game,second_player,PLAYER_2);
    gameSetTime(new_game,play_time);
    gameSetWinner(new_game,winner);
    int id_game=gameGetId(new_game);
    int *ptr_game_id=&id_game;
    if(mapPut(requested_tour->Games,ptr_game_id,new_game)!=MAP_SUCCESS)
    {
        gameDestroy(new_game);
        return CHESS_OUT_OF_MEMORY;
    }
    int id_winner=0;
    if(winner==FIRST_PLAYER)
        id_winner=first_player;
    else if(winner==SECOND_PLAYER)
        id_winner=second_player;
    if(setPlayerResults(chess->player_results,requested_tour,first_player,play_time,id_winner)!=CHESS_SUCCESS)
    return CHESS_OUT_OF_MEMORY;
    if(setPlayerResults(chess->player_results,requested_tour,second_player,play_time,id_winner)!=CHESS_SUCCESS)
    return CHESS_OUT_OF_MEMORY;
    if(setPlayerResults(requested_tour->Players_Results,requested_tour,first_player,play_time,id_winner)!=CHESS_SUCCESS)
    return CHESS_OUT_OF_MEMORY;
    if(setPlayerResults(requested_tour->Players_Results,requested_tour,second_player,play_time,id_winner)!=CHESS_SUCCESS)
    return CHESS_OUT_OF_MEMORY; 
    return CHESS_SUCCESS;   
}
ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id)
{
    int *ptr_id=&tournament_id;
    if(tournament_id<=0) return CHESS_INVALID_ID;
    if(!mapContains(chess->tournaments,ptr_id)) 
        return CHESS_TOURNAMENT_NOT_EXIST;
    if(mapRemove(chess->tournaments,ptr_id)!=MAP_SUCCESS)
        return CHESS_OUT_OF_MEMORY;
    else return CHESS_SUCCESS;
}
ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if(player_id<=0)
        return CHESS_INVALID_ID;    
}