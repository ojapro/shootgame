#include "DxLib.h"
#include "debug.h"
#include "screen.h"
void log(char *);

void DrawHitRect(int posx,int posy,HitRect rect){

    int sx = (int)(posx + rect.top_left.x);
    int sy = (int)(posy + rect.top_left.y);
    int ex = (int)(posx + rect.bottom_right.x);
    int ey = (int)(posy + rect.top_left.y);
    DrawLine(sx, sy, ex, ey, GetColor(128,128,255),FALSE);

    sx = (int)(posx + rect.bottom_right.x);
    sy = (int)(posy + rect.top_left.y);
    ex = (int)(posx + rect.bottom_right.x);
    ey = (int)(posy + rect.bottom_right.y);
    DrawLine(sx, sy, ex, ey, GetColor(128,128,255),FALSE);

    sx = (int)(posx + rect.top_left.x);
    sy = (int)(posy + rect.top_left.y );
    ex = (int)(posx + rect.top_left.x);
    ey = (int)(posy + rect.bottom_right.y);
    DrawLine(sx, sy, ex, ey, GetColor(128,128,255),FALSE);

    sx = (int)(posx + rect.top_left.x);
    sy = (int)(posy + rect.bottom_right.y);
    ex = (int)(posx + rect.bottom_right.x);
    ey = (int)(posy + rect.bottom_right.y);
    DrawLine(sx, sy, ex, ey, GetColor(128,128,255),FALSE);
}
void DrawHitCircle(int posx,int posy,HitCircle circle){
    DrawCircle((int)(posx + circle.point.x), (int)(posy + circle.point.y), circle.radius, GetColor(255,128,128),FALSE);
}

/*
void Trace(char *message){

}
#define DEBUG_TEXT_CLEAR_ERROR -1
void Debug(TraceLog *log,Screen *screen){
    //int clear_state = clsDx();
    //if(clear_state == DEBUG_TEXT_CLEAR_ERROR){printfDx("画面クリアError\n");};
    int begin_index = log->new_record_num - log->disp_rec_num;//ログ表示開始レコード指定
    for(int i = 0;i < log->disp_rec_num;i++){
        Trace trace = log->trace[(begin_index + i)];//いったんTraceに格納
        printfDx("[%10d] %s",trace.time,trace.message);
    }
}
void InitTraceMode(DebugMode *debugctrl,TraceLog *log){
    debugctrl->screen_display = true;
    debugctrl->screen_debug = true;
    debugctrl->playerinfo_debug = true;
    debugctrl->stage_display = true;
    debugctrl->stage_debug = true;
    debugctrl->file_state_debug = true;
    debugctrl->tracelog_enable = true;
    
    ZeroMemory( log, sizeof(TraceLog));
    log->trace = (Trace*)malloc(sizeof(Trace) * TRACE_LOG_MAX);//メモリ確保
    ZeroMemory( log->trace, sizeof(Trace) * TRACE_LOG_MAX);//メモリ初期化
    printfDx("TraceLog_size>>%d",sizeof(Trace) * TRACE_LOG_MAX);
    log->new_record_num = 0;
    log->disp_rec_num = 0;
    log->disp_rec_max = 10;
}
*/
int SceneControl(Screen *screen){
    //ファンクションキーに画面を対応させる
    if(CheckHitKey(KEY_INPUT_F1) != 0){
        screen->next_state = SCENE_TITLE;
    }
    if(CheckHitKey(KEY_INPUT_F2) != 0){
        screen->next_state = SCENE_SELECT;
    }
    if(CheckHitKey(KEY_INPUT_F3) != 0){
        screen->next_state = SCENE_GAME;
    }
    if(CheckHitKey(KEY_INPUT_F4) != 0){
        screen->next_state = SCENE_SCORE;
    }
    if(CheckHitKey(KEY_INPUT_F5) != 0){
        screen->next_state = SCENE_NOWLOAD;
    }
    return 0;
}
