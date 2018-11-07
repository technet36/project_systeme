//
// Created by moran on 07/11/2018.
//

#include "io_client.h"

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

    i = sendMessage(getpid(), CHOOSE_HORSE, fileDescriptor, idHorse, sizeof(int));

    return i;
}

int sendPlayerToServer(player_t* me, int fileDescriptor){
    int i;
    i = sendMessage(getpid(), NEW_PLAYER, fileDescriptor, me, sizeof(player_t));
    return i;
}

int sendMessageToNextPlayer(int pid, player_t* playerArray, int fileDescriptor){
    int i;
    i = sendMessage(pid, NEW_POS, fileDescriptor, playerArray, sizeof(player_t) * NB_PLAYER);

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
    int fileToRead = fileDescriptorServer, sizeToRead, temp = 0, i=0;
    struct timeval tv;
    messageInfo_t message;
    message.action = -1;
    message.pid = -1;
    tv.tv_sec = 3;
    tv.tv_usec =0;
    fd_set fileDescriptorSet;

    FD_ZERO( &fileDescriptorSet);
    FD_SET( fileDescriptorSibling, &fileDescriptorSet);
    FD_SET( fileDescriptorServer, &fileDescriptorSet);

    temp = select(max(fileDescriptorServer,fileDescriptorSibling)+1, &fileDescriptorSet, NULL, NULL, &tv);

    fileToRead = FD_ISSET(fileDescriptorServer, &fileDescriptorSet)? fileDescriptorServer : fileDescriptorSibling;

    i += read(fileToRead, &sizeToRead, sizeof(int) );
    i += read(fileToRead, &message.pid, sizeof(int));
    i += read(fileToRead, &message.action, sizeof(int));
    i += read(fileToRead, data, sizeToRead);

    if (message.pid == getpid() && message.action == NEW_POS) message.action = MSG_LOOPBACK;

    return message;
}
int displayError(error_t *error) {
    if (error->childFuncName !=NULL && error->funcName!=NULL && error->errCode != 0){
        fprintf(stderr,"\n%s() \t->\t%s() \t->\t %s\n",error->funcName,error->childFuncName, error->msg);
    } else if (error->errCode!=0){
        fprintf(stderr,"\n%s() \t-> \t%s\n", error->funcName, error->msg);
    }
    return error->errCode;
}