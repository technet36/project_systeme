//
// Created by moran on 07/11/2018.
//

#include "io_client.h"


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