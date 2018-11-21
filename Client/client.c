/*
 * Projet Système et Réseaux - L3 MIAGE
 * Maxence Morand
 * Thiam Cheikh
 *
 */
#include "client.h"
//#include <cygwin/in.h>


int initPlayer(player_t* me){
    askName(me->name);
    int j;
    me->has_ended = false;
    me->id = -1;
    me->nb_coups=0;
    for (j = 0; j < NB_HORSE_BY_PLAYER; ++j) {
        me->stable[j].id = j;
        me->stable[j].position = -1;
    }
    return 0;
}

/**
 *
 * @param argc has to be 3
 * @param argv ["client", ip_server, port_server]
 */
int main( int argc, char* argv[]){
    player_t me;
    player_t* allPlayer;
    //nt temp;
    int error =0;
    //messageInfo_t message;
    void* data = (void*)malloc(sizeof(player_t)*NB_PLAYER);
    io_config_t mySockets;


    initPlayer(&me);

    initIO_client(argv[1], argv[2], &mySockets);// -> init in/out server


    sendMeToServer(&me,&mySockets);

    //wait for server's response with id and the other player's sockets
    waitForMessage(data, &mySockets);
/*
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
*/
    sleep(50);

    closeSocket(&mySockets);
    printf("\n\n The end\n\n");
    return 0;
}
