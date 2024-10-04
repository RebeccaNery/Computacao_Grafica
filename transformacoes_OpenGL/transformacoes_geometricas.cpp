#include <GL/glut.h>
#include <stdio.h>
#define LARGURA 500
#define ALTURA 500
int xi, yi, xf, yf;
int velocidade = 5;
float escalaY = 1.5f;
float escalaX = 1.5f;
float fatorEscala = 0.1f;
float rotacaoZ = 5.0f;
float fatorRotacao = 5.0f;

struct VERTEX
{
    int x;
    int y;
};

struct OBJECT
{
    VERTEX *vertices; 
    int num_vertices;
};

OBJECT *obj;

OBJECT* createObject(){
    OBJECT *obj = (OBJECT *)malloc(sizeof(OBJECT));
    obj->num_vertices = 5;
    obj->vertices = (VERTEX *)malloc(sizeof(VERTEX) * obj->num_vertices);
    obj->vertices[0].x = 110;
    obj->vertices[0].y = 50;
    obj->vertices[1].x = 110;
    obj->vertices[1].y = 70;
    obj->vertices[2].x = 100;
    obj->vertices[2].y = 80;
    obj->vertices[3].x = 90;
    obj->vertices[3].y = 70;
    obj->vertices[4].x = 90;
    obj->vertices[4].y = 50;

    return obj;
}

VERTEX calculate_centroid(OBJECT *obj)
{
    int i;
    VERTEX cent;
    cent.x = 0;
    cent.y = 0;
    for (i = 0; i < obj->num_vertices; i++)
    {
        cent.x += obj->vertices[i].x;
        cent.y += obj->vertices[i].y;
    }
    cent.x /= obj->num_vertices;
    cent.y /= obj->num_vertices;
    return cent;
}
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
    obj = createObject();
    glClear(GL_COLOR_BUFFER_BIT);
    //Desenhar algo na tela
    //glPointSize(4.0);
    glLineWidth(4.0);
    //glColor3f(0.0f, 0.0f, 1.0f);
    glColor3d(0, 0, 255);
    glBegin(GL_POLYGON);
        for(int i=0; i<obj->num_vertices; i++){
            glVertex2i(obj->vertices[i].x, obj->vertices[i].y);
        }
    glEnd();
    
    glFlush();
}
void teclado(GLubyte tecla, GLint x, GLint y){
    if(tecla == 'a' || tecla == 'A'){
        glTranslatef(-velocidade, 0, 0);
    } else if(tecla == 'd' || tecla == 'D'){
        glTranslatef(velocidade, 0, 0);
    } else if(tecla == 'w' || tecla == 'W'){
        glTranslatef(0, -velocidade, 0);
    }else if(tecla == 's' || tecla == 'S'){
        glTranslatef(0, velocidade, 0);
    } else if(tecla == '-'){
        velocidade -= 5;
        printf("Velocidade ==> %d\n", velocidade);
        escalaY -= fatorEscala;
        printf("Escala Y ==> %f\n", escalaY);
        escalaX -= fatorEscala;
        printf("Escala X ==> %f\n", escalaX);
        rotacaoZ -= fatorRotacao;
        printf("Rotacao Z ==> %f\n", rotacaoZ);
    } else if(tecla == '+'){
        velocidade += 5;
        printf("Velocidade ==> %d\n", velocidade);
        escalaY += fatorEscala;
        printf("Escala Y ==> %f\n", escalaY);
        escalaX += fatorEscala;
        printf("Escala X ==> %f\n", escalaX);
        rotacaoZ += fatorRotacao;
        printf("Rotacao Z ==> %f\n", rotacaoZ);
    } 
    glutPostRedisplay();
    printf("tecla %c pressionada com o cursor na posicao x=%d; y=%d\n", tecla, x, y);
}
void tecladoEspecial(GLint tecla, GLint x, GLint y){
    printf("tecla %d pressionada com o cursor na posicao x=%d; y=%d\n", tecla, x, y);
    if (tecla == GLUT_KEY_UP){
        VERTEX centroide = calculate_centroid(obj);
        glTranslatef(centroide.x, centroide.y, 0.0f);
        glScalef(1.0f, escalaY, 1.0f);
        glTranslatef(-centroide.x, -centroide.y, 0.0f);
    }else if (tecla ==  GLUT_KEY_DOWN){
        VERTEX centroide = calculate_centroid(obj);
        glTranslatef(centroide.x, centroide.y, 0.0f);
        glScalef(1.0f, 0.5f, 1.0f);
        glTranslatef(-centroide.x, -centroide.y, 0.0f);
    }else if(tecla == GLUT_KEY_RIGHT){
        VERTEX centroide = calculate_centroid(obj);
        glTranslatef(centroide.x, centroide.y, 0.0f);
        glScalef(escalaX, 1.0f, 1.0f);
        glTranslatef(-centroide.x, -centroide.y, 0.0f);
    }else if(tecla == GLUT_KEY_LEFT){
        VERTEX centroide = calculate_centroid(obj);
        glTranslatef(centroide.x, centroide.y, 0.0f);
        glScalef(0.5f, 1.0f, 1.0f);
        glTranslatef(-centroide.x, -centroide.y, 0.0f);
    }
    glutPostRedisplay();
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
        if(acao == GLUT_DOWN){
            VERTEX centroide = calculate_centroid(obj);
            glTranslatef(centroide.x, centroide.y, 0.0f);
            glRotatef(rotacaoZ, 0.0f, 0.0f, 1.0f);
            glTranslatef(-centroide.x, -centroide.y, 0.0f);
            glutPostRedisplay();
        }
    } else if(botao == GLUT_RIGHT_BUTTON){
        printf("Botão direito ");
        if (acao == GLUT_DOWN){
            VERTEX centroide = calculate_centroid(obj);
            glTranslatef(centroide.x, centroide.y, 0.0f);
            glRotatef(-rotacaoZ, 0.0f, 0.0f, 1.0f);
            glTranslatef(-centroide.x, -centroide.y, 0.0f);
            glutPostRedisplay();
        }
    }
    if (acao == GLUT_DOWN){
        printf("pressionado na posição %dx%d\n", x, y);
    } else if (acao == GLUT_UP){
        printf("liberado na posição %dx%d\n", x, y);
    }
}

void mouseMovimento(GLint x, GLint y){
    printf("Movendo o mouse clicado na posicao %dx%d\n", x, y);
    //desenhaPonto(x, y);
}

void mouseMovimentoPassivo(GLint x, GLint y){
    printf("Movendo o mouse sem pressionar na posicao %dx%d\n", x, y);
}

int main(int argc, char **argv)
{
    initializeGlut(argc, argv);
    initializeOpenGL();
    glutDisplayFunc(desenha);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(tecladoEspecial);
    glutMouseFunc(mouseGerenciador);
    //glutMotionFunc(mouseMovimento);
    //glutPassiveMotionFunc(mouseMovimentoPassivo);
    
    //sempre no final colocar em loop infinito
    glutMainLoop();
    return 0;
}