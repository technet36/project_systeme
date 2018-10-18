//
// Created by moran on 11/10/2018.
//

#ifndef PROJECT_SYSTEME_PLAYER_H
#define PROJECT_SYSTEME_PLAYER_H
#include <stdio.h>
//#include "library.h"
#include "structures.h"
#include "io.h"
typedef struct {
    int inLast, inServer, outNext, outServer;
}pipes_PLAYER_t;

int main_PLAYER(int argc, char* argv[]);


int initPlayer(int idPlayer, player_t* me);
#endif //PROJECT_SYSTEME_PLAYER_H
