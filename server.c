#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "io.h"
#include "library.h"


int main(int argc, char* argv[]) {

    int i, roll, nextPlayer = 0, input, temp = 0, myAction;
    char  myCommand[30];

    game_t theGame;
    nextPlayer = init(&theGame);
    messageInfo_t message ;
    player_t* tempPlayer;
    void* data = (void*)malloc(sizeof(player_t)*NB_PLAYER);

    printf("Start 4 players \n");
    for (i = 0; i < NB_PLAYER; ++i) {
        if(fork()==0){
            sprintf(myCommand, "./player %d %d", i,getpid());
            execlp("/usr/bin/gnome-terminal", "gnome-terminal", "-e", myCommand ,NULL);
            //execl("/usr/bin/xterm", "xterm", NULL);

            exit(-1);
        } else{
            printf("exec player %d\n",i);
            sleep(1);
        }
    }

    i=0;

    while (i<NB_PLAYER){
        message = waitForPlayerMessageToServer(data);


        //printf("\nmessage.action : %d, message.pid : %d\n",message.action, message.pid);


        if(message.action == NEW_PLAYER){
            copyPlayer(theGame.players, data);
            printf("\ngot player %d : %s\n",((player_t*)data)->id,((player_t*)data)->name);
            ++i;
        }
        printf("\n\t wait for player press enter");
        //getchar();
    }

    //if (test()!=0)return 0;
displayPlayer(&theGame.players[0]);
displayPlayer(&theGame.players[1]);
displayPlayer(&theGame.players[2]);
displayPlayer(&theGame.players[3]);


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