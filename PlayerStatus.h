#ifndef PLAYERSTATUS_H_
#define PLAYERSTATUS_H

typedef struct playerStatus_t* PlayerStatus;

/**Creates elemnt of playerStatus*/
PlayerStatus playerStatusCreate();

/**Destorys elemnt of playerStatus*/
void playerStatusDestroy(PlayerStatus player);

/**Copies a given element of player_status*/
PlayerStatus copyPlayerStatus(PlayerStatus player_status);

/**Returns player's id*/
int getIdPlayerStatus(PlayerStatus player_status);

/**Sets player's id*/
void setIdPlayerStatus(PlayerStatus player_status, int id);

/**Returns number of winnings*/
int getNumberWins(PlayerStatus player_status);

/**Sets number of winnings*/
void setNumberWins(PlayerStatus player_status, int number_wins);

/**Returns number of loses*/
int getNumberLoses(PlayerStatus player_status);

/**Sets number of loses*/
void setNumberLoses(PlayerStatus player_status, int number_loses);

/**Returns number of Draws*/
int getNumberDraws(PlayerStatus player_status);

/**Sets number of Draws*/
void setNumberDraws(PlayerStatus player_status, int number_draws);

/**Returns sum of total time playing of player*/
int getSumOfTotalTimePlaying(PlayerStatus player_status);

/**Sets sum of total time playing of player*/
void setSumOfTotalTimePlaying(PlayerStatus player_status, int time);

/**Returns number of games played by player*/
int getNumberGames(PlayerStatus player_status);

/**Sets number of games played by player*/
void setNumberGames(PlayerStatus player_status, int number_games);
#endif