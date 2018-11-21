//
// Created by moran on 07/11/2018.
//

#ifndef PROJECT_SYSTEME_IO_SERVER_H
#define PROJECT_SYSTEME_IO_SERVER_H

#define WIN
#ifdef WIN /* si vous êtes sous Windows */

#include <winsock2.h>
#include <zconf.h>

#elif defined (linux) /* si vous êtes sous Linux */

//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else /* sinon vous êtes sur une plateforme non supportée */

#error not defined for this platform

#endif

#include "../structures.h"
#include "../display.h"
#include <zconf.h>



#define  max(a,b) ({(a)>(b)?(a):(b);})
#define  max4(a) ({ max ( max((a)[0],(a)[1]), max((a)[2],(a)[3]) ); })

typedef enum {DICE_ROLL, NEW_PLAYER, CHOOSE_HORSE, NEW_POS, MSG_LOOPBACK, ADDRESS_LAST }ACTION_T;

typedef struct {
    int sizeOfData;
    int id;
    ACTION_T action;
    char data[1024];
}datagram_t;

typedef struct {
    SOCKET clients[4];
    SOCKET serverSocket;
} io_config_t;


int closeIO(io_config_t* config);

void initIO_server(io_config_t* io_config, char* port);

void acceptClient(io_config_t* configSocket, int id);

//int sendDiceRoll(int* dice, int fileDescriptor);

player_t waitForPlayerConfig(io_config_t* myConfig);

int sendMessageToClient(io_config_t* socketTab, datagram_t* message);

int receiveMessageFromClient(datagram_t* message, io_config_t* socket);

//int broadCastPlayerArray(player_t* playerArray, int fileDescriptor);

int sendLastToOne(int idReceiver, io_config_t* sockTab);

#endif //PROJECT_SYSTEME_IO_SERVER_H
