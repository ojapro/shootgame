#include "structs.h"

#ifndef _DEBUG_H
#define _DEBUG_H

void DrawHitRect(int,int,HitRect);
void DrawHitCircle(int,int,HitCircle);
void InitDebugMode(DebugMode *debugctrl,TraceLog *log);
int SceneControl(Screen *);
#endif


