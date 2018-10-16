#include <sys/fcntl.h>
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


void diplayPlayer(player_t *player) {
    int i, j,k ,l;
    printf("\n");

    for (i = 0; i < NB_PLAYER; ++i) {
        printf("id:\t\t\t%d\t\t\t\t",player[i].id);

    }
    printf("\n");
    for (i = 0; i < NB_PLAYER; ++i) {
        printf("name:\t%s\t\t\t\t",player[i].name);
    }
    printf("\n");
    for (i = 0; i < NB_PLAYER; ++i) {
        printf("nb coup:\t%d\t\t\t\t",player[i].nb_coups);
    }
    printf("\n");
    for (i = 0; i < NB_PLAYER; ++i) {
        printf("has ended:\t%d\t\t\t\t",player[i].has_ended);
    }
    printf("\n");
    for (i = 0; i < NB_PLAYER; ++i) {
        printf("stable:\t\t\t\t\t\t");
    }
    printf("\n");
    for (i = 0; i < NB_HORSE_BY_PLAYER; ++i) {
        for (j = 0; j < NB_PLAYER; ++j) {
            printf("\tid:\t\t%d\t\t\t\t",player[j].stable[i].id);
        }
        printf("\n");
        for (j = 0; j < NB_PLAYER; ++j) {
            printf("\tpos:\t%d\t\t\t\t",player[j].stable[i].position);
        }
        printf("\n\n");
    }
    printf("\n");

}


char* printPlayer(player_t *player, char* printTo) {


    int f = open("file.txt",  O_WRONLY );

/* print some text */
    write(f,player, sizeof(player_t));

    close(f);


}

player_t readPlayer (){}

char *messageToFrame(message_t message) {
    return NULL;
}

message_t frameToMessage(char* frame) {
    message_t message;
    int size ;
    int id;

    //char tempSize[3] = {frame[0], frame[]};


    size = (int)(frame[0])*100;
    size += (int)(frame[1])*10;
    size += (int)(frame[2]);


    return message;
}

void displayBoardFromPlayersArray(player_t *players) {

}

void testParseurs(){

}