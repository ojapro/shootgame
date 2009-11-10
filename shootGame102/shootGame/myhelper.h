#include "DxLib.h"
#include <limits.h>
#include <math.h>

//構造体宣言
//座標またはベクトルを記録する構造体
struct Vector{
	float x,y;
};
typedef Vector Point2D;
//線を記録する構造体
struct Line2D{
	Point2D startpos, endpos;
	float katamuki;	//傾きをラジアン値で記録
	Vector speed;	//移動している場合は速度をセット
};
//球体を記録する構造体
struct Ball2D{
	Point2D position;
	float hankei;	//半径
};
//四角形を記録する構造体
struct Rect2D{
	Point2D lefttop;
	Point2D rightbottom;
	float width;
	float height;
};


//ライブラリ関数
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
//ベクトル関数
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
//当たり判定関数
bool HitTestLineAndBall(Line2D linein, Ball2D ballin);
bool IsPointAtLineFace(Line2D linein, Point2D ptin);
bool HitTestLineAndLine(Line2D line1, Line2D line2);
bool HitTestBallAndBall(Ball2D a, Ball2D b);
bool HitTestPointAndBox(Rect2D rect, Point2D pt);
//タイマー関数
void SetSimpleTimer(int idx, int time);
int GetPassedTime(int idx);


//グローバル変数
extern float g_frametime;
extern Rect2D g_framerect;	//画面領域（当たり判定）
extern Point2D g_current_field_pos;	//現在の左上座標
extern Rect2D g_stagesize;	//ステージサイズ

//定数宣言
const float ZEROVALUE = 1e-10f;
const float PIE = 3.1415926f;
const int SCROLL_LIMIT = 200;

