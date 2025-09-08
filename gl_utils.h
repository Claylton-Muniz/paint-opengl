#ifndef GL_UTILS_H
#define GL_UTILS_H

typedef enum {
    PONTO,
    QUADRADO,
    CIRCULO,
    LINHA,
    LINE_STRIP,
    LINE_LOOP,
    TRIANGLES,
    POLYGON,
    QUAD_STRIP
} Forma;

typedef struct
{
    Forma forma;
    float pontos[200][2];
    int num_pontos;
    int selecionado;
} Objeto;

// Declaração das funções
void setColor255(int r, int g, int b);
void setClearColor255(int r, int g, int b);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
int forma_precisa_multiclique(Forma f);
void keyboardSpecial(int key, int x, int y);
int pickPonto(float px, float py, float mx, float my, float tolerancia);
int pickLinha(float x1, float y1, float x2, float y2, float mx, float my, float tolerancia);
int pontoDentroPoligono(float px, float py, Objeto obj);

// Declarações externas das variáveis globais
extern Forma forma_atual;
extern int estado_multiclique;
extern float pontos_multiclique[20][2];
extern Objeto objetos[100];
extern int num_objetos;

#endif