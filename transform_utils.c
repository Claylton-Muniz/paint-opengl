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

void minkowskiSum(Objeto *obj, float raio) {
    // Verifica se o objeto é um polígono válido
    if (obj->num_pontos < 3) {
        return; // Não é um polígono válido
    }
    
    // Cria um novo objeto para armazenar o resultado
    Objeto resultado;
    resultado.forma = LINE_LOOP;
    resultado.num_pontos = 0;
    
    // Para cada vértice do polígono original
    for (int i = 0; i < obj->num_pontos; i++) {
        int prev = (i == 0) ? obj->num_pontos - 1 : i - 1;
        int next = (i == obj->num_pontos - 1) ? 0 : i + 1;
        
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
        
        // Soma as normais para obter a direção do offset
        float offsetX = n1x + n2x;
        float offsetY = n1y + n2y;
        
        // Normaliza o vetor de offset
        float offsetLen = sqrt(offsetX*offsetX + offsetY*offsetY);
        if (offsetLen > 0) {
            offsetX /= offsetLen;
            offsetY /= offsetLen;
        }
        
        // Calcula o novo vértice
        resultado.pontos[resultado.num_pontos][0] = obj->pontos[i][0] + offsetX * raio;
        resultado.pontos[resultado.num_pontos][1] = obj->pontos[i][1] + offsetY * raio;
        resultado.num_pontos++;
        
        // Para cantos muito agudos, adiciona vértices extras para suavizar
        float dot = n1x * n2x + n1y * n2y;
        if (dot < 0.5f) { // Ângulo agudo
            int num_segments = 5;
            for (int j = 1; j < num_segments; j++) {
                float t = (float)j / num_segments;
                float interpX = n1x * (1-t) + n2x * t;
                float interpY = n1y * (1-t) + n2y * t;
                
                float interpLen = sqrt(interpX*interpX + interpY*interpY);
                if (interpLen > 0) {
                    interpX /= interpLen;
                    interpY /= interpLen;
                }
                
                resultado.pontos[resultado.num_pontos][0] = obj->pontos[i][0] + interpX * raio;
                resultado.pontos[resultado.num_pontos][1] = obj->pontos[i][1] + interpY * raio;
                resultado.num_pontos++;
            }
        }
    }
    
    // Substitui o objeto original pelo resultado
    *obj = resultado;
}
