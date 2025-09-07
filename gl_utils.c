#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "gl_utils.h"
#include "transform_utils.h"
#include "file_utils.h"
#include "animation_utils.h"

// variáveis globais
Forma forma_atual = PONTO;
int estado_multiclique = 0;
float pontos_multiclique[20][2];
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

void mouse(int button, int state, int x, int y)
{
    y = 540 - y;
    float tolerancia = 5.0f;

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
                    if (pickPonto(objetos[i].pontos[0][0], objetos[i].pontos[0][1],
                                  x, y, tolerancia))
                    {
                        objeto_selecionado = i;
                        break;
                    }
                }
                else if (objetos[i].forma == CIRCULO || objetos[i].forma == QUADRADO)
                {
                    // Para círculos e quadrados, mantemos a abordagem de distância
                    // pois são formas fechadas que podem ser tratadas como polígonos
                    if (pontoDentroPoligono(x, y, objetos[i]))
                    {
                        objeto_selecionado = i;
                        break;
                    }
                }
                else if (objetos[i].forma == LINHA)
                {
                    for (int j = 0; j < objetos[i].num_pontos - 1; j++)
                    {
                        if (pickLinha(objetos[i].pontos[j][0], objetos[i].pontos[j][1],
                                      objetos[i].pontos[j + 1][0], objetos[i].pontos[j + 1][1],
                                      x, y, tolerancia))
                        {
                            objeto_selecionado = i;
                            break;
                        }
                    }
                    if (objeto_selecionado != -1)
                        break;
                }
                else if (objetos[i].forma == LINE_STRIP || objetos[i].forma == LINE_LOOP)
                {
                    for (int j = 0; j < objetos[i].num_pontos - 1; j++)
                    {
                        if (pickLinha(objetos[i].pontos[j][0], objetos[i].pontos[j][1],
                                      objetos[i].pontos[j + 1][0], objetos[i].pontos[j + 1][1],
                                      x, y, tolerancia))
                        {
                            objeto_selecionado = i;
                            break;
                        }
                    }
                    // Para LINE_LOOP, verifica também a conexão entre último e primeiro ponto
                    if (objetos[i].forma == LINE_LOOP && objeto_selecionado == -1)
                    {
                        int last = objetos[i].num_pontos - 1;
                        if (pickLinha(objetos[i].pontos[last][0], objetos[i].pontos[last][1],
                                      objetos[i].pontos[0][0], objetos[i].pontos[0][1],
                                      x, y, tolerancia))
                        {
                            objeto_selecionado = i;
                        }
                    }
                    if (objeto_selecionado != -1)
                        break;
                }
                else if (objetos[i].forma == POLYGON || objetos[i].forma == TRIANGLES ||
                         objetos[i].forma == QUAD_STRIP)
                {
                    if (pontoDentroPoligono(x, y, objetos[i]))
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
    char nome_arq[15] = "";

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
    else if (key == 'r' || key == 'R') // Adicione esta parte para rotação
    {
        // Rotaciona objeto
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                rotacionar(&objetos[i], -5.0f);
                glutPostRedisplay();
                break;
            }
        }
    }
    else if (key == 'e' || key == 'E') // Adicione esta parte para rotação
    {
        // Rotaciona objeto
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                rotacionar(&objetos[i], 5.0f);
                glutPostRedisplay();
                break;
            }
        }
    }
    else if (key == '+' || key == '=') // Tecla '+'
    {
        // Aumenta a escala do objeto selecionado
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                escalar(&objetos[i], 1.1f, 1.1f); // Aumenta 10%
                glutPostRedisplay();
                break;
            }
        }
    }
    else if (key == '-' || key == '_') // Tecla '-'
    {
        // Reduz a escala do objeto selecionado
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                escalar(&objetos[i], 0.9f, 0.9f); // Reduz 10%
                glutPostRedisplay();
                break;
            }
        }
    }
    else if (key == 'x' || key == 'X') // Reflexão em X
    {
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                refletirX(&objetos[i]);
                glutPostRedisplay();
                break;
            }
        }
    }
    else if (key == 'y' || key == 'Y') // Reflexão em Y
    {
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                refletirY(&objetos[i]);
                glutPostRedisplay();
                break;
            }
        }
    }
    else if (key == 'o' || key == 'O') // Reflexão na origem
    {
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                refletirOrigem(&objetos[i]);
                glutPostRedisplay();
                break;
            }
        }
    }
    else if (key == 'h' || key == 'H') // Cisalhamento em X
    {
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                cisalharX(&objetos[i], 0.2f); // Cisalhamento de 0.2 em X
                glutPostRedisplay();
                break;
            }
        }
    }
    else if (key == 'v' || key == 'V') // Cisalhamento em Y
    {
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                cisalharY(&objetos[i], 0.2f); // Cisalhamento de 0.2 em Y
                glutPostRedisplay();
                break;
            }
        }
    }
    else if (key == 's' || key == 'S') // Tecla 'S' para salvar
    {
        printf("Nome do arq: ");
        scanf("%s", nome_arq);
        strcat(nome_arq, ".txt");
        salvar_objetos(nome_arq);
        glutPostRedisplay();
    }
    else if (key == 'l' || key == 'L') // Tecla 'L' para carregar
    {
        printf("Nome do arq: ");
        scanf("%s", nome_arq);
        strcat(nome_arq, ".txt");
        carregar_objetos(nome_arq);
        glutPostRedisplay();
    }
    else if (key == 'a' || key == 'A')
    { // Tecla 'A' para iniciar/parar animação
        if (animacao_ativa())
        {
            parar_animacao();
            printf("Animação parada\n");
        }
        else
        {
            iniciar_animacao();
            printf("Animação iniciada\n");
        }
        glutPostRedisplay();
    }
}

void keyboardSpecial(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        // Move objeto selecionado para cima
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                transladar(&objetos[i], 0, 5);
                glutPostRedisplay();
                break;
            }
        }
        break;

    case GLUT_KEY_DOWN:
        // Move objeto selecionado para baixo
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                transladar(&objetos[i], 0, -5);
                glutPostRedisplay();
                break;
            }
        }
        break;

    case GLUT_KEY_LEFT:
        // Move objeto selecionado para esquerda
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                transladar(&objetos[i], -5, 0);
                glutPostRedisplay();
                break;
            }
        }
        break;

    case GLUT_KEY_RIGHT:
        // Move objeto selecionado para direita
        for (int i = 0; i < num_objetos; i++)
        {
            if (objetos[i].selecionado)
            {
                transladar(&objetos[i], 5, 0);
                glutPostRedisplay();
                break;
            }
        }
        break;
    }
}
