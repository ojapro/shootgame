#include "DxLib.h"
#include <limits.h>
#include <math.h>

//�\���̐錾
//���W�܂��̓x�N�g�����L�^����\����
struct Vector{
	float x,y;
};
typedef Vector Point2D;
//�����L�^����\����
struct Line2D{
	Point2D startpos, endpos;
	float katamuki;	//�X�������W�A���l�ŋL�^
	Vector speed;	//�ړ����Ă���ꍇ�͑��x���Z�b�g
};
//���̂��L�^����\����
struct Ball2D{
	Point2D position;
	float hankei;	//���a
};
//�l�p�`���L�^����\����
struct Rect2D{
	Point2D lefttop;
	Point2D rightbottom;
	float width;
	float height;
};


//���C�u�����֐�
Point2D PosInView(Point2D in);
int XInView(float inx);
int YInView(float iny);
void ScrollToLeft(float jikiposx);
void ScrollToRight(float jikiposx);
void ScrollToUp(float jikiposy);
void ScrollToDown(float jikiposy);
void DrawLineInView(float x1, float y1, float x2, float y2, int Color, int Thickness);
void DrawCircleInView(float x, float y, float r, int Color, int FillFlag);
void DrawAnimation(float x, float y, double ExtRate, double Angle,int TurnFlag,
				   int *imgarray, int allframe, float fps);
//�x�N�g���֐�
Vector CreateVector(Vector in, float veclen);
Vector AddVector(Vector v1, Vector v2);
Vector SubVector(Vector v1, Vector v2);
Vector AddVectorInFrameTime(Vector pos, Vector speed);
Vector AddVectorInFrameTime2(Vector pos, Vector speed, Vector accel);
Vector Normalize(Vector in);
Vector RotateVector(Vector in, float radian);
float VectorLengthSquare(Vector in);
float DotProduct(Vector v1, Vector v2);
float CrossProduct(Vector v1, Vector v2);
void SetLine2DKatamuki(Line2D *in);
void DrawLine2D(Line2D in, int Color, int Thickness);
void DrawBall2D(Ball2D in, int Color, int Fill);
//�����蔻��֐�
bool HitTestLineAndBall(Line2D linein, Ball2D ballin);
bool IsPointAtLineFace(Line2D linein, Point2D ptin);
bool HitTestLineAndLine(Line2D line1, Line2D line2);
bool HitTestBallAndBall(Ball2D a, Ball2D b);
bool HitTestPointAndBox(Rect2D rect, Point2D pt);
//�^�C�}�[�֐�
void SetSimpleTimer(int idx, int time);
int GetPassedTime(int idx);


//�O���[�o���ϐ�
extern float g_frametime;
extern Rect2D g_framerect;	//��ʗ̈�i�����蔻��j
extern Point2D g_current_field_pos;	//���݂̍�����W
extern Rect2D g_stagesize;	//�X�e�[�W�T�C�Y

//�萔�錾
const float ZEROVALUE = 1e-10f;
const float PIE = 3.1415926f;
const int SCROLL_LIMIT = 200;

