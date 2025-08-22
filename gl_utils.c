#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <gl_utils.h>

Forma forma_atual = PONTO;

// Estrutura para representar um objeto genérico
typedef struct
{
    Forma forma;
    //int tipo; // 0 para ponto, 1 para reta, 2 para polígono
    float x, y;
    int selecionado;
} Objeto;

// Lista de objetos
Objeto objetos[100];
int num_objetos = 0;

Forma forma_atual = PONTO; //variável global que guarda a forma a ser desenhada

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
    for (int i = 0; i < num_objetos; i++)
    {
        if (objetos[i].selecionado)
        {
            setColor255(255, 0, 0); // Vermelho para selecionado
        }
        else
        {
            setColor255(0, 0, 0); // Preto para não selecionado
        }

        switch(objetos[i].forma)
        {
            case PONTO:
            glPointSize(10.0f);
            glBegin(GL_POINTS);
            glVertex2f(objetos[i].x, objetos[i].y);
            glEnd();
            break;

            case QUADRADO:
            glBegin(GL_QUADS);
            glVertex2f(objetos[i].x - 15, objetos[i].y - 15);
            glVertex2f(objetos[i].x + 15, objetos[i].y - 15);
            glVertex2f(objetos[i].x + 15, objetos[i].y + 15);
            glVertex2f(objetos[i].x - 15, objetos[i].y + 15);
            glEnd();
            break;

            case CIRCULO:
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(objetos[i].x, objetos[i].y);
            for(int j = 0; j<= 360; j++){
                float degInRad = j * 3.14159 / 180;
                glVertex2f(
                    objetos[i].x + cos(degInRad) * 15,
                    objetos[i].y + sin(degInRad) * 15
                );
            }
            glEnd();
            break;
        }
    }
}

void mouse(int button, int state, int x, int y)
{
    // Inverte a coordenada Y, pois o OpenGL considera o canto inferior esquerdo como (0,0)
    y = 540 - y;

    if(y > 490 && y < 540)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if (x >= 10 && x <= 45)
            {
                forma_atual = PONTO;
                printf("TROCOU PARA PONTO");
            }
        } else if (x >= 53 && x <= 88)
        {
            forma_atual = QUADRADO;
            printf("TROCOU PARA QUADRADO");

        } else if (x >= 96 && x <= 131)
        {
            forma_atual = CIRCULO;
            printf("TROCOU PARA CIRCULO");
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        int objeto_selecionado = -1;

        // 1. Tenta selecionar um objeto existente
        for (int i = 0; i < num_objetos; i++)
        {
            // Calcula a distância do clique ao centro do objeto (ponto)
            float dist = sqrt(pow(objetos[i].x - x, 2) + pow(objetos[i].y - y, 2));
            if (dist < 10.0f)
            { // Raio de tolerância para seleção
                objeto_selecionado = i;
                break;
            }
        }

        // Desmarca todos os objetos
        for (int i = 0; i < num_objetos; i++)
        {
            objetos[i].selecionado = 0;
        }

        if (objeto_selecionado != -1)
        {
            // Marca o novo objeto como selecionado
            objetos[objeto_selecionado].selecionado = 1;
        }
        // 2. Se nenhum objeto foi selecionado e o clique está na área de desenho, cria um novo objeto
        else if (x > 20 && x < 940 && y > 10 && y < 480)
        {
            if (num_objetos < 100)
            {
                objetos[num_objetos].x = x;
                objetos[num_objetos].y = y;
                objetos[num_objetos].forma = forma_atual;
                //objetos[num_objetos].tipo = 0; // Tipo Ponto
                objetos[num_objetos].selecionado = 0;
                num_objetos++;
            }
        }

        glutPostRedisplay(); // Redesenha a tela para mostrar o novo objeto
    }
}

void keyboard(unsigned char key, int x, int y)
{
    // Backspace pode ser 8 ou 127 dependendo do sistema
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
            // Remove o objeto deslocando os elementos do array
            for (int i = objeto_a_remover; i < num_objetos - 1; i++)
            {
                objetos[i] = objetos[i + 1];
            }
            num_objetos--;
            glutPostRedisplay(); // Redesenha a tela após a exclusão
        }
    }
}