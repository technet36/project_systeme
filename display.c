//
// Created by moran on 07/11/2018.
//

#include "display.h"

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


int displayError(error_t *error) {
    if (error->childFuncName !=NULL && error->funcName!=NULL && error->errCode != 0){
        fprintf(stderr,"\n%s() \t->\t%s() \t->\t %s\n",error->funcName,error->childFuncName, error->msg);
    } else if (error->errCode!=0){
        fprintf(stderr,"\n%s() \t-> \t%s\n", error->funcName, error->msg);
    }
    return error->errCode;
}