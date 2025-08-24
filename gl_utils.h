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

// Estrutura para representar um objeto genérico
typedef struct
{
    Forma forma;
    float pontos[20][2]; // Armazena os pontos
    int num_pontos;
    int selecionado;
} Objeto;

// Declaração das funções
void setColor255(int r, int g, int b);
void setClearColor255(int r, int g, int b);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
int forma_precisa_multiclique(Forma f);
float distancia_ponto_segmento(float px, float py, float x1, float y1, float x2, float y2);
int ponto_dentro_poligono(float px, float py, Objeto obj);
void keyboardSpecial(int key, int x, int y);

// Declarações externas das variáveis globais
extern Forma forma_atual;
extern int estado_multiclique;
extern float pontos_multiclique[20][2];
extern Objeto objetos[100];
extern int num_objetos;

#endif