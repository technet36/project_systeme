/*
 * Projet Système et Réseaux - L3 MIAGE
 * Maxence Morand
 * Thiam Cheikh
 *
 */
#ifndef PROJECT_SYSTEME_IO_H
#define PROJECT_SYSTEME_IO_H

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>

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

typedef struct {
    int inLast, inServer, outNext, outServer;
}pipes_PLAYER_t;

int displayError(error_t* error);

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

int sendHorseServer(int* idHorse, int fileDescriptor);

int sendPlayerToServer(player_t* me, int fileDescriptor);

int sendMessageToNextPlayer(int pid,player_t* playerArray, int fileDescriptor);

int sendMessage(int pid, int action, int fileDescriptor, void* data, int sizeOfData);

messageInfo_t waitForMessage(void* data, int fileDescriptorA, int fileDescriptorB);

messageInfo_t waitForPlayerMessageToServer(void* data, int fileDescriptor);

int broadCastPlayerArray(player_t* playerArray, int fileDescriptor);

#endif //PROJECT_SYSTEME_IO_H
