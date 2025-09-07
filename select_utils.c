#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "gl_utils.h"

// algoritmo do tiro
int gerarCodigo(float x, float y, float xmin, float ymin, float xmax, float ymax)
{
    int code = 0;
    if (x < xmin) code |= 1; // Esquerda
    if (x > xmax) code |= 2; // Direita
    if (y < ymin) code |= 4; // Abaixo
    if (y > ymax) code |= 8; // Acima
    return code;
}

// Algoritmo para seleção de linha
int cohenSutherlandClip(float x0, float y0, float x1, float y1, 
                       float xmin, float ymin, float xmax, float ymax)
{
    int code0 = gerarCodigo(x0, y0, xmin, ymin, xmax, ymax);
    int code1 = gerarCodigo(x1, y1, xmin, ymin, xmax, ymax);
    int accept = 0;

    // Casos triviais
    while (1) {
        // ambos dentro
        if (!(code0 | code1)) {
            accept = 1;
            break;
        }
        // ambos fora do mesmo lado
        else if (code0 & code1) {
            break;
        }
        // Caso não trivial
        // precisa calcular interseção
        else {
            int codeOut = code0 ? code0 : code1;
            float x, y;

            // Calcular ponto de interseção
            if (codeOut & 8) { // Acima
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            } else if (codeOut & 4) { // Abaixo
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            } else if (codeOut & 2) { // Direita
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            } else if (codeOut & 1) { // Esquerda
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            // Substituir o ponto fora pelo ponto de interseção
            if (codeOut == code0) {
                x0 = x;
                y0 = y;
                code0 = gerarCodigo(x0, y0, xmin, ymin, xmax, ymax);
            } else {
                x1 = x;
                y1 = y;
                code1 = gerarCodigo(x1, y1, xmin, ymin, xmax, ymax);
            }
        }
    }
    return accept;
}

int pickPonto(float px, float py, float mx, float my, float tolerancia)
{
    return (mx >= px - tolerancia && mx <= px + tolerancia &&
            my >= py - tolerancia && my <= py + tolerancia);
}

// Seleção de linha usando o tiro
int pickLinha(float x1, float y1, float x2, float y2, 
             float mx, float my, float tolerancia)
{
    float xmin = mx - tolerancia;
    float xmax = mx + tolerancia;
    float ymin = my - tolerancia;
    float ymax = my + tolerancia;
    
    return cohenSutherlandClip(x1, y1, x2, y2, xmin, ymin, xmax, ymax);
}

// seleção de polígono
int pontoDentroPoligono(float px, float py, Objeto obj)
{
    int dentro = 0;
    int n = obj.num_pontos;
    
    for (int i = 0, j = n - 1; i < n; j = i++) {
        float xi = obj.pontos[i][0], yi = obj.pontos[i][1];
        float xj = obj.pontos[j][0], yj = obj.pontos[j][1];
        
        // Casos trivial
        if ((yi > py && yj > py) || // totalmente acima
            (yi < py && yj < py) || // totalmente abaixo
            (xi < px && xj < px))   // totalmente à esquerda
            continue;
        
        // Caso não trivial
        // calcula interseção
        if ((yi > py) != (yj > py)) {
            float xi_intersec = xj + (px - xj) * (yi - yj) / (xi - xj);
            if (xi_intersec > px) {
                dentro = !dentro;
            }
        }
    }
    return dentro;
}