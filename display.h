//
// Created by moran on 07/11/2018.
//

#ifndef PROJECT_SYSTEME_DISPLAY_H
#define PROJECT_SYSTEME_DISPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
/**
 * display the game board
 */
void displayGame(game_t* board);

/**
 * display the stats of the specified player
 */
void displayPlayer(player_t* player);

void displayBoardFromPlayersArray(player_t *players);//todo


int displayError(error_t* error);
#endif //PROJECT_SYSTEME_DISPLAY_H