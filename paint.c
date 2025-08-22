// #include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include "screen_utils.h"
#include "gl_utils.h"

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
        glBegin(GL_QUADS);
        glVertex2f(10 + (43 * i), 532);
        glVertex2f(45 + (43 * i), 532);
        glVertex2f(45 + (43 * i), 500);
        glVertex2f(10 + (43 * i), 500);
        glEnd();
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
    glutMainLoop();
    return 0;
}