// Talves precise do #include <windows.h> - alguém tira a dúvida depois
#include <GL/glut.h>

void getCenteredPosition(int windowWidth, int windowHeight, int *posX, int *posY) {
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    *posX = (screenWidth - windowWidth) / 2;
    *posY = (screenHeight - windowHeight) / 2;
}
