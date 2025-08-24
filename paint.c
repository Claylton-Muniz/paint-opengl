#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "screen_utils.h"
#include "gl_utils.h"
#include "draw_utils.h"

void init()
{
    // Definindo cor de fundo
    setClearColor255(40, 40, 40);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 960, 0, 540);
}

void menu()
{
    // retângulo do menu
    setColor255(63, 63, 63);
    glBegin(GL_QUADS);
    glVertex2f(0, 540);
    glVertex2f(960, 540);
    glVertex2f(960, 490);
    glVertex2f(0, 490);
    glEnd();

    setColor255(255, 255, 255);

    for (int i = 0; i < 9; i++)
    {
        // Define a cor de destaque para o botão selecionado
        if((i == 0 && forma_atual == PONTO) ||
           (i == 1 && forma_atual == QUADRADO) ||
           (i == 2 && forma_atual == CIRCULO) ||
           (i == 3 && forma_atual == LINHA) ||
           (i == 4 && forma_atual == LINE_STRIP) ||
           (i == 5 && forma_atual == LINE_LOOP) ||
           (i == 6 && forma_atual == TRIANGLES) ||
           (i == 7 && forma_atual == POLYGON) ||
           (i == 8 && forma_atual == QUAD_STRIP))
        {
            setColor255(255, 0, 0);
        }
        else 
        {
            setColor255(255, 255, 255);
        }

         // Desenha o quadrado de seleção
        glBegin(GL_QUADS);
        glVertex2f(10 + (43 * i), 532);
        glVertex2f(45 + (43 * i), 532);
        glVertex2f(45 + (43 * i), 500);
        glVertex2f(10 + (43 * i), 500);
        glEnd();

        float centroX = 27.5 + (43 * i);
        float centroY = 516;

        setColor255(0, 0, 0);

        if(i == 0){
            glPointSize(5.0);
            glBegin(GL_POINTS);
            glVertex2f(centroX, centroY);
            glEnd();
        } else if(i == 1){
            glBegin(GL_QUADS);
            glVertex2f(centroX - 10, centroY - 10);
            glVertex2f(centroX + 10, centroY - 10);
            glVertex2f(centroX + 10, centroY + 10);
            glVertex2f(centroX - 10, centroY + 10);
            glEnd();
        } else if (i == 2){
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(centroX, centroY);
            for(int j = 0; j <= 360; j++){
                float degInRad = j * 3.14159 / 180;
                glVertex2f(
                    centroX + cos(degInRad) * 10,
                    centroY + sin(degInRad) * 10
                );
            }
            glEnd();
        } else if (i == 3) {
            // Miniatura da Linha (GL_LINES)
            glBegin(GL_LINES);
            glVertex2f(centroX - 10, centroY + 8);
            glVertex2f(centroX + 10, centroY - 8);
            glEnd();
        } else if (i == 4) {
            // Miniatura da Strip (GL_LINE_STRIP)
            glBegin(GL_LINE_STRIP);
            glVertex2f(centroX - 10, centroY - 5);
            glVertex2f(centroX - 2, centroY + 10);
            glVertex2f(centroX + 5, centroY - 8);
            glVertex2f(centroX + 10, centroY + 2);
            glEnd();
        } else if (i == 5) {
            // Miniatura do Loop (GL_LINE_LOOP)
            glBegin(GL_LINE_LOOP);
            glVertex2f(centroX - 10, centroY - 5);
            glVertex2f(centroX - 2, centroY + 10);
            glVertex2f(centroX + 5, centroY - 8);
            glVertex2f(centroX + 10, centroY + 2);
            glEnd();
        } else if (i == 6) {
            // Miniatura do Triângulo (GL_TRIANGLES)
            glBegin(GL_TRIANGLES);
            glVertex2f(centroX, centroY + 10);
            glVertex2f(centroX - 10, centroY - 5);
            glVertex2f(centroX + 10, centroY - 5);
            glEnd();
        } else if (i == 7) {
            // Miniatura do Polígono (GL_POLYGON) - Exemplo: um pentágono
            glBegin(GL_POLYGON);
            for (int j = 0; j < 5; j++) {
                float angle = j * 2 * 3.14159 / 5;
                glVertex2f(centroX + cos(angle) * 10, centroY + sin(angle) * 10);
            }
            glEnd();
        } else if (i == 8) {
            // Miniatura da Tira de Quadrados (GL_QUAD_STRIP)
            glBegin(GL_QUAD_STRIP);
            glVertex2f(centroX - 10, centroY - 10);
            glVertex2f(centroX - 10, centroY + 10);
            glVertex2f(centroX + 0, centroY - 5);
            glVertex2f(centroX + 0, centroY + 5);
            glVertex2f(centroX + 10, centroY - 10);
            glVertex2f(centroX + 10, centroY + 10);
            glEnd();
        }
    }
}

void area_desenho()
{
    setColor255(255, 255, 255);
    glBegin(GL_QUADS);
    glVertex2f(20, 480);
    glVertex2f(940, 480);
    glVertex2f(940, 10);
    glVertex2f(20, 10);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    menu();
    area_desenho();
    desenha_objetos();
    glutSwapBuffers(); // Usando double buffering
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // double buffering

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
    glutSpecialFunc(keyboardSpecial);
    glutMainLoop();
    return 0;
}