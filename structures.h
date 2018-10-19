/*
 * Projet Système et Réseaux - L3 MIAGE
 * Maxence Morand
 * Thiam Cheikh
 *
 */
#ifndef PROJECT_SYSTEME_STRUCTURES_H
#define PROJECT_SYSTEME_STRUCTURES_H


#define NB_PLAYER 4
#define NB_HORSE_BY_PLAYER 4
#define NB_SQUARE_BOARD 80
#define NB_STAIRS_BY_PLAYER 6
#define NB_SQUARE_BY_PLAYER 14


typedef enum {false, true} bool;

typedef struct {
    int errCode;
    char *childFuncName;
    char *funcName;
    char msg[100];
}error_t;

typedef struct {
    int id_horse;
    int id_player;
} square_t;

/**
 * position = -1 -> stable
 * position = -2 -> end
 */
typedef struct {
    int id;
    int position;
} horse_t;

typedef  struct {
    int id;
    char name[10];
    horse_t stable[NB_HORSE_BY_PLAYER];
    int nb_coups;
    bool has_ended;
} player_t;

/**
 * board[0->55] -> race
 * board[56->61] -> stairs
 */
typedef struct {
    square_t board[NB_SQUARE_BOARD];
    player_t players[NB_PLAYER];
    bool has_ended;
} game_t;


#endif //PROJECT_SYSTEME_STRUCTURES_H
