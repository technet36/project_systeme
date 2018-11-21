//
// Created by moran on 07/11/2018.
//

#ifndef PROJECT_SYSTEME_DISPLAY_H
#define PROJECT_SYSTEME_DISPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include <errno.h>
/**
 * display the game board
 */
void displayGame(game_t* board);

/**
 * display the stats of the specified player
 */
void displayPlayer(player_t* player);

void displayBoardFromPlayersArray(player_t *players);//todo


void displayGame(game_t *theGame);

int displayError(my_error_t* error);

void askName(char* name);

void myPrintf(char* msg);

#endif //PROJECT_SYSTEME_DISPLAY_H
