#ifndef PROJECT_SYSTEME_IO_H
#define PROJECT_SYSTEME_IO_H

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int id;
    //int size;
    void* data;
    int action;
} message_t;

typedef struct {

} frame_t;

/*
 * FRAME STRUCT
 *
 */

char* messageToFrame(message_t message);

message_t frameToMessage(char* frame);

/**
 * display the game board
 */
void displayGame(game_t* board);

void displayBoardFromPlayersArray(player_t *players);

/**
 * display the stats of the specified player
 */
void displayPlayer(player_t* player);

int displayError(error_t* error);


char* printPlayer(player_t* player, char* printTo);


#endif //PROJECT_SYSTEME_IO_H
