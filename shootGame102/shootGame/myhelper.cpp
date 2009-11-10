#include "myhelper.h"

//グローバル変数
int g_lasttime;					//前回のループ終了時間（ミリ秒）
float g_frametime = 0.017f;		//1フレームの表示にかかった時間（秒）

//スクロール用データ
Rect2D g_framerect = { {-64,-64}, {640+64,480+64}, 768, 608};	//画面領域（当たり判定）
Point2D g_current_field_pos = {0,0};	//現在の左上座標
Rect2D g_stagesize = { {0,0}, {639,479}, 640,480};	//ステージサイズ

//スクロール用関数
//データ上の座標を画面表示用座標に変更する
Point2D PosInView(Point2D in){
	return SubVector(in, g_current_field_pos);
}
//そのX座標版とY座標版
int XInView(float inx){
	return (int)(inx - g_current_field_pos.x);
}
int YInView(float iny){
	return (int)(iny - g_current_field_pos.y);
}
/*画面を左へスクロール
（自キャラから200ピクセルを保つようスクロール）*/
void ScrollToLeft(float jikiposx){
	//スクロール可能かをチェック
	if ((int)jikiposx - SCROLL_LIMIT > (int)g_stagesize.lefttop.x){
		g_current_field_pos.x = jikiposx - SCROLL_LIMIT;
		g_framerect.lefttop.x = g_current_field_pos.x - 64;
		g_framerect.lefttop.y = g_current_field_pos.y - 64;
		g_framerect.rightbottom.x = g_current_field_pos.x + 640 + 64;
		g_framerect.rightbottom.y = g_current_field_pos.y + 480 + 64;
	}
}
/*画面を右へスクロール
（自キャラから200ピクセルを保つようスクロール）*/
void ScrollToRight(float jikiposx){
	if ((int)jikiposx + SCROLL_LIMIT < (int)g_stagesize.rightbottom.x){
		g_current_field_pos.x = jikiposx - (640 - SCROLL_LIMIT);
		g_framerect.lefttop.x = g_current_field_pos.x - 64;
		g_framerect.lefttop.y = g_current_field_pos.y - 64;
		g_framerect.rightbottom.x = g_current_field_pos.x + 640 + 64;
		g_framerect.rightbottom.y = g_current_field_pos.y + 480 + 64;
	}
}
/*画面を上へスクロール*/
void ScrollToUp(float jikiposy){
	//スクロール可能かをチェック
	if ((int)jikiposy - SCROLL_LIMIT > (int)g_stagesize.lefttop.y){
		g_current_field_pos.y = jikiposy - SCROLL_LIMIT;
		g_framerect.lefttop.x = g_current_field_pos.x - 64;
		g_framerect.lefttop.y = g_current_field_pos.y - 64;
		g_framerect.rightbottom.x = g_current_field_pos.x + 640 + 64;
		g_framerect.rightbottom.y = g_current_field_pos.y + 480 + 64;
	}
}
/*画面を下へスクロール*/
void ScrollToDown(float jikiposy){
	if ((int)jikiposy + SCROLL_LIMIT < (int)g_stagesize.rightbottom.y){
		g_current_field_pos.y = jikiposy - (480 - SCROLL_LIMIT);
		g_framerect.lefttop.x = g_current_field_pos.x - 64;
		g_framerect.lefttop.y = g_current_field_pos.y - 64;
		g_framerect.rightbottom.x = g_current_field_pos.x + 640 + 64;
		g_framerect.rightbottom.y = g_current_field_pos.y + 480 + 64;
	}
}

/*スクロールを計算に入れた描画関数*/
void DrawLineInView(float x1, float y1, float x2, float y2, int Color, int Thickness){
	DrawLine(XInView(x1), YInView(y1), XInView(x2), YInView(y2), 
			Color, Thickness);
}
void DrawCircleInView(float x, float y, float r, int Color, int FillFlag){
	DrawCircle(XInView(x), YInView(y), (int)r, Color, FillFlag);
}

/*アニメーション表示をする関数
画像を表示する座標xとy、
イメージデータの配列imgarray、総コマ数framenum、アニメの表示速度fps
アニメーション用の一時データanimtmpを引数とする。
1枚絵を表示する場合は、
imgarrayにint型変数のアドレス、allframeに1、fpsに1.0fを指定すること。
*/
void DrawAnimation(float x, float y, double ExtRate, double Angle,int TurnFlag,
				   int *imgarray, int allframe, float fps){
	//現在の経過秒数を求める
	float t = (float)( GetNowCount() & INT_MAX );
	//表示すべきコマ数を求める
	int animpat = (int)(t/(1000/fps)) % allframe;
	//そのコマを描画
	DrawRotaGraph(XInView(x),YInView(y), ExtRate, Angle, imgarray[animpat], TRUE, TurnFlag);
}

/*ベクトルを作成する関数
入力ベクトルを正規化してから長さを掛ける*/
Vector CreateVector(Vector in, float veclen){
	Vector result;
	in = Normalize(in);
	result.x = in.x * veclen;
	result.y = in.y * veclen;
	return result;
}
/*ベクトルの足し算を行う関数*/
Vector AddVector(Vector v1, Vector v2){
	Vector result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	return result;
}
/*ベクトルの引き算を行う関数
座標からベクトルを求める場合にも使える
その場合v1は終点、v2は始点とする*/
Vector SubVector(Vector v1, Vector v2){
	Vector result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	return result;
}

/*座標に速度ベクトルを足す関数（等速運動用）
g_frametimeを考慮する*/
Vector AddVectorInFrameTime(Vector pos, Vector speed){
	Vector result;
	result.x = pos.x + speed.x * g_frametime;
	result.y = pos.y + speed.y * g_frametime;
	return result;
}
/*座標に速度ベクトルと加速度を足す関数（等加速度運動用）
g_frametimeを考慮する*/
Vector AddVectorInFrameTime2(Vector pos, Vector speed, Vector accel){
	Vector result;
	result.x = pos.x + speed.x * g_frametime + accel.x * pow(g_frametime,2) / 2;
	result.y = pos.y + speed.y * g_frametime + accel.y * pow(g_frametime,2) / 2;
	return result;
}
/*ベクトルを正規化する関数*/
Vector Normalize(Vector in){
	Vector result;
	float l = sqrt( in.x * in.x + in.y * in.y );
	result.x = in.x / l;
	result.y = in.y / l;
	return result;
}
/*内積を求める関数*/
float DotProduct(Vector v1, Vector v2){
	return v1.x * v2.x + v1.y * v2.y;
}
/*外積を求める関数*/
float CrossProduct(Vector v1, Vector v2){
	return v1.x * v2.y - v1.y * v2.x;
}
/*ベクトルを回転させる関数*/
Vector RotateVector(Vector in, float radian){
	Vector result;
	result.x = -sin(radian)*in.y + cos(radian)*in.x;
	result.y =  cos(radian)*in.y + sin(radian)*in.x;
	return result;
}
/*ベクトルの長さの二乗を求める関数
長さが必要なときはsqrtで平方根を求める*/
float VectorLengthSquare(Vector in){
	return in.x * in.x + in.y * in.y;
}


/*Line2Dを描画する関数*/
void DrawLine2D(Line2D in, int Color, int Thickness){
	DrawLine(XInView(in.startpos.x), YInView(in.startpos.y),
		XInView(in.endpos.x), YInView(in.endpos.y), Color, Thickness);
}
/*Ball2Dを描画する関数*/
void DrawBall2D(Ball2D in, int Color, int FillFlag){
	DrawCircle(XInView(in.position.x), YInView(in.position.y),
		(int)in.hankei, Color, FillFlag);
}
/*線の傾きを求めてLine2Dのメンバkatamukiにセットする関数*/
void SetLine2DKatamuki(Line2D *in){
	in->katamuki = atan2(in->endpos.y - in->startpos.y, in->endpos.x - in->startpos.x);
}


/*Ball2DとLine2Dの当たり判定を行う関数*/
bool HitTestLineAndBall(Line2D linein, Ball2D ballin){
	Vector avec, bvec;
	avec = SubVector(ballin.position, linein.startpos);
	bvec = SubVector(linein.endpos, linein.startpos);
	float dot = DotProduct(avec, bvec);				//内積を求める
	float bl = VectorLengthSquare(bvec);		//ベクトルbの長さの二乗を求める
	if ( (dot > -ZEROVALUE) && (dot < bl) ) {		//衝突の可能性有り
		//円と線の距離を求める
		//ボールの中心から垂直におろした線の交点を求める
		Vector bnvec = Normalize(bvec);					//ベクトルbの正規化
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

/*頂点と線の位置関係を調べる関数
線の左または上側にあればTRUE、右または下側にあればFALSEを返す*/
bool IsPointAtLineFace(Line2D linein, Point2D ptin){
	Vector avec, bvec;
	avec = SubVector(ptin, linein.startpos);
	bvec = SubVector(linein.endpos, linein.startpos);
	float cross = CrossProduct(avec, bvec);
	//外積が正なら点は線の表側（左側）にある
	if ( cross > -ZEROVALUE )	return TRUE;
	return FALSE;
}

/*Line2D同士の当たり判定を行う関数*/
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


/*Ball2D同士の当たり判定を行う関数*/
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

/*Point2DとRect2Dの当たり判定を行う関数*/
bool HitTestPointAndBox(Rect2D rect, Point2D pt){
	if ( (rect.lefttop.x < pt.x) && (pt.x < rect.rightbottom.x) &&
		(rect.lefttop.y < pt.y) && (pt.y < rect.rightbottom.y) ){
			return TRUE;
	}
	return FALSE;
}

//シンプルタイマー関数
const int MAXTIMER = 32;
int g_goaltimes[MAXTIMER];
/*タイマーに目標値を設定して始動させる関数
目標値timeはミリ秒で設定する*/
void SetSimpleTimer(int idx, int time){
	if(idx>MAXTIMER) return;
	g_goaltimes[idx] = (GetNowCount() & INT_MAX) + time;
}
/*タイマーの結果を取得する関数
返値が負ならまだ目標値に達していない
正なら目標値を超えている*/
int GetPassedTime(int idx){
	return (GetNowCount() & INT_MAX) - g_goaltimes[idx];
}
