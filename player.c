#include "player.h"

int initPlayer(int idPlayer, player_t* me){
    printf("\nHi there new player\nWhat is you name ? : \n");
    scanf("%s",me->name);
    int j;
    me->has_ended = false;
    me->id = idPlayer;
    me->nb_coups=0;
    for (j = 0; j < NB_HORSE_BY_PLAYER; ++j) {
        me->stable[j].id = j;
        me->stable[j].position = -1;
    }
    return 0;
}

int main_PLAYER(int argc, char* argv[]){
    player_t me;
    player_t* allPlayer;
    int godFather;
    int temp;
    int error =0;
    messageInfo_t message;
    void* data = (void*)malloc(sizeof(player_t)*NB_PLAYER);
    pipes_PLAYER_t myPipes;

    if (argc==7){
        myPipes.inLast = atoi(argv[3]);
        myPipes.inServer = atoi(argv[4]);
        myPipes.outNext = atoi(argv[5]);
        myPipes.outServer = atoi(argv[6]);
        godFather = atoi(argv[2]);/*
        printf("\nThis is player %s\n",argv[1]);
        printf(" \npipe.inLast : %d",myPipes.inLast);
        printf(" \npipe.inServer : %d",myPipes.inServer);
        printf(" \npipe.outNext : %d",myPipes.outNext);
        printf(" \npipe.outServer : %d",myPipes.outServer);*/


        initPlayer(atoi(argv[1]), &me);

    }else{
        printf("\nerror ENTER to exit\n");
        getchar();
        exit(-1);
    }

    sendPlayerToServer(&me, myPipes.outServer);


    do{
        message = waitForMessage(data, myPipes.inLast, myPipes.inServer);
        if (message.action == NEW_POS){//data is the array of all players
            allPlayer = data;
            if(message.pid == godFather) { // New position from server
                error = sendMessageToNextPlayer( getpid(), allPlayer, myPipes.outNext);

            }else if(message.pid == getpid()) {
                //Loopback
                //todo : sig to server

            }else {
                //New position from the last player
                error = sendMessageToNextPlayer( message.pid, allPlayer, myPipes.outNext);
            }

        }else if(message.action == DICE_ROLL ){//data is the result of the dice roll
            printf("\nIt's your turn, ENTER to roll the dice : ");
            getchar();
            printf("\nYou've rolled a %d, choose a horse : ",*(int*)data);
            scanf("%d",&temp);
            error = sendHorseServer( &temp, myPipes.outServer);

        }
    }while (!me.has_ended && error ==0);

    printf("\n\n The end\n\n");
    getchar();
    return 0;
}
