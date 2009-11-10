#include "DxLib.h"
#include "shere.h"
#include "system.h"
#include "game.h"
#include "debug.h"
#include "executer.h"
#include "scenario.h"
#include "math.h"


//�摜�n���h��
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
   


  

    //�G�@������
    ZeroMemory( &fighter, sizeof(MyFighter));
    //���@������
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
    fighter.hit_circles[0].point.x = 0;//���΍��W
    fighter.hit_circles[0].point.y = 0;
    fighter.hit_circles[0].radius = 25;

    fighter.hit_rects[0].enable = true;
    fighter.hit_rects[0].top_left.x = 0;//���΍��W
    fighter.hit_rects[0].top_left.y = 0;
    fighter.hit_rects[0].bottom_right.x = FIGHTER_WIDTH;
    fighter.hit_rects[0].bottom_right.y = FIGHTER_HEIGHT;


    //���@�e�@������
    ZeroMemory( bullets, sizeof(MyBullet) * MAX_BULLETS );

    //�G�@������
    ZeroMemory( enemies, sizeof(Enemy) * MAX_ENEMIES );
    enemylibs = (Enemy*)malloc(sizeof(Enemy) * COMMAND_MAXNUM);


    /*
    //�G�@�f�[�^
    for(int i = 0;i < 3;i++){
        //�G���i�[
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
        enemies[i].hit_circles[0].point.x = 0;//���΍��W
        enemies[i].hit_circles[0].point.y = 0;
        enemies[i].hit_circles[0].radius = 15;

        enemies[i].hit_rects[0].enable = true;
        enemies[i].hit_rects[0].top_left.x = 0;//���΍��W
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

    //�G�e�@�[���N���A
    ZeroMemory( enemy_bullets, sizeof(EnemyBullet) * MAX_ENEMY_BULLETS );
    //�G���C�u�����H�@�������m�ہE�[���N���A
    enemylibs = (Enemy*)malloc(sizeof(Enemy) * ENEMY_MAX_KIND);//�G�p�������m��
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
    //���͌��m
    GetHitKeyStateAll( input_key_buff );

    if(input_key_buff[KEY_INPUT_A] == 1){//��
        fighter.move.x = -(fighter.speed);
        input_key_a_d = true;
    }else if(input_key_buff[KEY_INPUT_D] == 1){//��
        fighter.move.x = fighter.speed;
        input_key_a_d = true;
    }else{//x���ɑ΂��ē��͂���Ă��Ȃ��ꍇ
        fighter.move.x = 0;
        input_key_a_d = false;
    }

    if(input_key_buff[KEY_INPUT_W] == 1){//��
        fighter.move.y = -(fighter.speed);
        input_key_w_s = true;
    }else if(input_key_buff[KEY_INPUT_S] == 1){//��
        fighter.move.y = fighter.speed;
        input_key_w_s = true;
    }else{//y���ɑ΂��ē��͂���Ă��Ȃ��ꍇ
        fighter.move.y = 0;
        input_key_w_s = false;
    }

    //�΂߈ړ�����
    if((input_key_a_d == true) && (input_key_w_s == true)){
        fighter.move.x = fighter.move.x * 0.707f;// 0.707��1/��2
        fighter.move.y = fighter.move.y * 0.707f;
    }
    
    if(input_key_buff[KEY_INPUT_L] == 1){//�e�P����(L�L�[)
        bullet_shot = true;
        shot_type = BULLET_NOMAL;
    }
    if(input_key_buff[KEY_INPUT_K] == 1){//�e�Q����(K�L�[)
        bullet_shot = true;
        shot_type = BULLET_MISSILE;
    }
    if(input_key_buff[KEY_INPUT_J] == 1){//�e�R����(K�L�[)
        bullet_shot = true;
        shot_type = BULLET_HOMING;
    }

    if(bullet_shot == true){
        //���͂Ƃ肠�������[�v�� -> ���ƂŃ^�X�N��
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
                        //���i���邾���̒e
                        bullets[i].move.x = 20.0f;
                        bullets[i].move.y = 0.0f;
                        break;
                    case BULLET_MISSILE:
                        //�����x�����
                        bullets[i].move.x = 0.0f;
                        bullets[i].move.y = 0.0f;
                        bullets[i].accel.x = 0.2f;
                        bullets[i].accel.y = 0.0f;
                        break;
                    case BULLET_HOMING:
                        //�W�I���z�[�~���O�����->�z�[�~���O����
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



    //���@����(�ǔ���)
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

    //���e����
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
    
    //���@�`��
    DrawGraph( (int)fighter.pos.x - (fighter.image.size.width / 2), 
        (int)fighter.pos.y - (fighter.image.size.height / 2), fighter_image[0] , TRUE );
        //���@�����蔻��`��(debug)
    for(int i = 0;i < SYSTEM_HITMAX_CIRCLE ;i++){
        if(fighter.hit_rects[i].enable == false){
            break;//���������蔻��ȍ~�̓`�F�b�N���Ȃ�
        }
        DrawHitCircle((int)fighter.pos.x, (int)fighter.pos.y, fighter.hit_circles[i]);
    }
    for(int j = 0;j < SYSTEM_HITMAX_RECT ;j++){
        if(fighter.hit_rects[j].enable == false){
            break;//���������蔻��ȍ~�̓`�F�b�N���Ȃ�
        }
        float pos_from_width = (float)fighter.hit_rects[j].bottom_right.x / 2;
        float pos_from_height = (float)fighter.hit_rects[j].bottom_right.y / 2;
        DrawHitRect((int)(fighter.pos.x - pos_from_width),(int)(fighter.pos.y - pos_from_height),fighter.hit_rects[j]);
    }

    
    //���e�`��
    for(int i = 0;i < MAX_BULLETS ;i++){
        if(bullets[i].enable == true){
            DrawGraph((int)bullets[i].pos.x,(int)bullets[i].pos.y,bullet_fighter_image[bullets[i].type], TRUE);
        }
    }

    //�G�@�`��+�����蔻��`��(debug)
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
                break;//���������蔻��ȍ~�̓`�F�b�N���Ȃ�
            }
            DrawHitCircle((int)enemies[i].pos.x, (int)enemies[i].pos.y, enemies[i].hit_circles[j]);
        }
        for(int j = 0;j < SYSTEM_HITMAX_RECT ;j++){
            if(enemies[i].hit_rects[j].enable == false){
                break;//���������蔻��ȍ~�̓`�F�b�N���Ȃ�
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
            //�G���L���łȂ��ꍇ or ���s����R�}���h�������ꍇ
            continue;
        }
        if(strcmp(enemies->commands[enemies->command_index].command,"RMOVE") == 0){
            if(enemies->command_act_flg == COMMAND_NOACT){
                double tx = (double)atoi(enemies->commands[enemies->command_index].param1);
                double ty = (double)atoi(enemies->commands[enemies->command_index].param2);
                double rad = atan2(ty,tx);
                double mx = cos(rad) * enemies->speed;
                double my = sin(rad) * enemies->speed;
                double tmove = sqrt((tx * tx) + (ty * ty));//���ړ�����
                double mmove = sqrt((mx * mx) + (my * my));//�P�t���[���ړ�����
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
                //��΍��W�Ƃ̈ʒu���v�Z
                tx -= enemies->pos.x;
                ty -= enemies->pos.y;
                double rad = atan2(ty,tx);
                double mx = cos(rad) * enemies->speed;
                double my = sin(rad) * enemies->speed;
                double tmove = sqrt((tx * tx) + (ty * ty));//���ړ�����
                double mmove = sqrt((mx * mx) + (my * my));//�P�t���[���ړ�����
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
            if(enemies->move_flame == 0){//�ړ��I��
                enemies->pos.x = enemies->move_target.x;
                enemies->pos.y = enemies->move_target.y;
                enemies->move.x = 0;
                enemies->move.y = 0;
                enemies->command_act_flg = COMMAND_NOACT;
                enemies->command_index++;
                printfDx("�ړ��I��\n");
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
