#include <GL/glut.h>
#include <stdio.h>
#define LARGURA 500
#define ALTURA 500

const int LINHAS = 20;
const int COLUNAS = 20;
const int LARGURA_DO_BLOCO = 25;

typedef struct jogador{
    int x, y;
    bool possuiChave;
} Jogador;

Jogador jogador = {1, 1, false};

// Labirinto representado como matriz 2D
int labirinto[LINHAS][COLUNAS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 3, 1},
    {1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 4, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void initializeGlut(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(LARGURA,ALTURA);
    glutCreateWindow("PAC-MAN");
    
}
void initializeOpenGL(){
    glClearColor(0, 0, 0, 255);
    glMatrixMode(GL_MODELVIEW);
    //esquerda, direita, baixo, cima
    gluOrtho2D(0, LARGURA, ALTURA, 0);
}

void desenhaBloco(GLint i, GLint j, GLfloat R, GLfloat G, GLfloat B){
    glColor3f(R, G, B);
    glBegin(GL_QUADS);
        glVertex2d(i * LARGURA_DO_BLOCO, j * LARGURA_DO_BLOCO);
        glVertex2d((i+1) * LARGURA_DO_BLOCO, j * LARGURA_DO_BLOCO);
        glVertex2d((i+1) * LARGURA_DO_BLOCO, (j+1) * LARGURA_DO_BLOCO);
        glVertex2d(i * LARGURA_DO_BLOCO, (j+1) * LARGURA_DO_BLOCO);
    glEnd();
}

void desenhaLabirinto(){
    for (int i = 0; i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
            switch (labirinto[i][j])
            {
            case 0: //Desenha bloco vazio
                desenhaBloco(j, i, 1.0f, 1.0f, 1.0f);
                break;
            case 1: //Desenha Parede
                desenhaBloco(j, i, 0.0f, 0.0f, 1.0f);
                break;
            case 2: //Desenha porta trancada
                desenhaBloco(j, i, 1.0f, 0.0f, 0.0f);
                break;
            case 3: //Desenha Chave
                desenhaBloco(j, i, 1.0f, 1.0f, 0.0f);
                break;
            case 4: //Desenha Saida
                desenhaBloco(j, i, 0.0f, 1.0f, 0.0f);
                break;
            default:
                break;
            }
        }        
    }    
}

void desenhaJogador(){
    desenhaBloco(jogador.x, jogador.y, 1.0f, 0.0f, 1.0f);
}

void desenha(){
    glClear(GL_COLOR_BUFFER_BIT);
    desenhaLabirinto();
    desenhaJogador();
    glutSwapBuffers();
}

void moverJogador(GLint dx, GLint dy){
    int newX = jogador.x + dx;
    int newY = jogador.y + dy;

    if(labirinto[newY][newX] != 1){
        if(labirinto[newY][newX] == 2 && !jogador.possuiChave){
            printf("Porta trancada! Encontre a Chave! \n");
            return;
        } else if(labirinto[newY][newX] == 3){
            jogador.possuiChave = true;
            printf("Chave encontrada! \n");
            labirinto[newY][newX] = 0;
        } else if(labirinto[newY][newX] == 4){
            printf("VocÃª escapou do Labirinto!!! :) \n");
            exit(0);
        } 
        jogador.x = newX;
        jogador.y = newY;
        glutPostRedisplay();
    }
}

void teclado(GLint tecla, GLint, GLint){
    switch (tecla){
    case GLUT_KEY_LEFT:
        moverJogador(-1, 0);
        break;
    case GLUT_KEY_RIGHT:
        moverJogador(1, 0);
        break;
    case GLUT_KEY_UP:
        moverJogador(0, -1);
        break;
    case GLUT_KEY_DOWN:
        moverJogador(0, 1);
        break;
    default:
        break;
    }
}

int main(int argc, char **argv){
    initializeGlut(argc, argv);
    initializeOpenGL();
    glutDisplayFunc(desenha);
    glutSpecialFunc(teclado);
    glutMainLoop();
    return 0;
}