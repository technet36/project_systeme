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

int main(int argc, char* argv[]){
    player_t me;
    player_t* allPlayer;
    int godFather;
    int temp;
    int error =0;
    messageInfo_t message;
    void* data;
    if (argc==3){
        printf("\nThis is player %s\n",argv[1]);
        initPlayer(atoi(argv[1]), &me);
        godFather = atoi(argv[2]);

    }else{
        godFather = getppid();
        initPlayer(2, &me);

    }


    sendPlayerToServer(&me);


    do{
        message = waitForMessage(data, &me);
        if (message.action == NEW_POS){//data is the array of all players
            allPlayer = data;
            if(message.pid == godFather) {
                printf("\nNew position\n");
                error = sendMessageToNextPlayer( getpid(), allPlayer, &me);

            }else if(message.pid == getpid()) {
                printf("\nLoopback\n");
                //todo : sig to server

            }else {
                printf("\nNew position\n");
                error = sendMessageToNextPlayer( message.pid, allPlayer, &me);
            }

        }else if(message.action == DICE_ROLL ){//data is the result of the dice roll
            printf("\nIt's your turn %s, press ENTER to roll the dice : ",me.name);
            getchar();
            printf("\nYou've rolled a %d, choose a horse : ",data);
            scanf("%d",&temp);
            error = sendHorseServer( temp, &me);

        }
    }while (!me.has_ended && error ==0);


    return 0;
}
