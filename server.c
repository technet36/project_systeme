#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "io.h"
#include "library.h"


int main(int argc, char* argv[]) {

    int i, roll, nextPlayer = 0, input, temp = 0, myAction, serverPid = getpid();
    char  myCommand[30];

    game_t theGame;
    nextPlayer = init(&theGame);
    messageInfo_t message ;
    player_t* tempPlayer;
    void* data = (void*)malloc(sizeof(player_t)*NB_PLAYER);

    for (i = 0; i < NB_PLAYER; ++i) {
        if(fork()==0){
            sprintf(myCommand, "./player %d %d", i, serverPid);
            execlp("/usr/bin/gnome-terminal", "gnome-terminal", "-e", myCommand ,NULL);

            exit(-1);
        } else{
        }
    }

    i=0;

    while (i<NB_PLAYER){
        message = waitForPlayerMessageToServer(data);
        if(message.action == NEW_PLAYER){
            copyPlayer(theGame.players, data);
            printf("\ngot player %d : %s\n",((player_t*)data)->id,((player_t*)data)->name);
            ++i;
        }
    }


    do{
        displayGame(&theGame);
        broadCastPlayerArray(theGame.players, nextPlayer);

        printf("\t%s : ",theGame.players[nextPlayer].name);
        roll = diceRoll();

        sendDiceRoll(&roll,nextPlayer);

        while( (message = waitForPlayerMessageToServer(data)).action != CHOOSE_HORSE){}
        //data is the horse choosen

        printf("\nhorse choosen %d",*(int*)data);

    }while((nextPlayer = play(&theGame, nextPlayer, *(int*)data, roll))!= -1);

    displayGame(&theGame);

    return 0;
}