#ifndef _MYGL_H_
#define _MYGL_H_
#endif

#include "definitions.h"

//-----------------------------------------------------------------------------
void MyGlDraw(void);

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************

void PutPixel(int x, int y);
void DrawLine(int x1, int y1, int x2, int y2);
void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);