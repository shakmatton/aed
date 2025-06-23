#include <stdio.h>
#include <stdlib.h>
#include "arquivo.h"

// ==================== FUNÇÕES DE ARQUIVO ====================

FILE* abrirArquivoBinario(char arquivo[]) {
    FILE* file = fopen(arquivo, "rb+");  // Tenta abrir existente para leitura/escrita
    if (!file) {
        file = fopen(arquivo, "wb+");    // Se não existir, cria novo
        if (!file) {
            printf("Erro ao criar/abrir arquivo: %s\n", arquivo);
            return NULL;
        }

        // Arquivo novo - inicializa cabeçalho
        Cabecalho cabecalho = {
            -1, sizeof(Cabecalho),  // livros: cabeca, topo
            -1, sizeof(Cabecalho),  // usuarios: cabeca, topo
            -1, sizeof(Cabecalho),  // emprestimos: cabeca, topo
            0, 0, 0                 // contadores zerados
        };
        fwrite(&cabecalho, sizeof(Cabecalho), 1, file);
        fflush(file);
        printf("Arquivo criado e inicializado: %s\n", arquivo);
    }
    return file;
}

int lerCabecalho(FILE* file, Cabecalho* cabecalho) {
    rewind(file);  // volta para o início
    return fread(cabecalho, sizeof(Cabecalho), 1, file) == 1;
}

int escreverCabecalho(FILE* file, Cabecalho* cabecalho) {
    rewind(file);  // volta para o início
    int result = fwrite(cabecalho, sizeof(Cabecalho), 1, file) == 1;
    fflush(file);
    return result;
}

void imprimirCabecalho(Cabecalho* cabecalho) {
    printf("\n=== INFORMAÇÕES DO ARQUIVO ===\n");
    printf("Total de livros: %d\n", cabecalho->total_livros);
    printf("Total de usuários: %d\n", cabecalho->total_usuarios);
    printf("Total de empréstimos: %d\n", cabecalho->total_emprestimos);
    printf("Primeira posição de livros: %d\n", cabecalho->pos_cabeca_livros);
    printf("Próxima posição livre (livros): %d\n", cabecalho->pos_topo_livros);
    printf("Primeira posição de usuários: %d\n", cabecalho->pos_cabeca_usuarios);
    printf("Próxima posição livre (usuários): %d\n", cabecalho->pos_topo_usuarios);
    printf("==============================\n\n");
}
