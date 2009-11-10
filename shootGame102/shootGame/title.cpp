#include "title.h"
#include "DxLib.h"

int DrawTitle(Screen *screen){
    //printfDx("DRAW_TITLE\n");
    return DRAW_SUCCESS;
}

//Ç†Ç∆Ç≈ï™ó£Ç∑ÇÈ
int DrawSelect(Screen *){
    //printfDx("DRAW_SELECT\n");
    return DRAW_SUCCESS;
}
int DrawScore(Screen *){
    //printfDx("DRAW_SCORE\n");
    return DRAW_SUCCESS;
}
int DrawNowLoad(Screen *){
    //printfDx("DRAW_NOWLOAD\n");
    return DRAW_SUCCESS;
}
/********************************************************
******************âÊñ ëJà⁄ÅEèâä˙âªä÷òA*******************
********************************************************/
int InitTitle(Screen *screen){
    printfDx("<InitTitle>\n");
    return INIT_SUCCESS;
}
int InitSelect(Screen *screen){
    printfDx("<InitSelect>\n");
    return INIT_SUCCESS;
}

int InitScore(Screen *screen){
    printfDx("<InitScore>\n");
    return INIT_SUCCESS;
}
int InitNowLoad(Screen *screen){
    printfDx("<InitNowLoad>\n");
    //if(LoadFile() == 0){return INIT_FALED;}
    return INIT_SUCCESS;
}
