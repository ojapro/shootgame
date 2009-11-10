#include "boolean.h"
#include "structs.h"
#include "dispflgs.h"

#ifndef _SCENE_H
#define _SCENE_H

    #define CHANGE_SCENE       1
    #define NOT_CHANGE_SCENE   0

    #define UPDATE_SCENE       1//âÊñ çXêVÇ∑ÇÈ
    #define NO_UPDATE_SCENE    0//ÇµÇ»Ç¢

    #define INVALID_SCENE_STATUS    0
    #define VALID_SCENE_STATUS      1
    #define NO_CHANGE_SCENE         2
    #define TRANSITION_SUCCESS      1

    #define SWITCH_FALED            0
    #define SWITCH_SUCCESS          1
    #define NOT_UPDATED             2
    #define UPDATE_SUCCESS          3



    #define SCENE_BLANK             0
    #define SCENE_TITLE             1
    #define SCENE_SELECT	        2
    #define SCENE_GAME		        3
    #define SCENE_SCORE	            4
    #define SCENE_NOWLOAD	        5

    #define NO_ACTIVE               0

    int UpdateScreen(Screen *);
    int InitState(Screen *);

#endif

