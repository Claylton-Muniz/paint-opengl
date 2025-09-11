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

// Função auxiliar para checar orientação
int orientacao(float x1, float y1, float x2, float y2, float x3, float y3) {
    float val = (y2 - y1) * (x3 - x2) - (x2 - x1) * (y3 - y2);
    if (fabs(val) < 1e-6) return 0;  // colinear
    return (val > 0) ? 1 : 2;        // 1 = horário, 2 = anti-horário
}

int segmentosSeIntersectam(float x1, float y1, float x2, float y2,
                           float x3, float y3, float x4, float y4) {
    int o1 = orientacao(x1, y1, x2, y2, x3, y3);
    int o2 = orientacao(x1, y1, x2, y2, x4, y4);
    int o3 = orientacao(x3, y3, x4, y4, x1, y1);
    int o4 = orientacao(x3, y3, x4, y4, x2, y2);

    // Caso geral
    if (o1 != o2 && o3 != o4) return 1;

    // Casos especiais (colinearidade)
    return 0;
}

// Função para verificar se um polígono é convexo
int ehPoligonoConvexo(Objeto *obj)
{
    if (obj->num_pontos < 3)
        return 1;

    int n = obj->num_pontos;
    int sinal = 0;

    for (int i = 0; i < n; i++)
    {
        float x1 = obj->pontos[i][0];
        float y1 = obj->pontos[i][1];
        float x2 = obj->pontos[(i + 1) % n][0];
        float y2 = obj->pontos[(i + 1) % n][1];
        float x3 = obj->pontos[(i + 2) % n][0];
        float y3 = obj->pontos[(i + 2) % n][1];

        float cross = (x2 - x1) * (y3 - y2) - (y2 - y1) * (x3 - x2);

        if (cross != 0)
        {
            if (sinal == 0)
            {
                sinal = (cross > 0) ? 1 : -1;
            }
            else if ((cross > 0 && sinal == -1) || (cross < 0 && sinal == 1))
            {
                return 0; // Não convexo
            }
        }
    }

    return 1; // Convexo
}

// Função para calcular a área de um polígono
float calcularAreaPoligono(Objeto *obj)
{
    if (obj->num_pontos < 3)
        return 0;

    float area = 0;
    int n = obj->num_pontos;

    for (int i = 0; i < n; i++)
    {
        float x1 = obj->pontos[i][0];
        float y1 = obj->pontos[i][1];
        float x2 = obj->pontos[(i + 1) % n][0];
        float y2 = obj->pontos[(i + 1) % n][1];

        area += (x1 * y2 - x2 * y1);
    }

    return fabs(area) / 2.0f;
}

// Função para verificar se dois polígonos se intersectam
int poligonosSeIntersectam(Objeto *a, Objeto *b)
{
    // Verifica se qualquer aresta de A intersecta qualquer aresta de B
    for (int i = 0; i < a->num_pontos; i++)
    {
        int next_i = (i + 1) % a->num_pontos;
        float x1 = a->pontos[i][0], y1 = a->pontos[i][1];
        float x2 = a->pontos[next_i][0], y2 = a->pontos[next_i][1];

        for (int j = 0; j < b->num_pontos; j++)
        {
            int next_j = (j + 1) % b->num_pontos;
            float x3 = b->pontos[j][0], y3 = b->pontos[j][1];
            float x4 = b->pontos[next_j][0], y4 = b->pontos[next_j][1];

            if (segmentosSeIntersectam(x1, y1, x2, y2, x3, y3, x4, y4))
            {
                return 1;
            }
        }
    }

    // Verifica se um polígono está completamente dentro do outro
    if (pontoDentroPoligono(a->pontos[0][0], a->pontos[0][1], *b) ||
        pontoDentroPoligono(b->pontos[0][0], b->pontos[0][1], *a))
    {
        return 1;
    }

    return 0;
}

// Função para decompor polígono em triângulos (triangulação simples)
void decomporEmTriangulos(Objeto *obj, Objeto triangulos[], int *num_triangulos)
{
    *num_triangulos = 0;

    if (obj->num_pontos < 3)
        return;

    // Triangulação simples (apenas para polígonos simples)
    for (int i = 1; i < obj->num_pontos - 1; i++)
    {
        if (*num_triangulos >= 50)
            break; // Limite de segurança

        triangulos[*num_triangulos].forma = TRIANGLES;
        triangulos[*num_triangulos].num_pontos = 3;

        triangulos[*num_triangulos].pontos[0][0] = obj->pontos[0][0];
        triangulos[*num_triangulos].pontos[0][1] = obj->pontos[0][1];

        triangulos[*num_triangulos].pontos[1][0] = obj->pontos[i][0];
        triangulos[*num_triangulos].pontos[1][1] = obj->pontos[i][1];

        triangulos[*num_triangulos].pontos[2][0] = obj->pontos[i + 1][0];
        triangulos[*num_triangulos].pontos[2][1] = obj->pontos[i + 1][1];

        (*num_triangulos)++;
    }
}

// Função para unir polígonos convexos
Objeto unirPoligonosConvexos(Objeto triangulos[], int num_triangulos)
{
    if (num_triangulos == 0)
    {
        Objeto vazio;
        vazio.num_pontos = 0;
        return vazio;
    }

    if (num_triangulos == 1)
    {
        return triangulos[0];
    }

    // Para simplificar, vamos pegar o fecho convexo de todos os pontos
    Objeto todosPontos;
    todosPontos.forma = LINE_LOOP;
    todosPontos.num_pontos = 0;

    // Coleta todos os pontos dos triângulos
    for (int i = 0; i < num_triangulos; i++)
    {
        for (int j = 0; j < triangulos[i].num_pontos; j++)
        {
            if (todosPontos.num_pontos < 200)
            {
                todosPontos.pontos[todosPontos.num_pontos][0] = triangulos[i].pontos[j][0];
                todosPontos.pontos[todosPontos.num_pontos][1] = triangulos[i].pontos[j][1];
                todosPontos.num_pontos++;
            }
        }
    }

    // Remove pontos duplicados (simplificado)
    for (int i = 0; i < todosPontos.num_pontos; i++)
    {
        for (int j = i + 1; j < todosPontos.num_pontos; j++)
        {
            float dx = todosPontos.pontos[i][0] - todosPontos.pontos[j][0];
            float dy = todosPontos.pontos[i][1] - todosPontos.pontos[j][1];
            if (dx * dx + dy * dy < 1.0f)
            { // Tolerância pequena
                // Remove ponto j
                for (int k = j; k < todosPontos.num_pontos - 1; k++)
                {
                    todosPontos.pontos[k][0] = todosPontos.pontos[k + 1][0];
                    todosPontos.pontos[k][1] = todosPontos.pontos[k + 1][1];
                }
                todosPontos.num_pontos--;
                j--;
            }
        }
    }

    // Calcula o fecho convexo de todos os pontos
    andrewConvexHull(&todosPontos);

    return todosPontos;
}

// Função principal para transformar polígono côncavo em convexo
void transformarConcavoEmConvexo(Objeto *obj)
{
    if (obj->num_pontos < 3)
        return;

    // Verifica se já é convexo
    if (ehPoligonoConvexo(obj))
    {
        printf("Polígono já é convexo.\n");
        return;
    }

    printf("Transformando polígono côncavo em convexo...\n");
    printf("Polígono original tem %d vértices.\n", obj->num_pontos);

    // Passo 1: Decompor em triângulos (polígonos convexos menores)
    Objeto triangulos[50];
    int num_triangulos = 0;
    decomporEmTriangulos(obj, triangulos, &num_triangulos);

    printf("Decomposto em %d triângulos.\n", num_triangulos);

    // Passo 2: Identificar e remover triângulos problemáticos
    Objeto triangulosValidos[50];
    int num_triangulosValidos = 0;

    for (int i = 0; i < num_triangulos; i++)
    {
        int problema = 0;

        // Verifica se este triângulo colide com outros
        for (int j = 0; j < num_triangulos; j++)
        {
            if (i != j && poligonosSeIntersectam(&triangulos[i], &triangulos[j]))
            {
                problema = 1;
                break;
            }
        }

        if (!problema)
        {
            triangulosValidos[num_triangulosValidos] = triangulos[i];
            num_triangulosValidos++;
        }
    }

    printf("Triângulos com problemas de colisão removidos: %d\n",
           num_triangulos - num_triangulosValidos);

    // Passo 3: Unir os triângulos válidos em um polígono convexo
    if (num_triangulosValidos > 0)
    {
        Objeto resultado = unirPoligonosConvexos(triangulosValidos, num_triangulosValidos);

        // Preserva o tipo original do objeto
        Forma forma_original = obj->forma;
        *obj = resultado;
        obj->forma = forma_original;

        printf("Polígono convexo resultante tem %d vértices.\n", obj->num_pontos);
    }
    else
    {
        printf("Nenhum triângulo válido encontrado. Usando fecho convexo original.\n");
        andrewConvexHull(obj);
    }
}


