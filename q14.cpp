#include <cmath>
#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>

const double PI = 3.141592653589793238463;

typedef struct coordenada
{
    double x;
    double y;
} coordenada;

typedef struct poligono
{
    coordenada *pontos;
    int qtdVertices;
} poligono;

double x_pos = -1.0;
double y_pos = 9.0;
double passos = 360*2;
int pulseInterval = passos;
double currentScale = 1;
double aux;
int cont = 0;

poligono *p;
coordenada *pontoPivo;

poligono *criaPoligono ()
{
    poligono *novo = (poligono*) malloc(sizeof(poligono));
    novo->qtdVertices = 4;
    novo->pontos = (coordenada*) malloc(novo->qtdVertices*sizeof(coordenada));
    novo->pontos[0].x = x_pos;
    novo->pontos[0].y = y_pos;
    novo->pontos[1].x = x_pos;
    novo->pontos[1].y = y_pos-1.0;
    novo->pontos[2].x = x_pos+1.0;
    novo->pontos[2].y = y_pos-1.0;
    novo->pontos[3].x = x_pos+1.0;
    novo->pontos[3].y = y_pos;
    return novo;
}

void redefineCoordenadaPoligono ()
{
    p->pontos[0].x = x_pos;
    p->pontos[0].y = y_pos;
    p->pontos[1].x = x_pos;
    p->pontos[1].y = y_pos-1.0;
    p->pontos[2].x = x_pos+1.0;
    p->pontos[2].y = y_pos-1.0;
    p->pontos[3].x = x_pos+1.0;
    p->pontos[3].y = y_pos;
}

coordenada *calculaCentro(poligono *p1)
{
    coordenada *centro = (coordenada*) malloc(sizeof(coordenada));
    centro->x = 0;
    centro->y = 0;

    for (int i=0; i < p1->qtdVertices; i++)
    {
        centro->x += p1->pontos[i].x;
        centro->y += p1->pontos[i].y;
    }

    centro->x /= p1->qtdVertices;
    centro->y /= p1->qtdVertices;

    return centro;
}

void desenhaPoligono (poligono* p1)
{
    glBegin(GL_POLYGON);
    for (int i=0; i < p1->qtdVertices; i++)
    {
        glVertex2f(p1->pontos[i].x, p1->pontos[i].y);
    }
    glEnd();
}

void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    glLoadIdentity();

    desenhaPoligono(p);
 
    glutSwapBuffers();
}

void reshape (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
}

void inicializa (void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0.0, 200.0, 0.0, 200.0);

    p = criaPoligono();
    pontoPivo = (coordenada*) malloc(sizeof(coordenada));
    pontoPivo->x = 2.0;
    pontoPivo->y = 2.0;
}

void Multiply(double N[2][2], poligono *p, double Result[2][4])
{
    double* M[2][p->qtdVertices];
    for (int i=0; i<p->qtdVertices; i++)
    {
        M[0][i] = &(p->pontos[i].x);
        M[1][i] = &(p->pontos[i].y);
    }

    for (int I = 0; I < 2; ++I)
    {
        for (int J = 0; J < 4; ++J)
        {
            double SumElements = 0.0f;
            for (int K = 0; K < 2; ++K)
            {
                SumElements += N[I][K] * *(M[K][J]);
            }
            Result[I][J] = SumElements;
        }
    }

    for (int a = 0; a < 2; a++){
        for(int b = 0; b < 4; b++){
            *(M[a][b]) = Result[a][b];
        }
    }
}

double degreeToRad(double degree){
    return (2.0*PI*degree*1.0)/360.0;
}

void translacao_origem(double d1, double d2)
{
    for (int i=0; i < p->qtdVertices; i++)
    {
        p->pontos[i].x += d1;
        p->pontos[i].y += d2;
    }
}

void timer(int value) {
    glutTimerFunc(1000/144, timer, 0);
    coordenada *c = calculaCentro(p);
    
    translacao_origem(-pontoPivo->x, -pontoPivo->y);
    double T [2][2] = {{cos(degreeToRad(360/passos)), sin(degreeToRad(360/passos))},
                      {-sin(degreeToRad(360/passos)), cos(degreeToRad(360/passos))}};
    double result [2][4] = {};
    Multiply(T, p, result);
    translacao_origem(pontoPivo->x, pontoPivo->y);

    glutPostRedisplay();
}

int main (int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Questao 14");
    inicializa();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
