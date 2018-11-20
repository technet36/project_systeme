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
        exit(errno);
    }
    if(listen(socketTab->serverSocket, 5) == SOCKET_ERROR){
        perror("listen()");
        exit(errno);
    }

}

void createServerAndListen(io_config_t* socketTab, char* serverPort){
}

void sendLastToOne(int idReceiver, io_config_t* sockTab){
    SOCKADDR addrToSend;
    int sockLen;
    int idLast = ( idReceiver + NB_PLAYER - 1)%NB_PLAYER;
    getpeername(sockTab->clients[idLast], &addrToSend, &sockLen );
    sendMessage(idReceiver, ADDRESS_LAST, sockTab, &addrToSend, sockLen);

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

messageInfo_t waitForPlayerMessageToServer(void* data, int fileDescriptor){

    int f, sizeToRead = 0, i=0, temp = 0;
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

int sendMessage(int id, ACTION_T action, io_config_t* socketTab, void* data, int sizeOfData) {

    int error, sizeToSend = sizeOfData + 3 * sizeof(int);
    datagram_t datagram;

    datagram.sizeOfData = sizeOfData;
    datagram.id = id;
    datagram.action = action;
    datagram.data = data;
    error = send(socketTab->clients[id], &datagram, sizeToSend,0);

    return error;
    /*
    bytesRead += write(fileDescriptor,&sizeOfData, sizeof(int));
    bytesRead += write(fileDescriptor, &pid, sizeof(int));
    bytesRead += write(fileDescriptor, &action , sizeof(int));
    bytesRead += write(fileDescriptor, data, sizeOfData);

    return (bytesRead == (sizeOfData + 3* sizeof(int) ) ) -1;//return -1 if error
*/
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