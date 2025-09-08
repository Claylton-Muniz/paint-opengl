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

// Calcula o produto vetorial entre dois vetores
float crossProduct(float x1, float y1, float x2, float y2) {
    return x1 * y2 - x2 * y1;
}

// Verifica se um polígono é convexo
int isConvex(const Objeto *obj) {
    if (obj->num_pontos < 3) return 1; // Pontos e linhas são considerados convexos
    
    int n = obj->num_pontos;
    int sign = 0;
    
    for (int i = 0; i < n; i++) {
        float x1 = obj->pontos[i][0];
        float y1 = obj->pontos[i][1];
        float x2 = obj->pontos[(i+1)%n][0];
        float y2 = obj->pontos[(i+1)%n][1];
        float x3 = obj->pontos[(i+2)%n][0];
        float y3 = obj->pontos[(i+2)%n][1];
        
        // Calcula os vetores
        float dx1 = x2 - x1;
        float dy1 = y2 - y1;
        float dx2 = x3 - x2;
        float dy2 = y3 - y2;
        
        // Calcula o produto vetorial
        float cross = crossProduct(dx1, dy1, dx2, dy2);
        
        if (cross != 0) {
            if (sign == 0) {
                sign = (cross > 0) ? 1 : -1;
            } else if (sign * cross < 0) {
                return 0; // Não é convexo
            }
        }
    }
    
    return 1; // É convexo
}

// Calcula o fecho convexo de um polígono usando o algoritmo de Graham Scan
void convexHull(Objeto *obj) {
    if (obj->num_pontos < 3 || isConvex(obj)) return;
    
    // Encontra o ponto mais abaixo e mais à esquerda
    int min_idx = 0;
    for (int i = 1; i < obj->num_pontos; i++) {
        if (obj->pontos[i][1] < obj->pontos[min_idx][1] || 
            (obj->pontos[i][1] == obj->pontos[min_idx][1] && 
             obj->pontos[i][0] < obj->pontos[min_idx][0])) {
            min_idx = i;
        }
    }
    
    // Troca o ponto mínimo com o primeiro ponto
    float temp_x = obj->pontos[0][0];
    float temp_y = obj->pontos[0][1];
    obj->pontos[0][0] = obj->pontos[min_idx][0];
    obj->pontos[0][1] = obj->pontos[min_idx][1];
    obj->pontos[min_idx][0] = temp_x;
    obj->pontos[min_idx][1] = temp_y;
    
    // Ordena os pontos por ângulo polar em relação ao ponto inicial
    for (int i = 2; i < obj->num_pontos; i++) {
        int j = i;
        while (j > 1) {
            float x1 = obj->pontos[j][0] - obj->pontos[0][0];
            float y1 = obj->pontos[j][1] - obj->pontos[0][1];
            float x2 = obj->pontos[j-1][0] - obj->pontos[0][0];
            float y2 = obj->pontos[j-1][1] - obj->pontos[0][1];
            
            float cross = crossProduct(x1, y1, x2, y2);
            
            if (cross > 0) {
                // Troca os pontos
                float temp_x = obj->pontos[j][0];
                float temp_y = obj->pontos[j][1];
                obj->pontos[j][0] = obj->pontos[j-1][0];
                obj->pontos[j][1] = obj->pontos[j-1][1];
                obj->pontos[j-1][0] = temp_x;
                obj->pontos[j-1][1] = temp_y;
                j--;
            } else {
                break;
            }
        }
    }
    
    // Construção do fecho convexo
    Objeto hull;
    hull.forma = obj->forma;
    hull.num_pontos = 0;
    
    // Adiciona os primeiros três pontos
    for (int i = 0; i < 3 && i < obj->num_pontos; i++) {
        hull.pontos[hull.num_pontos][0] = obj->pontos[i][0];
        hull.pontos[hull.num_pontos][1] = obj->pontos[i][1];
        hull.num_pontos++;
    }
    
    // Processa os pontos restantes
    for (int i = 3; i < obj->num_pontos; i++) {
        while (hull.num_pontos >= 2) {
            float x1 = hull.pontos[hull.num_pontos-1][0] - hull.pontos[hull.num_pontos-2][0];
            float y1 = hull.pontos[hull.num_pontos-1][1] - hull.pontos[hull.num_pontos-2][1];
            float x2 = obj->pontos[i][0] - hull.pontos[hull.num_pontos-1][0];
            float y2 = obj->pontos[i][1] - hull.pontos[hull.num_pontos-1][1];
            
            float cross = crossProduct(x1, y1, x2, y2);
            
            if (cross <= 0) {
                // Remove o último ponto do hull
                hull.num_pontos--;
            } else {
                break;
            }
        }
        
        // Adiciona o ponto atual ao hull
        hull.pontos[hull.num_pontos][0] = obj->pontos[i][0];
        hull.pontos[hull.num_pontos][1] = obj->pontos[i][1];
        hull.num_pontos++;
    }
    
    // Copia o hull de volta para o objeto original
    *obj = hull;
}

// Implementação melhorada da Minkowski Sum
void minkowskiSum(Objeto *obj, float raio) {
    if (obj->num_pontos < 3) return;
    
    // Primeiro, calcula o fecho convexo do polígono
    convexHull(obj);
    
    // Se já é convexo, aplica uma expansão uniforme
    Ponto centro = calcularCentro(obj);
    
    for (int i = 0; i < obj->num_pontos; i++) {
        // Calcula a direção do centro para o vértice
        float dir_x = obj->pontos[i][0] - centro.x;
        float dir_y = obj->pontos[i][1] - centro.y;
        
        // Normaliza o vetor
        float length = sqrt(dir_x * dir_x + dir_y * dir_y);
        if (length > 0) {
            dir_x /= length;
            dir_y /= length;
        }
        
        // Expande o vértice na direção radial
        obj->pontos[i][0] += dir_x * raio;
        obj->pontos[i][1] += dir_y * raio;
    }
}