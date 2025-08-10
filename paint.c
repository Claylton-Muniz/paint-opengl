// #include <windows.h>
#include <GL/glut.h>
#include "screen_utils.h"

void init() {
    // Definindo cor de fundo
    glClearColor(1, 1, 1, 0);

    // Vamos usar a projeção só para não utilizar a
    // padrão, pq ela não é intuitiva para o 2d
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 960, 0, 540);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // desenhe aqui
    
    // acho legal fazer uma funçao para converter os valores. Ex: 1 = 255
    glColor3f(1, 0, 0); 
    glBegin(GL_QUAD_STRIP);
        glVertex2f(0, 540);
        glVertex2f(960, 540);
        glVertex2f(0, 490);
        glVertex2f(960, 490);
    glEnd();

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