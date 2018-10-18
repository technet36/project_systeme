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
    void* data = (void*)malloc(sizeof(player_t)*NB_PLAYER);


    if (argc==3){
        godFather = atoi(argv[2]);
        printf("\nThis is player %s\n",argv[1]);
        initPlayer(atoi(argv[1]), &me);

    }else{
        printf("\nerror\n");
        godFather = getppid();
        initPlayer(2, &me);
        exit(-1);
    }

    sendPlayerToServer(&me);


    do{
        message = waitForMessage(data, &me);
        if (message.action == NEW_POS){//data is the array of all players
            allPlayer = data;
            if(message.pid == godFather) { // New position from server
                error = sendMessageToNextPlayer( getpid(), allPlayer, &me);

            }else if(message.pid == getpid()) {
                //Loopback
                //todo : sig to server

            }else {
                //New position from the last player
                error = sendMessageToNextPlayer( message.pid, allPlayer, &me);
            }

        }else if(message.action == DICE_ROLL ){//data is the result of the dice roll
            printf("\nIt's your turn, ENTER to roll the dice : ");
            getchar();
            printf("\nYou've rolled a %d, choose a horse : ",*(int*)data);
            scanf("%d",&temp);
            error = sendHorseServer( &temp, &me);

        }
    }while (!me.has_ended && error ==0);

    printf("\n\n The end\n\n");
    getchar();
    return 0;
}
