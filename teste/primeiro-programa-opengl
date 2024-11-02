#include <GL/glut.h>
#include <stdio.h>
#define LARGURA 500
#define ALTURA 500

void initializeGlut(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
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

void desenha(){
    printf("Entrei no metodo desenha\n");
    glClear(GL_COLOR_BUFFER_BIT);
    //Desenhar algo na tela
    glPointSize(4.0);
    glLineWidth(4.0);
    glBegin(GL_LINE_LOOP);
        glColor3f(1.0f, 0.0f, 0.0f);    //defini a cor vermelho
        glVertex2i(250, 100);           //desenho o primeiro vertice
        glColor3f(0.0f, 1.0f, 0.0f);    //defini a cor verde
        glVertex2i(100, 350);           //desenho o segundo vertice
        glColor3f(0.0f, 0.0f, 1.0f);    //defini a cor azul
        glVertex2i(400, 350);           //desenho o terceiro vertice
    glEnd();
    glFlush();
}
void teclado(GLubyte tecla, GLint x, GLint y){
    printf("tecla %c pressionada com o cursor na posicao x=%d; y=%d\n", tecla, x, y);
}
void tecladoEspecial(GLint tecla, GLint x, GLint y){
    printf("tecla %d pressionada com o cursor na posicao x=%d; y=%d\n", tecla, x, y);
}
int main(int argc, char **argv)
{
    initializeGlut(argc, argv);
    initializeOpenGL();
    glutDisplayFunc(desenha);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(tecladoEspecial);
    
    //sempre no final colocar em loop infinito
    glutMainLoop();
    return 0;
}