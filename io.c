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



int sendHorseServer(int idHorse, player_t* me){
    int f , i;
    char* fileDescriptorName;

    if (me->id==0) fileDescriptorName = "P0ToServer";
    if (me->id==1) fileDescriptorName = "P1ToServer";
    if (me->id==2) fileDescriptorName = "P2ToServer";
    if (me->id==3) fileDescriptorName = "P3ToServer";

    f = open(fileDescriptorName,  O_WRONLY | O_APPEND| O_TRUNC | O_CREAT ); // should not need the file but does in fact
    i = sendMessage(getpid(), CHOOSE_HORSE, f, &idHorse, sizeof(int));
    close(f);

    return i;
}

int sendPlayerToServer(player_t* me){
    int f , i;
    char* fileDescriptorName;

    if (me->id==0) fileDescriptorName = "P0ToServer";
    if (me->id==1) fileDescriptorName = "P1ToServer";
    if (me->id==2) fileDescriptorName = "P2ToServer";
    if (me->id==3) fileDescriptorName = "P3ToServer";

    f = open(fileDescriptorName,  O_WRONLY | O_APPEND| O_TRUNC | O_CREAT ); // should not need the file but does in fact
    i = sendMessage(getpid(), NEW_PLAYER, f, me, sizeof(player_t));
    printf("P%d sent player %d",me->id,i);
    close(f);
    return i;
}

int sendMessageToNextPlayer(int pid, player_t* playerArray, player_t* me){
    char* fileDescriptorName;
    int f, i;

    if (me->id==0) fileDescriptorName = "P0ToP1";
    if (me->id==1) fileDescriptorName = "P1ToP2";
    if (me->id==2) fileDescriptorName = "P2ToP3";
    if (me->id==3) fileDescriptorName = "P3ToP0";


    f = open(fileDescriptorName,  O_WRONLY | O_APPEND| O_TRUNC | O_CREAT ); // should not need the file but does in fact
    i = sendMessage(pid, NEW_POS, f, playerArray, sizeof(player_t) * NB_PLAYER);

    close(f);
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

messageInfo_t waitForMessage(void* data, player_t* me) {
    int f, sizeToRead, temp = 0;
    messageInfo_t message;
    message.action = -1;
    message.pid = -1;


    char* fileDescriptorParentName;
    char* fileDescriptorSiblingName;

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

    while(temp==0){
        if(read(fileFromParent,&sizeToRead, sizeof(int) ) != -1){
            temp = 1;
            f = fileFromParent;

        } else if(read(fileFromSibling,&sizeToRead, sizeof(int) ) != -1){
            temp = 1;
            f = fileFromSibling;
        }
    }
    data = (void*)malloc(sizeToRead);

    read(f, &message.pid, sizeof(int));
    read(f, &message.action, sizeof(int));
    read(f, data, sizeToRead);

    close(fileFromParent);
    close(fileFromSibling);

    if (message.pid == getpid() && message.action == NEW_POS) message.action = MSG_LOOPBACK;


    return message;
}

messageInfo_t waitForPlayerMessageToServer(void* data){

    int f, sizeToRead, temp = 0;
    messageInfo_t message;
    message.action = -1;
    message.pid = -1;


    int fileFromP0 = open("P0ToServer",  O_RDONLY );//need the file
    int fileFromP1 = open("P1ToServer",  O_RDONLY );//need the file
    int fileFromP2 = open("P2ToServer",  O_RDONLY );//need the file
    int fileFromP3 = open("P3ToServer",  O_RDONLY );//need the file

    while(temp==0){
        if(read(fileFromP0,&sizeToRead, sizeof(int) ) != -1){
            temp = 1;
            f = fileFromP0;

        } else if(read(fileFromP1,&sizeToRead, sizeof(int) ) != -1){
            temp = 1;
            f = fileFromP1;

        }else if(read(fileFromP2,&sizeToRead, sizeof(int) ) != -1){
            temp = 1;
            f = fileFromP2;

        }else if(read(fileFromP3,&sizeToRead, sizeof(int) ) != -1){
            temp = 1;
            f = fileFromP3;
        }
    }
    data = (void*)malloc(sizeToRead);

    read(f, &message.pid, sizeof(int));
    read(f, &message.action, sizeof(int));
    read(f, data, sizeToRead);

    close(fileFromP0);
    close(fileFromP1);
    close(fileFromP2);
    close(fileFromP3);

    return message;
}




/*

void testParseurs(){

}

int printPlayer(player_t *player, char* printTo) {
    int f = open("filsToPere.txt",  O_WRONLY | O_APPEND | O_TRUNC | O_CREAT); // should not need the file but does in fact

    if(write(f,player, sizeof(player_t)) == -1){
        perror("Write");
        close(f);
        return -1;
    }
    close(f);

    return 0;

}

int readPlayer (player_t* newPlayer){
    int f = open("filsToPere.txt",  O_RDONLY );//need the file

    if (read(f,newPlayer, sizeof(player_t) ) == -1){
        perror("read");
        close(f);
        return -1;
    }
    close(f);
    return 0;
}
*/