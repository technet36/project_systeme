#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "io.h"
#include "library.h"


int main(int argc, char* argv[]) {

    int i, roll, nextPlayer = 0, input, temp;
    char  myCommand[30];
    game_t theGame;
    nextPlayer = init(&theGame);
    messageInfo_t message;
    player_t* tempPlayer;
    void* data;

    printf("Start 4 players ");
    for (i = 0; i < NB_PLAYER; ++i) {
        if(fork()==0){
            sprintf(myCommand, "./player %d %d", i,getpid());
            execlp("/usr/bin/gnome-terminal", "gnome-terminal", "-e", myCommand ,NULL);
            //execl("/usr/bin/xterm", "xterm", NULL);

            exit(-1);
        } else{
            sleep(2);
        }

    }

    i=0;

    while (i<NB_PLAYER){
        message = waitForPlayerMessageToServer(data);
        if(message.action == NEW_PLAYER){
            tempPlayer = data;
            copyPlayer(theGame.players, tempPlayer);
            printf("\ngot player %d : %s\n",tempPlayer->id,tempPlayer->name);
            ++i;
        }
    }

    if (test()!=0)return 0;



    do{
        displayGame(&theGame);
        printf("%s : ",theGame.players[nextPlayer].name);
        roll = diceRoll();
        printf(" Roll : %d\nChoose the horse : ",roll);

        scanf("%d",&input);

        //diplayPlayer(theGame.players);

    }while((nextPlayer = play(&theGame, nextPlayer, input, roll))!= -1);

    displayGame(&theGame);

    return 0;
}