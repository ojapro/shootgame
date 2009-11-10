#include "DxLib.h"

//WinMain‚©‚çŠJŽn
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){
	ChangeWindowMode(true);
	if(DxLib_Init() == -1){
		return -1;
	}





	DrawPixel(100,100,0xFFFFFF);

	WaitKey();

	DxLib_End();
	return 0;
}