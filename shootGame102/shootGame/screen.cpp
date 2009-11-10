#include "screen.h"
#include "shere.h"
#include "debug.h"
#include "game.h"
#include "loader.h"
#include "title.h"


int UpdateScreen(Screen *);
int IsChange(Screen *);
int ChangeState(Screen *);
int Draw(Screen *);
int TransitionStart(Screen *screen);

int (*InitScene)(Screen *);
int (*DrawScene)(Screen *);



int InitState(Screen *screen){
    screen->scene_state = SCENE_BLANK;
    screen->next_state = SCENE_TITLE;
    return INIT_SUCCESS;
}
int UpdateScreen(Screen *screen){
    SceneControl(screen);//�f�o�b�O�p
    if(screen->update_screen == NO_UPDATE_SCENE){
        return NOT_UPDATED;
    }
    if(IsChange(screen) == CHANGE_SCENE){//��ʐ؂�ւ��g���K
        //��ʑJ�ڍs���ꍇ
        //>> screen->next_state = NEXT_SCENE_CODE
        if(ChangeState(screen) == INVALID_SCENE_STATUS){
            return SWITCH_FALED;
        }
        InitScene(screen);
        TransitionStart(screen);//�J�ڊԃt���[��������
        return SWITCH_SUCCESS;
    }
    Draw(screen);//�`��
    return UPDATE_SUCCESS;
}

int ChangeState(Screen *screen){//��ʑJ�ڃX�e�[�^�X������������
    if(screen->scene_state == screen->next_state){
        //��ʑJ�ڃi�V
        return NO_CHANGE_SCENE;
    }

        switch(screen->next_state){
		case SCENE_TITLE:
            InitScene = InitTitle;
            DrawScene = DrawTitle;
			break;
		case SCENE_SELECT:
            InitScene = InitSelect;
            DrawScene = DrawSelect;
            break;
		case SCENE_GAME:
            InitScene = InitGame;
            DrawScene = DrawGame;
			break;
		case SCENE_SCORE:
            InitScene = InitScore;
            DrawScene = DrawScore;
			break;
        case SCENE_NOWLOAD:
            InitScene = InitNowLoad;
            DrawScene = DrawNowLoad;
			break;
        default:
            return INVALID_SCENE_STATUS;
	}
    screen->scene_state = screen->next_state;
    return VALID_SCENE_STATUS;
}

int IsChange(Screen *screen){
    if(screen->scene_state == screen->next_state){//��ʐ؂�ւ�����
        return NOT_CHANGE_SCENE;
    }
    return CHANGE_SCENE;
}


int Draw(Screen *screen){//   ���C���`�揈��
    ClsDrawScreen();
    //printfDx("�e�X�g�P\n");
    DrawScene(screen);//�X�e�[�W��`�悳����
	ScreenFlip();
    return DRAW_SUCCESS;
}


int TransitionStart(Screen *screen){
    //screen->stage = createStage();
    return TRANSITION_SUCCESS;
}

