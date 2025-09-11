#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "gl_utils.h"
#include "transform_utils.h"

#define PI 3.14159265358979323846

// Aplica uma matriz de transformação 3x3 a um ponto
void aplicarMatriz(float ponto[2], float M[3][3])
{
    float x = ponto[0];
    float y = ponto[1];

    ponto[0] = M[0][0] * x + M[0][1] * y + M[0][2];
    ponto[1] = M[1][0] * x + M[1][1] * y + M[1][2];
}

// Aplica uma matriz de transformação a todos os pontos de um objeto
void aplicarMatrizObjeto(Objeto *obj, float M[3][3])
{
    for (int i = 0; i < obj->num_pontos; i++)
    {
        aplicarMatriz(obj->pontos[i], M);
    }
}

// Translada um objeto por (tx, ty)
void transladar(Objeto *obj, float tx, float ty)
{
    float T[3][3] = {{1, 0, tx}, {0, 1, ty}, {0, 0, 1}};
    aplicarMatrizObjeto(obj, T);
}

// Calcula o centro de um objeto
Ponto calcularCentro(const Objeto *obj)
{
    Ponto c = {0, 0};
    for (int i = 0; i < obj->num_pontos; i++)
    {
        c.x += obj->pontos[i][0];
        c.y += obj->pontos[i][1];
    }
    c.x /= obj->num_pontos;
    c.y /= obj->num_pontos;
    return c;
}

// Rotaciona um objeto pelo ângulo especificado
void rotacionar(Objeto *obj, float angulo)
{
    Ponto c = calcularCentro(obj);
    float rad = angulo * PI / 180.0;

    // Matriz de translação para a origem
    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};

    // Matriz de rotação
    float R[3][3] = {
        {cos(rad), -sin(rad), 0},
        {sin(rad), cos(rad), 0},
        {0, 0, 1}};

    // Matriz de translação de volta
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);   // Move para origem
    aplicarMatrizObjeto(obj, R);      // Rotaciona
    aplicarMatrizObjeto(obj, Tvolta); // Move de volta
}

void escalar(Objeto *obj, float sx, float sy)
{
    Ponto c = calcularCentro(obj);

    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};
    float S[3][3] = {{sx, 0, 0}, {0, sy, 0}, {0, 0, 1}};
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);
    aplicarMatrizObjeto(obj, S);
    aplicarMatrizObjeto(obj, Tvolta);
}

// Reflexão em relação ao eixo X
void refletirX(Objeto *obj)
{
    Ponto c = calcularCentro(obj);

    // Matriz de translação para a origem
    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};

    // Matriz de reflexão em X
    float Rx[3][3] = {{1, 0, 0}, {0, -1, 0}, {0, 0, 1}};

    // Matriz de translação de volta
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);   // Move para origem
    aplicarMatrizObjeto(obj, Rx);     // Reflexão em X
    aplicarMatrizObjeto(obj, Tvolta); // Move de volta
}

// Reflexão em relação ao eixo Y
void refletirY(Objeto *obj)
{
    Ponto c = calcularCentro(obj);

    // Matriz de translação para a origem
    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};

    // Matriz de reflexão em Y
    float Ry[3][3] = {{-1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

    // Matriz de translação de volta
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);   // Move para origem
    aplicarMatrizObjeto(obj, Ry);     // Reflexão em Y
    aplicarMatrizObjeto(obj, Tvolta); // Move de volta
}

// Reflexão em relação à origem
void refletirOrigem(Objeto *obj)
{
    Ponto c = calcularCentro(obj);

    // Matriz de translação para a origem
    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};

    // Matriz de reflexão na origem
    float Ro[3][3] = {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}};

    // Matriz de translação de volta
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);   // Move para origem
    aplicarMatrizObjeto(obj, Ro);     // Reflexão na origem
    aplicarMatrizObjeto(obj, Tvolta); // Move de volta
}

// Cisalhamento no eixo X
void cisalharX(Objeto *obj, float shx)
{
    Ponto c = calcularCentro(obj);

    // Matriz de translação para a origem
    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};

    // Matriz de cisalhamento em X
    float Shx[3][3] = {{1, shx, 0}, {0, 1, 0}, {0, 0, 1}};

    // Matriz de translação de volta
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);   // Move para origem
    aplicarMatrizObjeto(obj, Shx);    // Cisalhamento em X
    aplicarMatrizObjeto(obj, Tvolta); // Move de volta
}

// Cisalhamento no eixo Y
void cisalharY(Objeto *obj, float shy)
{
    Ponto c = calcularCentro(obj);

    // Matriz de translação para a origem
    float Tida[3][3] = {{1, 0, -c.x}, {0, 1, -c.y}, {0, 0, 1}};

    // Matriz de cisalhamento em Y
    float Shy[3][3] = {{1, 0, 0}, {shy, 1, 0}, {0, 0, 1}};

    // Matriz de translação de volta
    float Tvolta[3][3] = {{1, 0, c.x}, {0, 1, c.y}, {0, 0, 1}};

    aplicarMatrizObjeto(obj, Tida);   // Move para origem
    aplicarMatrizObjeto(obj, Shy);    // Cisalhamento em Y
    aplicarMatrizObjeto(obj, Tvolta); // Move de volta
}

// Função auxiliar para comparar pontos por ângulo polar
int comparePoints(const void *a, const void *b)
{
    float *p1 = (float *)a;
    float *p2 = (float *)b;

    // Compara por coordenada y, depois por coordenada x
    if (p1[1] != p2[1])
    {
        return (p1[1] > p2[1]) ? 1 : -1;
    }
    return (p1[0] > p2[0]) ? 1 : -1;
}

float crossProduct(float x1, float y1, float x2, float y2)
{
    return x1 * y2 - x2 * y1;
}

// Calcula o fecho convexo usando o algoritmo de Andrew's monotone chain
void andrewConvexHull(Objeto *obj)
{
    if (obj->num_pontos < 3)
        return;

    // Ordena pontos por coordenadas
    qsort(obj->pontos, obj->num_pontos, sizeof(obj->pontos[0]), comparePoints);

    Objeto hull;
    hull.forma = LINE_LOOP;
    hull.num_pontos = 0;

    // Constroi o casco inferior
    for (int i = 0; i < obj->num_pontos; i++)
    {
        while (hull.num_pontos >= 2)
        {
            float x1 = hull.pontos[hull.num_pontos - 1][0] - hull.pontos[hull.num_pontos - 2][0];
            float y1 = hull.pontos[hull.num_pontos - 1][1] - hull.pontos[hull.num_pontos - 2][1];
            float x2 = obj->pontos[i][0] - hull.pontos[hull.num_pontos - 1][0];
            float y2 = obj->pontos[i][1] - hull.pontos[hull.num_pontos - 1][1];

            if (crossProduct(x1, y1, x2, y2) <= 0)
            {
                break;
            }
            hull.num_pontos--;
        }
        hull.pontos[hull.num_pontos][0] = obj->pontos[i][0];
        hull.pontos[hull.num_pontos][1] = obj->pontos[i][1];
        hull.num_pontos++;
    }

    // Constroi o casco superior
    int lower_hull_size = hull.num_pontos;
    for (int i = obj->num_pontos - 2; i >= 0; i--)
    {
        while (hull.num_pontos > lower_hull_size)
        {
            float x1 = hull.pontos[hull.num_pontos - 1][0] - hull.pontos[hull.num_pontos - 2][0];
            float y1 = hull.pontos[hull.num_pontos - 1][1] - hull.pontos[hull.num_pontos - 2][1];
            float x2 = obj->pontos[i][0] - hull.pontos[hull.num_pontos - 1][0];
            float y2 = obj->pontos[i][1] - hull.pontos[hull.num_pontos - 1][1];

            if (crossProduct(x1, y1, x2, y2) <= 0)
            {
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

void minkowskiSum(Objeto *a, Objeto *b, Objeto *resultado) {
    resultado->forma = LINE_LOOP;
    resultado->num_pontos = 0;
    
    // Para cada ponto em A, soma com cada ponto em B
    for (int i = 0; i < a->num_pontos; i++) {
        for (int j = 0; j < b->num_pontos; j++) {
            if (resultado->num_pontos < 200) {
                resultado->pontos[resultado->num_pontos][0] = a->pontos[i][0] + b->pontos[j][0];
                resultado->pontos[resultado->num_pontos][1] = a->pontos[i][1] + b->pontos[j][1];
                resultado->num_pontos++;
            }
        }
    }
    
    // Calcula o fecho convexo do resultado
    andrewConvexHull(resultado);
}

// Função para verificar colisão usando soma de Minkowski
// Dois polígonos A e B colidem se a origem está contida em A ⊕ (-B)
int poligonosColidemMinkowski(Objeto *a, Objeto *b) {
    // Cria -B
    Objeto b_negativo = *b;
    refletirOrigem(&b_negativo);
    
    // Calcula A ⊕ (-B)
    Objeto minkowski;
    minkowskiSum(a, &b_negativo, &minkowski);
    
    // Verifica se a origem (0,0) está dentro de A ⊕ (-B)
    return pontoDentroPoligono(0, 0, minkowski);
}

// Função para decompor polígono em triângulos usando ear clipping
void decomporEmTriangulosEarClipping(Objeto *obj, Objeto triangulos[], int *num_triangulos) {
    *num_triangulos = 0;
    
    if (obj->num_pontos < 3) return;
    
    // Cria uma cópia do polígono para trabalhar
    Objeto poligono = *obj;
    
    while (poligono.num_pontos > 3) {
        int ear_found = 0;
        
        for (int i = 0; i < poligono.num_pontos; i++) {
            int prev = (i - 1 + poligono.num_pontos) % poligono.num_pontos;
            int next = (i + 1) % poligono.num_pontos;
            
            float x_prev = poligono.pontos[prev][0], y_prev = poligono.pontos[prev][1];
            float x_curr = poligono.pontos[i][0], y_curr = poligono.pontos[i][1];
            float x_next = poligono.pontos[next][0], y_next = poligono.pontos[next][1];
            
            // Verifica se é um ouvido
            float cross = (x_next - x_curr) * (y_prev - y_curr) - 
                         (y_next - y_curr) * (x_prev - x_curr);
            
            if (cross > 0) { // Vértice convexo
                // Cria triângulo
                triangulos[*num_triangulos].forma = TRIANGLES;
                triangulos[*num_triangulos].num_pontos = 3;
                
                triangulos[*num_triangulos].pontos[0][0] = x_prev;
                triangulos[*num_triangulos].pontos[0][1] = y_prev;
                triangulos[*num_triangulos].pontos[1][0] = x_curr;
                triangulos[*num_triangulos].pontos[1][1] = y_curr;
                triangulos[*num_triangulos].pontos[2][0] = x_next;
                triangulos[*num_triangulos].pontos[2][1] = y_next;
                
                (*num_triangulos)++;
                
                // Remove o vértice atual do polígono
                for (int j = i; j < poligono.num_pontos - 1; j++) {
                    poligono.pontos[j][0] = poligono.pontos[j + 1][0];
                    poligono.pontos[j][1] = poligono.pontos[j + 1][1];
                }
                poligono.num_pontos--;
                
                ear_found = 1;
                break;
            }
        }
        
        if (!ear_found) break; // Prevenção contra loop infinito
    }
    
    // Adiciona o último triângulo
    if (poligono.num_pontos == 3) {
        triangulos[*num_triangulos] = poligono;
        triangulos[*num_triangulos].forma = TRIANGLES;
        (*num_triangulos)++;
    }
}

// Função principal para transformar polígono côncavo em convexo usando Minkowski
void transformarConcavoEmConvexo(Objeto *obj) {
    if (obj->num_pontos < 3) return;
       
    // Decompor em triângulos usando ear clipping
    Objeto triangulos[50];
    int num_triangulos = 0;
    decomporEmTriangulosEarClipping(obj, triangulos, &num_triangulos);
    
    // Identificar triângulos que colidem usando Minkowski
    Objeto triangulosValidos[50];
    int num_triangulosValidos = 0;
    
    for (int i = 0; i < num_triangulos; i++) {
        int colide_com_outro = 0;
        
        // Verifica se este triângulo colide com outros triângulos
        for (int j = 0; j < num_triangulos; j++) {
            if (i != j && poligonosColidemMinkowski(&triangulos[i], &triangulos[j])) {
                colide_com_outro = 1;
                break;
            }
        }
        
        if (!colide_com_outro) {
            triangulosValidos[num_triangulosValidos] = triangulos[i];
            num_triangulosValidos++;
        }
    }
    
    // Se todos os triângulos são válidos, usar o fecho convexo original
    if (num_triangulosValidos == num_triangulos) {
        andrewConvexHull(obj);
        return;
    }
    
    // Para triângulos problemáticos, ajustar posições
    for (int i = 0; i < num_triangulos; i++) {
        int colide = 0;
        for (int j = 0; j < num_triangulos; j++) {
            if (i != j && poligonosColidemMinkowski(&triangulos[i], &triangulos[j])) {
                colide = 1;
                break;
            }
        }
        
        if (colide) {
            // Move ligeiramente o triângulo para evitar colisão
            transladar(&triangulos[i], 2.0f, 2.0f);
        }
    }
    
    // Unir todos os triângulos em um polígono convexo
    Objeto todosPontos;
    todosPontos.forma = LINE_LOOP;
    todosPontos.num_pontos = 0;
    
    // Coleta todos os pontos
    for (int i = 0; i < num_triangulos; i++) {
        for (int j = 0; j < triangulos[i].num_pontos; j++) {
            if (todosPontos.num_pontos < 200) {
                todosPontos.pontos[todosPontos.num_pontos][0] = triangulos[i].pontos[j][0];
                todosPontos.pontos[todosPontos.num_pontos][1] = triangulos[i].pontos[j][1];
                todosPontos.num_pontos++;
            }
        }
    }
    
    // Remove pontos duplicados
    for (int i = 0; i < todosPontos.num_pontos; i++) {
        for (int j = i + 1; j < todosPontos.num_pontos; j++) {
            float dx = todosPontos.pontos[i][0] - todosPontos.pontos[j][0];
            float dy = todosPontos.pontos[i][1] - todosPontos.pontos[j][1];
            if (dx * dx + dy * dy < 1.0f) {
                for (int k = j; k < todosPontos.num_pontos - 1; k++) {
                    todosPontos.pontos[k][0] = todosPontos.pontos[k + 1][0];
                    todosPontos.pontos[k][1] = todosPontos.pontos[k + 1][1];
                }
                todosPontos.num_pontos--;
                j--;
            }
        }
    }
    
    // Calcula o fecho convexo
    andrewConvexHull(&todosPontos);
    
    // Preserva o tipo original
    Forma forma_original = obj->forma;
    *obj = todosPontos;
    obj->forma = forma_original;
}