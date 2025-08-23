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










void desenha_objetos()
{
    // Desenha as formas já salvas
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
            for(int j = 0; j <= 360; j++){
                float degInRad = j * 3.14159 / 180;
                glVertex2f(
                    objetos[i].pontos[0][0] + cos(degInRad) * 15,
                    objetos[i].pontos[0][1] + sin(degInRad) * 15
                );
            }
            glEnd();
            break;

            case LINHA:
            glBegin(GL_LINES);
            for (int j = 0; j < objetos[i].num_pontos; j++)
            {
                glVertex2f(objetos[i].pontos[j][0], objetos[i].pontos[j][1]);
            }
            glEnd();
            break;

            case LINE_STRIP:
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < objetos[i].num_pontos; j++)
            {
                glVertex2f(objetos[i].pontos[j][0], objetos[i].pontos[j][1]);
            }
            glEnd();
            break;

            case LINE_LOOP:
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < objetos[i].num_pontos; j++)
            {
                glVertex2f(objetos[i].pontos[j][0], objetos[i].pontos[j][1]);
            }
            glEnd();
            break;

            case TRIANGLES:
            glBegin(GL_TRIANGLES);
            for (int j = 0; j < objetos[i].num_pontos; j++)
            {
                glVertex2f(objetos[i].pontos[j][0], objetos[i].pontos[j][1]);
            }
            glEnd();
            break;
            
            case POLYGON:
            glBegin(GL_POLYGON);
            for (int j = 0; j < objetos[i].num_pontos; j++)
            {
                glVertex2f(objetos[i].pontos[j][0], objetos[i].pontos[j][1]);
            }
            glEnd();
            break;
            
            case QUAD_STRIP:
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j < objetos[i].num_pontos; j++)
            {
                glVertex2f(objetos[i].pontos[j][0], objetos[i].pontos[j][1]);
            }
            glEnd();
            break;
        }
    }

    // Desenha a forma atual que está em processo de criação
    if (estado_multiclique > 0)
    {
        setColor255(150, 150, 150); // Cor de pré-visualização
        glBegin(GL_POINTS);
        for (int i = 0; i < estado_multiclique; i++)
        {
            glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
        }
        glEnd();

        switch (forma_atual)
        {
            case LINHA:
            glBegin(GL_LINES);
            for (int i = 0; i < estado_multiclique; i++)
            {
                glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
            }
            glEnd();
            break;

            case LINE_STRIP:
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < estado_multiclique; i++)
            {
                glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
            }
            glEnd();
            break;

            case LINE_LOOP:
            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < estado_multiclique; i++)
            {
                glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
            }
            glEnd();
            break;

            case TRIANGLES:
            glBegin(GL_TRIANGLES);
            for (int i = 0; i < estado_multiclique; i++)
            {
                glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
            }
            glEnd();
            break;

            case POLYGON:
            glBegin(GL_POLYGON);
            for (int i = 0; i < estado_multiclique; i++)
            {
                glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
            }
            glEnd();
            break;

            case QUAD_STRIP:
            glBegin(GL_QUAD_STRIP);
            for (int i = 0; i < estado_multiclique; i++)
            {
                glVertex2f(pontos_multiclique[i][0], pontos_multiclique[i][1]);
            }
            glEnd();
            break;
        }
    }
}











void mouse(int button, int state, int x, int y)
{
    y = 540 - y;

    // Lógica para seleção de forma no menu
    if (y > 490 && y < 540)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if (x >= 10 && x <= 45) {
                forma_atual = PONTO;
                printf("Ponto selecionado\n");
            } else if (x >= 53 && x <= 88) {
                forma_atual = QUADRADO;
                printf("Quadrado selecionado\n");
            } else if (x >= 96 && x <= 131) {
                forma_atual = CIRCULO;
                printf("Circulo selecionado\n");
            } else if (x >= 139 && x <= 174) {
                forma_atual = LINHA;
                printf("Linha selecionado\n");
            } else if (x >= 182 && x <= 217) {
                forma_atual = LINE_STRIP;
                printf("LineStrip selecionado\n");
            } else if (x >= 225 && x <= 260) {
                forma_atual = LINE_LOOP;
                printf("LineLoop selecionado\n");
            } else if (x >= 268 && x <= 303) {
                forma_atual = TRIANGLES;
                printf("Triangulo selecionado\n");
            } else if (x >= 311 && x <= 346) {
                forma_atual = POLYGON;
                printf("Poligono selecionado\n");
            } else if (x >= 354 && x <= 389) {
                forma_atual = QUAD_STRIP;
                printf("QuadStrip selecionado\n");
            }
            estado_multiclique = 0; // Reseta o estado do multiclique
        }
        glutPostRedisplay();
        return;
    }

    // Lógica para desenhar/selecionar na área de desenho
    if (x > 20 && x < 940 && y > 10 && y < 480)
    {
        // Limpa a seleção anterior
        for (int i = 0; i < num_objetos; i++) objetos[i].selecionado = 0;

        // Se for um clique com o botão direito, finaliza o desenho
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        {
            if (estado_multiclique > 0)
            {
                // Salva a forma
                objetos[num_objetos].forma = forma_atual;
                objetos[num_objetos].num_pontos = estado_multiclique;
                for (int i = 0; i < estado_multiclique; i++)
                {
                    objetos[num_objetos].pontos[i][0] = pontos_multiclique[i][0];
                    objetos[num_objetos].pontos[i][1] = pontos_multiclique[i][1];
                }
                num_objetos++;
                estado_multiclique = 0; // Reseta o estado para um novo desenho
            }
        }
        // Se for um clique com o botão esquerdo, adiciona um ponto
        else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            // Se o limite de pontos for atingido, salva a forma atual
            if (estado_multiclique >= 20)
            {
                // Salva a forma
                objetos[num_objetos].forma = forma_atual;
                objetos[num_objetos].num_pontos = estado_multiclique;
                for (int i = 0; i < estado_multiclique; i++)
                {
                    objetos[num_objetos].pontos[i][0] = pontos_multiclique[i][0];
                    objetos[num_objetos].pontos[i][1] = pontos_multiclique[i][1];
                }
                num_objetos++;
                estado_multiclique = 0; // Reseta o estado
            }
            // Adiciona o ponto ao array temporário
            pontos_multiclique[estado_multiclique][0] = x;
            pontos_multiclique[estado_multiclique][1] = y;
            estado_multiclique++;
        }
    }
    glutPostRedisplay();
}
















void keyboard(unsigned char key, int x, int y)
{
    if (key == 8 || key == 127)
    {
        int objeto_a_remover = -1;
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                objeto_a_remover = i;
                break;
            }
        }

        if (objeto_a_remover != -1)
        {
            for (int i = objeto_a_remover; i < num_objetos - 1; i++)
            {
                objetos[i] = objetos[i + 1];
            }
            num_objetos--;
            glutPostRedisplay();
        }
    }
}