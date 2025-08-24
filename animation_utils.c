#include <GL/glut.h>
#include <math.h>
#include "gl_utils.h"
#include "animation_utils.h"
#include "transform_utils.h"

static int animacao_ligada = 0;
static float tempo_animacao = 0.0f;

void iniciar_animacao()
{
    animacao_ligada = 1;
    tempo_animacao = 0.0f;
    glutTimerFunc(16, atualizar_animacao, 0); // ~60 FPS
}

void parar_animacao()
{
    animacao_ligada = 0;
}

int animacao_ativa()
{
    return animacao_ligada;
}

void atualizar_animacao(int value)
{
    if (!animacao_ligada)
        return;

    tempo_animacao += 0.05f;

    // Aplica animação de pulsação em todos os objetos selecionados
    for (int i = 0; i < num_objetos; i++)
    {
        Objeto *obj = &objetos[i];
        Ponto centro = calcularCentro(obj);

        // Animação de pulsação (escala)
        float pulsacao = 0.05f * sin(tempo_animacao * 2.0f) + 1.0f;

        // Move para origem, escala, move de volta
        transladar(obj, -centro.x, -centro.y);
        escalar(obj, pulsacao, pulsacao);
        transladar(obj, centro.x, centro.y);
    }

    glutPostRedisplay();
    glutTimerFunc(16, atualizar_animacao, 0); // Continua a animação
}