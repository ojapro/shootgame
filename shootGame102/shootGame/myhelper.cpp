#include "myhelper.h"

//�O���[�o���ϐ�
int g_lasttime;					//�O��̃��[�v�I�����ԁi�~���b�j
float g_frametime = 0.017f;		//1�t���[���̕\���ɂ����������ԁi�b�j

//�X�N���[���p�f�[�^
Rect2D g_framerect = { {-64,-64}, {640+64,480+64}, 768, 608};	//��ʗ̈�i�����蔻��j
Point2D g_current_field_pos = {0,0};	//���݂̍�����W
Rect2D g_stagesize = { {0,0}, {639,479}, 640,480};	//�X�e�[�W�T�C�Y

//�X�N���[���p�֐�
//�f�[�^��̍��W����ʕ\���p���W�ɕύX����
Point2D PosInView(Point2D in){
	return SubVector(in, g_current_field_pos);
}
//����X���W�ł�Y���W��
int XInView(float inx){
	return (int)(inx - g_current_field_pos.x);
}
int YInView(float iny){
	return (int)(iny - g_current_field_pos.y);
}
/*��ʂ����փX�N���[��
�i���L��������200�s�N�Z����ۂ悤�X�N���[���j*/
void ScrollToLeft(float jikiposx){
	//�X�N���[���\�����`�F�b�N
	if ((int)jikiposx - SCROLL_LIMIT > (int)g_stagesize.lefttop.x){
		g_current_field_pos.x = jikiposx - SCROLL_LIMIT;
		g_framerect.lefttop.x = g_current_field_pos.x - 64;
		g_framerect.lefttop.y = g_current_field_pos.y - 64;
		g_framerect.rightbottom.x = g_current_field_pos.x + 640 + 64;
		g_framerect.rightbottom.y = g_current_field_pos.y + 480 + 64;
	}
}
/*��ʂ��E�փX�N���[��
�i���L��������200�s�N�Z����ۂ悤�X�N���[���j*/
void ScrollToRight(float jikiposx){
	if ((int)jikiposx + SCROLL_LIMIT < (int)g_stagesize.rightbottom.x){
		g_current_field_pos.x = jikiposx - (640 - SCROLL_LIMIT);
		g_framerect.lefttop.x = g_current_field_pos.x - 64;
		g_framerect.lefttop.y = g_current_field_pos.y - 64;
		g_framerect.rightbottom.x = g_current_field_pos.x + 640 + 64;
		g_framerect.rightbottom.y = g_current_field_pos.y + 480 + 64;
	}
}
/*��ʂ���փX�N���[��*/
void ScrollToUp(float jikiposy){
	//�X�N���[���\�����`�F�b�N
	if ((int)jikiposy - SCROLL_LIMIT > (int)g_stagesize.lefttop.y){
		g_current_field_pos.y = jikiposy - SCROLL_LIMIT;
		g_framerect.lefttop.x = g_current_field_pos.x - 64;
		g_framerect.lefttop.y = g_current_field_pos.y - 64;
		g_framerect.rightbottom.x = g_current_field_pos.x + 640 + 64;
		g_framerect.rightbottom.y = g_current_field_pos.y + 480 + 64;
	}
}
/*��ʂ����փX�N���[��*/
void ScrollToDown(float jikiposy){
	if ((int)jikiposy + SCROLL_LIMIT < (int)g_stagesize.rightbottom.y){
		g_current_field_pos.y = jikiposy - (480 - SCROLL_LIMIT);
		g_framerect.lefttop.x = g_current_field_pos.x - 64;
		g_framerect.lefttop.y = g_current_field_pos.y - 64;
		g_framerect.rightbottom.x = g_current_field_pos.x + 640 + 64;
		g_framerect.rightbottom.y = g_current_field_pos.y + 480 + 64;
	}
}

/*�X�N���[�����v�Z�ɓ��ꂽ�`��֐�*/
void DrawLineInView(float x1, float y1, float x2, float y2, int Color, int Thickness){
	DrawLine(XInView(x1), YInView(y1), XInView(x2), YInView(y2), 
			Color, Thickness);
}
void DrawCircleInView(float x, float y, float r, int Color, int FillFlag){
	DrawCircle(XInView(x), YInView(y), (int)r, Color, FillFlag);
}

/*�A�j���[�V�����\��������֐�
�摜��\��������Wx��y�A
�C���[�W�f�[�^�̔z��imgarray�A���R�}��framenum�A�A�j���̕\�����xfps
�A�j���[�V�����p�̈ꎞ�f�[�^animtmp�������Ƃ���B
1���G��\������ꍇ�́A
imgarray��int�^�ϐ��̃A�h���X�Aallframe��1�Afps��1.0f���w�肷�邱�ƁB
*/
void DrawAnimation(float x, float y, double ExtRate, double Angle,int TurnFlag,
				   int *imgarray, int allframe, float fps){
	//���݂̌o�ߕb�������߂�
	float t = (float)( GetNowCount() & INT_MAX );
	//�\�����ׂ��R�}�������߂�
	int animpat = (int)(t/(1000/fps)) % allframe;
	//���̃R�}��`��
	DrawRotaGraph(XInView(x),YInView(y), ExtRate, Angle, imgarray[animpat], TRUE, TurnFlag);
}

/*�x�N�g�����쐬����֐�
���̓x�N�g���𐳋K�����Ă��璷�����|����*/
Vector CreateVector(Vector in, float veclen){
	Vector result;
	in = Normalize(in);
	result.x = in.x * veclen;
	result.y = in.y * veclen;
	return result;
}
/*�x�N�g���̑����Z���s���֐�*/
Vector AddVector(Vector v1, Vector v2){
	Vector result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	return result;
}
/*�x�N�g���̈����Z���s���֐�
���W����x�N�g�������߂�ꍇ�ɂ��g����
���̏ꍇv1�͏I�_�Av2�͎n�_�Ƃ���*/
Vector SubVector(Vector v1, Vector v2){
	Vector result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	return result;
}

/*���W�ɑ��x�x�N�g���𑫂��֐��i�����^���p�j
g_frametime���l������*/
Vector AddVectorInFrameTime(Vector pos, Vector speed){
	Vector result;
	result.x = pos.x + speed.x * g_frametime;
	result.y = pos.y + speed.y * g_frametime;
	return result;
}
/*���W�ɑ��x�x�N�g���Ɖ����x�𑫂��֐��i�������x�^���p�j
g_frametime���l������*/
Vector AddVectorInFrameTime2(Vector pos, Vector speed, Vector accel){
	Vector result;
	result.x = pos.x + speed.x * g_frametime + accel.x * pow(g_frametime,2) / 2;
	result.y = pos.y + speed.y * g_frametime + accel.y * pow(g_frametime,2) / 2;
	return result;
}
/*�x�N�g���𐳋K������֐�*/
Vector Normalize(Vector in){
	Vector result;
	float l = sqrt( in.x * in.x + in.y * in.y );
	result.x = in.x / l;
	result.y = in.y / l;
	return result;
}
/*���ς����߂�֐�*/
float DotProduct(Vector v1, Vector v2){
	return v1.x * v2.x + v1.y * v2.y;
}
/*�O�ς����߂�֐�*/
float CrossProduct(Vector v1, Vector v2){
	return v1.x * v2.y - v1.y * v2.x;
}
/*�x�N�g������]������֐�*/
Vector RotateVector(Vector in, float radian){
	Vector result;
	result.x = -sin(radian)*in.y + cos(radian)*in.x;
	result.y =  cos(radian)*in.y + sin(radian)*in.x;
	return result;
}
/*�x�N�g���̒����̓������߂�֐�
�������K�v�ȂƂ���sqrt�ŕ����������߂�*/
float VectorLengthSquare(Vector in){
	return in.x * in.x + in.y * in.y;
}


/*Line2D��`�悷��֐�*/
void DrawLine2D(Line2D in, int Color, int Thickness){
	DrawLine(XInView(in.startpos.x), YInView(in.startpos.y),
		XInView(in.endpos.x), YInView(in.endpos.y), Color, Thickness);
}
/*Ball2D��`�悷��֐�*/
void DrawBall2D(Ball2D in, int Color, int FillFlag){
	DrawCircle(XInView(in.position.x), YInView(in.position.y),
		(int)in.hankei, Color, FillFlag);
}
/*���̌X�������߂�Line2D�̃����okatamuki�ɃZ�b�g����֐�*/
void SetLine2DKatamuki(Line2D *in){
	in->katamuki = atan2(in->endpos.y - in->startpos.y, in->endpos.x - in->startpos.x);
}


/*Ball2D��Line2D�̓����蔻����s���֐�*/
bool HitTestLineAndBall(Line2D linein, Ball2D ballin){
	Vector avec, bvec;
	avec = SubVector(ballin.position, linein.startpos);
	bvec = SubVector(linein.endpos, linein.startpos);
	float dot = DotProduct(avec, bvec);				//���ς����߂�
	float bl = VectorLengthSquare(bvec);		//�x�N�g��b�̒����̓������߂�
	if ( (dot > -ZEROVALUE) && (dot < bl) ) {		//�Փ˂̉\���L��
		//�~�Ɛ��̋��������߂�
		//�{�[���̒��S���琂���ɂ��낵�����̌�_�����߂�
		Vector bnvec = Normalize(bvec);					//�x�N�g��b�̐��K��
		float dot2 = DotProduct(avec, bnvec);
		bnvec.x *= dot2;						
		bnvec.y *= dot2; 
		Point2D kouten = AddVector(linein.startpos, bnvec);
		Vector dist = SubVector(ballin.position, kouten);
		float al = VectorLengthSquare(dist);
#ifdef _DEBUG
		DrawBall2D(ballin, GetColor(128,128,128),FALSE);
		DrawLineInView(linein.startpos.x, linein.startpos.y,
			linein.startpos.x+avec.x, linein.startpos.y+avec.y, 65535,1);
		DrawLineInView(linein.startpos.x, linein.startpos.y, 
			kouten.x, kouten.y, GetColor(255,0,0),1);
		DrawLineInView(kouten.x, kouten.y, 
			ballin.position.x, ballin.position.y, GetColor(255,0,0),1);
#endif
		if ( al < (ballin.hankei * ballin.hankei)  ){
			return TRUE;
		}
	}
	return FALSE;
}

/*���_�Ɛ��̈ʒu�֌W�𒲂ׂ�֐�
���̍��܂��͏㑤�ɂ����TRUE�A�E�܂��͉����ɂ����FALSE��Ԃ�*/
bool IsPointAtLineFace(Line2D linein, Point2D ptin){
	Vector avec, bvec;
	avec = SubVector(ptin, linein.startpos);
	bvec = SubVector(linein.endpos, linein.startpos);
	float cross = CrossProduct(avec, bvec);
	//�O�ς����Ȃ�_�͐��̕\���i�����j�ɂ���
	if ( cross > -ZEROVALUE )	return TRUE;
	return FALSE;
}

/*Line2D���m�̓����蔻����s���֐�*/
bool HitTestLineAndLine(Line2D line1, Line2D line2){
	Vector avec, bvec, cvec, dvec, evec, fvec;
	avec = SubVector(line1.endpos, line1.startpos);		//a2-a1
	bvec = SubVector(line2.startpos, line1.startpos);	//b1-a1
	cvec = SubVector(line2.endpos, line1.startpos);		//b2-a1
	dvec = SubVector(line2.endpos, line2.startpos);		//b2-b1
	evec = SubVector(line1.startpos, line2.startpos);	//a1-b1
	fvec = SubVector(line1.endpos, line2.startpos);		//a2-b1

	float a,b,c,d;
	a = CrossProduct(avec, bvec);
	b = CrossProduct(avec, cvec);
	c = CrossProduct(dvec, evec);
	d = CrossProduct(dvec, fvec);
	
	if( ((a*b) < ZEROVALUE) && ((c*d) < ZEROVALUE) ){
		return TRUE;
	}
	return FALSE;
}


/*Ball2D���m�̓����蔻����s���֐�*/
bool HitTestBallAndBall(Ball2D ball1, Ball2D ball2){
#ifdef _DEBUG
		DrawBall2D(ball1, GetColor(128,128,128),FALSE);
		DrawBall2D(ball2, GetColor(128,128,128),FALSE);
#endif
	if ( pow(ball1.position.x - ball2.position.x, 2) + pow(ball1.position.y - ball2.position.y,2)
		< pow(ball1.hankei+ball2.hankei, 2) ){
		return TRUE;
	}else{
		return FALSE;
	}
}

/*Point2D��Rect2D�̓����蔻����s���֐�*/
bool HitTestPointAndBox(Rect2D rect, Point2D pt){
	if ( (rect.lefttop.x < pt.x) && (pt.x < rect.rightbottom.x) &&
		(rect.lefttop.y < pt.y) && (pt.y < rect.rightbottom.y) ){
			return TRUE;
	}
	return FALSE;
}

//�V���v���^�C�}�[�֐�
const int MAXTIMER = 32;
int g_goaltimes[MAXTIMER];
/*�^�C�}�[�ɖڕW�l��ݒ肵�Ďn��������֐�
�ڕW�ltime�̓~���b�Őݒ肷��*/
void SetSimpleTimer(int idx, int time){
	if(idx>MAXTIMER) return;
	g_goaltimes[idx] = (GetNowCount() & INT_MAX) + time;
}
/*�^�C�}�[�̌��ʂ��擾����֐�
�Ԓl�����Ȃ�܂��ڕW�l�ɒB���Ă��Ȃ�
���Ȃ�ڕW�l�𒴂��Ă���*/
int GetPassedTime(int idx){
	return (GetNowCount() & INT_MAX) - g_goaltimes[idx];
}
