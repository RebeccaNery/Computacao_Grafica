#include <cstdlib>
#include <iostream>
#include <math.h>

using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#define LARGURA 500
#define ALTURA 500


int tx = 2;
int ty = 0;
int anguloRotacao = 0;

int circuloX = LARGURA/2;
int circuloY = ALTURA/2;
int circuloRaio = 50;



int numSegmentos = 360;


void initializeGlut(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(200, 50);
    glutInitWindowSize(LARGURA,ALTURA);
    glutCreateWindow("Meu primeiro programa com OpenGL");
    
}
void initializeOpenGL(){
    glClearColor(255, 255, 255, 255);
    glMatrixMode(GL_MODELVIEW);
    //esquerda, direita, baixo, cima
    gluOrtho2D(0, LARGURA, ALTURA, 0);
    glFlush();
}

void desenhaCirculo(){
  // Desenha um cÃ­rculo no centro da janela
  glColor3f(0.99, 0.76, 0.74); //altera o atributo de cor
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(circuloX, circuloY); 
  for (int i = 0; i <= numSegmentos; i++){
    glColor3f(1.0, (i % (numSegmentos / 2)) / 100, 0.0); //altera o atributo de cor
    glVertex2f(circuloX + circuloRaio * cos((i + anguloRotacao) * 3.14159 / 180.0),
               circuloY + circuloRaio * sin((i + anguloRotacao) * 3.14159 / 180.0));
  }
  glEnd();
}

void desenhaObjeto(){
    //Desenhar algo na tela
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);    //defini a cor vermelho
        glVertex2i(250, 200);           //desenho o primeiro vertice
        glColor3f(0.0f, 1.0f, 0.0f);    //defini a cor verde
        glVertex2i(175, 300);           //desenho o segundo vertice
        glColor3f(0.0f, 0.0f, 1.0f);    //defini a cor azul
        glVertex2i(325, 300);           //desenho o terceiro vertice
    glEnd();
}

void rotacionaCirculo(int){
    if(anguloRotacao >= 360){
        anguloRotacao -= anguloRotacao;
    }
    anguloRotacao++;
    
    glutPostRedisplay();

    glutTimerFunc(20, rotacionaCirculo, 0);
}

void transladaCirculo(int){
    if(circuloX + circuloRaio + tx >= LARGURA || circuloX - circuloRaio + tx <= 0){
        tx = -tx;
    }
    circuloX += tx;
    glutPostRedisplay();

    glutTimerFunc(20, transladaCirculo, 0);
}

void transladaCirculo2(){
    if(circuloX + circuloRaio + tx >= LARGURA || circuloX - circuloRaio + tx <= 0){
        tx = -tx;
    }
    circuloX += tx;
    glutPostRedisplay();
}

void desenha(){
    glClear(GL_COLOR_BUFFER_BIT);
    desenhaCirculo();
    glFlush();

}
void teclado(GLubyte tecla, GLint x, GLint y){
    
}
void tecladoEspecial(GLint tecla, GLint x, GLint y){
    switch (tecla)
    {
    case GLUT_KEY_LEFT:
        tx -= 5;
        break;
    case GLUT_KEY_RIGHT:
        tx += 5;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
int main(int argc, char **argv)
{
    initializeGlut(argc, argv);
    initializeOpenGL();
    glutDisplayFunc(desenha);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(tecladoEspecial);
    glutTimerFunc(0, rotacionaCirculo, 0);
    glutTimerFunc(0, transladaCirculo, 0);
    //glutIdleFunc(transladaCirculo2);
    
    //sempre no final colocar em loop infinito
    glutMainLoop();
    return 0;
}