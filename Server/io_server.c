//
// Created by moran on 07/11/2018.
//

#include "io_server.h"


void displayGame(game_t *theGame) {
    int i, j;
    printf("\n");
    for (j = 0; j < NB_PLAYER; ++j) {


        printf("\n\n%s:\n",theGame->players[j].name);
        for (i = 0; i < NB_HORSE_BY_PLAYER+2; ++i) {
            printf("_");
        }
        printf("\t\t");
        for (i = 0; i < NB_SQUARE_BY_PLAYER; ++i) {
            printf("___");
        }
        printf("\t\t");
        for (i = 0; i < NB_STAIRS_BY_PLAYER; ++i) {
            printf("__");
        }
        printf("\t\t");
        for (i = 0; i < NB_HORSE_BY_PLAYER+2; ++i) {
            printf("_");
        }
        printf("\n");



        printf("|");
        for (i = 0; i < NB_HORSE_BY_PLAYER; ++i) {
            printf("%c",(char)(theGame->players[j].stable[i].position!=-1?'-':48+theGame->players[j].stable[i].id));//48 = ASCII code of '0'
        }
        printf("|\t\t");

        for (i = j*NB_SQUARE_BY_PLAYER; i < (j+1)*NB_SQUARE_BY_PLAYER; ++i) {
            printf("|%c%c",(char)(theGame->board[i].id_player==-1?'-':48+theGame->board[i].id_player), (char)theGame->board[i].id_horse==-1?'-':48+theGame->board[i].id_horse);//48 = ASCII code of '0'
        }
        printf("|\t\t");
        for (i = 0; i < NB_STAIRS_BY_PLAYER; ++i) {
            printf("|%c",(char)theGame->board[NB_PLAYER*NB_SQUARE_BY_PLAYER+j*NB_STAIRS_BY_PLAYER+i].id_horse==-1?'-':48+theGame->board[NB_PLAYER*NB_SQUARE_BY_PLAYER+i].id_horse);//48 = ASCII code of '0'
        }
        printf("|\t\t|");
        for (i = 0; i < NB_HORSE_BY_PLAYER; ++i) {
            printf("%c",(char)(theGame->players[j].stable[i].position!=-2?'-':48+theGame->players[j].stable[i].id));//48 = ASCII code of '0'
        }
        printf("|\n");




        for (i = 0; i < NB_HORSE_BY_PLAYER+2; ++i) {
            printf("¯");
        }
        printf("\t\t");
        for (i = 0; i < NB_SQUARE_BY_PLAYER; ++i) {
            printf("¯¯¯");
        }
        printf("\t\t");
        for (i = 0; i < NB_STAIRS_BY_PLAYER; ++i) {
            printf("¯¯");
        }
        printf("\t\t");
        for (i = 0; i < NB_HORSE_BY_PLAYER+2; ++i) {
            printf("¯");
        }
        printf("\n");
    }
}

void displayPlayer(player_t *player) {
    int i, j,k ,l;
    printf("\n");
    printf("id:\t\t\t%d\t\t\t\t",player->id);
    printf("\n");
    printf("name:\t%s\t\t\t\t",player->name);
    printf("\n");
    printf("nb coup:\t%d\t\t\t\t",player->nb_coups);
    printf("\n");
    printf("has ended:\t%d\t\t\t\t",player->has_ended);
    printf("\n");
    printf("stable:\t\t\t\t\t\t");
    printf("\n");
    for (i = 0; i < NB_HORSE_BY_PLAYER; ++i) {
        printf("\tid:\t\t%d\t\t\t\t",player->stable[i].id);
        printf("\n");
        printf("\tpos:\t%d\t\t\t\t",player->stable[i].position);
        printf("\n\n");
    }
    printf("\n");

}
void initIO(pipes_t* myPipes, int pipeForChildren[4][4], int pipeForChildrenToClose[4][4]){
    int i=0;
    int P0ToP1[2];
    int P1ToP2[2];
    int P2ToP3[2];
    int P3ToP0[2];

    int PxToServer[2];

    int serverToP0[2];
    int serverToP1[2];
    int serverToP2[2];
    int serverToP3[2];


    i += pipe(P0ToP1);
    i += pipe(P1ToP2);
    i += pipe(P2ToP3);
    i += pipe(P3ToP0);
    i += pipe(PxToServer);
    i += pipe(serverToP0);
    i += pipe(serverToP1);
    i += pipe(serverToP2);
    i += pipe(serverToP3);


    myPipes->inPx = PxToServer[0];
    myPipes->outPx[0] = serverToP0[1];
    myPipes->outPx[1] = serverToP1[1];
    myPipes->outPx[2] = serverToP2[1];
    myPipes->outPx[3] = serverToP3[1];

    //pipe[0] for reading, pipe[1] for writing
    pipeForChildren[0][0] = P3ToP0[0];      //P0 inLast
    pipeForChildren[0][1] = serverToP0[0];  //P0 inServer
    pipeForChildren[0][2] = P0ToP1[1];      //P0 outNext
    pipeForChildren[0][3] = PxToServer[1];  //P0 outServer

    pipeForChildren[1][0] = P0ToP1[0];      //P1 inLast
    pipeForChildren[1][1] = serverToP1[0];  //P1 inServer
    pipeForChildren[1][2] = P1ToP2[1];      //P1 outNext
    pipeForChildren[1][3] = PxToServer[1];  //P1 outServer

    pipeForChildren[2][0] = P1ToP2[0];      //P2 inLast
    pipeForChildren[2][1] = serverToP2[0];  //P2 inServer
    pipeForChildren[2][2] = P2ToP3[1];      //P2 outNext
    pipeForChildren[2][3] = PxToServer[1];  //P2 outServer

    pipeForChildren[3][0] = P2ToP3[0];      //P3 inLast
    pipeForChildren[3][1] = serverToP3[0];  //P3 inServer
    pipeForChildren[3][2] = P3ToP0[1];      //P3 outNext
    pipeForChildren[3][3] = PxToServer[1];  //P3 outServer


    pipeForChildrenToClose[0][0] = P3ToP0[1];
    pipeForChildrenToClose[0][1] = serverToP0[1];
    pipeForChildrenToClose[0][2] = P0ToP1[0];
    pipeForChildrenToClose[0][3] = PxToServer[0];

    pipeForChildrenToClose[1][0] = P0ToP1[1];
    pipeForChildrenToClose[1][1] = serverToP1[1];
    pipeForChildrenToClose[1][2] = P1ToP2[0];
    pipeForChildrenToClose[1][3] = PxToServer[0];

    pipeForChildrenToClose[2][0] = P1ToP2[1];
    pipeForChildrenToClose[2][1] = serverToP2[1];
    pipeForChildrenToClose[2][2] = P2ToP3[0];
    pipeForChildrenToClose[2][3] = PxToServer[0];

    pipeForChildrenToClose[3][0] = P2ToP3[1];
    pipeForChildrenToClose[3][1] = serverToP3[1];
    pipeForChildrenToClose[3][2] = P3ToP0[0];
    pipeForChildrenToClose[3][3] = PxToServer[0];

}

int sendDiceRoll(int* dice, int fileDescriptor){
    int f, i;

    i = sendMessage(getpid(), DICE_ROLL, fileDescriptor, dice, sizeof(int));

    return i;

}

void displayBoardFromPlayersArray(player_t *players) {

}

messageInfo_t waitForPlayerMessageToServer(void* data, int fileDescriptor){

    int f, sizeToRead, i=0, temp = 0;
    messageInfo_t message;
    message.action = -1;
    message.pid = -1;

    i += read(fileDescriptor,&sizeToRead, sizeof(int));
    i += read(fileDescriptor, &message.pid, sizeof(int));
    i += read(fileDescriptor, &message.action, sizeof(int));
    i += read(fileDescriptor, data, sizeToRead);

    return message;
}

int broadCastPlayerArray(player_t* playerArray, int fileDescriptor) {
    int f, i;
    i = sendMessage(getpid(), NEW_POS, fileDescriptor, playerArray, sizeof(player_t) * NB_PLAYER);
    return (i== sizeof(player_t)*NB_PLAYER*NB_PLAYER)-1;

}

int sendMessage(int pid, int action, int fileDescriptor, void* data, int sizeOfData) {

    int bytesRead =0;
    bytesRead += write(fileDescriptor,&sizeOfData, sizeof(int));
    bytesRead += write(fileDescriptor, &pid, sizeof(int));
    bytesRead += write(fileDescriptor, &action , sizeof(int));
    bytesRead += write(fileDescriptor, data, sizeOfData);

    return (bytesRead == (sizeOfData + 3* sizeof(int) ) ) -1;//return -1 if error
}
int displayError(error_t *error) {
    if (error->childFuncName !=NULL && error->funcName!=NULL && error->errCode != 0){
        fprintf(stderr,"\n%s() \t->\t%s() \t->\t %s\n",error->funcName,error->childFuncName, error->msg);
    } else if (error->errCode!=0){
        fprintf(stderr,"\n%s() \t-> \t%s\n", error->funcName, error->msg);
    }
    return error->errCode;
}