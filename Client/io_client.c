//
// Created by moran on 07/11/2018.
//

#include "io_client.h"


/*

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

*/


datagram_t waitForMessage(void* data, io_config_t* myConfig) {
    int sizeToRead=0, i=0;
    char* tempRead;
    tempRead = malloc(sizeof(int));
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec =0;
    fd_set fileDescriptorSet;
    SOCKET socketToRead;
    datagram_t message;

    FD_ZERO( &fileDescriptorSet);
    FD_SET( myConfig->meToLast, &fileDescriptorSet);
    FD_SET( myConfig->meToServer, &fileDescriptorSet);

    select((int)max(myConfig->meToLast,myConfig->meToServer)+1, &fileDescriptorSet, NULL, NULL, (PTIMEVAL) &tv);

    socketToRead = FD_ISSET(myConfig->meToLast, &fileDescriptorSet)? myConfig->meToLast : myConfig->meToServer;

    recv(socketToRead, tempRead, sizeof(int),0);
    message.sizeOfData = atoi(tempRead);

    recv(socketToRead,tempRead , sizeof(int),0);
    message.id = atoi(tempRead);

    recv(socketToRead,tempRead , sizeof(int),0);
    message.action = (ACTION_T) atoi(tempRead);

    recv(socketToRead, data, message.sizeOfData,0);
    message.data = malloc((size_t) sizeToRead);
    message.data = data;

    //if (message.pid == getpid() && message.action == NEW_POS) message.action = MSG_LOOPBACK;

    SOCKADDR_IN* temp = (SOCKADDR_IN *) data;
    printf("\naddr: O port: %d",temp->sin_port);
    return message;
}

int initIO_client(char *serverName, char *serverPort, io_config_t* sockTab) {

    struct hostent *serverInfo = NULL;
    SOCKADDR_IN tempAddr;
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif


    if((sockTab->meToServer = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        perror("socket()");
        exit(errno);
    }

    serverInfo = gethostbyname(serverName); /* on récupère les informations de l'hôte auquel on veut se connecter */


    if (serverInfo == NULL) /* l'hôte n'existe pas */
    {
        fprintf (stderr, "Unknown host %s.\n", serverName);
        exit(EXIT_FAILURE);
    }
    printf("\nPort : %s",serverPort);

    tempAddr.sin_addr = *(IN_ADDR *) serverInfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure serverInfo */
    tempAddr.sin_port = htons(atoi(serverPort)); /* on utilise htons pour le serverPort */
    tempAddr.sin_family = AF_INET;

    if(connect(sockTab->meToServer,(SOCKADDR *) &tempAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        perror("connect()");
        closeSocket(sockTab);
        exit(errno);
    }

    return 0;
}

//int connectToLast(SOCKADDR* lastAddr, ){}

void closeSocket(io_config_t* mySockets) {

    closesocket(mySockets->meToServer);
    closesocket(mySockets->meToLast);
    closesocket(mySockets->meToNext);
#ifdef WIN32
    WSACleanup();
#endif
}
