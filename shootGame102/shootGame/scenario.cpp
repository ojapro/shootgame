#include "DxLib.h"
#include "structs.h"
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <stdlib.h>
#include <string.h>
#include "scenario.h"

#define DELIMITER ';'
#define FILE_END_DELIMITER '<'
#define LINE_END 0
#define LINE_CONTINUE 1
#define FILE_LINE_END 2
#define LINE_READ_ERROR -1
#define FILE_OPEN_ERROR -1

#define APPLY_ENEMYDATA_FALED   0
#define APPLY_ENEMYDATA_SUCCESS 1
#define ENEMY_CHANGE 2




int LoadAndSetEnemyLib(char *filename , Enemy *enemies){
    #define READ_ERROR -1
    #define READ_COMPLETE 0
        
    #define FILE_BUFF_MAX 1000 
    #define TOKEN_BUFFMAX 64
    #define ONELINE_BUFFMAX 64
    

    char buff[FILE_BUFF_MAX];
    char one_line_buff[ONELINE_BUFFMAX];
    int read_stat = 0;
    int stopcnt = 0;
    int command_index = 0;
    int readbytes = 0;
    int write_mode = 0;
    int file_stat = open( filename, O_RDONLY);
    if(file_stat == FILE_OPEN_ERROR){printfDx("%s : file open error",filename);return false;}

    //初期化
    ZeroMemory( buff, sizeof(char) * FILE_BUFF_MAX );
    enemies->commands = (Command*)malloc(sizeof(Command) * ENEMY_MAX_COMMAND);//コマンド用メモリ確保
    ZeroMemory( enemies->commands, sizeof(Command) * ENEMY_MAX_COMMAND);//コマンド初期化
    
    
    while(1){
        ZeroMemory( buff, sizeof(char) * FILE_BUFF_MAX );
        readbytes = read(file_stat, buff, FILE_BUFF_MAX);
        if(readbytes == READ_ERROR){ printfDx("ファイル読み込みエラー code(%d)\n",readbytes);break; }//異常終了
        //printfDx("%s",buff);
        
        OptimizationSenarioString(buff,FILE_BUFF_MAX);

        int line_stat = 0;
        char *buff_read_point = buff;
        char *limit_ptr = (buff + FILE_BUFF_MAX);
        while(true){
            buff_read_point = readLine(buff_read_point,one_line_buff,FILE_BUFF_MAX,ONELINE_BUFFMAX,&line_stat);
            //変換　one_line_buff;
            if(line_stat == LINE_CONTINUE){//1行が中途半端で終わっている場合
                break;//以降のファイル内容を読み込む
            }
            if(line_stat == FILE_LINE_END){
                //ファイルの最後の行
                printfDx("正常終了\n",readbytes);
                read_stat = READ_COMPLETE;
                break;
            }
            if(line_stat == LINE_READ_ERROR){
                //コマンド認識エラー
                break;
            }
            if(command_index >= COMMAND_MAXNUM){
                printfDx("シナリオコマンド数オーバー");
                break;//異常終了
            }
            Command tmp_command = CreateCommand(one_line_buff);
            int apply_stat = ApplyEnemyData(enemies,tmp_command);
            if(apply_stat == ENEMY_CHANGE){
                //次の敵データへ
                enemies++;
            }else{
                if(enemies->all_command > 0){
                    char *cmd = enemies->commands[(enemies->all_command - 1)].command;
                    char *p1 =  enemies->commands[(enemies->all_command - 1)].param1;
                    char *p2 =  enemies->commands[(enemies->all_command - 1)].param2;
                    char *p3 =  enemies->commands[(enemies->all_command - 1)].param3;
                    char *p4 =  enemies->commands[(enemies->all_command - 1)].param4;
                    char *p5 =  enemies->commands[(enemies->all_command - 1)].param5;
                    char *p6 =  enemies->commands[(enemies->all_command - 1)].param6;
                    printfDx("Command [%s  %s, %s, %s, %s, %s, %s] \n",cmd,p1,p2,p3,p4,p5,p6);
                }
            }
            
        }
        if(line_stat == FILE_LINE_END || readbytes < FILE_BUFF_MAX){ break; }
    }
    
    close(file_stat);
    return read_stat;
}


Boolean LoadScenario(char *filename,Scenario *scenario){
    #define READ_ERROR -1
    #define READ_COMPLETE 0
        
    #define FILE_BUFF_MAX 280 
    #define TOKEN_BUFFMAX 64
    #define ONELINE_BUFFMAX 64


    char buff[FILE_BUFF_MAX];
    char one_line_buff[ONELINE_BUFFMAX];
    Boolean read_stat = false;
    int stopcnt = 0;
    int command_index = 0;
    int readbytes = 0;
    int file_stat = open( filename, O_RDONLY);
    if(file_stat == FILE_OPEN_ERROR){printfDx("%s : file open error",filename);return false;}

    //初期化
    ZeroMemory( scenario, sizeof(Scenario));
    ZeroMemory( buff, sizeof(char) * FILE_BUFF_MAX );
    ZeroMemory( one_line_buff, sizeof(char) * ONELINE_BUFFMAX );
    scenario->commands = (Command*)malloc(sizeof(Command) * COMMAND_MAXNUM);//コマンド用メモリ確保
    ZeroMemory( scenario->commands, sizeof(Command) * COMMAND_MAXNUM);//コマンド初期化
    scenario->max_command = COMMAND_MAXNUM;

    
    while(1){
        ZeroMemory( buff, sizeof(char) * FILE_BUFF_MAX );
        readbytes = read(file_stat, buff, FILE_BUFF_MAX);
         if(readbytes == READ_ERROR){ printfDx("ファイル読み込みエラー code(%d)\n",readbytes);break; }//異常終了
        //printfDx("%s",buff);
        
        OptimizationSenarioString(buff,FILE_BUFF_MAX);

        int line_stat = 0;
        char *buff_read_point = buff;
        char *limit_ptr = (buff + FILE_BUFF_MAX);
        while(true){
            //if(stopcnt2 > 2){break;}
            //stopcnt2++;
            buff_read_point = readLine(buff_read_point,one_line_buff,FILE_BUFF_MAX,ONELINE_BUFFMAX,&line_stat);
            //変換　one_line_buff;
            if(line_stat == LINE_CONTINUE){//1行が中途半端で終わっている場合
                break;//以降のファイル内容を読み込む
            }
            if(line_stat == FILE_LINE_END){
                //ファイルの最後の行
                printfDx("正常終了\n",readbytes);
                read_stat = true;
                break;
            }
            if(line_stat == LINE_READ_ERROR){
                //コマンド認識エラー
                break;
            }
            if(command_index >= COMMAND_MAXNUM){
                printfDx("シナリオコマンド数オーバー");
                break;//異常終了
            }
            scenario->commands[command_index] = CreateCommand(one_line_buff);
            command_index++;
            char *cmd =  scenario->commands[command_index].command;
            char *p1 =  scenario->commands[command_index].param1;
            char *p2 =  scenario->commands[command_index].param2;
            char *p3 =  scenario->commands[command_index].param3;
            char *p4 =  scenario->commands[command_index].param4;
            char *p5 =  scenario->commands[command_index].param5;
            char *p6 =  scenario->commands[command_index].param6;


            printf("Command [%s  %s, %s, %s, %s, %s, %s] ",cmd,p1,p2,p3,p4,p5,p6);
            
        }
        if(line_stat == FILE_LINE_END || readbytes < FILE_BUFF_MAX){ break; }
    }
    
    close(file_stat);
    return read_stat;
}
Command CreateCommand(char *line){
    Command com;
    char command[COMMAND_IDENTIFIER_LENGTH];
    char param1[COMMAND_PARAM_LENGTH];
    char param2[COMMAND_PARAM_LENGTH];
    char param3[COMMAND_PARAM_LENGTH];
    char param4[COMMAND_PARAM_LENGTH];
    char param5[COMMAND_PARAM_LENGTH];
    char param6[COMMAND_PARAM_LENGTH];
    
    char *command_ptr = command;
    char *param1_ptr = param1;
    char *param2_ptr = param2;
    char *param3_ptr = param3;
    char *param4_ptr = param4;
    char *param5_ptr = param5;
    char *param6_ptr = param6;


    ZeroMemory( &com, sizeof(Command));
    ZeroMemory( command, sizeof(char) * COMMAND_IDENTIFIER_LENGTH );
    ZeroMemory( param1, sizeof(char) * COMMAND_PARAM_LENGTH );
    ZeroMemory( param2, sizeof(char) * COMMAND_PARAM_LENGTH );
    ZeroMemory( param3, sizeof(char) * COMMAND_PARAM_LENGTH );
    ZeroMemory( param4, sizeof(char) * COMMAND_PARAM_LENGTH );
    ZeroMemory( param5, sizeof(char) * COMMAND_PARAM_LENGTH );
    ZeroMemory( param6, sizeof(char) * COMMAND_PARAM_LENGTH );


    //命令部抽出
    while(1){
            if(*line < 'A' || *line > 'Z'){
                *command_ptr = '\0';
                break;
            }
        *command_ptr = *line;
        command_ptr++;
        line++;
    }
    //パラム１抽出
    while(1){
        if(*line == ',' ){
            *param1_ptr = '\0';
            line++;
            break;
        }
        if(*line == '\0'){ 
            *param1_ptr = '\0'; 
            break; 
        }
        *param1_ptr = *line;
        param1_ptr++;
        line++;
    }
    while(1){
        if(*line == ',' ){
            *param1_ptr = '\0';
            line++;
            break;
        }
        if(*line == '\0'){ 
            *param1_ptr = '\0'; 
            break; 
        }
        *param2_ptr = *line;
        param2_ptr++;
        line++;
    }
    while(1){
        if(*line == ',' ){
            *param1_ptr = '\0';
            line++;
            break;
        }
        if(*line == '\0'){ 
            *param1_ptr = '\0'; 
            break; 
        }
        *param3_ptr = *line;
        param3_ptr++;
        line++;
    }
    //param4
    while(1){
        if(*line == ',' ){
            *param1_ptr = '\0';
            line++;
            break;
        }
        if(*line == '\0'){ 
            *param1_ptr = '\0'; 
            break; 
        }
        *param4_ptr = *line;
        param4_ptr++;
        line++;
    }
    while(1){
        if(*line == ',' ){
            *param1_ptr = '\0';
            line++;
            break;
        }
        if(*line == '\0'){ 
            *param1_ptr = '\0'; 
            break; 
        }
        *param5_ptr = *line;
        param5_ptr++;
        line++;
    }
    while(1){
        if(*line == ',' ){
            *param1_ptr = '\0';
            line++;
            break;
        }
        if(*line == '\0'){ 
            *param1_ptr = '\0'; 
            break; 
        }
        *param6_ptr = *line;
        param6_ptr++;
        line++;
    }
    strcpy(com.command,command);
    strcpy(com.param1,param1);
    strcpy(com.param2,param2);
    strcpy(com.param3,param3);
    strcpy(com.param4,param4);
    strcpy(com.param5,param5);
    strcpy(com.param6,param6);
    return com;
}

void OptimizationSenarioString(char *src_str,int length){
    unityNewLine(src_str,length);
    removeComment(src_str,length);
    removeSpace(src_str,length);
    return;
}
void removeSpace(char *src_str,int length){
    char *pts = src_str;
    char *ptt = src_str;
    int str_cnt = 0;
    while(str_cnt < length){
        str_cnt++;
        if(*pts == '\0' || *pts == EOF){break;}
        if(*pts == ' '){pts++;continue;}
        //文字寄せ
        *ptt = *pts;
        ptt++;
        pts++;
    }
    *ptt = '\0';
    return;
}
void removeComment(char *src_str,int length){
    char *pts = src_str;
    char *ptt = src_str;
    Boolean comment_flg = false;
    int str_cnt = 0;
    while(str_cnt < length){
        str_cnt++;
        if(*pts == '\0' || *pts == EOF){ break; }
        if(*pts == '#'){
            comment_flg = true; 
            pts++;
            continue;
        }
        if(*pts == '\n'){
            comment_flg = false;
            pts++;
            continue;
        }
        if(comment_flg == false){
            //文字寄せ
            *ptt = *pts;
            ptt++;
        }
        pts++;
    }
    *ptt = '\0';
    return;
}
void unityNewLine(char *src_str,int length){
    char *pts = src_str;
    char *ptt = src_str;
    int str_cnt = 0;
    Boolean cr_flg = false;
    while(str_cnt < length){
        str_cnt++;
        if(*pts == '\0' || *pts == EOF){break;}
        if(*pts == '\n' && cr_flg == true){
            cr_flg = false;
            continue;
        }
        if(*pts == '\r'){
            *pts = '\n';//変換
            cr_flg = true;
        }
        
        //文字寄せ
        *ptt = *pts;
        ptt++;
        pts++;
    }
    *ptt = '\0';
    return;
}
char* readLine(char *src_str,char *cpy_str,int src_length,int cpy_length,int *flg){
    char *src_limit_ptr = src_str + src_length;
    char *cpy_limit_ptr = cpy_str + cpy_length;
    if(*flg == LINE_CONTINUE){//途切れていた場合、途中から再開
        while(*cpy_str != '\0'){
            if(cpy_str > cpy_limit_ptr){//入りきらない場合
                *flg = LINE_READ_ERROR;
                return src_str;
            }
            cpy_str++;
        }
    }


    while(1){
        if(cpy_str > cpy_limit_ptr){//入りきらない場合
            *flg = LINE_READ_ERROR;
        }
        if(*src_str == DELIMITER || *src_str == '\n'){
            printfDx(" >>DELIMITER\n");
            *flg = LINE_END;
            break;
        }
        if(*src_str == FILE_END_DELIMITER || *src_str == EOF || *src_str == '\0'){
            printfDx(" >>FILE_END_DELIMITER\n");
            *flg = FILE_LINE_END;
            break;
        }
        if(src_str > src_limit_ptr){//1行が中途半端で終わっている場合
            printfDx(" >>next_Load>> ");
            *flg = LINE_CONTINUE;
            break;
        }
        *cpy_str = *src_str;
        printfDx("%c",*cpy_str);
        cpy_str++;
        src_str++;
    }
    
    *cpy_str = '\0';
    src_str++;
    return src_str;
}


int ApplyEnemyData(Enemy *enemy,Command tmp_command){
    if(strcmp(tmp_command.command,"END") == 0){//敵切り替えの場合
        return ENEMY_CHANGE;
    }
    if(strcmp(tmp_command.command,"ID") == 0){
        enemy->type = atoi(tmp_command.param1);
    }
    if(strcmp(tmp_command.command,"NAME") == 0){
        strcpy(enemy->name,tmp_command.param1);
    }
    if(strcmp(tmp_command.command,"HP") == 0){
        enemy->HP.max = enemy->HP.value = atoi(tmp_command.param1);
    }
    if(strcmp(tmp_command.command,"SPEED") == 0){
        enemy->speed = atoi(tmp_command.param1);
    }
    if(strcmp(tmp_command.command,"RMOVE") == 0){
        enemy->commands[enemy->all_command] = tmp_command;
        enemy->all_command++;
    }
    if(strcmp(tmp_command.command,"AMOVE") == 0){
        enemy->commands[enemy->all_command] = tmp_command;
        enemy->all_command++;
    }
    return APPLY_ENEMYDATA_SUCCESS;
}