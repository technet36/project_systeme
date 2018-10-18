#ifndef PROJECT_SYSTEME_LIBRARY_H
#define PROJECT_SYSTEME_LIBRARY_H
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "structures.h"
#include "io.h"

void copyPlayer(player_t *players, player_t* playerSrc);

int nextStair(int position, int stairsEntrancePosition, int firstStair);

int goToSquare (game_t *theGame, int idPlayer, int idHorse, int newPosition);

/**
 *
 * @param game
 * @param position
 * @param dice
 * @param stairsEntrancePosition
 * @return the next position
 */
int isThereObstacle(game_t *game, int position, int dice, int stairsEntrancePosition);

/**
 * initialyze the board (and the players for now
 * @param theGame
 * @return the id of the nest player
 */
int init (game_t* theGame);

/**
 * @return a pseudo-random number between 1 and 6
 */
int diceRoll();

/**
 * playthe roll for the horse of the player.
 * @return the id of next player or -1 if end
 */
int play(game_t* theGame ,int idPlayer, int idHorse, int dice);

int test();

#endif