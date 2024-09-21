#include "definitions.h"
#include <iostream>
#include <cmath>
#include <stdio.h>

using namespace std;

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
    //*************************************************************************
    // Chame aqui as funções do mygl.h
    //*************************************************************************
    //PutPixel(256, 256);
    //DrawLine(250, 250, 512, 0);
    //DrawLine(250, 250, 0, 512);
    //DrawLine(250, 250, 512, 512);
    //DrawLine(250, 250, 0, 0);
    DrawTriangle(100, 100, 200, 200, 300, 100);
    DrawTriangle(100, 100, 200, 200, 100, 300);
    DrawTriangle(400, 400, 450, 450, 300, 400);

}

void PutPixel(int x, int y) {
    // Escreve um pixel vermelho na posicao (x, y) da tela:
    FBptr[4*x + 4*512*y + 0] = 255; // componente R
    FBptr[4*x + 4*512*y + 1] = 0;   // componente G
    FBptr[4*x + 4*512*y + 2] = 0;   // componente B
    FBptr[4*x + 4*512*y + 3] = 255; // componente A

}

void DrawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1); // Diferença absoluta em x
    int dy = abs(y2 - y1); // Diferença absoluta em y

    int sx = (x1 < x2) ? 1 : -1; // Direção de incremento para x
    int sy = (y1 < y2) ? 1 : -1; // Direção de incremento para y

    int erro = dx - dy; // Valor do erro inicial

    while (true) {
        // Desenha o pixel atual (cor: vermelho, RGBA = 255, 0, 0, 255)
        PutPixel(x1, y1);

        // Se alcançou o ponto final, interrompe o loop
        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * erro;

        // Ajusta o erro e as coordenadas x
        if (e2 > -dy) {
            erro -= dy;
            x1 += sx;
        }

        // Ajusta o erro e as coordenadas y
        if (e2 < dx) {
            erro += dx;
            y1 += sy;
        }
    }
}

void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    DrawLine(x1, y1, x2, y2);
    DrawLine(x2, y2, x3, y3);
    DrawLine(x3, y3, x1, y1);
}