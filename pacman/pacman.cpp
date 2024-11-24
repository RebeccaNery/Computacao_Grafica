#include <GL/glut.h>
#include <stdio.h>
#include <cmath>

#define LARGURA 500
#define ALTURA 500

const int LINHAS = 20;
const int COLUNAS = 20;
const int LARGURA_DO_BLOCO = 25;

int anguloMin = 30;  // Ângulo inicial da boca aberta
int anguloMax = 330; // Ângulo inicial da boca aberta
int deltaAngulo = 2; // A velocidade de abertura/fechamento da boca
//bool abrindo = true; // Variável de controle da direção da boca (true = abrindo, false = fechando)
int direcao = 0; // 1 = direita, 2 = esquerda, 3 = cima, 4 = baixo
int contaPontos = 0;


typedef struct jogador{
    int x, y;
    bool possuiChave;
} Jogador;

Jogador jogador = {1, 1, false};

// Labirinto representado como matriz 2D
int labirinto[LINHAS][COLUNAS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 5, 5, 5, 1, 5, 5, 5, 5, 1, 5, 1, 5, 5, 5, 1, 5, 5, 3, 1},
    {1, 1, 1, 5, 1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1},
    {1, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 5, 1},
    {1, 5, 5, 5, 1, 5, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 1},
    {1, 1, 1, 5, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1, 1, 1, 5, 1, 5, 1},
    {1, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 1, 5, 5, 5, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1, 5, 1},
    {1, 5, 1, 5, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 1, 5, 1},
    {1, 5, 1, 5, 1, 1, 5, 1, 5, 1, 5, 1, 1, 1, 5, 1, 5, 1, 5, 1},
    {1, 5, 1, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 1},
    {1, 5, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 5, 1},
    {1, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 5, 1},
    {1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 1, 5, 5, 5, 5, 5, 1},
    {1, 5, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1},
    {1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 2, 5, 5, 5, 5, 4, 1},
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

void desenhaCirculo(GLint i, GLint j, GLint raio, GLint anguloMin, GLint anguloMax, GLfloat R, GLfloat G, GLfloat B){
    glColor3f(R, G, B);
    glBegin(GL_POLYGON);
        for (int k = 0; k < 360; k++){
            float angulo = k * 3.14159265f / 180.0f;
            float x = i * LARGURA_DO_BLOCO + 12.5 + cos(angulo) * raio; //aumentei o raio para 12.5
            float y = j * LARGURA_DO_BLOCO + 12.5 + sin(angulo) * raio; //aumentei o raio para 12.5
            glVertex2f(x, y);
        }
    glEnd();
}


/*void desenhaBoca(GLint i, GLint j, GLint raio, GLint anguloMin, GLint anguloMax){ //desenha um círculo amarelo em cima do círculo preto
    glColor3f(1.0, 1.0, 0.0); 

   if (direcao == 2){
    glPushMatrix();  
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(i*LARGURA_DO_BLOCO + 12.5, j*LARGURA_DO_BLOCO + 12.5); //centro do círculo
        for (int k = anguloMin; k <= anguloMax; k++){ //abertura da boca
        float angulo = k * 3.14159265f / 180.0f; //converte o ângulo de graus para radianos
        glVertex2f(i*LARGURA_DO_BLOCO + 12.5 + raio * cos(angulo),
               j*LARGURA_DO_BLOCO + 12.5 + raio * sin(angulo)); //desenha os vértices do círculo
        }
    glEnd();
    glPopMatrix(); 
    glFlush();
    }else{
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(i*LARGURA_DO_BLOCO + 12.5, j*LARGURA_DO_BLOCO + 12.5); //centro do círculo
        for (int k = anguloMin; k <= anguloMax; k++){ //abertura da boca
        float angulo = k * 3.14159265f / 180.0f; //converte o ângulo de graus para radianos
        glVertex2f(i*LARGURA_DO_BLOCO + 12.5 + raio * cos(angulo),
               j*LARGURA_DO_BLOCO + 12.5 + raio * sin(angulo)); //desenha os vértices do círculo
        }
    glEnd();
    glFlush();
    //glPopMatrix();
    }*/

    void desenhaBoca(GLint i, GLint j, GLint raio, GLint anguloMin, GLint anguloMax) {
    glColor3f(1.0, 1.0, 0.0); 

    glPushMatrix();  // Salva o estado atual da matriz
    // Move o sistema de coordenadas para o centro da boca
    glTranslatef(i * LARGURA_DO_BLOCO + 12.5, j * LARGURA_DO_BLOCO + 12.5, 0.0f);

    if (direcao == 2) {  // Caso de rotação
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f); // Aplica rotação no eixo Z
    }else if (direcao == 3) {  // Caso de rotação
        glRotatef(270.0f, 0.0f, 0.0f, 1.0f); // Aplica rotação no eixo Z
    }else if (direcao == 4) {  // Caso de rotação
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f); // Aplica rotação no eixo Z
    }

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f); // Centro do círculo no novo sistema de coordenadas
        for (int k = anguloMin; k <= anguloMax; k++) { // Abertura da boca
            float angulo = k * 3.14159265f / 180.0f; // Converte o ângulo de graus para radianos
            glVertex2f(raio * cos(angulo), raio * sin(angulo)); // Vértices do círculo
        }
    glEnd();

    glPopMatrix();  // Restaura o estado da matriz
    glFlush();
}




void desenhaLabirinto(){
    for (int i = 0; i < LINHAS; i++){
        for (int j = 0; j < COLUNAS; j++){
            switch (labirinto[i][j])
            {
            case 0: //Desenha bloco vazio
                desenhaBloco(j, i, 0.0f, 0.0f, 0.0f);
                break;
            case 1: //Desenha Parede
                desenhaBloco(j, i, 0.0f, 0.0f, 0.5f);
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
            case 5: //Desenha bloco em branco + bolinha
                desenhaBloco(j, i, 0.0f, 0.0f, 0.0f);
                desenhaCirculo(j, i, 3, 0, 360, 1.0f, 1.0f, 0.0f);
                break;
            default:
                break;
            }
        }        
    }    
}

void desenhaJogador(){
    //desenhaBloco(jogador.x, jogador.y, 1.0f, 0.0f, 1.0f);
    desenhaCirculo(jogador.x, jogador.y, 11.0, 0, 360, 0.0f, 0.0f, 0.0f);
    desenhaBoca(jogador.x, jogador.y, 12.5, anguloMin, anguloMax);
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
        } else if(labirinto[newY][newX] == 5){
            contaPontos++;
            printf("Pontos: %d \n", contaPontos); //nao esta aparecendo na tela !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            labirinto[newY][newX] = 0; //pacman comeu a bolinha e ela sumiu!
            
        }
        jogador.x = newX;
        jogador.y = newY;
        
        glutPostRedisplay();
    }
}

void moverJogadorDINAMICO(GLint valor){
int dx = 0, dy = 0;

switch (direcao)
{
case 1: //direita
    dx=1; dy=0;
    break;
case 2: //esquerda  
    dx=-1; dy=0;
    break;  
case 3: //cima
    dx=0; dy=-1;
    break;
case 4: //baixo 
    dx=0; dy=1;
    break;
default:
    break;
}

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
            printf("Voce escapou do Labirinto!!! :) \n");
            exit(0);
        } else if(labirinto[newY][newX] == 5){
            labirinto[newY][newX] = 0; //pacman comeu a bolinha e ela sumiu!
            contaPontos++;
            printf("Pontos: %d \n", contaPontos);
        }

        jogador.x = newX;
        jogador.y = newY;
        glutPostRedisplay();
    }


        glutTimerFunc(200, moverJogadorDINAMICO, 0);
    }

void mexeBoca(GLint valor){
    // Abrir ou fechar a boca de acordo com os limites
    anguloMin += deltaAngulo;
    anguloMax -= deltaAngulo;

    // Verifica se chegou no limite de abertura e inverte o movimento
    if (anguloMin >= 0 && anguloMax <= 360) {
        deltaAngulo = -deltaAngulo; // Inverte a direção
    }
    if (anguloMin <= 30 && anguloMax >= 330) {
        deltaAngulo = -deltaAngulo; // Inverte a direção novamente
    }

    // Redesenha a cena com a nova posição da boca
    glutPostRedisplay();

    // Reprograma o próximo frame da animação
    glutTimerFunc(30, mexeBoca, 0); // Intervalo de 30ms
}

void teclado(GLint tecla, GLint, GLint){
    switch (tecla){
    case GLUT_KEY_LEFT:
        direcao = 2;
        //moverJogador(-1, 0);
        break;
    case GLUT_KEY_RIGHT:
        direcao = 1;
        //moverJogador(1, 0);
        break;
    case GLUT_KEY_UP:
    direcao = 3;
        //moverJogador(0, -1);
        break;
    case GLUT_KEY_DOWN:
    direcao = 4;
        //moverJogador(0, 1);
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
    glutTimerFunc(0, mexeBoca, 0);
    glutTimerFunc(0, moverJogadorDINAMICO, 0);

    glutMainLoop();
    return 0;
}