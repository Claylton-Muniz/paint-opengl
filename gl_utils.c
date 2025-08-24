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

float distancia_ponto_segmento(float px, float py,
                               float x1, float y1,
                               float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    if (dx == 0 && dy == 0)
    { // segmento degenerado
        dx = px - x1;
        dy = py - y1;
        return sqrt(dx * dx + dy * dy);
    }

    float t = ((px - x1) * dx + (py - y1) * dy) / (dx * dx + dy * dy);
    if (t < 0)
        t = 0;
    else if (t > 1)
        t = 1;

    float projx = x1 + t * dx;
    float projy = y1 + t * dy;

    dx = px - projx;
    dy = py - projy;
    return sqrt(dx * dx + dy * dy);
}

int ponto_dentro_poligono(float px, float py, Objeto obj)
{
    int dentro = 0;
    for (int i = 0, j = obj.num_pontos - 1; i < obj.num_pontos; j = i++)
    {
        float xi = obj.pontos[i][0], yi = obj.pontos[i][1];
        float xj = obj.pontos[j][0], yj = obj.pontos[j][1];

        int intersect = ((yi > py) != (yj > py)) &&
                        (px < (xj - xi) * (py - yi) / (yj - yi) + xi);
        if (intersect)
            dentro = !dentro;
    }
    return dentro;
}

void mouse(int button, int state, int x, int y)
{
    y = 540 - y;

    // Clique no menu de seleção
    if (y > 490 && y < 540)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if (x >= 10 && x <= 45)
                forma_atual = PONTO;
            else if (x >= 53 && x <= 88)
                forma_atual = QUADRADO;
            else if (x >= 96 && x <= 131)
                forma_atual = CIRCULO;
            else if (x >= 139 && x <= 174)
                forma_atual = LINHA;
            else if (x >= 182 && x <= 217)
                forma_atual = LINE_STRIP;
            else if (x >= 225 && x <= 260)
                forma_atual = LINE_LOOP;
            else if (x >= 268 && x <= 303)
                forma_atual = TRIANGLES;
            else if (x >= 311 && x <= 346)
                forma_atual = POLYGON;
            else if (x >= 354 && x <= 389)
                forma_atual = QUAD_STRIP;

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
            int objeto_selecionado = -1;

            // Verifica se clicou em cima de algum objeto existente
            for (int i = 0; i < num_objetos; i++)
            {
                if (objetos[i].forma == PONTO)
                {
                    float dist = sqrt(pow(objetos[i].pontos[0][0] - x, 2) +
                                      pow(objetos[i].pontos[0][1] - y, 2));
                    if (dist < 10.0f)
                    { // tolerância
                        objeto_selecionado = i;
                        break;
                    }
                }

                else if (objetos[i].forma == CIRCULO)
                {
                    float dx = x - objetos[i].pontos[0][0];
                    float dy = y - objetos[i].pontos[0][1];
                    if (sqrt(dx * dx + dy * dy) <= 15)
                    { // raio
                        objeto_selecionado = i;
                        break;
                    }
                }

                else if (objetos[i].forma == QUADRADO)
                {
                    float cx = objetos[i].pontos[0][0];
                    float cy = objetos[i].pontos[0][1];
                    if (x >= cx - 15 && x <= cx + 15 && y >= cy - 15 && y <= cy + 15)
                    {
                        objeto_selecionado = i;
                        break;
                    }
                }

                else if (objetos[i].forma == LINHA || objetos[i].forma == LINE_STRIP || objetos[i].forma == LINE_LOOP)
                {
                    for (int j = 0; j < objetos[i].num_pontos - 1; j++)
                    {
                        float dist = distancia_ponto_segmento(
                            x, y,
                            objetos[i].pontos[j][0], objetos[i].pontos[j][1],
                            objetos[i].pontos[j + 1][0], objetos[i].pontos[j + 1][1]);
                        if (dist < 5.0f)
                        { // tolerância
                            objeto_selecionado = i;
                            break;
                        }
                    }

                    // para o LINE_LOOP, liga último ao primeiro - visualmente ele já fazia isso, 
                    // mas para o nosso objeto não então não podiamos selecionar de todos os lados
                    if (objetos[i].forma == LINE_LOOP)
                    {
                        float dist = distancia_ponto_segmento(
                            x, y,
                            objetos[i].pontos[objetos[i].num_pontos - 1][0],
                            objetos[i].pontos[objetos[i].num_pontos - 1][1],
                            objetos[i].pontos[0][0],
                            objetos[i].pontos[0][1]);
                        if (dist < 5.0f)
                        {
                            objeto_selecionado = i;
                            break;
                        }
                    }
                }

                else if (objetos[i].forma == POLYGON || objetos[i].forma == TRIANGLES || objetos[i].forma == QUAD_STRIP)
                {
                    if (ponto_dentro_poligono(x, y, objetos[i]))
                    {
                        objeto_selecionado = i;
                        break;
                    }
                }
            }

            // Desmarca todos
            for (int i = 0; i < num_objetos; i++)
                objetos[i].selecionado = 0;

            if (objeto_selecionado != -1)
            {
                objetos[objeto_selecionado].selecionado = 1;
            }
            else
            {
                if (forma_precisa_multiclique(forma_atual)) // o ponto por exemplo, a gente só vai adcionar 1
                {
                    pontos_multiclique[estado_multiclique][0] = x;
                    pontos_multiclique[estado_multiclique][1] = y;
                    estado_multiclique++;
                }
                else
                {
                    pontos_multiclique[0][0] = x;
                    pontos_multiclique[0][1] = y;
                    estado_multiclique = 1;
                }
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

        // Desmarca todos os objetos
        for (int i = 0; i < num_objetos; i++)
        {
            objetos[i].selecionado = 0;
        }
        glutPostRedisplay();
    }
    else if (key == 8 || key == 127) // Backspace ou Delete
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
                objetos[i] = objetos[i + 1];
            num_objetos--;
            glutPostRedisplay();
        }
    }
}
