#ifndef GL_UTILS_H
#define GL_UTILS_H

typedef enum {
    PONTO,
    QUADRADO,
    CIRCULO,
} Forma;

extern Forma forma_atual;

// Declaração da função
void setColor255(int r, int g, int b);
void setClearColor255(int r, int g, int b);
void desenha_objetos();
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);

#endif