#include <math.h>
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