// Talves precise do #include <windows.h> - alguém tira a dúvida depois
#include <GL/glut.h>

void setColor255(int r, int g, int b) {
    glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);
}

void setClearColor255(int r, int g, int b) {
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 0);
}
