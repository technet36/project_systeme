/*
 * Projet Système et Réseaux - L3 MIAGE
 * Maxence Morand
 * Thiam Cheikh
 *
 */

#ifndef PROJECT_SYSTEME_PLAYER_H
#define PROJECT_SYSTEME_PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "io_client.h"

#include "../structures.h"
#include "../display.h"

int main_PLAYER(int id, int ppid, int inLast, int inServer, int outNext, int outServer);


int initPlayer(int idPlayer, player_t* me);
#endif //PROJECT_SYSTEME_PLAYER_H
