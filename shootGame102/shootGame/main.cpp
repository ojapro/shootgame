


#include "screen.h"
#include "shere.h"
#include "main.h"
#include <windows.h>
#include <tchar.h>

#define INIT_SUCCESS 1
#define INIT_FALED 0

char input_key_buff[ 256 ];//キー制御フラグ　いったんここにおく

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int showCmd){
    Screen screen;
    //DebugMode debug_ctrl;
    //TraceLog log;
	if (strcmp(pCmdLine, "-f") != 0){ ChangeWindowMode(TRUE);}//ウィンドウモードに設定
	//DXライブラリの初期化
	if( DxLib_Init() == -1 ) {return -1;}
	//描画対象を裏画面に設定
	SetDrawScreen(DX_SCREEN_BACK);
	//ファイル読み込み
    //InitDebugMode(&debug_ctrl,&log);
    InitState(&screen);

	while(ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){
        if(UpdateScreen(&screen) == 0){
            break;
        }
	}

	DxLib_End();
	return 0;
}



