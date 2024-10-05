#include <GL/glut.h>
#include <stdio.h>
#define LARGURA 500
#define ALTURA 500
int xi, yi, xf, yf, auxiliar, contador=0;
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

void limparTela() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutPostRedisplay(); 
    //glFlush();  
}


void desenhaTrianguloProfessor(){
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
    //printf("tecla %c pressionada com o cursor na posicao x=%d; y=%d\n", tecla, x, y);
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
    glColor3f(0.0f, 0.0f, 1.0f); //azul
    glPointSize(8.0);
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
    glFlush();
}
void desenhaLinha(GLint xi, GLint yi, GLint xf, GLint yf){
    glLineWidth(8.0);
    glColor3f(1.0f, 0.0f, 0.0f); 
    glBegin(GL_LINES);
        glVertex2i(xi, yi);
        glVertex2i(xf, yf);
    glEnd();
    glFlush();
}

void controleDeCliques(GLint botao, GLint acao, GLint x, GLint y) {
    glColor3f(1.0f, 0.0f, 0.0f);
    
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
            primeiroclique = false;
        }
    }
}



void desenhaLinhaCliques(){
    glClear(GL_COLOR_BUFFER_BIT);

    if (primeiroclique){
        desenhaPonto(xi, yi);
    }

    if(segundoclique){ 
        desenhaPonto(xf, yf);        
        desenhaLinha(xi, yi, xf, yf);
    }
    
    glFlush();
}

void desenhaRetanguloCliques(){
    glClear(GL_COLOR_BUFFER_BIT);

    if (primeiroclique){
        desenhaPonto(xi, yi);
    }

    if(segundoclique){ 
        desenhaPonto(xf, yf);        
        glBegin(GL_QUADS);
            glVertex2i(xi, yi);
            glVertex2i(xf, yi);
            glVertex2i(xf, yf);
            glVertex2i(xi, yf);
        glEnd();
    }
    
    glFlush();
}

void regras_menu(GLint opcao) {
    switch (opcao) {
        case 4:
            limparTela();
            printf("Tela limpa\n");
            break;
        case 5:
            printf("Saindo...\n");
            exit(0);  // Sair do programa
            break;
    }
    glutPostRedisplay();
}

void regras_submenu(GLint opcao) {
    printf("Opção %d selecionada\n", opcao);
    switch (opcao) {
        case 1:
            printf("Opção 'Desenhar Ponto' selecionada\n");
            desenhaPonto(xi, yi);
            break;
        case 2:
            printf("Opção 'Desenhar Linha' selecionada\n");
            desenhaLinhaCliques();
            break;
        case 3:
            printf("Opção 'Desenhar Retangulo' selecionada\n");
            desenhaRetanguloCliques();
            break;
    }
    glutPostRedisplay();
}

void criarMenu() {
    int submenu = glutCreateMenu(regras_submenu);
    glutAddMenuEntry("Desenhar Ponto", 1);
    glutAddMenuEntry("Desenhar Linha", 2);
    glutAddMenuEntry("Desenhar Retangulo", 3);
    
    
    // Criar menu e adicionar opções
    int menu = glutCreateMenu(regras_menu);
    glutAddSubMenu("Desenhar", submenu);
    glutAddMenuEntry("Limpar Tela", 4);
    glutAddMenuEntry("Sair", 5);

    // Associar o menu ao botão direito do mouse
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void mouseGerenciador(GLint botao, GLint acao, GLint x, GLint y){
    glColor3f(1.0f, 0.0f, 0.0f);
    if(botao == GLUT_LEFT_BUTTON){
        printf("Botão esquerdo ");
        if(acao == GLUT_DOWN){
            desenhaPonto(x, y);
            auxiliar = 1;
            
        }
    } else if(botao == GLUT_RIGHT_BUTTON){
        printf("Botão direito ");
        auxiliar = 0;
    }

    if (acao == GLUT_DOWN){
        printf("pressionado na posição %dx%d\n", x, y);
    } else if (acao == GLUT_UP){
        printf("liberado na posição %dx%d\n", x, y);
    }


}

void mouseMovimento(GLint x, GLint y){
    //printf("Movendo o mouse clicado na posicao %dx%d\n", x, y);
    if(auxiliar == 1){
        desenhaPonto(x, y);
    }
    
}

void mouseMovimentoPassivo(GLint x, GLint y){
    printf("Movendo o mouse sem pressionar na posicao %dx%d\n", x, y);
}

void desenha(){
    glClear(GL_COLOR_BUFFER_BIT);
    /*desenhaPonto(xi, yi);
    desenhaRetanguloCliques();
    desenhaLinhaCliques();*/
    //glFlush();
    glutPostRedisplay();
}


int main(int argc, char **argv)
{
    
    initializeGlut(argc, argv);
    initializeOpenGL();

// ----------------------- O GLUT SÓ ACEITA UMA FUNÇÃO DE DISPLAY DE CADA VEZ!!! --------------------
    //glutDisplayFunc(desenhaTrianguloProfessor);
    //glutDisplayFunc(desenhaLinhaCliques);
    //glutDisplayFunc(desenhaRetanguloCliques);
    glutDisplayFunc(desenha);

// ----------------------- FUNÇÕES DO TECLADO  --------------------
    //glutKeyboardFunc(teclado);
    //glutSpecialFunc(tecladoEspecial);

// ----------------------- FUNÇÕES DO MOUSE  --------------------
    glutMouseFunc(mouseGerenciador);                //mouse pressionado --- sempre usar essa
    glutMouseFunc(controleDeCliques);               // sempre usar essa
    //glutMotionFunc(mouseMovimento);               //mouse pressionado e movendo ao mesmo tempo
    //glutPassiveMotionFunc(mouseMovimentoPassivo); //mouse movendo sem pressionar
    
criarMenu();


    glutMainLoop();                                 //sempre no final colocar em loop infinito  --> mantém a janela aberta
    return 0;
}