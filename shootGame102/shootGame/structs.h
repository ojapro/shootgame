#include "system.h"
#include "boolean.h"
#include "time.h"

#ifndef _STRUCTS_H
#define _STRUCTS_H


//システム関連
struct Playerinfo{
    int score;
};

//座標計算用
struct Position{
    float x,y;
};
struct Size{
    int height,width;
};

typedef Position Vector;

//パラメータ用
struct ValueLimit{
    int max,value;
};
typedef ValueLimit HitPoint;
typedef ValueLimit Energy;


//画像用構造体
struct Image{//画像構造体
    int type;
    Size size;//画像サイズ
    int rotate;//回転
    int alpha;//透明度
};

//図形
struct Rect{
    Position top_left, bottom_right;
};

//当たり判定用構造体
struct HitRect{
    Boolean enable;
    Position top_left, bottom_right;
};
struct HitCircle{
    Boolean enable;
    Position point;
    float radius;
};


#define COMMAND_IDENTIFIER_LENGTH 20 
#define COMMAND_PARAM_LENGTH 20

//ステージシナリオ
struct Command{
    char command[COMMAND_IDENTIFIER_LENGTH];
    char param1[COMMAND_PARAM_LENGTH];
    char param2[COMMAND_PARAM_LENGTH];
    char param3[COMMAND_PARAM_LENGTH];
    char param4[COMMAND_PARAM_LENGTH];
    char param5[COMMAND_PARAM_LENGTH];
    char param6[COMMAND_PARAM_LENGTH];
};

#define COMMAND_MAXNUM 500
struct Scenario{
    Command *commands;
    int max_command,cursor;
    time_t next_execute_time;
};



//実体
struct MyFighter{
    int type;
    float speed;
    Image image;
    Position pos;//座標
    Vector move;//移動量
    HitPoint HP;//体力(max,value)
    Energy energy_slot;//攻撃用エネルギー(max,value)
    HitRect hit_rects[SYSTEM_HITMAX_RECT];
    HitCircle hit_circles[SYSTEM_HITMAX_CIRCLE];
};


struct MyBullet{
    Boolean enable;//処理対象
    int type;
    Image image;
    Position pos;//座標
    Vector move;//移動量
    Vector accel;//移動量加速度
    int power;//威力
    HitRect hit_rects[SYSTEM_HITMAX_RECT];
    HitCircle hit_circles[SYSTEM_HITMAX_CIRCLE];
};
#define ENEMY_MAX_KIND      100
#define ENEMY_MAX_COMMAND   10
#define MAX_NAME_VALUE      20

#define COMMAND_NOACT       0
#define COMMAND_ACT         1

struct Enemy{
    Boolean enable;
    int type;
    char *name;
    float speed;
    Image image;
    Position pos;//座標
    Vector move;//移動量
    HitPoint HP;//体力
    HitRect hit_rects[SYSTEM_HITMAX_RECT];
    HitCircle hit_circles[SYSTEM_HITMAX_CIRCLE];
    Command *commands;//独立
    int all_command,command_act_flg,command_index;
    int move_flame;
    Position move_target;
};



struct EnemyBullet{
    Boolean enable;
    int type;
    Image image;
    Position pos;//座標
    Vector move;//移動量
    int rotate;//回転
    int alpha;//透明度
    int power;//威力
};




//フィールド表示オブジェクト（敵味方・弾など）
struct FieldObjects {
    MyFighter fighter; 
    Enemy enemies[MAX_ENEMIES];
    MyBullet bullets[MAX_BULLETS];
    EnemyBullet enemy_bullets[MAX_ENEMY_BULLETS];
    Scenario st1_scenario;
};




struct Pointstate{
    int state;
    char *ptr;
};

#define LOAD_IMAGE_MAX 200
#define LOAD_SOUND_MAX 200
#define LOAD_MATERIAL_MAX LOAD_IMAGE_MAX + LOAD_SOUND_MAX
struct Loader{
    int enable;
    char *filename;
    int states;
};
struct Stage{ 
    unsigned int image_enables, image_loaded, sound_enables, sound_loaded, nowloading;
    Loader imageList[LOAD_IMAGE_MAX];
    Loader soundList[LOAD_SOUND_MAX];
    Loader *materialList[LOAD_MATERIAL_MAX];
    FieldObjects objects;

    Scenario snr;
};
struct Filename{
    char *filename;
};
struct Part{
    void *part;
};
#define SCENE_STACK_MAX 256
struct Screen{
    int update_screen;
    Part *parts;//画面構成要素　配列
    Filename *needfile;
    int scene_state,next_state,transition_flg;//画面
    int scene_cursor;
    Playerinfo player_info;
};


//デバッガ関連
struct DebugMode{
    Boolean screen_display,screen_debug;
    Boolean playerinfo_debug;
    Boolean stage_display,stage_debug;
    Boolean file_state_debug;
    Boolean tracelog_enable;
};

#define MESSAGE_LENGTH 255
struct Trace{
    unsigned int num;
    int time;
    char message[MESSAGE_LENGTH];
};

#define TRACE_LOG_MAX 50000
struct TraceLog{
    int new_record_num, disp_rec_num, disp_rec_max;
    Trace *trace;
};


#endif
