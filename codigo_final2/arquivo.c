#include <stdio.h>
#include <stdlib.h>
#include "arquivo.h"
#include "estruturas.h"

#define AREA_LIVROS 1000000
#define AREA_USUARIOS 100000


/**
 * Propósito: Abre arquivo binário para leitura/escrita, criando se não existir
 * Pré-condição: nome do arquivo válido
 * Pós-condição: retorna ponteiro para arquivo ou NULL se erro
 */

FILE* abrirArquivoBinario(char arquivo[]) {
    FILE* file = fopen(arquivo, "rb+");  // tenta abrir existente
    if (!file) {
        file = fopen(arquivo, "wb+");    // cria novo
        if (!file) {
            perror("Erro ao criar/abrir arquivo");
            return NULL;
        }

        // Calcula posições iniciais para cada área
        long pos_base = sizeof(Cabecalho);
        Cabecalho cabecalho = {
            .pos_cabeca_livros = -1,
            .pos_topo_livros = pos_base,
            .pos_cabeca_usuarios = -1,
            .pos_topo_usuarios = pos_base + AREA_LIVROS,
            .pos_cabeca_emprestimos = -1,
            .pos_topo_emprestimos = pos_base + AREA_LIVROS + AREA_USUARIOS,
            .total_livros = 0,
            .total_usuarios = 0,
            .total_emprestimos = 0
        };

        fwrite(&cabecalho, sizeof(Cabecalho), 1, file);
        fflush(file);
    }
    return file;
}

/**
 * Propósito: Lê o cabeçalho do arquivo
 * Pré-condição: arquivo aberto e válido
 * Pós-condição: retorna 1 se sucesso, 0 se erro
 */
int lerCabecalho(FILE* file, Cabecalho* cabecalho) {
    if (!file || !cabecalho) return 0;
    rewind(file);
    return fread(cabecalho, sizeof(Cabecalho), 1, file) == 1;
}

/**
 * Propósito: Escreve o cabeçalho no arquivo
 * Pré-condição: arquivo aberto e cabeçalho válido
 * Pós-condição: retorna 1 se sucesso, 0 se erro
 */
int escreverCabecalho(FILE* file, Cabecalho* cabecalho) {
    if (!file || !cabecalho) return 0;
    rewind(file);
    int result = fwrite(cabecalho, sizeof(Cabecalho), 1, file) == 1;
    fflush(file);
    return result;
}

/**
 * Propósito: Imprime informações do cabeçalho (para debug)
 * Pré-condição: cabeçalho válido
 * Pós-condição: informações impressas na tela
 */
void imprimirCabecalho(Cabecalho* cabecalho) {
    if (!cabecalho) return;
    printf("\n=== INFORMAÇÕES DO ARQUIVO ===\n");
    printf("Total de livros: %d\n", cabecalho->total_livros);
    printf("Total de usuários: %d\n", cabecalho->total_usuarios);
    printf("Total de empréstimos: %d\n", cabecalho->total_emprestimos);
    printf("Primeira posição de livros: %d\n", cabecalho->pos_cabeca_livros);
    printf("Próxima posição livre (livros): %d\n", cabecalho->pos_topo_livros);
    printf("Primeira posição de usuários: %d\n", cabecalho->pos_cabeca_usuarios);
    printf("Próxima posição livre (usuários): %d\n", cabecalho->pos_topo_usuarios);
    printf("Primeira posição de empréstimos: %d\n", cabecalho->pos_cabeca_emprestimos);
    printf("Próxima posição livre (emprestimos): %d\n", cabecalho->pos_topo_emprestimos);
    printf("==============================\n\n");
}
