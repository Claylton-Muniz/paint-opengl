#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "gl_utils.h"

Forma forma_atual = PONTO;
int estado_multiclique = 0;
float pontos_multiclique[20][2];

// Estrutura para representar um objeto genérico
typedef struct
{
    Forma forma;
    float pontos[20][2]; // Armazena os pontos
    int num_pontos;
    int selecionado;
} Objeto;

// Lista de objetos
Objeto objetos[100];
int num_objetos = 0;

void setColor255(int r, int g, int b)
{
    glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

void setClearColor255(int r, int g, int b)
{
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 0);
}

int forma_precisa_multiclique(Forma f)
{
    return (f == LINHA || f == LINE_STRIP || f == LINE_LOOP ||
            f == TRIANGLES || f == POLYGON || f == QUAD_STRIP);
}

void desenha_objetos()
{
    // Desenha os objetos já salvos
    for (int i = 0; i < num_objetos; i++)
    {
        setColor255(0, 0, 0);

        switch (objetos[i].forma)
        {
        case PONTO:
            glPointSize(10.0f);
            glBegin(GL_POINTS);
            glVertex2f(objetos[i].pontos[0][0], objetos[i].pontos[0][1]);
            glEnd();
            break;

        case QUADRADO:
            glBegin(GL_QUADS);
            glVertex2f(objetos[i].pontos[0][0] - 15, objetos[i].pontos[0][1] - 15);
            glVertex2f(objetos[i].pontos[0][0] + 15, objetos[i].pontos[0][1] - 15);
            glVertex2f(objetos[i].pontos[0][0] + 15, objetos[i].pontos[0][1] + 15);
            glVertex2f(objetos[i].pontos[0][0] - 15, objetos[i].pontos[0][1] + 15);
            glEnd();
            break;

        case CIRCULO:
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(objetos[i].pontos[0][0], objetos[i].pontos[0][1]);
            for (int j = 0; j <= 360; j++)
            {
                float degInRad = j * 3.14159 / 180;
                glVertex2f(
                    objetos[i].pontos[0][0] + cos(degInRad) * 15,
                    objetos[i].pontos[0][1] + sin(degInRad) * 15);
            }
            glEnd();
            break;

        case LINHA:
            glBegin(GL_LINES);
            for (int j = 0; j < objetos[i].num_pontos; j++)
                glVertex2f(objetos[i].pontos[j][0], objetos[i].pontos[j][1]);
            glEnd();
            break;

        case LINE_STRIP:
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < objetos[i].num_pontos; j++)
                glVertex2f(objetos[i].pontos[j][0], objetos[i].pontos[j][1]);
            glEnd();
            break;

        case LINE_LOOP:
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < objetos[i].num_pontos; j++)
                glVertex2f(objetos[i].pontos[j][0], objetos[i].pontos[j][1]);
            glEnd();
            break;

        case TRIANGLES:
            glBegin(GL_TRIANGLES);
            for (int j = 0; j < objetos[i].num_pontos; j++)
                glVertex2f(objetos[i].pontos[j][0], objetos[i].pontos[j][1]);
            glEnd();
            break;

        case POLYGON:
            glBegin(GL_POLYGON);
            for (int j = 0; j < objetos[i].num_pontos; j++)
                glVertex2f(objetos[i].pontos[j][0], objetos[i].pontos[j][1]);
            glEnd();
            break;

        case QUAD_STRIP:
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j < objetos[i].num_pontos; j++)
                glVertex2f(objetos[i].pontos[j][0], objetos[i].pontos[j][1]);
            glEnd();
            break;
        }
    }

    // Desenha a forma em pré-visualização
    if (estado_multiclique > 0)
    {
        setColor255(0, 0, 150); // azul da pré-visualização

        glBegin(GL_POINTS);
        for (int i = 0; i < estado_multiclique; i++)
        {
            glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
        }
        glEnd();

        switch (forma_atual)
        {
        case PONTO:
            glPointSize(10.0f);
            glBegin(GL_POINTS);
            glVertex2f(pontos_multiclique[0][0], pontos_multiclique[0][1]);
            glEnd();
            break;

        case QUADRADO:
            glBegin(GL_QUADS);
            glVertex2f(pontos_multiclique[0][0] - 15, pontos_multiclique[0][1] - 15);
            glVertex2f(pontos_multiclique[0][0] + 15, pontos_multiclique[0][1] - 15);
            glVertex2f(pontos_multiclique[0][0] + 15, pontos_multiclique[0][1] + 15);
            glVertex2f(pontos_multiclique[0][0] - 15, pontos_multiclique[0][1] + 15);
            glEnd();
            break;

        case CIRCULO:
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(pontos_multiclique[0][0], pontos_multiclique[0][1]);
            for (int j = 0; j <= 360; j++)
            {
                float degInRad = j * 3.14159 / 180;
                glVertex2f(
                    pontos_multiclique[0][0] + cos(degInRad) * 15,
                    pontos_multiclique[0][1] + sin(degInRad) * 15);
            }
            glEnd();
            break;

        case LINHA:
            glBegin(GL_LINES);
            for (int i = 0; i < estado_multiclique; i++)
                glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
            glEnd();
            break;

        case LINE_STRIP:
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < estado_multiclique; i++)
                glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
            glEnd();
            break;

        case LINE_LOOP:
            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < estado_multiclique; i++)
                glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
            glEnd();
            break;

        case TRIANGLES:
            glBegin(GL_TRIANGLES);
            for (int i = 0; i < estado_multiclique; i++)
                glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
            glEnd();
            break;

        case POLYGON:
            glBegin(GL_POLYGON);
            for (int i = 0; i < estado_multiclique; i++)
                glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
            glEnd();
            break;

        case QUAD_STRIP:
            glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < estado_multiclique; i++)
                glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
            glEnd();
            break;
        }
    }
}

void mouse(int button, int state, int x, int y)
{
    y = 540 - y;

    // Clique no menu de seleção
    if (y > 490 && y < 540)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if (x >= 10 && x <= 45) forma_atual = PONTO;
            else if (x >= 53 && x <= 88) forma_atual = QUADRADO;
            else if (x >= 96 && x <= 131) forma_atual = CIRCULO;
            else if (x >= 139 && x <= 174) forma_atual = LINHA;
            else if (x >= 182 && x <= 217) forma_atual = LINE_STRIP;
            else if (x >= 225 && x <= 260) forma_atual = LINE_LOOP;
            else if (x >= 268 && x <= 303) forma_atual = TRIANGLES;
            else if (x >= 311 && x <= 346) forma_atual = POLYGON;
            else if (x >= 354 && x <= 389) forma_atual = QUAD_STRIP;

            estado_multiclique = 0; // reset ao trocar forma
        }
        glutPostRedisplay();
        return;
    }

    // Área de desenho
    if (x > 20 && x < 940 && y > 10 && y < 480)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if (forma_precisa_multiclique(forma_atual)) // o ponto por exemplo, a gente só vai adcionar 1
            {
                pontos_multiclique[estado_multiclique][0] = x;
                pontos_multiclique[estado_multiclique][1] = y;
                estado_multiclique++;
            }
            else
            {
                // Ponto único: só guarda para pré-visualização
                pontos_multiclique[0][0] = x;
                pontos_multiclique[0][1] = y;
                estado_multiclique = 1;
            }
        }
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 13) // Enter
    {
        if (estado_multiclique > 0)
        {
            objetos[num_objetos].forma = forma_atual;
            objetos[num_objetos].num_pontos = estado_multiclique;
            for (int i = 0; i < estado_multiclique; i++)
            {
                objetos[num_objetos].pontos[i][0] = pontos_multiclique[i][0];
                objetos[num_objetos].pontos[i][1] = pontos_multiclique[i][1];
            }
            num_objetos++;
            estado_multiclique = 0;
            glutPostRedisplay();
        }
    }
    else if (key == 27) // ESC
    {
        estado_multiclique = 0; // cancela
        glutPostRedisplay();
    }
}
