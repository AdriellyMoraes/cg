#include <cmath>
#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>

typedef struct coordenada
{
    double x;
    double y;
} coordenada;

typedef struct quadrado
{
    coordenada p1;
    coordenada p2;
    coordenada p3;
    coordenada p4;
} quadrado;

double x_pos = -1.0;
double y_pos = 9.0;
double passos = 360*2;
double devRotation = 0;
double currentDegree = 0.0;
double internalRotation = 0;
int cont = 0;

coordenada *c;
quadrado *q;

quadrado *criaQuadrado ()
{
    quadrado *novo = (quadrado*) malloc(sizeof(quadrado));
    novo->p1.x = x_pos;
    novo->p1.y = y_pos;
    novo->p2.x = x_pos;
    novo->p2.y = y_pos-1.0;
    novo->p3.x = x_pos+1.0;
    novo->p3.y = y_pos-1.0;
    novo->p4.x = x_pos+1.0;
    novo->p4.y = y_pos;
    return novo;
}

coordenada *calculaCentro(quadrado *q1)
{
    coordenada *centro = (coordenada*) malloc(sizeof(coordenada));
    centro->x = (q1->p1.x + q1->p2.x + q1->p3.x + q1->p4.x)/4.0;
    centro->y = (q1->p1.y + q1->p2.y + q1->p3.y + q1->p4.y)/4.0;
    return centro;
}

void desenhaQuadrado (quadrado* q1)
{
    glBegin(GL_POLYGON);
    glVertex2f(q1->p1.x, q1->p1.y);
    glVertex2f(q1->p2.x, q1->p2.y);
    glVertex2f(q1->p3.x, q1->p3.y);
    glVertex2f(q1->p4.x, q1->p4.y);
    glEnd();
}

void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    glLoadIdentity();
    c = calculaCentro(q);
    devRotation += (360.0/(passos));

    glRotatef ((GLfloat) -devRotation, 0.0, 0.0, 1.0);
    glTranslatef(c->x, c->y, 0);
    internalRotation += (360.0*4)/(passos);
    glRotatef ((GLfloat) -(internalRotation+devRotation), 0.0, 0.0, 1.0);
    glTranslatef(-c->x, -c->y, 0);

    desenhaQuadrado(q);

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

    q = criaQuadrado();
}

void Multiply(double N[2][2], quadrado *q, double Result[2][4])
{
    double* M[2][4] = {{&(q->p1.x), &(q->p2.x), &(q->p3.x), &(q->p4.x)}, {&(q->p1.y), &(q->p2.y), &(q->p3.y), &(q->p4.y)}};

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

void timer(int value) {
    cont = cont + 1;
    currentDegree = std::fmod(((360.0/((int)passos))*cont), 90);
    glutTimerFunc(1000/144, timer, 0);
    glutPostRedisplay();
}

int main (int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Questao 17");
    inicializa();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
