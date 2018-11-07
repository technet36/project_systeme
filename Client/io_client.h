//
// Created by moran on 07/11/2018.
//

#ifndef PROJECT_SYSTEME_IO_CLIENT_H
#define PROJECT_SYSTEME_IO_CLIENT_H

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>

#include "../structures.h"


#define  max(a,b) ({(a)>(b)?(a):(b);})

typedef struct {
    int inLast, inServer, outNext, outServer;
}pipes_PLAYER_t;


typedef enum {DICE_ROLL, NEW_PLAYER, CHOOSE_HORSE, NEW_POS, MSG_LOOPBACK }ACTION_T;

typedef struct {
    int pid;
    int action;
}messageInfo_t;

/**
 * display the stats of the specified player
 */
void displayPlayer(player_t* player);

void displayBoardFromPlayersArray(player_t *players);//todo

int sendHorseServer(int* idHorse, int fileDescriptor);

int sendPlayerToServer(player_t* me, int fileDescriptor);

int sendMessageToNextPlayer(int pid,player_t* playerArray, int fileDescriptor);

int sendMessage(int pid, int action, int fileDescriptor, void* data, int sizeOfData);


int displayError(error_t* error);

messageInfo_t waitForMessage(void* data, int fileDescriptorA, int fileDescriptorB);
#endif //PROJECT_SYSTEME_IO_CLIENT_H
