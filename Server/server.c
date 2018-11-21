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

    int i=0, j, roll, nextPlayerId = 0, input, temp = 0, myAction, serverPid = getpid(), lastPlayer = 0;
    game_t theGame;
    io_config_t myIOConfig;
    player_t tempPlayer = {-1}, emptyPlayer = {0};

    nextPlayerId = init(&theGame);
    initIO_server(&myIOConfig, argv[2]);


    //wait for 4 players
    for(i=0;i<NB_PLAYER;++i){
        printf("\nWaiting for client connect(%d)",i);
        acceptClient(&myIOConfig,i);
    }

    for (i = 0; i < NB_PLAYER; ++i) {
        printf("\nWait for messages [%d]",i);
        tempPlayer = waitForPlayerConfig(&myIOConfig);
        displayPlayer(&tempPlayer);
        if(tempPlayer.id != -1){
            displayPlayer(&tempPlayer);
            copyPlayer(theGame.players,&tempPlayer);
            printf("\ngot player %d : %s\n",tempPlayer.id,tempPlayer.name);
        }
    }

    for(i=0;i<NB_PLAYER;++i){
        //send (lastaddr)
        printf("\nsendLastToOne(%d)",i);
        sendLastToOne(i,&myIOConfig);

    }

//wait for playerInfos


    /*
    while (i<NB_PLAYER){
        message = waitForPlayerMessageToServer(data, myPipes.inPx);

    }
/*
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

    closeIO(&myIOConfig);
    return 0;
}