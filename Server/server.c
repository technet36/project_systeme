/*
 * Projet Système et Réseaux - L3 MIAGE
 * Maxence Morand
 * Thiam Cheikh
 *
 */
#include "io_server.h"
#include "library.h"
#include "../structures.h"
#include "../display.h"

//TODO : refactor io
//TODO : tests
//TODO : ad sockets
//TODO : add grafic

int main(int argc, char* argv[]) {

    int i=0, j, roll, nextPlayer = 0, input, temp = 0, myAction, serverPid = getpid(), lastPlayer = 0;

    printf("working");

    game_t theGame;
    nextPlayer = init(&theGame);
    messageInfo_t message ;
    player_t* tempPlayer;
    void* data = (void*)malloc(sizeof(player_t)*NB_PLAYER);
    io_config_t myIOConfig;

    initIO_server(&myIOConfig, "15000");

    //wait for 4 players
    for(i=0;i<NB_PLAYER;++i){
        acceptClient(&myIOConfig,i);
    }
    /*

    i=0;
    while (i<NB_PLAYER){
        message = waitForPlayerMessageToServer(data, myPipes.inPx);
        if(message.action == NEW_PLAYER){
            copyPlayer(theGame.players, data);
            printf("\ngot player %d : %s\n",((player_t*)data)->id,((player_t*)data)->name);
            ++i;
        }
    }

    //broadcast needed sockets
    broadCastPlayerArray(theGame.players, myPipes.outPx[nextPlayer]);
    displayGame(&theGame);
    do{

        fflush(0);
        roll = diceRoll();
        sendDiceRoll(&roll,myPipes.outPx[nextPlayer]);
        while( (message = waitForPlayerMessageToServer(data,myPipes.inPx)).action != CHOOSE_HORSE){
            //ignore the messages
        }
        //data is the horse choosen
        lastPlayer = nextPlayer;
        nextPlayer = play(&theGame, nextPlayer, *(int*)data, roll);
        displayGame(&theGame);
        broadCastPlayerArray(theGame.players, myPipes.outPx[lastPlayer]);

    }while((nextPlayer)!= -1);
*/
    displayGame(&theGame);

    return 0;
}