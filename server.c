/*
 * Projet Système et Réseaux - L3 MIAGE
 * Maxence Morand
 * Thiam Cheikh
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "io.h"
#include "library.h"
#include "player.h"


int main(int argc, char* argv[]) {

    int i=0, j, roll, nextPlayer = 0, input, temp = 0, myAction, serverPid = getpid(), lastPlayer = 0;


    game_t theGame;
    nextPlayer = init(&theGame);
    messageInfo_t message ;
    player_t* tempPlayer;
    void* data = (void*)malloc(sizeof(player_t)*NB_PLAYER);
    pipes_t myPipes;


    int P0ToP1[2];
    int P1ToP2[2];
    int P2ToP3[2];
    int P3ToP0[2];

    int PxToServer[2];

    int serverToP0[2];
    int serverToP1[2];
    int serverToP2[2];
    int serverToP3[2];

    int pipeForChildren[4][4];
    int pipeForChildrenToClose[4][4];

    i += pipe(P0ToP1);
    i += pipe(P1ToP2);
    i += pipe(P2ToP3);
    i += pipe(P3ToP0);
    i += pipe(PxToServer);
    i += pipe(serverToP0);
    i += pipe(serverToP1);
    i += pipe(serverToP2);
    i += pipe(serverToP3);


    myPipes.inPx = PxToServer[0];
    myPipes.outPx[0] = serverToP0[1];
    myPipes.outPx[1] = serverToP1[1];
    myPipes.outPx[2] = serverToP2[1];
    myPipes.outPx[3] = serverToP3[1];

    //pipe[0] for reading, pipe[1] for writing
    pipeForChildren[0][0] = P3ToP0[0];      //P0 inLast
    pipeForChildren[0][1] = serverToP0[0];  //P0 inServer
    pipeForChildren[0][2] = P0ToP1[1];      //P0 outNext
    pipeForChildren[0][3] = PxToServer[1];  //P0 outServer

    pipeForChildren[1][0] = P0ToP1[0];      //P1 inLast
    pipeForChildren[1][1] = serverToP1[0];  //P1 inServer
    pipeForChildren[1][2] = P1ToP2[1];      //P1 outNext
    pipeForChildren[1][3] = PxToServer[1];  //P1 outServer

    pipeForChildren[2][0] = P1ToP2[0];      //P2 inLast
    pipeForChildren[2][1] = serverToP2[0];  //P2 inServer
    pipeForChildren[2][2] = P2ToP3[1];      //P2 outNext
    pipeForChildren[2][3] = PxToServer[1];  //P2 outServer

    pipeForChildren[3][0] = P2ToP3[0];      //P3 inLast
    pipeForChildren[3][1] = serverToP3[0];  //P3 inServer
    pipeForChildren[3][2] = P3ToP0[1];      //P3 outNext
    pipeForChildren[3][3] = PxToServer[1];  //P3 outServer


    pipeForChildrenToClose[0][0] = P3ToP0[1];
    pipeForChildrenToClose[0][1] = serverToP0[1];
    pipeForChildrenToClose[0][2] = P0ToP1[0];
    pipeForChildrenToClose[0][3] = PxToServer[0];

    pipeForChildrenToClose[1][0] = P0ToP1[1];
    pipeForChildrenToClose[1][1] = serverToP1[1];
    pipeForChildrenToClose[1][2] = P1ToP2[0];
    pipeForChildrenToClose[1][3] = PxToServer[0];

    pipeForChildrenToClose[2][0] = P1ToP2[1];
    pipeForChildrenToClose[2][1] = serverToP2[1];
    pipeForChildrenToClose[2][2] = P2ToP3[0];
    pipeForChildrenToClose[2][3] = PxToServer[0];

    pipeForChildrenToClose[3][0] = P2ToP3[1];
    pipeForChildrenToClose[3][1] = serverToP3[1];
    pipeForChildrenToClose[3][2] = P3ToP0[0];
    pipeForChildrenToClose[3][3] = PxToServer[0];

    for (i = 0; i < NB_PLAYER; ++i) {
        if((temp = fork())==0){
            for (j = 0; j < 4; ++j) {
                close(pipeForChildrenToClose[i][j]);
            }
            main_PLAYER(i, serverPid, pipeForChildren[i][0], pipeForChildren[i][1], pipeForChildren[i][2], pipeForChildren[i][3]);
            exit(0);
        }
    }
    close(P0ToP1[0]);
    close(P1ToP2[0]);
    close(P2ToP3[0]);
    close(P3ToP0[0]);

    close(P0ToP1[1]);
    close(P1ToP2[1]);
    close(P2ToP3[1]);
    close(P3ToP0[1]);

    close(serverToP0[0]);
    close(serverToP1[0]);
    close(serverToP2[0]);
    close(serverToP3[0]);

    close(PxToServer[1]);

    i=0;
    while (i<NB_PLAYER){
        message = waitForPlayerMessageToServer(data, myPipes.inPx);
        if(message.action == NEW_PLAYER){
            copyPlayer(theGame.players, data);
            printf("\ngot player %d : %s\n",((player_t*)data)->id,((player_t*)data)->name);
            ++i;
        }
    }

    broadCastPlayerArray(theGame.players, myPipes.outPx[nextPlayer]);
    displayGame(&theGame);
    do{

        fflush(0);
        roll = diceRoll();
        sendDiceRoll(&roll,myPipes.outPx[nextPlayer]);
        while( (message = waitForPlayerMessageToServer(data,myPipes.inPx)).action != CHOOSE_HORSE){/*ignore the messages*/}
        //data is the horse choosen
        lastPlayer = nextPlayer;
        nextPlayer = play(&theGame, nextPlayer, *(int*)data, roll);
        displayGame(&theGame);
        broadCastPlayerArray(theGame.players, myPipes.outPx[lastPlayer]);

    }while((nextPlayer)!= -1);

    displayGame(&theGame);

    return 0;
}