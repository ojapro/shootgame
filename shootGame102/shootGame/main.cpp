


#include "screen.h"
#include "shere.h"
#include "main.h"
#include <windows.h>
#include <tchar.h>

#define INIT_SUCCESS 1
#define INIT_FALED 0

char input_key_buff[ 256 ];//�L�[����t���O�@�������񂱂��ɂ���

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int showCmd){
    Screen screen;
    //DebugMode debug_ctrl;
    //TraceLog log;
	if (strcmp(pCmdLine, "-f") != 0){ ChangeWindowMode(TRUE);}//�E�B���h�E���[�h�ɐݒ�
	//DX���C�u�����̏�����
	if( DxLib_Init() == -1 ) {return -1;}
	//�`��Ώۂ𗠉�ʂɐݒ�
	SetDrawScreen(DX_SCREEN_BACK);
	//�t�@�C���ǂݍ���
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



