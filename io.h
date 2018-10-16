#ifndef PROJECT_SYSTEME_IO_H
#define PROJECT_SYSTEME_IO_H

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>


typedef enum {DICE_ROLL, NEW_PLAYER, CHOOSE_HORSE, NEW_POS, MSG_LOOPBACK }ACTION_T;

typedef struct {
    int pid;
    int action;
}messageInfo_t;



int displayError(error_t* error);

/**
 * display the game board
 */
void displayGame(game_t* board);

/**
 * display the stats of the specified player
 */
void displayPlayer(player_t* player);

void displayBoardFromPlayersArray(player_t *players);



int sendHorseServer(int idHorse, player_t* me);

int sendPlayerToServer(player_t* me);

int sendMessageToNextPlayer(int pid,player_t* playerArray, player_t* me);

int sendMessage(int pid, int action, int fileDescriptor, void* data, int sizeOfData);

messageInfo_t waitForMessage(void* data, player_t* me);


#endif //PROJECT_SYSTEME_IO_H
