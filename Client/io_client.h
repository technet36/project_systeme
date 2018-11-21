//
// Created by moran on 07/11/2018.
//

#ifndef PROJECT_SYSTEME_IO_CLIENT_H
#define PROJECT_SYSTEME_IO_CLIENT_H
#define WIN


#ifdef WIN /* si vous êtes sous Windows */

#include <winsock2.h>

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


typedef struct {
    SOCKET meToServer, meToNext, meToLast;
} io_config_t;


typedef enum {DICE_ROLL, NEW_PLAYER, CHOOSE_HORSE, NEW_POS, MSG_LOOPBACK }ACTION_T;

typedef struct {
    int sizeOfData;
    int id;
    ACTION_T action;
    void* data;
}datagram_t;
/*

int sendHorseServer(int* idHorse, int fileDescriptor);

int sendPlayerToServer(player_t* me, int fileDescriptor);

int sendMessageToNextPlayer(int pid,player_t* playerArray, int fileDescriptor);

int sendMessage(int pid, int action, int fileDescriptor, void* data, int sizeOfData);

messageInfo_t waitForMessage(void* data, int fileDescriptorA, int fileDescriptorB);
*/
int initIO_client( char* ip, char* port, io_config_t* sock);

datagram_t waitForMessage(void* data, io_config_t* myConfig);

int sendMeToServer(player_t* me, io_config_t* socketTab);

int sendMessage(datagram_t* datagram, SOCKET dest);

void closeSocket(io_config_t* mySockets);

/*
 * initSocket()
 *
 * sentMeToServer()
 *
 * getPlayers()
 *
 * initPlayers()
 *
 * waitForMessage()
 *
 * sendToNextPlayer()
 *
 * sendToServer()
 *
 * endTurn()
 *
 * sentHorseServer()
 *
 * closeSocket()
 */


#endif //PROJECT_SYSTEME_IO_CLIENT_H
