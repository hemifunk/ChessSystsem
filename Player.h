#ifndef PLAYER_H_
#define PLAYER_H_

typedef struct Player_t* Player;

/**Creates a player*/
Player playerCreate(int id);

/**Destroys a player*/
void playerDestroy(Player player);

/**Copies a given player*/
Player playerCopy(Player player_status);

/**Returns player's id*/
int playerGetId(Player player);

/**Returns number of winnings*/
int playerGetNumWins(Player player);

/**Sets number of winnings*/
void playerSetNumWins(Player player, int num_wins);

/**Returns number of loses*/
int playerGetNumLoses(Player player);

/**Sets number of loses*/
void playerSetNumLoses(Player player, int num_loses);

/**Returns number of Draws*/
int playerGetNumDraws(Player player);

/**Sets number of Draws*/
void playerSetNumDraws(Player player, int num_draws);

/**Returns sum of total time playing of player*/
int playerGetTotalPlayTime(Player player);

/**Sets sum of total time playing of player*/
void playerSetTotalGameTime(Player player, int time);

/**Returns number of games played by player*/
int playerGetNumGames(Player player);

/**Sets number of games played by player*/
void playerSetNumGames(Player player, int num_games);

float playerGetLevel(Player player);

void playerSetLevel(Player player, float level);

#endif
