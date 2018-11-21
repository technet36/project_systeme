//
// Created by moran on 07/11/2018.
//

#include "io_server.h"


void initIO_server(io_config_t* socketTab, char* serverPort){

    SOCKADDR_IN tempAddr;
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0){
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
    if( (socketTab->serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        perror("socket()");
        exit(errno);
    }

    tempAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    tempAddr.sin_family = AF_INET;
    tempAddr.sin_port = htons(atoi(serverPort));

    if( bind(socketTab->serverSocket, (SOCKADDR *) &tempAddr, sizeof (SOCKADDR)) == SOCKET_ERROR){
        perror("bind()");
        closeIO(socketTab);
        exit(errno);
    }
    if(listen(socketTab->serverSocket, 5) == SOCKET_ERROR){
        perror("listen()");
        closeIO(socketTab);
        exit(errno);
    }

}

int sendLastToOne(int idReceiver, io_config_t* sockTab){
    datagram_t datagram;
    SOCKADDR addrToSend;
    int sockLen;
    int idLast = ( idReceiver + NB_PLAYER - 1)%NB_PLAYER;
    getpeername(sockTab->clients[idLast], &addrToSend, &sockLen );

    datagram.action = ADDRESS_LAST;
    datagram.id = idReceiver;
    datagram.sizeOfData = sizeof(SOCKADDR);
    memcpy(datagram.data,&addrToSend, sizeof(SOCKADDR));

    return sendMessageToClient(sockTab, &datagram);
}

void acceptClient(io_config_t* configSocket, int id){
    printf("\nacceptClient()");
    SOCKADDR_IN csin = {0};
    int addrSize = sizeof(csin);
int temp;
    if( id < 0 || id >= sizeof(configSocket->clients)
        || -1 == ( configSocket->clients[id] = accept(configSocket->serverSocket, (SOCKADDR *) &csin, &addrSize ) ) ){
        perror("accept ()");
        closeIO(configSocket);
        exit(errno);
    }
    getpeername(configSocket->clients[id], (SOCKADDR *) &csin, &addrSize);
    printf("\nid: %d, port: %d",id,csin.sin_port);
}

int sendDiceRoll(int* dice, int fileDescriptor){
    int f, i;

    //i = sendMessage(getpid(), DICE_ROLL, fileDescriptor, dice, sizeof(int));
    return i;

}

player_t waitForPlayerConfig(io_config_t* myConfig){
    datagram_t message;
    int i = 0, errCode;
    for(i=0;(errCode = receiveMessageFromClient(&message,myConfig)) != 0 && i < 4;++i );
    if (i > 3 && errCode==0 && message.action == NEW_PLAYER) {
        return *(player_t *) message.data;
    } else {
        return (player_t) {-1};
    }
}
/*
int broadCastPlayerArray(player_t* playerArray, int fileDescriptor) {
    int f, i;
    //i = sendMessage(getpid(), NEW_POS, fileDescriptor, playerArray, sizeof(player_t) * NB_PLAYER);
    return (i== sizeof(player_t)*NB_PLAYER*NB_PLAYER)-1;
}*/

/**
 * @param datagram empty
 * @param ioConf with array of clients
 * @return 0 on success
 */
int receiveMessageFromClient(datagram_t* datagram, io_config_t* ioConf){
    int i,bytesRead = 0;
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec =0;
    fd_set fileDescriptorSet;
    SOCKET socketToRead = 0;
    char* tempRead;
    tempRead = malloc(sizeof(int));

    FD_ZERO( &fileDescriptorSet);

    for (i = 0; i < NB_PLAYER; ++i) {
        FD_SET( ioConf->clients[i], &fileDescriptorSet);
    }

    // todo: remove max4 and do a function using NB_PLAYER
    if(select((int)max4(ioConf->clients), &fileDescriptorSet, NULL, NULL, (PTIMEVAL) &tv) == -1)
    {
        perror("select()");
        closeIO(ioConf);
        return -1;
    }

    for (i = 0; i < NB_PLAYER; ++i) {
        if (FD_ISSET(ioConf->clients[i], &fileDescriptorSet)) {
            socketToRead = ioConf->clients[i];
            i = NB_PLAYER;
        }
    }

    bytesRead += recv(socketToRead, tempRead, sizeof(int),0);
    datagram->sizeOfData = atoi(tempRead);

    bytesRead += recv(socketToRead,tempRead , sizeof(int),0);
    datagram->id = atoi(tempRead);

    bytesRead += recv(socketToRead,tempRead , sizeof(int),0);
    datagram->action = (ACTION_T) atoi(tempRead);

    bytesRead += recv(socketToRead, datagram->data, datagram->sizeOfData,0);

    return bytesRead - (datagram->sizeOfData + 3 * sizeof(int));
}

/**
 * @param socketTab
 * @param datagram full
 * @return 0 on success
 */
int sendMessageToClient(io_config_t* socketTab, datagram_t* datagram) {
    int bytesSent, sizeToSend = datagram->sizeOfData + 3 * sizeof(int);
    bytesSent = send(socketTab->clients[datagram->id], (char*) datagram, sizeToSend,0);
    if(bytesSent == -1){
        perror("send()");
        closeIO(socketTab);
        return -1;
    }
    return bytesSent - (datagram->sizeOfData + 3 * sizeof(int));
}

int closeIO(io_config_t* socketTab){
    closesocket(socketTab->serverSocket);
    closesocket(socketTab->clients[0]);
    closesocket(socketTab->clients[1]);
    closesocket(socketTab->clients[2]);
    closesocket(socketTab->clients[3]);
#ifdef WIN32
    WSACleanup();
#endif
    return 0;
}