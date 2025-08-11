// #include <windows.h>
#include <GL/glut.h>
#include "screen_utils.h"
#include "gl_utils.h"

void init() {
    // Definindo cor de fundo
    setClearColor255(40, 40, 40);

    // Vamos usar a projeção só para não utilizar a
    // padrão, pq ela não é intuitiva para o 2d
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 960, 0, 540);
}

void menu() {

    // retangulo do menu
    setColor255(63, 63, 63); // Função criada para ficar mais intuitivo
    glBegin(GL_QUAD_STRIP);
        glVertex2f(0, 540);
        glVertex2f(960, 540);
        glVertex2f(0, 490);
        glVertex2f(960, 490);
    glEnd();

    setColor255(255, 255, 255);    
    for (int i = 0; i < 9; i++)
    {
        glBegin(GL_QUAD_STRIP);
            glVertex2f(10 + (43 * i), 532);
            glVertex2f(45 + (43 * i), 532);
            glVertex2f(10 + (43 * i), 500);
            glVertex2f(45 + (43 * i), 500);
        glEnd();
    }


}

void area_desenho() {
    setColor255(255, 255, 255);
    glBegin(GL_QUAD_STRIP);
        glVertex2f(20, 480);
        glVertex2f(940, 480);
        glVertex2f(20, 10);
        glVertex2f(940, 10);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // desenhe aqui
    menu();
    area_desenho();

    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    // talvez seja melhor trocar pelo GLUT_DOUBLE  para as animações - pesquisem
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // 16:9
    int width = 960;
    int height = 540;

    glutInitWindowSize(width, height); 

    int posX, posY;
    getCenteredPosition(width, height, &posX, &posY);
    
    glutInitWindowPosition(200, 250);
    glutCreateWindow("Ultra-Paint");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}