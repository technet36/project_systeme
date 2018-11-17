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
*/
int initIO_client(char *serverName, char *serverPort, io_config_t* sockTab) {

    struct hostent *serverInfo = NULL;

#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif


    if((sockTab->mySocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        perror("socket()");
        exit(errno);
    }
    //bind(sockTab->mySocket, (SOCKADDR *) , sizeof(SOCKADDR));

    serverInfo = gethostbyname(serverName); /* on récupère les informations de l'hôte auquel on veut se connecter */
    //printf()
    if (serverInfo == NULL) /* l'hôte n'existe pas */
    {
        fprintf (stderr, "Unknown host %s.\n", serverName);
        exit(EXIT_FAILURE);
    }

    sockTab->serverAddr.sin_addr = *(IN_ADDR *) serverInfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure serverInfo */
    sockTab->serverAddr.sin_port = htons(15000); /* on utilise htons pour le serverPort */
    sockTab->serverAddr.sin_family = AF_INET;

    if(connect(sockTab->mySocket,(SOCKADDR *) &sockTab->serverAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        perror("connect()");
        closeSocket(sockTab);
        exit(errno);
    }

    return 0;
}

void closeSocket(io_config_t* mySockets) {

    closesocket(mySockets->mySocket);
#ifdef WIN32
    WSACleanup();
#endif
}
