#include <stdio.h>
#include <unistd.h>
#include "library.h"


int main(int argc, char* argv[]) {

    int i, roll, nextPlayer = 0, input;

    game_t theGame;
    nextPlayer = init(&theGame);

    if (test()!=0)return 0;

    printf("Start 4 players ");
    for (i = 0; i < NB_PLAYER; ++i) {
        execlp("./player","./player",(char)i,(char)NULL);
    }



    do{
        display(&theGame);
        printf("%s : ",theGame.players[nextPlayer].name);
        roll = diceRoll();
        printf(" Roll : %d\nChoose the horse : ",roll);

        scanf("%d",&input);

        //diplayPlayer(theGame.players);

    }while((nextPlayer = play(&theGame, nextPlayer, input, roll))!= -1);

    display(&theGame);

    return 0;
}