#include <math.h>
#include <stdlib.h> 
#include "gl_utils.h"
#include "transform_utils.h"

#define PI 3.14159265358979323846

// Aplica uma matriz de transformação 3x3 a um ponto
void aplicarMatriz(float ponto[2], float M[3][3]) {
    float x = ponto[0];
    float y = ponto[1];
    
    ponto[0] = M[0][0]*x + M[0][1]*y + M[0][2];
    ponto[1] = M[1][0]*x + M[1][1]*y + M[1][2];
}

// Aplica uma matriz de transformação a todos os pontos de um objeto
void aplicarMatrizObjeto(Objeto *obj, float M[3][3]) {
    for (int i = 0; i < obj->num_pontos; i++) {
        aplicarMatriz(obj->pontos[i], M);
    }
}

// Translada um objeto por (tx, ty)
void transladar(Objeto *obj, float tx, float ty) {
    float T[3][3] = {{1, 0, tx}, {0, 1, ty}, {0, 0, 1}};
    aplicarMatrizObjeto(obj, T);
}

// Calcula o centro de um objeto
Ponto calcularCentro(const Objeto *obj) {
    Ponto c = {0, 0};
    for (int i = 0; i < obj->num_pontos; i++) {
        c.x += obj->pontos[i][0];
        c.y += obj->pontos[i][1];
    }
    c.x /= obj->num_pontos;
    c.y /= obj->num_pontos;
    return c;
}

// Rotaciona um objeto pelo ângulo especificado
void rotacionar(Objeto *obj, float angulo) {
    Ponto c = calcularCentro(obj);
    float rad = angulo * PI / 180.0;

    // Matriz de translação para a origem
    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};
    
    // Matriz de rotação
    float R[3][3] = {
        {cos(rad), -sin(rad), 0},
        {sin(rad),  cos(rad), 0},
        {0, 0, 1}
    };
    
    // Matriz de translação de volta
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);    // Move para origem
    aplicarMatrizObjeto(obj, R);       // Rotaciona
    aplicarMatrizObjeto(obj, Tvolta);  // Move de volta
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

// Reflexão em relação ao eixo X
void refletirX(Objeto *obj) {
    Ponto c = calcularCentro(obj);
    
    // Matriz de translação para a origem
    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};
    
    // Matriz de reflexão em X
    float Rx[3][3] = {{1, 0, 0}, {0, -1, 0}, {0, 0, 1}};
    
    // Matriz de translação de volta
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);    // Move para origem
    aplicarMatrizObjeto(obj, Rx);      // Reflexão em X
    aplicarMatrizObjeto(obj, Tvolta);  // Move de volta
}

// Reflexão em relação ao eixo Y
void refletirY(Objeto *obj) {
    Ponto c = calcularCentro(obj);
    
    // Matriz de translação para a origem
    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};
    
    // Matriz de reflexão em Y
    float Ry[3][3] = {{-1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    
    // Matriz de translação de volta
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);    // Move para origem
    aplicarMatrizObjeto(obj, Ry);      // Reflexão em Y
    aplicarMatrizObjeto(obj, Tvolta);  // Move de volta
}

// Reflexão em relação à origem
void refletirOrigem(Objeto *obj) {
    Ponto c = calcularCentro(obj);
    
    // Matriz de translação para a origem
    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};
    
    // Matriz de reflexão na origem
    float Ro[3][3] = {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}};
    
    // Matriz de translação de volta
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);    // Move para origem
    aplicarMatrizObjeto(obj, Ro);      // Reflexão na origem
    aplicarMatrizObjeto(obj, Tvolta);  // Move de volta
}

// Cisalhamento no eixo X
void cisalharX(Objeto *obj, float shx) {
    Ponto c = calcularCentro(obj);
    
    // Matriz de translação para a origem
    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};
    
    // Matriz de cisalhamento em X
    float Shx[3][3] = {{1, shx, 0}, {0, 1, 0}, {0, 0, 1}};
    
    // Matriz de translação de volta
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);    // Move para origem
    aplicarMatrizObjeto(obj, Shx);     // Cisalhamento em X
    aplicarMatrizObjeto(obj, Tvolta);  // Move de volta
}

// Cisalhamento no eixo Y
void cisalharY(Objeto *obj, float shy) {
    Ponto c = calcularCentro(obj);
    
    // Matriz de translação para a origem
    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};
    
    // Matriz de cisalhamento em Y
    float Shy[3][3] = {{1, 0, 0}, {shy, 1, 0}, {0, 0, 1}};
    
    // Matriz de translação de volta
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);    // Move para origem
    aplicarMatrizObjeto(obj, Shy);     // Cisalhamento em Y
    aplicarMatrizObjeto(obj, Tvolta);  // Move de volta
}

// Função auxiliar para comparar pontos por ângulo polar
int comparePoints(const void *a, const void *b) {
    float *p1 = (float *)a;
    float *p2 = (float *)b;
    
    // Compara por coordenada y, depois por coordenada x
    if (p1[1] != p2[1]) {
        return (p1[1] > p2[1]) ? 1 : -1;
    }
    return (p1[0] > p2[0]) ? 1 : -1;
}

float crossProduct(float x1, float y1, float x2, float y2) {
    return x1 * y2 - x2 * y1;
}

// Calcula o fecho convexo usando o algoritmo de Andrew's monotone chain
void andrewConvexHull(Objeto *obj) {
    if (obj->num_pontos < 3) return;
    
    // Ordena pontos por coordenadas
    qsort(obj->pontos, obj->num_pontos, sizeof(obj->pontos[0]), comparePoints);
    
    Objeto hull;
    hull.forma = LINE_LOOP;
    hull.num_pontos = 0;
    
    // Constroi o hull inferior
    for (int i = 0; i < obj->num_pontos; i++) {
        while (hull.num_pontos >= 2) {
            float x1 = hull.pontos[hull.num_pontos-1][0] - hull.pontos[hull.num_pontos-2][0];
            float y1 = hull.pontos[hull.num_pontos-1][1] - hull.pontos[hull.num_pontos-2][1];
            float x2 = obj->pontos[i][0] - hull.pontos[hull.num_pontos-1][0];
            float y2 = obj->pontos[i][1] - hull.pontos[hull.num_pontos-1][1];
            
            if (crossProduct(x1, y1, x2, y2) <= 0) {
                break;
            }
            hull.num_pontos--;
        }
        hull.pontos[hull.num_pontos][0] = obj->pontos[i][0];
        hull.pontos[hull.num_pontos][1] = obj->pontos[i][1];
        hull.num_pontos++;
    }
    
    // Constroi o hull superior
    int lower_hull_size = hull.num_pontos;
    for (int i = obj->num_pontos - 2; i >= 0; i--) {
        while (hull.num_pontos > lower_hull_size) {
            float x1 = hull.pontos[hull.num_pontos-1][0] - hull.pontos[hull.num_pontos-2][0];
            float y1 = hull.pontos[hull.num_pontos-1][1] - hull.pontos[hull.num_pontos-2][1];
            float x2 = obj->pontos[i][0] - hull.pontos[hull.num_pontos-1][0];
            float y2 = obj->pontos[i][1] - hull.pontos[hull.num_pontos-1][1];
            
            if (crossProduct(x1, y1, x2, y2) <= 0) {
                break;
            }
            hull.num_pontos--;
        }
        hull.pontos[hull.num_pontos][0] = obj->pontos[i][0];
        hull.pontos[hull.num_pontos][1] = obj->pontos[i][1];
        hull.num_pontos++;
    }
    
    // Remove o último ponto (é duplicado do primeiro)
    hull.num_pontos--;
    
    *obj = hull;
}
