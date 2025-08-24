#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "gl_utils.h"

void desenha_objetos()
{
    // Desenha os objetos já salvos
    for (int i = 0; i < num_objetos; i++)
    {
        if (objetos[i].selecionado)
            setColor255(255, 0, 0); // vermelho se selecionado
        else
            setColor255(0, 0, 0); // preto normal

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