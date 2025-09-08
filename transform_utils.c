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
    hull.forma = POLYGON;
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

// Implementação verdadeira do Minkowski Sum com um disco
void trueMinkowskiSum(Objeto *obj, float raio) {
    if (obj->num_pontos < 3) return;
    
    // Primeiro calculamos o fecho convexo
    andrewConvexHull(obj);
    
    // Para um disco, a Minkowski Sum é equivalente a "inflar" o polígono
    // Criamos um novo objeto com vértices adicionais para representar a soma
    
    Objeto resultado;
    resultado.forma = LINE_LOOP;
    resultado.num_pontos = 0;
    
    int n = obj->num_pontos;
    
    // Para cada aresta do polígono convexo, adicionamos um arco circular
    for (int i = 0; i < n; i++) {
        int prev = (i == 0) ? n - 1 : i - 1;
        int next = (i == n - 1) ? 0 : i + 1;
        
        // Vetores das arestas adjacentes
        float v1x = obj->pontos[i][0] - obj->pontos[prev][0];
        float v1y = obj->pontos[i][1] - obj->pontos[prev][1];
        float v2x = obj->pontos[next][0] - obj->pontos[i][0];
        float v2y = obj->pontos[next][1] - obj->pontos[i][1];
        
        // Normaliza os vetores
        float len1 = sqrt(v1x*v1x + v1y*v1y);
        float len2 = sqrt(v2x*v2x + v2y*v2y);
        
        if (len1 > 0) {
            v1x /= len1;
            v1y /= len1;
        }
        if (len2 > 0) {
            v2x /= len2;
            v2y /= len2;
        }
        
        // Calcula as normais externas
        float n1x = -v1y;
        float n1y = v1x;
        float n2x = -v2y;
        float n2y = v2x;
        
        // Calcula a bissetriz (soma das normais)
        float bisx = n1x + n2x;
        float bisy = n1y + n2y;
        
        // Normaliza a bissetriz
        float len_bis = sqrt(bisx*bisx + bisy*bisy);
        if (len_bis > 0) {
            bisx /= len_bis;
            bisy /= len_bis;
        }
        
        // Calcula o fator de escala para a bissetriz
        float dot = n1x * bisx + n1y * bisy;
        float scale = (dot != 0) ? raio / dot : raio;
        
        // Adiciona o vértice expandido
        resultado.pontos[resultado.num_pontos][0] = obj->pontos[i][0] + bisx * scale;
        resultado.pontos[resultado.num_pontos][1] = obj->pontos[i][1] + bisy * scale;
        resultado.num_pontos++;
        
        // Para cantos, adiciona pontos extras para suavizar a curva
        float angle = acos(n1x * n2x + n1y * n2y);
        if (angle < 2.8) { // Ângulo agudo (aproximadamente 160 graus)
            int segments = 5;
            for (int j = 1; j < segments; j++) {
                float t = (float)j / segments;
                float interp_x = n1x * (1-t) + n2x * t;
                float interp_y = n1y * (1-t) + n2y * t;
                
                float len_interp = sqrt(interp_x*interp_x + interp_y*interp_y);
                if (len_interp > 0) {
                    interp_x /= len_interp;
                    interp_y /= len_interp;
                }
                
                resultado.pontos[resultado.num_pontos][0] = obj->pontos[i][0] + interp_x * raio;
                resultado.pontos[resultado.num_pontos][1] = obj->pontos[i][1] + interp_y * raio;
                resultado.num_pontos++;
            }
        }
    }
    
    *obj = resultado;
}