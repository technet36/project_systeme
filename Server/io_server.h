//
// Created by moran on 07/11/2018.
//

#ifndef PROJECT_SYSTEME_IO_SERVER_H
#define PROJECT_SYSTEME_IO_SERVER_H

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>

#include "../structures.h"

#define  max(a,b) ({(a)>(b)?(a):(b);})


typedef enum {DICE_ROLL, NEW_PLAYER, CHOOSE_HORSE, NEW_POS, MSG_LOOPBACK }ACTION_T;

typedef struct {
    int pid;
    int action;
}messageInfo_t;

typedef struct {
    int outPx[4];
    int inPx;
}pipes_t;


/**
 * display the game board
 */
void displayGame(game_t* board);

void initIO(pipes_t* myPipes, int[4][4], int[4][4]);

/**
 * display the stats of the specified player
 */
void displayPlayer(player_t* player);

void displayBoardFromPlayersArray(player_t *players);//todo
int sendDiceRoll(int* dice, int fileDescriptor);

messageInfo_t waitForPlayerMessageToServer(void* data, int fileDescriptor);

int sendMessage(int pid, int action, int fileDescriptor, void* data, int sizeOfData);

int broadCastPlayerArray(player_t* playerArray, int fileDescriptor);


int displayError(error_t* error);
#endif //PROJECT_SYSTEME_IO_SERVER_H
