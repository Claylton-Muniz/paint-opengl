void mouse(int button, int state, int x, int y)
{
    y = 540 - y;

    // Lógica para seleção de forma no menu
    if (y > 490 && y < 540)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if (x >= 10 && x <= 45) {forma_atual = PONTO; printf("Ponto selecionado\n");}
            else if (x >= 53 && x <= 88) {forma_atual = QUADRADO; printf("Quadrado selecionado\n");}
            else if (x >= 96 && x <= 131) {forma_atual = CIRCULO; printf("Circulo selecionado\n");}
            else if (x >= 139 && x <= 174) {forma_atual = LINHA; printf("Linha selecionado\n");}
            else if (x >= 182 && x <= 217) {forma_atual = LINE_STRIP; printf("LineStrip selecionado\n");}
            else if (x >= 225 && x <= 260) {forma_atual = LINE_LOOP; printf("LineLoop selecionado\n");}
            else if (x >= 268 && x <= 303) {forma_atual = TRIANGLES; printf("Triangulo selecionado\n");}
            else if (x >= 311 && x <= 346) {forma_atual = POLYGON; printf("Poligono selecionado\n");}
            else if (x >= 354 && x <= 389) {forma_atual = QUAD_STRIP; printf("QuadStrip selecionado\n");}
            estado_multiclique = 0; // Reseta o estado
        }
        glutPostRedisplay();
        return;
    }

    // Lógica para desenhar/selecionar na área de desenho
    if (x > 20 && x < 940 && y > 10 && y < 480)
    {
        // Limpa a seleção anterior
        for (int i = 0; i < num_objetos; i++) objetos[i].selecionado = 0;
        
        // Se for um clique com o botão direito, finaliza o desenho do polígono
        if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        {
            if(forma_atual == POLYGON && estado_multiclique > 2)
            {
                objetos[num_objetos].forma = POLYGON;
                objetos[num_objetos].num_pontos = estado_multiclique;
                for (int i = 0; i < estado_multiclique; i++)
                {
                    objetos[num_objetos].pontos[i][0] = pontos_multiclique[i][0];
                    objetos[num_objetos].pontos[i][1] = pontos_multiclique[i][1];
                }
                num_objetos++;
                estado_multiclique = 0;
            }
        }
        
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            // Tenta selecionar um objeto existente
            // ... (lógica de seleção de objeto)

            // Lógica para formas de clique único
            if (forma_atual == PONTO || forma_atual == QUADRADO || forma_atual == CIRCULO)
            {
                objetos[num_objetos].pontos[0][0] = x;
                objetos[num_objetos].pontos[0][1] = y;
                objetos[num_objetos].num_pontos = 1;
                objetos[num_objetos].forma = forma_atual;
                objetos[num_objetos].selecionado = 0;
                num_objetos++;
            }
            // Lógica para linha (2 cliques)
            else if (forma_atual == LINHA)
            {
                if (estado_multiclique == 0)
                {
                    pontos_multiclique[0][0] = x;
                    pontos_multiclique[0][1] = y;
                    estado_multiclique = 1;
                }
                else
                {
                    objetos[num_objetos].pontos[0][0] = pontos_multiclique[0][0];
                    objetos[num_objetos].pontos[0][1] = pontos_multiclique[0][1];
                    objetos[num_objetos].pontos[1][0] = x;
                    objetos[num_objetos].pontos[1][1] = y;
                    objetos[num_objetos].num_pontos = 2;
                    objetos[num_objetos].forma = LINHA;
                    objetos[num_objetos].selecionado = 0;
                    num_objetos++;
                    estado_multiclique = 0;
                }
            }
            // Lógica para outras formas (múltiplos cliques)
            else if (forma_atual == LINE_STRIP || forma_atual == LINE_LOOP || forma_atual == TRIANGLES || forma_atual == QUAD_STRIP)
            {
                pontos_multiclique[estado_multiclique][0] = x;
                pontos_multiclique[estado_multiclique][1] = y;
                estado_multiclique++;
                
                // Desenha a forma quando o número mínimo de cliques for alcançado
                if ((forma_atual == TRIANGLES && estado_multiclique == 3) ||
                    (forma_atual == LINE_STRIP && estado_multiclique >= 2) ||
                    (forma_atual == LINE_LOOP && estado_multiclique >= 2) ||
                    (forma_atual == QUAD_STRIP && estado_multiclique >= 4)
                )
                {
                    objetos[num_objetos].forma = forma_atual;
                    objetos[num_objetos].num_pontos = estado_multiclique;
                    for (int i = 0; i < estado_multiclique; i++)
                    {
                        objetos[num_objetos].pontos[i][0] = pontos_multiclique[i][0];
                        objetos[num_objetos].pontos[i][1] = pontos_multiclique[i][1];
                    }
                    num_objetos++;
                    estado_multiclique = 0;
                }
            }
        }
    }
    glutPostRedisplay();
}