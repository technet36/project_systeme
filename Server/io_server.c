//
// Created by moran on 07/11/2018.
//

#include "io_server.h"


void initIO_server(io_config_t* socketTab, char* serverPort){

    struct hostent *hostinfo = NULL;
    /*socketTab->clients[0] = {0};
    socketTab->clients[1] = {0};
    socketTab->clients[2] = {0};
    socketTab->clients[3] = {0};
    socketTab->serverAddr = {0};*/

#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
    printf("\nsocket()");
    if( (socketTab->serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        perror("socket()");
        exit(errno);
    }


    socketTab->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    socketTab->serverAddr.sin_family = AF_INET;

    socketTab->serverAddr.sin_port = htons(15000);
    printf("\nbind()");

    if( bind(socketTab->serverSocket, (SOCKADDR *) &socketTab->serverAddr, sizeof (SOCKADDR)) == SOCKET_ERROR)
    {
        perror("bind()");
        exit(errno);
    }

    printf("\nlisten()");
    if(listen(socketTab->serverSocket, 5) == SOCKET_ERROR)
    {
        perror("listen()");
        exit(errno);
    }

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

int sendMessage(int pid, int action, int fileDescriptor, void* data, int sizeOfData) {

    int bytesRead =0;
    bytesRead += write(fileDescriptor,&sizeOfData, sizeof(int));
    bytesRead += write(fileDescriptor, &pid, sizeof(int));
    bytesRead += write(fileDescriptor, &action , sizeof(int));
    bytesRead += write(fileDescriptor, data, sizeOfData);

    return (bytesRead == (sizeOfData + 3* sizeof(int) ) ) -1;//return -1 if error
}

int closeIO(io_config_t* socketTab){
    closesocket(socketTab->serverSocket);
#ifdef WIN32
    WSACleanup();
#endif
    return 0;
}