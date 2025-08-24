#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "gl_utils.h"

float distancia_ponto_segmento(float px, float py,
                               float x1, float y1,
                               float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    if (dx == 0 && dy == 0)
    { // segmento degenerado
        dx = px - x1;
        dy = py - y1;
        return sqrt(dx * dx + dy * dy);
    }

    float t = ((px - x1) * dx + (py - y1) * dy) / (dx * dx + dy * dy);
    if (t < 0)
        t = 0;
    else if (t > 1)
        t = 1;

    float projx = x1 + t * dx;
    float projy = y1 + t * dy;

    dx = px - projx;
    dy = py - projy;
    return sqrt(dx * dx + dy * dy);
}

int ponto_dentro_poligono(float px, float py, Objeto obj)
{
    int dentro = 0;
    for (int i = 0, j = obj.num_pontos - 1; i < obj.num_pontos; j = i++)
    {
        float xi = obj.pontos[i][0], yi = obj.pontos[i][1];
        float xj = obj.pontos[j][0], yj = obj.pontos[j][1];

        int intersect = ((yi > py) != (yj > py)) &&
                        (px < (xj - xi) * (py - yi) / (yj - yi) + xi);
        if (intersect)
            dentro = !dentro;
    }
    return dentro;
}
