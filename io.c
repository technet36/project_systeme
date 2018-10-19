#include "io.h"


int displayError(error_t *error) {
    if (error->childFuncName !=NULL && error->funcName!=NULL && error->errCode != 0){
        fprintf(stderr,"\n%s() \t->\t%s() \t->\t %s\n",error->funcName,error->childFuncName, error->msg);
    } else if (error->errCode!=0){
        fprintf(stderr,"\n%s() \t-> \t%s\n", error->funcName, error->msg);
    }
    return error->errCode;
}

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

void displayBoardFromPlayersArray(player_t *players) {

}



int sendHorseServer(int* idHorse, int fileDescriptor){
    int i;

    //f = open("PToServer",  O_WRONLY | O_APPEND| O_TRUNC | O_CREAT ); // should not need the file but does in fact
    i = sendMessage(getpid(), CHOOSE_HORSE, fileDescriptor, idHorse, sizeof(int));
    //close(f);

    return i;
}

int sendPlayerToServer(player_t* me, int fileDescriptor){
    int i;
    //f = open("PToServer",  O_WRONLY | O_APPEND| O_TRUNC | O_CREAT ); // should not need the file but does in fact
    i = sendMessage(getpid(), NEW_PLAYER, fileDescriptor, me, sizeof(player_t));
    //close(f);
    return i;
}

int sendMessageToNextPlayer(int pid, player_t* playerArray, int fileDescriptor){
    //char* fileDescriptorName;
    int i;
/*
    if (me->id==0) fileDescriptorName = "P0ToP1";
    if (me->id==1) fileDescriptorName = "P1ToP2";
    if (me->id==2) fileDescriptorName = "P2ToP3";
    if (me->id==3) fileDescriptorName = "P3ToP0";


    f = open(fileDescriptorName,  O_WRONLY | O_APPEND| O_TRUNC | O_CREAT ); // should not need the file but does in fact
    */
    i = sendMessage(pid, NEW_POS, fileDescriptor, playerArray, sizeof(player_t) * NB_PLAYER);

    //close(f);
    return i;

}

int sendMessage(int pid, int action, int fileDescriptor, void* data, int sizeOfData) {

    int bytesRead =0;
    bytesRead += write(fileDescriptor,&sizeOfData, sizeof(int));
    bytesRead += write(fileDescriptor, &pid, sizeof(int));
    bytesRead += write(fileDescriptor, &action , sizeof(int));
    bytesRead += write(fileDescriptor, data, sizeOfData);

    return (bytesRead == (sizeOfData + 3* sizeof(int) ) ) -1;//return -1 if error
}

messageInfo_t waitForMessage(void* data, int fileDescriptorSibling, int fileDescriptorServer) {
    int fileToRead, sizeToRead, temp = 0, i=0;
    messageInfo_t message;
    message.action = -1;
    message.pid = -1;
    fd_set fileDescriptorSet;
    FD_ZERO( &fileDescriptorSet);
    FD_SET( fileDescriptorSibling, &fileDescriptorSet);
    FD_SET( fileDescriptorServer, &fileDescriptorSet);

printf("\n waitForMessage()\n");

    /*char* fileDescriptorParentName;
    //char* fileDescriptorSiblingName;
    //char* fileNameRead;
    //int fileToEmpty;

    if (me->id==0) {
        fileDescriptorParentName = "ServerToP0";
        fileDescriptorSiblingName = "P3ToP0";
    }else if (me->id==1) {
        fileDescriptorParentName = "ServerToP1";
        fileDescriptorSiblingName = "P0ToP1";
    } else if (me->id==2) {
        fileDescriptorParentName = "ServerToP2";
        fileDescriptorSiblingName = "P1ToP2";
    }else if (me->id==3) {
        fileDescriptorParentName = "ServerToP3";
        fileDescriptorSiblingName = "P2ToP3";
    }

    int fileFromParent = open(fileDescriptorParentName,  O_RDONLY );//need the file
    int fileFromSibling= open(fileDescriptorSiblingName,  O_RDONLY );//need the file

  //  while(temp==0){
        if(read(fileDescriptorServer,&sizeToRead, sizeof(int) ) > 0){
    //        temp = 1;
            f = fileDescriptorServer;
    //        fileNameRead = fileDescriptorParentName;

        } else if(read( fileDescriptorSibling,&sizeToRead, sizeof(int) ) > 0){
    //        temp = 1;
            f = fileDescriptorSibling;
    //        fileNameRead = fileDescriptorSiblingName;
        }
    //}*/

    printf("fd parent: %d, fd child : %d\n",fileDescriptorServer, fileDescriptorSibling);
    select(2, &fileDescriptorSet, NULL, NULL, NULL);
    fileToRead = FD_ISSET(fileDescriptorServer, &fileDescriptorSet)?fileDescriptorServer:fileDescriptorSibling;


    i += read(fileToRead, &sizeToRead, sizeof(int) );
    i += read(fileToRead, &message.pid, sizeof(int));
    i += read(fileToRead, &message.action, sizeof(int));
    i += read(fileToRead, data, sizeToRead);
printf("i : %d,fileToRead: %d\n",i,fileToRead);
    //close(fileFromParent);
    //close(fileFromSibling);

    //to empty the file
    /*fileToEmpty = open(fileNameRead, O_WRONLY | O_APPEND| O_TRUNC | O_CREAT);
    close(fileToEmpty);*/

    if (message.pid == getpid() && message.action == NEW_POS) message.action = MSG_LOOPBACK;

    return message;
}



int sendDiceRoll(int* dice, int fileDescriptor){
    //char* fileDescriptorName;
    int f, i;

    /*if (nextPlayer==0) fileDescriptorName = "ServerToP0";
    if (nextPlayer==1) fileDescriptorName = "ServerToP1";
    if (nextPlayer==2) fileDescriptorName = "ServerToP2";
    if (nextPlayer==3) fileDescriptorName = "ServerToP3";


    f = open(fileDescriptorName,  O_WRONLY | O_APPEND| O_TRUNC | O_CREAT ); // should not need the file but does in fact
    */
    i = sendMessage(getpid(), DICE_ROLL, fileDescriptor, dice, sizeof(int));


    //close(f);
    return i;

}

messageInfo_t waitForPlayerMessageToServer(void* data, int fileDescriptor){

    int f, sizeToRead, i=0, temp = 0;
    messageInfo_t message;
    message.action = -1;
    message.pid = -1;
    //int fileToServer = open("PToServer",  O_RDONLY );//need the file

    i += read(fileDescriptor,&sizeToRead, sizeof(int));
    i += read(fileDescriptor, &message.pid, sizeof(int));
    i += read(fileDescriptor, &message.action, sizeof(int));
    i += read(fileDescriptor, data, sizeToRead);
    //close(fileToServer);
    //to empty the file
    //fileToServer = open("PToServer", O_WRONLY | O_APPEND| O_TRUNC | O_CREAT);
    //close(fileToServer);
printf("i : %d fd: %d \n",i,fileDescriptor);

    return message;
}

int broadCastPlayerArray(player_t* playerArray, int fileDescriptor) {
    int f, i;/*
    char* fileDescriptorName;

    if (nextPlayer==0) fileDescriptorName = "ServerToP0";
    if (nextPlayer==1) fileDescriptorName = "ServerToP1";
    if (nextPlayer==2) fileDescriptorName = "ServerToP2";
    if (nextPlayer==3) fileDescriptorName = "ServerToP3";

    f = open(fileDescriptorName,  O_WRONLY | O_APPEND| O_TRUNC | O_CREAT ); // should not need the file but does in fact
*/
    i = sendMessage(getpid(), NEW_POS, fileDescriptor, playerArray, sizeof(player_t) * NB_PLAYER);
    //close(f);
printf("i : %d fd: %d \n",i,fileDescriptor);
    return (i== sizeof(player_t)*NB_PLAYER*NB_PLAYER)-1;

}
