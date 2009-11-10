#include "DxLib.h"
#include "shere.h"
#include "system.h"
#include "game.h"
#include "debug.h"
#include "executer.h"
#include "scenario.h"
#include "math.h"


//画像ハンドル
    int fighter_image[3];
    int enemy_image[3];
    int bullet_fighter_image[3];
    int bullet_enemy_image[1];
    int bg_image;

    MyFighter fighter; 
    Enemy enemies[MAX_ENEMIES];
    MyBullet bullets[MAX_BULLETS];
    EnemyBullet enemy_bullets[MAX_ENEMY_BULLETS];
    Scenario st1_scenario;
    Enemy *enemylibs;


int InitGame(Screen *screen){
    printfDx("<InitGame>\n");
   


  

    //敵　初期化
    ZeroMemory( &fighter, sizeof(MyFighter));
    //自機初期化
    fighter.type = MY_FIGHTER;
    fighter.pos.x = 100.0;
    fighter.pos.y = 240.0;
    fighter.move.x = 0.0;
    fighter.move.y = 0.0;
    fighter.image.alpha = 100;
    fighter.image.rotate = 0;
    fighter.image.size.height = FIGHTER_WIDTH;
    fighter.image.size.width = FIGHTER_HEIGHT;
    fighter.image.type = fighter.type;
    fighter.HP.max = 1000;
    fighter.HP.value = 1000;
    fighter.energy_slot.max = 1000;
    fighter.energy_slot.value = 1000;
    fighter.speed = 10.0;

    fighter.hit_circles[0].enable = true;
    fighter.hit_circles[0].point.x = 0;//相対座標
    fighter.hit_circles[0].point.y = 0;
    fighter.hit_circles[0].radius = 25;

    fighter.hit_rects[0].enable = true;
    fighter.hit_rects[0].top_left.x = 0;//相対座標
    fighter.hit_rects[0].top_left.y = 0;
    fighter.hit_rects[0].bottom_right.x = FIGHTER_WIDTH;
    fighter.hit_rects[0].bottom_right.y = FIGHTER_HEIGHT;


    //自機弾　初期化
    ZeroMemory( bullets, sizeof(MyBullet) * MAX_BULLETS );

    //敵　初期化
    ZeroMemory( enemies, sizeof(Enemy) * MAX_ENEMIES );
    enemylibs = (Enemy*)malloc(sizeof(Enemy) * COMMAND_MAXNUM);


    /*
    //敵　データ
    for(int i = 0;i < 3;i++){
        //敵情報格納
        enemies[i].enable = true;
        enemies[i].type = ENEMY_SMALL;
        enemies[i].HP.max = 10;
        enemies[i].HP.value = 10;
        enemies[i].image.alpha = 100;
        enemies[i].image.rotate = 0;
        enemies[i].image.size.width = ENEMY_WIDTH;
        enemies[i].image.size.height = ENEMY_HEIGHT;
        enemies[i].image.type = enemies[i].type;

        enemies[i].hit_circles[0].enable = true;
        enemies[i].hit_circles[0].point.x = 0;//相対座標
        enemies[i].hit_circles[0].point.y = 0;
        enemies[i].hit_circles[0].radius = 15;

        enemies[i].hit_rects[0].enable = true;
        enemies[i].hit_rects[0].top_left.x = 0;//相対座標
        enemies[i].hit_rects[0].top_left.y = 0;
        enemies[i].hit_rects[0].bottom_right.x = ENEMY_WIDTH;
        enemies[i].hit_rects[0].bottom_right.y = ENEMY_HEIGHT;
        enemies[i].hit_circles[0].point.y = 0;
        enemies[i].hit_circles[0].radius = 15;
        enemies[i].pos.x = i * 10.0f + 540.0f;
        enemies[i].pos.y = i * 100.0f + 200.0f;
        enemies[i].speed = 5;
        enemies[i].move.x = -0.3 * (i + 1);
    }*/

    //敵弾　ゼロクリア
    ZeroMemory( enemy_bullets, sizeof(EnemyBullet) * MAX_ENEMY_BULLETS );
    //敵ライブラリ？　メモリ確保・ゼロクリア
    enemylibs = (Enemy*)malloc(sizeof(Enemy) * ENEMY_MAX_KIND);//敵用メモリ確保
    ZeroMemory( enemylibs, sizeof(Enemy) * ENEMY_MAX_KIND );
    //
    for(int index = 0;index < ENEMY_MAX_KIND;index++){
        enemylibs[index].commands = (Command*)malloc(sizeof(Command) * ENEMY_MAX_COMMAND);
        ZeroMemory( enemylibs[index].commands, sizeof(Command) * ENEMY_MAX_COMMAND );
        enemylibs[index].name = (char*)malloc(sizeof(char) * MAX_NAME_VALUE);
        ZeroMemory( enemylibs[index].name, sizeof(char) * MAX_NAME_VALUE );
    }
    

    if(LoadDivGraph( "image\\myfighter_s.bmp" , 3 , 3 , 1 , 50 , 50 , fighter_image ) == -1)    {return false;};
    if(LoadDivGraph( "image\\enemy_s.bmp" , 3 , 1 , 3 , 30 , 30 , enemy_image ) == -1)          {return false;};
    if(LoadDivGraph( "image\\tama.bmp" , 3 , 1 , 3 , 50 , 50 , bullet_fighter_image ) == -1)    {return false;};
    if(LoadDivGraph( "image\\teki_tama.bmp" , 1 , 1 , 1 , 50 , 50 , bullet_enemy_image ) == -1) {return false;};
    if(LoadDivGraph( "image\\bg.jpg" , 1 , 1 , 1 , 640 , 480 , &bg_image ) == -1)               {return false;};
    if(LoadScenario("stage1_script.txt",&st1_scenario) == false){return false;};
    if(LoadAndSetEnemyLib("ememy_script.txt",enemylibs) == 0){return false;};

    return true;
}
int DrawGame(Screen *screen){
    int debug = PromoteScenario(&st1_scenario);
    //printfDx("[%d] = PromoteScenario\n",debug);]
    Enemy *en = enemies;
    debug = EnemyAction(en);
    


    
    Boolean bullet_shot = false;
    int shot_type = 0;
    Boolean input_key_a_d = false;
    Boolean input_key_w_s = false;
    int speed_tmp = 0;
    //入力検知
    GetHitKeyStateAll( input_key_buff );

    if(input_key_buff[KEY_INPUT_A] == 1){//←
        fighter.move.x = -(fighter.speed);
        input_key_a_d = true;
    }else if(input_key_buff[KEY_INPUT_D] == 1){//→
        fighter.move.x = fighter.speed;
        input_key_a_d = true;
    }else{//x軸に対して入力されていない場合
        fighter.move.x = 0;
        input_key_a_d = false;
    }

    if(input_key_buff[KEY_INPUT_W] == 1){//↑
        fighter.move.y = -(fighter.speed);
        input_key_w_s = true;
    }else if(input_key_buff[KEY_INPUT_S] == 1){//↓
        fighter.move.y = fighter.speed;
        input_key_w_s = true;
    }else{//y軸に対して入力されていない場合
        fighter.move.y = 0;
        input_key_w_s = false;
    }

    //斜め移動処理
    if((input_key_a_d == true) && (input_key_w_s == true)){
        fighter.move.x = fighter.move.x * 0.707f;// 0.707≒1/√2
        fighter.move.y = fighter.move.y * 0.707f;
    }
    
    if(input_key_buff[KEY_INPUT_L] == 1){//弾１発射(Lキー)
        bullet_shot = true;
        shot_type = BULLET_NOMAL;
    }
    if(input_key_buff[KEY_INPUT_K] == 1){//弾２発射(Kキー)
        bullet_shot = true;
        shot_type = BULLET_MISSILE;
    }
    if(input_key_buff[KEY_INPUT_J] == 1){//弾３発射(Kキー)
        bullet_shot = true;
        shot_type = BULLET_HOMING;
    }

    if(bullet_shot == true){
        //今はとりあえずループで -> あとでタスク化
        for(int i = 0;i < MAX_BULLETS ;i++){
            if(bullets[i].enable == false){
                bullets[i].enable = true;
                bullets[i].pos.x = fighter.pos.x;
                bullets[i].pos.y = fighter.pos.y;
                bullets[i].image.size.height = 50;
                bullets[i].image.size.width = 50;
                bullets[i].image.rotate = 0;
                bullets[i].image.alpha = 100;
                bullets[i].power = 100;
                bullets[i].type = shot_type;
                switch(bullets[i].type){
                    case BULLET_NOMAL:
                        //直進するだけの弾
                        bullets[i].move.x = 20.0f;
                        bullets[i].move.y = 0.0f;
                        break;
                    case BULLET_MISSILE:
                        //加速度つける玉
                        bullets[i].move.x = 0.0f;
                        bullets[i].move.y = 0.0f;
                        bullets[i].accel.x = 0.2f;
                        bullets[i].accel.y = 0.0f;
                        break;
                    case BULLET_HOMING:
                        //標的をホーミングする玉->ホーミング処理
                        bullets[i].move.x = 0.0f;
                        bullets[i].move.y = 0.0f;
                        bullets[i].accel.x = 0.0f;
                        bullets[i].accel.y = 0.0f;
                        break;
                }
                break;
            }
        }
    }



    //自機処理(壁判定)
    int half_width = fighter.image.size.width / 2;
    int half_height = fighter.image.size.height / 2;
    if(fighter.pos.x <= (0 + half_width) && fighter.move.x < 0){
        fighter.move.x = 0;
        fighter.pos.x = (float)(0 + half_width);
    }
    if(fighter.pos.x >= (640 - half_width) && fighter.move.x > 0){
        fighter.move.x = 0;
        fighter.pos.x = (float)(640 - half_width);
    }
    if(fighter.pos.y <= (0 + half_height) && fighter.move.y < 0){
        fighter.move.y = 0;
        fighter.pos.y = (float)(0 + half_height);
    }
    if(fighter.pos.y >= (480 - half_height) && fighter.move.y > 0){
        fighter.move.y = 0;
        fighter.pos.y = (float)(480 - half_height);
    }

    fighter.pos.x += fighter.move.x;
    fighter.pos.y += fighter.move.y;

    //自弾処理
    for(int i = 0;i < MAX_BULLETS ;i++){
        if(bullets[i].enable == true){
            if(bullets[i].type == BULLET_MISSILE){
                bullets[i].move.x += bullets[i].accel.x;
                bullets[i].move.y += bullets[i].accel.y;
            }
            bullets[i].pos.x += bullets[i].move.x;
            bullets[i].pos.y += bullets[i].move.y;
            if(bullets[i].pos.x > (640 + (bullets[i].image.size.width / 2))){
                bullets[i].enable = false;
            }
        }
    }
    
    //自機描画
    DrawGraph( (int)fighter.pos.x - (fighter.image.size.width / 2), 
        (int)fighter.pos.y - (fighter.image.size.height / 2), fighter_image[0] , TRUE );
        //自機あたり判定描画(debug)
    for(int i = 0;i < SYSTEM_HITMAX_CIRCLE ;i++){
        if(fighter.hit_rects[i].enable == false){
            break;//無効当たり判定以降はチェックしない
        }
        DrawHitCircle((int)fighter.pos.x, (int)fighter.pos.y, fighter.hit_circles[i]);
    }
    for(int j = 0;j < SYSTEM_HITMAX_RECT ;j++){
        if(fighter.hit_rects[j].enable == false){
            break;//無効当たり判定以降はチェックしない
        }
        float pos_from_width = (float)fighter.hit_rects[j].bottom_right.x / 2;
        float pos_from_height = (float)fighter.hit_rects[j].bottom_right.y / 2;
        DrawHitRect((int)(fighter.pos.x - pos_from_width),(int)(fighter.pos.y - pos_from_height),fighter.hit_rects[j]);
    }

    
    //自弾描画
    for(int i = 0;i < MAX_BULLETS ;i++){
        if(bullets[i].enable == true){
            DrawGraph((int)bullets[i].pos.x,(int)bullets[i].pos.y,bullet_fighter_image[bullets[i].type], TRUE);
        }
    }

    //敵機描画+当たり判定描画(debug)
    for(int i = 0;i < MAX_ENEMIES ;i++){
        enemies[i].pos.x += enemies[i].move.x;
        enemies[i].pos.y += enemies[i].move.y;
        if(enemies[i].enable == true){
            float pos_from_width = (float)enemies[i].image.size.width / 2;
            float pos_from_height = (float)enemies[i].image.size.height / 2;
            DrawGraph((int)(enemies[i].pos.x - pos_from_width), (int)(enemies[i].pos.y - pos_from_width), enemy_image[enemies[i].type], TRUE);
        }
        for(int j = 0;j < SYSTEM_HITMAX_CIRCLE ;j++){
            if(enemies[i].hit_circles[j].enable == false){
                break;//無効当たり判定以降はチェックしない
            }
            DrawHitCircle((int)enemies[i].pos.x, (int)enemies[i].pos.y, enemies[i].hit_circles[j]);
        }
        for(int j = 0;j < SYSTEM_HITMAX_RECT ;j++){
            if(enemies[i].hit_rects[j].enable == false){
                break;//無効当たり判定以降はチェックしない
            }
            float pos_from_width = (float)enemies[i].hit_rects[j].bottom_right.x / 2;
            float pos_from_height = (float)enemies[i].hit_rects[j].bottom_right.y / 2;
            DrawHitRect((int)(enemies[i].pos.x - pos_from_width),(int)(enemies[i].pos.y - pos_from_height),enemies[i].hit_rects[j]);  
        }
    }

    
    return DRAW_SUCCESS;
}

int EnemyAction(Enemy *enemies){

    for(Enemy *limit = (enemies + MAX_ENEMIES);enemies < limit;enemies++){

        if((enemies->enable == false) 
        || (enemies->command_index == enemies->all_command)){
            //敵が有効でない場合 or 実行するコマンドが無い場合
            continue;
        }
        if(strcmp(enemies->commands[enemies->command_index].command,"RMOVE") == 0){
            if(enemies->command_act_flg == COMMAND_NOACT){
                double tx = (double)atoi(enemies->commands[enemies->command_index].param1);
                double ty = (double)atoi(enemies->commands[enemies->command_index].param2);
                double rad = atan2(ty,tx);
                double mx = cos(rad) * enemies->speed;
                double my = sin(rad) * enemies->speed;
                double tmove = sqrt((tx * tx) + (ty * ty));//総移動距離
                double mmove = sqrt((mx * mx) + (my * my));//１フレーム移動距離
                double flame = tmove / mmove;
                enemies->move_flame = (int)flame;
                enemies->move_target.x = enemies->pos.x + tx;
                enemies->move_target.y = enemies->pos.y + ty;
                enemies->move.x = (float)mx;
                enemies->move.y = (float)my;
                printfDx("RMOVE >> target:%.2f,%.2f move:%.2f,%.2f flame:%d\n",tx,ty,mx,my,(int)flame);
                enemies->command_act_flg = COMMAND_ACT;
                
            }
        }
        if(strcmp(enemies->commands[enemies->command_index].command,"AMOVE") == 0){
            if(enemies->command_act_flg == COMMAND_NOACT){
                double tx = (double)atoi(enemies->commands[enemies->command_index].param1);
                double ty = (double)atoi(enemies->commands[enemies->command_index].param2);
                //絶対座標との位置を計算
                tx -= enemies->pos.x;
                ty -= enemies->pos.y;
                double rad = atan2(ty,tx);
                double mx = cos(rad) * enemies->speed;
                double my = sin(rad) * enemies->speed;
                double tmove = sqrt((tx * tx) + (ty * ty));//総移動距離
                double mmove = sqrt((mx * mx) + (my * my));//１フレーム移動距離
                double flame = tmove / mmove;
                enemies->move_flame = (int)flame;
                enemies->move_target.x = enemies->pos.x + tx;
                enemies->move_target.y = enemies->pos.y + ty;
                enemies->move.x = (float)mx;
                enemies->move.y = (float)my;
                printfDx("AMOVE >> target:%.2f,%.2f move:%.2f,%.2f flame:%d\n",tx,ty,mx,my,(int)flame);
                enemies->command_act_flg = COMMAND_ACT;
            }
        }
        if(enemies->move_flame > 0){
            enemies->move_flame--; 
            if(enemies->move_flame == 0){//移動終了
                enemies->pos.x = enemies->move_target.x;
                enemies->pos.y = enemies->move_target.y;
                enemies->move.x = 0;
                enemies->move.y = 0;
                enemies->command_act_flg = COMMAND_NOACT;
                enemies->command_index++;
                printfDx("移動終了\n");
            }
        }
    }
    
    return ENEMY_ACTION_SUCCESS;
}
int setControl(Enemy *enemy){
    if(strcmp(enemy->commands->command,"RMOVE") == 0){
        
    }
    return SET_CTRL_SUCCESS;
}
/*
Command createCommand(char *s){
    //Command command_tmp;
    //ZeroMemory( command_tmp, sizeof(Command));
    //return command_tmp; 
}
*/
char* getStringWithLimitString(char *src_str,char *cpy_str,char limit){
    while(1){
        if(*src_str == limit){
            break;
        }
        *cpy_str = *src_str;
        cpy_str++;
        src_str++;
    }
    cpy_str = '\0';
    return src_str;
}
HitPoint hit(HitPoint hp,int num){
    hp.value -= num;
    if(hp.value < 0){
        hp.value = 0;
    }
    return hp;
}

HitPoint heal(HitPoint hp,int num){
    hp.value += num;
    if(hp.value > hp.max){
        hp.value = hp.max;
    }
    return hp;
}
