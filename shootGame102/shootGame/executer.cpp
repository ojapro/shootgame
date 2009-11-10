#include "DxLib.h"
#include "shere.h"
#include "system.h"
#include "executer.h"
#include "debug.h"
#include "game.h"


int PromoteScenario(Scenario *scenario){
    int exe_result;
    clock_t tim;
    tim = clock();

    if(scenario->cursor == scenario->max_command){//シナリオが終端に達した
        return END_OF_SCENARIO;
    }
    if(tim < scenario->next_execute_time){//待ち
        return PROMOTE_WAIT;
    }
    while(1){
        exe_result = ExecuteScenario(&(scenario->commands[scenario->cursor]),&tim);
        switch(exe_result){
            case EXECUTE_WAIT:
                scenario->next_execute_time = tim;
            case EXECUTE_SUCCESS:
            case INVALID_EXECUTE_COMMAND:
                scenario->cursor++;//EXECUTE_WAIT or EXECUTE_SUCCESSの場合、コマンドを進める
                break;
        }
        if(exe_result != EXECUTE_SUCCESS){
            break;
        }
    }
    switch(exe_result){
        case EXECUTE_FAILED :
        return PROMOTE_FALED;

        case EXECUTE_SUCCESS :
        return PROMOTE_SUCCESS;
        
        case EXECUTE_WAIT :
        return PROMOTE_WAIT;
        
        case COMMAND_END :
        return END_OF_SCENARIO;
        
    }
    return PROMOTE_FALED;
}
int ExecuteScenario(Command *command,clock_t *time){
    if(strcmp(command->command,"") == 0){//命令なし
        //printfDx("<命令なし>\n");
        return COMMAND_END;
    }
    if(strcmp(command->command,"WAIT") == 0){//WAIT命令
        *time += atoi(command->param1);
        printfDx("<%sﾐﾘ秒待ってね>\n",command->param1);
        return EXECUTE_WAIT;
    }
    if(strcmp(command->command,"ENEMY") == 0){//ENEMY命令
        printfDx("<敵出現>\n");
        int x = atoi(command->param3);
        int y = atoi(command->param4);
        int type = atoi(command->param2);
        PlaceEnemy(command->param1,type,x,y);
        return EXECUTE_SUCCESS;
    }
    
    return INVALID_EXECUTE_COMMAND;
}

int PlaceEnemy(char *name,int type,int x,int y){
    //敵　データ
    extern Enemy enemies[MAX_ENEMIES];//外部参照
    extern Enemy *enemylibs;
    
    Enemy sample_enemy = SearchLib(type,enemylibs);
    sample_enemy.enable = true;
    sample_enemy.type = ENEMY_SMALL;
    sample_enemy.HP.max = 10;
    sample_enemy.HP.value = 10;
    sample_enemy.image.alpha = 100;
    sample_enemy.image.rotate = 0;
    sample_enemy.image.size.width = ENEMY_WIDTH;
    sample_enemy.image.size.height = ENEMY_HEIGHT;
    sample_enemy.image.type = sample_enemy.type;
    sample_enemy.hit_circles[0].enable = true;
    sample_enemy.hit_circles[0].point.x = 0;//相対座標
    sample_enemy.hit_circles[0].point.y = 0;
    sample_enemy.hit_circles[0].radius = 15;
    sample_enemy.hit_rects[0].enable = true;
    sample_enemy.hit_rects[0].top_left.x = 0;//相対座標
    sample_enemy.hit_rects[0].top_left.y = 0;
    sample_enemy.hit_rects[0].bottom_right.x = ENEMY_WIDTH;
    sample_enemy.hit_rects[0].bottom_right.y = ENEMY_HEIGHT;
    sample_enemy.hit_circles[0].point.y = 0;
    sample_enemy.hit_circles[0].radius = 15;
    sample_enemy.pos.x = (float)x;
    sample_enemy.pos.y = (float)y;
    //sample_enemy.speed = 5;
    //sample_enemy.move.x = 0;
    sample_enemy.command_act_flg = COMMAND_NOACT;

    for(int i = 0;i < MAX_ENEMIES;i++){
        if(enemies[i].enable == false){
            enemies[i] = sample_enemy;
            break;
        }
    }
    return PLACE_SUCCESS;
}

Enemy SearchLib(int type,Enemy *lib){
    Enemy result;
    for(int i = 0;i < ENEMY_MAX_KIND;i++){
        if(lib[i].type == 0){//末端
            break;
        }
        if(lib[i].type == type){
            result = lib[i];
        }
    }
    return result;
}