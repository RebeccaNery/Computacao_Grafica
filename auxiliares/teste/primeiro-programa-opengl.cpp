#include <GL/glut.h>
#include <stdio.h>
#define LARGURA 500
#define ALTURA 500
int xi, yi, xf, yf;
bool desenhoContinuo = false;
bool primeiroclique = false;
bool segundoclique = false;


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
    glClear(GL_COLOR_BUFFER_BIT);
    //Desenhar algo na tela
    glPointSize(4.0);
    glLineWidth(4.0);
    glBegin(GL_TRIANGLES);
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
    if (tecla == 'r' || tecla == 'R'){
        glColor3f(1.0f, 0.0f, 0.0f); //vermelho
    }else if (tecla == 'g' || tecla == 'G'){
        glColor3f(0.0f, 1.0f, 0.0f); //verde
    }else if (tecla == 'b' || tecla == 'B'){
        glColor3f(0.0f, 0.0f, 1.0f); //azul
    }
}
void tecladoEspecial(GLint tecla, GLint x, GLint y){
    printf("tecla %d pressionada com o cursor na posicao x=%d; y=%d\n", tecla, x, y);
}
void desenhaPonto(GLint x, GLint y){
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
    glFlush();
}
void desenhaLinha(GLint xi, GLint yi, GLint xf, GLint yf){
    glBegin(GL_LINES);
        glVertex2i(xi, yi);
        glVertex2i(xf, yf);
    glEnd();
    glFlush();
}
void mouseGerenciador(GLint botao, GLint acao, GLint x, GLint y){
    if(botao == GLUT_LEFT_BUTTON){
        printf("BotÃ£o esquerdo ");
        if(acao == GLUT_DOWN){
            desenhaPonto(x, y);
            desenhoContinuo = true;
        }
    } else if(botao == GLUT_RIGHT_BUTTON){
        printf("BotÃ£o direito ");
    }
    if (acao == GLUT_DOWN){
        printf("pressionado na posiÃ§Ã£o %dx%d\n", x, y);
    } else if (acao == GLUT_UP){
        printf("liberado na posiÃ§Ã£o %dx%d\n", x, y);
    }
}

void mouseMovimento(GLint x, GLint y){
    if(desenhoContinuo){
            desenhaPonto(x, y);
    }
}

void mouseMovimentoPassivo(GLint x, GLint y){
    printf("Movendo o mouse sem pressionar na posicao %dx%d\n", x, y);
}

void controleDeCliques(GLint botao, GLint acao, GLint x, GLint y) {
    
    if (botao == GLUT_LEFT_BUTTON && acao == GLUT_DOWN) {

        if (!primeiroclique) {
            // Registrar as coordenadas do primeiro clique
            xi = x;
            yi = y;
            primeiroclique = true;
            printf("primeiro clique ==> xi: %d, yi: %d\n", xi, yi);
            desenhaPonto(xi, yi);
        } else {
            // Registrar as coordenadas do segundo clique
            xf = x;
            yf = y;
            segundoclique = true;
            printf("segundo clique ==> xf: %d, yf: %d\n", xf, yf);
            desenhaPonto(xf, yf);
            desenhaLinha(xi, yi, xf, yf);
            primeiroclique = false;
        }
    }
}

int main(int argc, char **argv)
{
    initializeGlut(argc, argv);
    initializeOpenGL();
    glutDisplayFunc(desenha);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(tecladoEspecial);
    glutMouseFunc(mouseGerenciador);
    glutMouseFunc(controleDeCliques);
    glutMotionFunc(mouseMovimento);
    glutPassiveMotionFunc(mouseMovimentoPassivo);
    
    //sempre no final colocar em loop infinito
    glutMainLoop();
    return 0;
}