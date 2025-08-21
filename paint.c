// #include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "screen_utils.h"
#include "gl_utils.h"

// Estrutura para representar um objeto genérico
typedef struct {
    int tipo; // 0 para ponto, 1 para reta, 2 para polígono
    float x, y;
    int selecionado;
} Objeto;

// Lista de objetos
Objeto objetos[100];
int num_objetos = 0;

void init() {
    // Definindo cor de fundo
    setClearColor255(40, 40, 40);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 960, 0, 540);
}

void menu() {
    // retângulo do menu
    setColor255(63, 63, 63);
    glBegin(GL_QUADS);
        glVertex2f(0, 540);
        glVertex2f(960, 540);
        glVertex2f(960, 490);
        glVertex2f(0, 490);
    glEnd();

    setColor255(255, 255, 255);
    for (int i = 0; i < 9; i++) {
        glBegin(GL_QUADS);
            glVertex2f(10 + (43 * i), 532);
            glVertex2f(45 + (43 * i), 532);
            glVertex2f(45 + (43 * i), 500);
            glVertex2f(10 + (43 * i), 500);
        glEnd();
    }
}

void area_desenho() {
    setColor255(255, 255, 255);
    glBegin(GL_QUADS);
        glVertex2f(20, 480);
        glVertex2f(940, 480);
        glVertex2f(940, 10);
        glVertex2f(20, 10);
    glEnd();
}

void desenha_objetos() {
    for (int i = 0; i < num_objetos; i++) {
        if (objetos[i].selecionado) {
            setColor255(255, 0, 0); // Vermelho para selecionado
        } else {
            setColor255(0, 0, 0);   // Preto para não selecionado
        }

        // Por enquanto, desenhamos apenas pontos
        glPointSize(10.0f);
        glBegin(GL_POINTS);
            glVertex2f(objetos[i].x, objetos[i].y);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    menu();
    area_desenho();
    desenha_objetos();

    glutSwapBuffers(); // Usando double buffering
}

void mouse(int button, int state, int x, int y) {
    // Inverte a coordenada Y, pois o OpenGL considera o canto inferior esquerdo como (0,0)
    y = 540 - y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int objeto_selecionado = -1;

        // 1. Tenta selecionar um objeto existente
        for (int i = 0; i < num_objetos; i++) {
            // Calcula a distância do clique ao centro do objeto (ponto)
            float dist = sqrt(pow(objetos[i].x - x, 2) + pow(objetos[i].y - y, 2));
            if (dist < 10.0f) { // Raio de tolerância para seleção
                objeto_selecionado = i;
                break;
            }
        }

        // Desmarca todos os objetos
        for (int i = 0; i < num_objetos; i++) {
            objetos[i].selecionado = 0;
        }

        if (objeto_selecionado != -1) {
            // Marca o novo objeto como selecionado
            objetos[objeto_selecionado].selecionado = 1;
        }
        // 2. Se nenhum objeto foi selecionado e o clique está na área de desenho, cria um novo objeto
        else if (x > 20 && x < 940 && y > 10 && y < 480) {
            if (num_objetos < 100) {
                objetos[num_objetos].x = x;
                objetos[num_objetos].y = y;
                objetos[num_objetos].tipo = 0; // Tipo Ponto
                objetos[num_objetos].selecionado = 0;
                num_objetos++;
            }
        }

        glutPostRedisplay(); // Redesenha a tela para mostrar o novo objeto
    }
}

void keyboard(unsigned char key, int x, int y) {
    //Backspace pode ser 8 ou 127 dependendo do sistema
    if (key == 8 || key == 127) {
        int objeto_a_remover = -1;
        for (int i = 0; i < num_objetos; i++) {
            if (objetos[i].selecionado) {
                objeto_a_remover = i;
                break;
            }
        }

        if (objeto_a_remover != -1) {
            // Remove o objeto deslocando os elementos do array
            for (int i = objeto_a_remover; i < num_objetos - 1; i++) {
                objetos[i] = objetos[i + 1];
            }
            num_objetos--;
            glutPostRedisplay(); // Redesenha a tela após a exclusão
        }
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //double buffering

    int width = 960;
    int height = 540;
    glutInitWindowSize(width, height);

    int posX, posY;
    getCenteredPosition(width, height, &posX, &posY);

    glutInitWindowPosition(posX, posY);
    glutCreateWindow("Ultra-Paint");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);       
    glutKeyboardFunc(keyboard); 
    glutMainLoop();
    return 0;
}