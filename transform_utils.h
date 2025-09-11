#ifndef TRANSFORM_UTILS_H
#define TRANSFORM_UTILS_H

#include "gl_utils.h"

// Estrutura para representar um ponto (para a função calcularCentro)
typedef struct {
    float x;
    float y;
} Ponto;

// Declaração das funções de transformação
void aplicarMatriz(float ponto[2], float M[3][3]);
void aplicarMatrizObjeto(Objeto *obj, float M[3][3]);
void transladar(Objeto *obj, float tx, float ty);
void rotacionar(Objeto *obj, float angulo);
Ponto calcularCentro(const Objeto *obj);
void escalar(Objeto *obj, float sx, float sy);
void refletirX(Objeto *obj);
void refletirY(Objeto *obj);
void refletirOrigem(Objeto *obj);
void cisalharX(Objeto *obj, float shx);
void cisalharY(Objeto *obj, float shy);
int comparePoints(const void *a, const void *b);
float crossProduct(float x1, float y1, float x2, float y2);
void andrewConvexHull(Objeto *obj);

#endif