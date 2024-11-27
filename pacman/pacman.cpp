// GRUPO: Rebecca Nery, Anita Donato, Ruan Vitor

#include <GL/glut.h>
#include <stdio.h>
#include <cmath>
#include <string.h>
#include <time.h> 
//GERENCIAMENTO DO SOM >>>>>>>>>>
#include <GL/glut.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
//FONTE
#include <ft2build.h>
#include FT_FREETYPE_H
FT_Library ft;
FT_Face face;
GLuint* textures;

Mix_Chunk* somMorte = nullptr;
Mix_Chunk* somInicio = nullptr;

#define LARGURA 500
#define ALTURA 500

const int LINHAS = 20;
const int COLUNAS = 20;
const int LARGURA_DO_BLOCO = 25;
float distancia = 0.0f;
int anguloMin = 30;  // Ângulo inicial da boca aberta
int anguloMax = 330; // Ângulo inicial da boca aberta
int deltaAngulo = 2; // velocidade de abertura/fechamento da boca
int direcaoJogador = 0, direcaoFantasma = 0; // 1 = direita, 2 = esquerda, 3 = cima, 4 = baixo
int contaPontos = 0;
int dx = 0, dy = 0, fx = 0, fy = 0;
int vidas = 3;
char mensagem[80];
bool gameOver = false; // Flag para verificar se o jogo acabou
bool pacmanMorto = false;

typedef struct jogador{
    int x, y;
    bool possuiChave;
} Jogador;

Jogador jogador = {1, 1, false};

typedef struct fantasma{
    int x, y;
} Fantasma;

Fantasma fantasma = {10, 11};

// Labirinto representado como matriz 2D
int labirinto[LINHAS][COLUNAS] = {
    
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 5, 5, 5, 5, 5, 5, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 5, 5, 5, 5, 5, 5, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} 
};

void resetaLabirinto(){
int labirintoOriginal[LINHAS][COLUNAS] = {
    
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 5, 5, 5, 5, 5, 5, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1, 5, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 5, 5, 5, 5, 5, 5, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1},
    {1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
memcpy(labirinto, labirintoOriginal, sizeof(labirintoOriginal)); // Copia o labirinto original para o atual
}

void inicializarAudio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Erro ao inicializar SDL_mixer: " << Mix_GetError() << std::endl;
        exit(1);
    }

    /// Carregar os sons
    somMorte = Mix_LoadWAV("pacman_death.wav");
    somInicio = Mix_LoadWAV("pacman_beginning.wav");

    if (!somMorte || !somInicio) {
        std::cerr << "Erro ao carregar som: " << Mix_GetError() << std::endl;
        exit(1);
    }
}

void verificarMorte() {
        Mix_PlayChannel(-1, somMorte, 0);
    
}

void tocarSomInicio() {
    Mix_PlayChannel(-1, somInicio, 0);
}

void finalizarAudio() {
    Mix_FreeChunk(somMorte);
    Mix_FreeChunk(somInicio);
    Mix_CloseAudio();
    SDL_Quit();
}

void initializeGlut(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(LARGURA,ALTURA);
    glutCreateWindow("PAC-MAN DIFERENCIADO");
    
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

void initFreeType() {
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Erro ao inicializar a biblioteca FreeType!" << std::endl;
        exit(1);
    } else {
    std::cout << "FreeType inicializado com sucesso!" << std::endl;
    }

    // Substitua com o caminho correto no seu sistema
    if (FT_New_Face(ft, "/mnt/c/Users/rebec/Repositorios/Computacao_Grafica/pacman/Minecraft.ttf", 0, &face)) {
        std::cerr << "Erro ao carregar a fonte!" << std::endl;
        exit(1);
    } else {
    std::cout << "Fonte carregada com sucesso!" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);  // Define o tamanho da fonte

    // Gerar as texturas para os caracteres
    textures = new GLuint[128];  // Assumindo ASCII de 0 a 127
    glGenTextures(128, textures);  // Gerar as texturas
    std::cout << "Texturas geradas para os caracteres!" << std::endl;

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Erro ao carregar o caractere " << c << std::endl;
            continue;
        }

// Verifique o tamanho da textura carregada
    std::cout << "Carregando caractere " << c << ": Largura = " << face->glyph->bitmap.width
              << ", Altura = " << face->glyph->bitmap.rows << std::endl;

        // Cria a textura para o caractere
        glBindTexture(GL_TEXTURE_2D, textures[c]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        GLenum err = glGetError();
if (err != GL_NO_ERROR) {
    std::cerr << "Erro OpenGL: " << gluErrorString(err) << std::endl;
}
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

void desenhaPlacar2(GLint x, GLint y, const char* texto) {
    glColor3f(1.0, 1.0, 0.0); // Cor do texto
    glEnable(GL_TEXTURE_2D);  // Ativa o uso de texturas

    glPushMatrix();  // Salva o estado atual da matriz de transformação
    glTranslatef(x, y, 0);  // Move para a posição onde o texto será desenhado

    // Para cada caractere no texto
    for (const char* c = texto; *c != '\0'; c++) {
        unsigned char character = *c;
        if (character < 128) {  // Verifica se o caractere está dentro do intervalo suportado
            // Configura a textura para o caractere
            glBindTexture(GL_TEXTURE_2D, textures[character]);

            GLfloat w = face->glyph->bitmap.width;  // Largura do caractere
            GLfloat h = face->glyph->bitmap.rows;  // Altura do caractere

            // Desenha o caractere como um quadrado com a textura aplicada
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0); // Posição inferior esquerda
            glTexCoord2f(1.0f, 0.0f); glVertex2f(w, 0); // Posição inferior direita
            glTexCoord2f(1.0f, 1.0f); glVertex2f(w, h); // Posição superior direita
            glTexCoord2f(0.0f, 1.0f); glVertex2f(0, h); // Posição superior esquerda
            glEnd();

            // Move para a próxima posição de texto
            x += w; // Avança a posição na direção X
        }
    }

    glPopMatrix();  // Restaura a matriz de transformação
    glDisable(GL_TEXTURE_2D);  // Desativa o uso de texturas
}

void desenhaPlacar(GLint x, GLint y, const char* texto) {
    glColor3f(1.0, 1.0, 0.0); // Cor do texto
    glRasterPos2f(x, y); // Define a posição inicial do texto
    for (const char* c = texto; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Renderiza cada caractere
    }
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

void desenhaBoca(GLint i, GLint j, GLint raio, GLint anguloMin, GLint anguloMax) {
    glColor3f(1.0, 1.0, 0.0); 
    glPushMatrix();  // Salva o estado atual da matriz
    glTranslatef(i * LARGURA_DO_BLOCO + 12.5, j * LARGURA_DO_BLOCO + 12.5, 0.0f); // Move o sistema de coordenadas para o centro da boca

    if (direcaoJogador == 2) {  // Caso de rotação para a esquerda
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f); // Aplica rotação de 180 graus no eixo Z (para a esquerda)
    }else if (direcaoJogador == 3) {  // Caso de rotação para cima
        glRotatef(270.0f, 0.0f, 0.0f, 1.0f); // Aplica rotação de 270 graus no eixo Z (para cima)
    }else if (direcaoJogador == 4) {  // Caso de rotação para baixo
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f); // Aplica rotação de 90 graus no eixo Z (para baixo)
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
            case 0: //Desenha bloco vazio - preto
                desenhaBloco(j, i, 0.0f, 0.0f, 0.0f);
                break;
            case 1: //Desenha Parede - azul 
                desenhaBloco(j, i, 0.0f, 0.0f, 0.5f);
                break;
            case 5: //Desenha bloco preto + bolinha
                desenhaBloco(j, i, 0.0f, 0.0f, 0.0f);
                desenhaCirculo(j, i, 3, 0, 360, 1.0f, 1.0f, 0.0f);
                break;
            case 6: //bolinhas magicas
                desenhaBloco(j, i, 0.0f, 0.0f, 0.0f);
                desenhaCirculo(j, i, 5, 0, 360, 1.0f, 0.0f, 1.0f);
                break;
            default:
                desenhaBloco(j, i, 1.0f, 1.0f, 1.0f);
                break;
            }
        }        
    }    
}

void desenhaJogador(){
    desenhaCirculo(jogador.x, jogador.y, 11.0, 0, 360, 0.0f, 0.0f, 0.0f);
    desenhaBoca(jogador.x, jogador.y, 12.5, anguloMin, anguloMax);
}

void desenhaFantasma(){
    desenhaBloco(fantasma.x, fantasma.y, 1.0f, 0.0f, 1.0f);
}

int aindaTemBolinhas(){
    for(int i = 0; i < LINHAS; i++){
        for(int j = 0; j < COLUNAS; j++){
            if(labirinto[i][j] == 5){
                return 1;
            }
        }
    }
    return 0;
}

bool verificaColisao() {
    float distancia = sqrt(pow(jogador.x - fantasma.x, 2) + pow(jogador.y - fantasma.y, 2));
    return distancia < 1;
}

void desenha(){
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameOver) {
        if (aindaTemBolinhas() == 0){
            sprintf(mensagem, "PARABENS! PLACAR: %d", contaPontos);
            desenhaPlacar(170, 487, mensagem);
        }
        sprintf(mensagem, "Game Over! 'seta subindo'");
        desenhaPlacar(20, 230, mensagem);
        sprintf(mensagem, "para reiniciar ou 'seta descendo' para sair.");
        desenhaPlacar(20, 250, mensagem);
    }else{
        desenhaLabirinto();
        desenhaJogador();
        desenhaFantasma();
        sprintf(mensagem, "PLACAR: %d", contaPontos);
        desenhaPlacar(20, 20, mensagem);
        sprintf(mensagem, "VIDAS: %d", vidas);
        desenhaPlacar(400, 20, mensagem);
        
    }
    
    glutSwapBuffers();
}

int determinaDirecaoJogador(){
    switch (direcaoJogador){
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
        dx=0; dy=0;
            break;
    }
return dx, dy;
}

int determinaDirecaoFantasma(){
    switch (direcaoFantasma){
        case 1: //direita
            fx=1; fy=0;
            break;
        case 2: //esquerda
            fx=-1; fy=0;
            break;
        case 3: //cima
            fx=0; fy=-1;
            break;
        case 4: //baixo
            fx=0; fy=1;
            break;
        default:
        fx=0; fy=0;
            break;
    }
return fx, fy;
}

void moverJogador(GLint valor){
dx, dy = determinaDirecaoJogador();
int newX = jogador.x + dx;
int newY = jogador.y + dy;

    if(labirinto[newY][newX] != 1){ // controle de colisões
        if(labirinto[newY][newX] == 5 ){
            labirinto[newY][newX] = 0; //pacman comeu a bolinha e ela sumiu!
            contaPontos++;
        } else if(labirinto[newY][newX] == 6){
            labirinto[newY][newX] = 0; //pacman comeu a bolinha e ficou mais rápido por 10 passos!
            contaPontos++;     
        }

        if (verificaColisao()) {
             
            verificarMorte();
            direcaoFantasma = 0;
            direcaoJogador = 0;
            fantasma.x = 10;
            fantasma.y = 11;
            jogador.x = 1;
            jogador.y = 1;
            vidas--;
            printf("Vidas: %d \n", vidas);
            if (vidas == 0) {
                gameOver = true;
            }
        }else{
            if(aindaTemBolinhas() == 0){
            gameOver = true;
            }
            jogador.x = newX;
            jogador.y = newY;
        }

        glutPostRedisplay();
    }// if != 1
        
        glutTimerFunc(150, moverJogador, 0);
    }// void moverJogador

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
    glutTimerFunc(16, mexeBoca, 0); // Intervalo de 30ms
}

void moverFantasma(GLint valor){
fx, fy = determinaDirecaoFantasma();
int newX = fantasma.x + fx;
int newY = fantasma.y + fy;

    if(labirinto[newY][newX] != 1){ // controle de colisões
        if (verificaColisao()) {
            verificarMorte();
            direcaoFantasma = 0;
            direcaoJogador = 0;
            fantasma.x = 10;
            fantasma.y = 11;
            jogador.x = 1;
            jogador.y = 1;
            vidas--;
            printf("Vidas: %d \n", vidas);
            if (vidas == 0) {
                gameOver = true;
            }
        }else{
        fantasma.x = newX;
        fantasma.y = newY;
        glutPostRedisplay();
        }
        
    }// if != 1


    
        
        glutTimerFunc(150, moverFantasma, 0);
    }

void tecladoEspecial(GLint tecla, GLint, GLint){
    if (gameOver){
        if(tecla == GLUT_KEY_UP){
            jogador.x = 1;
            jogador.y = 1;
            contaPontos = 0;
            gameOver = false;
            resetaLabirinto();
        }else if(tecla == GLUT_KEY_DOWN){
            exit(0);
        }
        return;
    }

    switch (tecla){
    case GLUT_KEY_LEFT:
        direcaoJogador = 2;
        break;
    case GLUT_KEY_RIGHT:
        direcaoJogador = 1;
        break;
    case GLUT_KEY_UP:
        direcaoJogador = 3;
        break;
    case GLUT_KEY_DOWN:
        direcaoJogador = 4;
        break;
    default:
        break;
    }

}

void tecladoPadrao(GLubyte tecla, GLint, GLint){
    switch (tecla){
    case 'd':
        direcaoFantasma = 1;
        break;
    case 'a':
        direcaoFantasma = 2;
        break;
    case 'w':
        direcaoFantasma = 3;
        break;
    case 's':
        direcaoFantasma = 4;
        break;
    default:
        break;
    }
    printf("Direcao: %d \n", direcaoFantasma);
}

int main(int argc, char **argv){

    initializeGlut(argc, argv);
    initializeOpenGL();
    //initFreeType();
        inicializarAudio();
        tocarSomInicio();
        
        
    glutDisplayFunc(desenha);
    glutSpecialFunc(tecladoEspecial);
    glutKeyboardFunc(tecladoPadrao);
    glutTimerFunc(0, mexeBoca, 0);
    glutTimerFunc(0, moverJogador, 0);
    glutTimerFunc(0, moverFantasma, 0);

    glutMainLoop();    

    finalizarAudio();

    return 0;
}