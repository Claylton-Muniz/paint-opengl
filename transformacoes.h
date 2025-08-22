#ifndef TRANSFORMACOES_H
#define TRANSFORMACOES_H

typedef struct {
    float x, y;
} Ponto;

typedef struct {
    int n;       // número de vértices
    Ponto *v;    // array de vértices
} Objeto;

// Funções auxiliares
void aplicarMatriz(Ponto *p, float M[3][3]);
void aplicarMatrizObjeto(Objeto *obj, float M[3][3]);
Ponto calcularCentro(const Objeto *obj);

// Transformações
void transladar(Objeto *obj, float tx, float ty);
void rotacionar(Objeto *obj, float angulo);
void escalar(Objeto *obj, float sx, float sy);
void refletirX(Objeto *obj);
void refletirY(Objeto *obj);
void refletirOrigem(Objeto *obj);
void cisalharX(Objeto *obj, float shx);
void cisalharY(Objeto *obj, float shy);

#endif
