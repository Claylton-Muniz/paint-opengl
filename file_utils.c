#include <stdio.h>
#include <string.h>
#include "gl_utils.h"
#include "file_utils.h"

void salvar_objetos(const char* filename) {
    char path[21];

    snprintf(path, sizeof(path), "imgs/%s", filename);

    FILE* file = fopen(path, "w"); // isso pode da erro, solucionar o problema depois

    // Escreve o número total de objetos
    fprintf(file, "%d\n", num_objetos);

    for (int i = 0; i < num_objetos; i++) {
        Objeto* obj = &objetos[i];
        
        fprintf(file, "%d ", obj->forma);
        fprintf(file, "%d ", obj->num_pontos);
        fprintf(file, "%d ", obj->selecionado);
        for (int j = 0; j < obj->num_pontos; j++) {
            fprintf(file, "%.2f %.2f ", obj->pontos[j][0], obj->pontos[j][1]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Objetos salvos em: %s\n", path);
}

void carregar_objetos(const char* filename) {
    char path[21];

    snprintf(path, sizeof(path), "imgs/%s", filename);

    FILE* file = fopen(path, "r"); // mesmo erro

    // Lê o número total de objetos
    int total_objetos;
    fscanf(file, "%d", &total_objetos);
    
    // Limpa os objetos atuais
    num_objetos = 0;

    for (int i = 0; i < total_objetos; i++) {
        if (num_objetos >= 100) {
            printf("Limite máximo de objetos atingido!\n");
            break;
        }

        Objeto* obj = &objetos[num_objetos];
        int forma, num_pontos, selecionado;
        
        // Lê os dados básicos do objeto
        fscanf(file, "%d %d %d", &forma, &num_pontos, &selecionado);
        
        obj->forma = (Forma)forma;
        obj->num_pontos = num_pontos;
        obj->selecionado = selecionado;
        
        // Lê todos os pontos
        for (int j = 0; j < num_pontos; j++) {
            fscanf(file, "%f %f", &obj->pontos[j][0], &obj->pontos[j][1]);
        }
        
        num_objetos++;
    }

    fclose(file);
    printf("Objetos carregados de: %s\n", path);
}