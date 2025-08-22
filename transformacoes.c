#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "transformacoes.h"

#define PI 3.14159265358979323846

void aplicarMatriz(Ponto *p, float M[3][3]) {
    float x = p->x, y = p->y;
    p->x = M[0][0]*x + M[0][1]*y + M[0][2];
    p->y = M[1][0]*x + M[1][1]*y + M[1][2];
}

void aplicarMatrizObjeto(Objeto *obj, float M[3][3]) {
    for (int i=0; i<obj->n; i++)
        aplicarMatriz(&obj->v[i], M);
}

Ponto calcularCentro(const Objeto *obj) {
    Ponto c = {0, 0};
    for (int i=0; i<obj->n; i++) {
        c.x += obj->v[i].x;
        c.y += obj->v[i].y;
    }
    c.x /= obj->n;
    c.y /= obj->n;
    return c;
}

void transladar(Objeto *obj, float tx, float ty) {
    float T[3][3] = {{1,0,tx},{0,1,ty},{0,0,1}};
    aplicarMatrizObjeto(obj, T);
}

void rotacionar(Objeto *obj, float angulo) {
    Ponto c = calcularCentro(obj);
    float rad = angulo * PI / 180.0;

    float Tida[3][3] = {{1,0,-c.x},{0,1,-c.y},{0,0,1}};
    float R[3][3] = {{cos(rad), -sin(rad), 0},
                     {sin(rad),  cos(rad), 0},
                     {0,0,1}};
    float Tvolta[3][3] = {{1,0,c.x},{0,1,c.y},{0,0,1}};

    aplicarMatrizObjeto(obj, Tida);
    aplicarMatrizObjeto(obj, R);
    aplicarMatrizObjeto(obj, Tvolta);
}

void escalar(Objeto *obj, float sx, float sy) {
    Ponto c = calcularCentro(obj);

    float Tida[3][3] = {{1,0,-c.x},{0,1,-c.y},{0,0,1}};
    float S[3][3] = {{sx,0,0},{0,sy,0},{0,0,1}};
    float Tvolta[3][3] = {{1,0,c.x},{0,1,c.y},{0,0,1}};

    aplicarMatrizObjeto(obj, Tida);
    aplicarMatrizObjeto(obj, S);
    aplicarMatrizObjeto(obj, Tvolta);
}

void refletirX(Objeto *obj) {
    float Rx[3][3] = {{1,0,0},{0,-1,0},{0,0,1}};
    aplicarMatrizObjeto(obj, Rx);
}

void refletirY(Objeto *obj) {
    float Ry[3][3] = {{-1,0,0},{0,1,0},{0,0,1}};
    aplicarMatrizObjeto(obj, Ry);
}

void refletirOrigem(Objeto *obj) {
    float Ro[3][3] = {{-1,0,0},{0,-1,0},{0,0,1}};
    aplicarMatrizObjeto(obj, Ro);
}

void cisalharX(Objeto *obj, float shx) {
    float Shx[3][3] = {{1,shx,0},{0,1,0},{0,0,1}};
    aplicarMatrizObjeto(obj, Shx);
}

void cisalharY(Objeto *obj, float shy) {
    float Shy[3][3] = {{1,0,0},{shy,1,0},{0,0,1}};
    aplicarMatrizObjeto(obj, Shy);
}
