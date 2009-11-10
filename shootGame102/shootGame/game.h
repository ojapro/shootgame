#include "boolean.h"
#include "structs.h"
#include "dispflgs.h"

#ifndef _STAGE_H
#define _STAGE_H

    #define STAGE_WIDTH     680
    #define STAGE_HEIGHT    480

    #define MY_FIGHTER 1
    #define ENEMY_SMALL 0
    #define ENEMY_HARD 1
    #define ENEMY_FAST 2

    #define FIGHTER_WIDTH 50
    #define FIGHTER_HEIGHT 50
    #define ENEMY_WIDTH 30
    #define ENEMY_HEIGHT 30

    #define BULLET_NOMAL    0
    #define BULLET_MISSILE  1
    #define BULLET_HOMING   2

    #define SET_CTRL_FALED      0
    #define SET_CTRL_SUCCESS    1

    #define ENEMY_ACTION_FALED      0
    #define ENEMY_ACTION_SUCCESS    1

    //ä÷êîêÈåæ

    Command createCommand(char *);
    char* getStringWithLimitString(char *,char *,char);
    HitPoint hit(HitPoint, int);
    HitPoint heal(HitPoint, int);

    extern Boolean LoadScenario(char *,Scenario *);
    int InitGame(Screen *);
    int DrawGame(Screen *);
    HitPoint hit(HitPoint,int);
    Boolean LoadFile();
    int SetControl(Enemy);
    int EnemyAction(Enemy *);




#endif
