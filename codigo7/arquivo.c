#include <stdio.h>
#include <stdlib.h>
#include "arquivo.h"
#include "estruturas.h"

// Tamanhos máximos estimados para cada seção (em bytes)
#define AREA_LIVROS 1000000    // ~10.000 livros
#define AREA_USUARIOS 100000   // ~2.000 usuários

/**
 * @brief Abre (ou cria) o arquivo binário de dados.
 *
 * @param arquivo Nome do arquivo (por exemplo, "biblioteca.dat").
 * @return FILE* Ponteiro para o arquivo aberto em modo "rb+" ou "wb+"; NULL em caso de erro.
 *
 * @pré O sistema de arquivos deve permitir criação/leitura/escrita.
 * @pós Se o arquivo não existia, ele é criado e inicializado com um cabeçalho padrão.
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
 * @brief Lê o cabeçalho do arquivo binário.
 *
 * @param file      Ponteiro para o arquivo (já aberto).
 * @param cabecalho Ponteiro onde será armazenado o cabeçalho lido.
 * @return int 1 em sucesso, 0 em falha.
 *
 * @pré file não pode ser NULL.
 * @pós cabecalho contém os dados do início do arquivo.
 */
int lerCabecalho(FILE* file, Cabecalho* cabecalho) {
    if (!file || !cabecalho) return 0;
    rewind(file);
    return fread(cabecalho, sizeof(Cabecalho), 1, file) == 1;
}

/**
 * @brief Escreve (atualiza) o cabeçalho no início do arquivo.
 *
 * @param file      Ponteiro para o arquivo (já aberto).
 * @param cabecalho Ponteiro para os dados do cabeçalho a serem gravados.
 * @return int 1 em sucesso, 0 em falha.
 *
 * @pré file e cabecalho não podem ser NULL.
 * @pós cabeçalho no arquivo binário é sobrescrito pelos novos dados.
 */
int escreverCabecalho(FILE* file, Cabecalho* cabecalho) {
    if (!file || !cabecalho) return 0;
    rewind(file);
    int result = fwrite(cabecalho, sizeof(Cabecalho), 1, file) == 1;
    fflush(file);
    return result;
}

/**
 * @brief Imprime informações internas do cabeçalho (para depuração/estatísticas).
 *
 * @param cabecalho Ponteiro para os dados do cabeçalho.
 *
 * @pré cabecalho não pode ser NULL.
 * @pós Exibe no stdout as posições e contadores armazenados.
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
