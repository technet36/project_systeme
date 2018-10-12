#include "library.h"



//#############################
//-----PRIVATE FONCTIONS--------
//#############################

int displayError(error_t* error){
    if (error->childFuncName !=NULL && error->funcName!=NULL && error->errCode != 0){
        fprintf(stderr,"\n%s() \t->\t%s() \t->\t %s\n",error->funcName,error->childFuncName, error->msg);
    } else if (error->errCode!=0){
        fprintf(stderr,"\n%s() \t-> \t%s\n", error->funcName, error->msg);
    }
    return error->errCode;
}

void initPlayers(player_t *players) {
    int i ,j;
    char* playerName[4];
    playerName[0] = "toto_0";
    playerName[1] = "toto_1";
    playerName[2] = "toto_2";
    playerName[3] = "toto_3";
    for (i = 0; i < NB_PLAYER; ++i) {
        players[i].has_ended = false;
        players[i].name = playerName[i];
        players[i].id = i;
        players[i].nb_coups=0;
        for (j = 0; j < NB_HORSE_BY_PLAYER; ++j) {
            players[i].stable[j].id = j;
            players[i].stable[j].position = -1;
        }
    }

}

int goToSquare (game_t *theGame, int idPlayer, int idHorse, int newPosition){
    if ( newPosition >= NB_SQUARE_BOARD || newPosition < -2 || idPlayer<0 || idPlayer >= NB_PLAYER || idHorse < 0 || idHorse >= NB_HORSE_BY_PLAYER ){//bad arguments
        error_t newError;
        newError.funcName = __func__;
        newError.errCode = 1;
        newError.childFuncName = NULL;

        sprintf(newError.msg,"invalid arguments : idPlayer : %d , idHorse : %d , newPosition : %d", idPlayer, idHorse, newPosition);
        displayError(&newError);
        return -1;
    }
    int i;
    int oldPosition = theGame->players[idPlayer].stable[idHorse].position;

    if (newPosition == -2){//#################if horse just ended check if it was last horse
        theGame->players[idPlayer].has_ended = true;
        for (i = 0; i < NB_HORSE_BY_PLAYER; ++i) {//check if player has ended
            if (theGame->players[idPlayer].stable[i].position != -2) theGame->players[idPlayer].has_ended = false;
        }
    }


    //##############updates begins here

    if ( theGame->board[newPosition].id_horse != -1){//################if the square is not empty
        theGame->players[ theGame->board[newPosition].id_player ].stable[ theGame->board[newPosition].id_horse ].position = -1;
    }
    if (newPosition > -1){//#########  put horse in new square
        theGame->board[newPosition].id_horse = idHorse;
        theGame->board[newPosition].id_player = idPlayer;
    }
    if (oldPosition != -1){//########## empty the last position
        theGame->board[oldPosition].id_horse = -1;
        theGame->board[oldPosition].id_player = -1;
    }
    //################ set the rigth position to the horse
    theGame->players[idPlayer].stable[idHorse].position = newPosition;

    return 0;
}

int nextStair(int position, int stairsEntrancePosition, int firstStair){

    if (position == stairsEntrancePosition ){//first stair
        return firstStair;
    } else if(position == (firstStair+NB_STAIRS_BY_PLAYER-1)) {//last stair
        return -2;
    } else {//others
        return position+1;
    }
}

/**
 * @param game
 * @param position
 * @param dice
 * @param stairsEntrancePosition
 * @return the next position
 */
int isThereObstacle(game_t *game, int position, int dice, int stairsEntrancePosition) {
    int i;

    for (i = 1 ; i < dice; ++i) {
        if(game->board[(i+position)%(NB_SQUARE_BY_PLAYER*NB_PLAYER)].id_player != -1 || (i+position)%(NB_SQUARE_BY_PLAYER*NB_PLAYER) == stairsEntrancePosition){//there is an obstacle
            return position + 2 * i - dice;
        }
    }
    return (dice+position)%(NB_SQUARE_BY_PLAYER*NB_PLAYER);
}


//#############################
//-----PUBLIC FONCTIONS--------
//#############################


int diceRoll() {
    return (rand()%6)+1;
}

int init(game_t* myGame) {
    srand((unsigned int)time(0));//seed for rand()
    myGame->has_ended = false;
    int i;
    initPlayers(myGame->players);
    for (i = 0; i < NB_SQUARE_BOARD; ++i) {
        myGame->board[i].id_horse = -1;
        myGame->board[i].id_player = -1;
    }
    return 0;
}

void display(game_t *theGame) {
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

int play(game_t* theGame, int idPlayer, int idHorse, int dice) {

    int i;
    theGame->players[idPlayer].nb_coups++;
    int stairsEntrancePosition = (NB_SQUARE_BY_PLAYER*NB_PLAYER+NB_SQUARE_BY_PLAYER*idPlayer-1)%(NB_PLAYER*NB_SQUARE_BY_PLAYER);
    int firstStair = NB_SQUARE_BY_PLAYER*NB_PLAYER+NB_STAIRS_BY_PLAYER*idPlayer;
    int position = theGame->players[idPlayer].stable[idHorse].position;
    //invalid horse->next player , other argument trusted todo : test 0<dice<7
    if (idHorse>=NB_HORSE_BY_PLAYER || idHorse<0){
        for (i = idPlayer+1; i != idPlayer; ++i) {
            i%=NB_PLAYER;
            if (!theGame->players[i].has_ended) return i;
        }
        return idPlayer;//play again if he is the last one
    }


    if(position == -2){//############################################################################### already ended
        return idPlayer;
    } else if (position == -1) {//###################################################################### wanna escape stable
        if (dice==6){
            goToSquare(theGame, idPlayer, idHorse, NB_SQUARE_BY_PLAYER*idPlayer);
        }
    } else if (position == stairsEntrancePosition || position >= firstStair) {//######################## wanna go on the stairs

        int newPosition = nextStair(position, stairsEntrancePosition, firstStair);

        if(newPosition == -2 && dice == 6 ){
            goToSquare(theGame, idPlayer, idHorse, newPosition);
        } else if (dice == (newPosition - firstStair + 1)){
            goToSquare(theGame, idPlayer, idHorse, newPosition);
        }

    } else {//########################################################################################## casual case on the race
        int nextPosition = isThereObstacle(theGame, position, dice, stairsEntrancePosition);

        goToSquare(theGame,idPlayer, idHorse, nextPosition);
    }


    if(dice == 6 && !theGame->players[idPlayer].has_ended){
        theGame->players[idPlayer].nb_coups--;
        return idPlayer;
    } else {
        for (i = idPlayer+1; i != idPlayer; ++i) {
            if (!theGame->players[i%NB_PLAYER].has_ended) return i%NB_PLAYER;
        }
        return -1; //game end
    }
}

int testWhereHorseShouldBe(game_t *testGame,int idPlayer, int idHorse, int position, error_t* error){
    error->childFuncName = __func__;
    error->errCode = 0;
    int i;

    //Check argument
    if(idHorse >= NB_HORSE_BY_PLAYER || idHorse < 0 || idPlayer >= NB_PLAYER || idPlayer < 0 || position >= NB_SQUARE_BOARD || position<-2){
        error->errCode = 1;
        sprintf(error->msg,"Invalid argument idPlayer = %d , idHorse = %d , position = %d", idPlayer, idHorse, position);
    }
    //Check player[]
    else if(testGame->players[idPlayer].stable[idHorse].position != position){
        error->errCode = 2;
        sprintf(error->msg,"players[%d].stable[%d].position = %d  but should be %d", idPlayer, idHorse, testGame->players[idPlayer].stable[idHorse].position , position);
    }
    //Check board[]
    else if ( position>0 && testGame->board[position].id_player != idPlayer && testGame->board[position].id_horse != idHorse){
        error->errCode = 2;
        sprintf(error->msg,"testGame->board[%d]= {idPlayer : %d,idHorse : %d} but should be {idPlayer : %d,idHorse : %d}", position, testGame->board[position].id_player,testGame->board[position].id_horse, idPlayer, idHorse);
    }
    //Check duplicate
    else {
        for (i = 0; i < NB_SQUARE_BOARD; ++i) {
            if(testGame->board[i].id_horse == idHorse && testGame->board[i].id_player == idPlayer){
                sprintf(error->msg,"Found duplicate horse on board[%d] when it sould be at %d", i, position);
            }
        }
    }
    return error->errCode;
}

int testInit( error_t* error){
    game_t game;
    init(&game);
    int i,j,temp=0;
    error->funcName = __func__;
    for (i = 0; i < NB_PLAYER; ++i) {
        for (j = 0; j < NB_HORSE_BY_PLAYER; ++j) {
            if (testWhereHorseShouldBe(&game,i ,j, -1, error)!= 0){
                displayError(error);
                ++temp;
            }
        }
    }
    return temp;
}

int testStart( error_t* error){
    game_t game;
    init(&game);
    error->funcName = __func__;
    int nextPlayer , i, errorCount=0;

    for (i = 0; i < 6; ++i) {//######## test for non 6 dice roll
        nextPlayer =  play(&game,0,0,i);
        if (game.board[0].id_player != -1 || game.board[0].id_horse != -1 || game.players[0].stable[0].position != -1 ){
            error->errCode = 2;
            sprintf(error->msg, "Has escape stable with a %d dice roll: expected {idPlayer : -1 , idHorse : -1 , position : -1 } got {%d , %d , %d }", i, game.board[0].id_player, game.board[0].id_horse, game.players[0].stable[0].position);
            displayError(error);
            ++errorCount;
        }
        if (nextPlayer != 1){
            error->errCode = 3;
            sprintf(error->msg, "Allowed to play after rolling a %d , expected {nextPlayer: 0 } got {%d }", i, nextPlayer);
            displayError(error);
            ++errorCount;
        }
    }

    nextPlayer =  play(&game,0,0,6);

    if (game.board[0].id_player != 0 || game.board[0].id_horse != 0 || game.players[0].stable[0].position != 0 ){
        error->errCode = 2;
        sprintf(error->msg, "Could not escape stable : expected {idPlayer : 0 , idHorse : 0 , position : 0 } got {%d , %d , %d }",game.board[0].id_player, game.board[0].id_horse, game.players[0].stable[0].position);
        displayError(error);
        ++errorCount;
    }
    if (nextPlayer != 0){
        error->errCode = 3;
        sprintf(error->msg, "Not allowed to play after rolling a 6 , expected {nextPlayer: 0 } got {%d }",nextPlayer);
        displayError(error);
        ++errorCount;
    }

    return errorCount;
}

int testObstacle( error_t* error){
    game_t game;
    init(&game);
    error->funcName = __func__;
    error->childFuncName = NULL;
    int nextPosition, errorCount=0;

    play(&game, 0,0,6);
    play(&game, 0,0,3);
    play(&game, 0,1,6);

    /*
     * current state :
     * [01|--|--|00|--|--
     */


    if ((nextPosition = isThereObstacle(&game, 0,5,55) )!= 1){
        error->childFuncName = NULL;
        error->errCode = 1;
        sprintf(error->msg,"Invalid result from isThereObstacle() expected , got %d", nextPosition);
        displayError(error);
        ++errorCount;
    }

    play(&game, 0,1,5);

    /*
     * should be
     * [--|01|--|00|--
     */


    if (testWhereHorseShouldBe(&game,0,1,1,error)!= 0){
        displayError(error);
        ++errorCount;
    }

    init(&game);

    play(&game, 0,0,6);
    play(&game, 0,0,54);

    /*
     * current state :
     * 50|51|52|53|54|55|56|57|58
     * --|--|--|--|00|--|S1|S2|S3
     */

    if ((nextPosition = isThereObstacle(&game, 54,4,55) )!= 52){
        error->childFuncName = NULL;
        error->errCode = 1;
        sprintf(error->msg,"Invalid result from isThereObstacle() expected 52, got %d", nextPosition);
        displayError(error);
        ++errorCount;
    }
    play(&game, 0,0,4);

    /*
     * should be :
     * 50|51|52|53|54|55|56|57|58
     * --|--|00|--|--|--|S1|S2|S3
     */
    if (testWhereHorseShouldBe(&game,0,0,52,error)!= 0){
        displayError(error);
        ++errorCount;
    }


    return errorCount;
}

/**
 *
 * @return the number of error
 */
int test() {
    int errorCount = 0, errorCountTemp=0;
    error_t newError;

    if ((errorCountTemp = testInit(&newError)) == 0){
        fprintf(stdout,"\n\n====================================================\n\t\t\ttestInit() pass  :: total error : %d\n====================================================\n",errorCount);
    }else {
        errorCount +=errorCountTemp;
        fprintf(stderr,"\n\n====================================================\n\t\t\ttestInit() NOT pass error : %d | total error : %d\n====================================================\n",errorCountTemp,errorCount);
    }



    if ((errorCountTemp = testStart(&newError)) == 0){
        fprintf(stdout,"\n\n====================================================\n\t\t\ttestStart() pass  :: total error : %d\n====================================================\n",errorCount);
    }else {
        errorCount +=errorCountTemp;
        fprintf(stderr,"\n\n====================================================\n\t\t\ttestStart() NOT pass error : %d | total error : %d\n====================================================\n",errorCountTemp,errorCount);
    }


    if ((errorCountTemp = testObstacle(&newError)) == 0){
        fprintf(stdout,"\n\n====================================================\n\t\t\ttestObstacle() pass  :: total error : %d\n====================================================\n",errorCount);
    }else {
        errorCount +=errorCountTemp;
        fprintf(stderr,"\n\n====================================================\n\t\t\ttestObstacle() NOT pass error : %d | total error : %d\n====================================================\n",errorCountTemp,errorCount);
    }


    //todo : unitary test

    //todo : functional tests


    if (errorCount==0){
        fprintf(stdout,"\n\n====================================================\n\t\t\tAll test pass  : congratulation %d\n====================================================\n",errorCount);
    }else {

        fprintf(stderr,"\n\n====================================================\n\t\t\tAll test NOT pass  total error : %d\n====================================================\n",errorCount);
    }

    return errorCount;
}
