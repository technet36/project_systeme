#include "player.h"

int initPlayer(int idPlayer, player_t* me){
    printf("\nHi there new player(%d)\nWhat is your name ? : \n",idPlayer);
    fflush(0);
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

int main_PLAYER(int id, int ppid, int inLast, int inServer, int outNext, int outServer){
    player_t me;
    player_t* allPlayer;
    int godFather;
    int temp;
    int error =0;
    messageInfo_t message;
    void* data = (void*)malloc(sizeof(player_t)*NB_PLAYER);
    pipes_PLAYER_t myPipes;

    myPipes.inLast = inLast;
    myPipes.inServer = inServer;
    myPipes.outNext = outNext;
    myPipes.outServer = outServer;
    godFather = ppid;


    initPlayer(id, &me);

    sendPlayerToServer(&me, myPipes.outServer);
    do{
        message = waitForMessage(data, myPipes.inLast, myPipes.inServer);
        if (message.action == NEW_POS){//data is the array of all players
            allPlayer = data;
            if(message.pid == godFather) { // New position from server
                //printf("got message from server\n");
                error = sendMessageToNextPlayer( getpid(), allPlayer, myPipes.outNext);

            }else if(message.pid == getpid()) {
                //Loopback
                //todo : sig to server

            }else {
                //New position from the last player
                //printf("\ngot update from last player");
                error = sendMessageToNextPlayer( message.pid, allPlayer, myPipes.outNext);
            }

        }else if(message.action == DICE_ROLL ){//data is the result of the dice roll
            printf("\nIt's your turn %s, ENTER to roll the dice :",me.name);
            //scanf()
            while(getchar()!=EOF && getchar()!='\n');
            printf("\nYou've rolled a %d, choose a horse : ",*(int*)data);
            scanf("%d",&temp);
            error = sendHorseServer( &temp, myPipes.outServer);

        }
    }while (!me.has_ended && error ==0);

    printf("\n\n The end\n\n");
    return 0;
}
