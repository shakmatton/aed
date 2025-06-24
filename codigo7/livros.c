/* livros.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "livros.h"
#include "arquivo.h"
#include "estruturas.h"

// Função de comparação case-insensitive portável
int string_compare_insensitive(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower(*a) != tolower(*b)) return 1;
        a++;
        b++;
    }
    return *a != *b;
}

/**
 * @brief Cadastra um novo livro no arquivo binário.
 * @param file  Ponteiro para o arquivo aberto.
 * @param livro Ponteiro para a estrutura com dados do livro.
 * @return int 1 em sucesso, 0 em falha ou duplicata.
 *
 * @pré file deve estar aberto e livro deve conter dados válidos.
 * @pós Se bem-sucedido, livro inserido no início da lista e cabeçalho atualizado.
 */
int cadastrarLivro(FILE* file, Livro* livro) {
    Cabecalho cabecalho;
    if (!lerCabecalho(file, &cabecalho)) {
        printf("Erro ao ler cabeçalho\n");
        return 0;
    }

    // Verifica duplicata de código
    int pos = cabecalho.pos_cabeca_livros;
    Livro aux;
    while (pos != -1) {
        fseek(file, pos, SEEK_SET);
        if (fread(&aux, sizeof(Livro), 1, file) != 1) {
            perror("Erro de leitura");
            return 0;
        }
        if (aux.codigo == livro->codigo) {
            printf("Erro: Livro com código %d já existe.\n", livro->codigo);
            return 0;
        }
        pos = aux.proxima_pos;
    }

    // Insere no arquivo
    fseek(file, cabecalho.pos_topo_livros, SEEK_SET);

    if (cabecalho.pos_cabeca_livros == -1) {
        // Primeira inserção
        cabecalho.pos_cabeca_livros = cabecalho.pos_topo_livros;
        livro->proxima_pos = -1;
    } else {
        // Insere no início da lista
        livro->proxima_pos = cabecalho.pos_cabeca_livros;
        cabecalho.pos_cabeca_livros = cabecalho.pos_topo_livros;
    }

    if (fwrite(livro, sizeof(Livro), 1, file) != 1) {
        perror("Erro ao escrever livro");
        return 0;
    }

    cabecalho.pos_topo_livros += sizeof(Livro);
    cabecalho.total_livros++;
    if (!escreverCabecalho(file, &cabecalho)) {
        printf("Erro ao atualizar cabeçalho\n");
        return 0;
    }

    printf("Livro cadastrado com sucesso!\n");
    return 1;
}

/**
 * @brief Busca um livro pelo código.
 * @param file   Ponteiro para arquivo.
 * @param codigo Código do livro a buscar.
 * @param livro  Ponteiro para receber dados do livro.
 * @return int 1 se encontrado, 0 caso contrário.
 *
 * @pré file aberto.
 * @pós livro preenchido se retorno for 1.
 */
int buscarLivroPorCodigo(FILE* file, int codigo, Livro* livro) {
    Cabecalho cabecalho;
    if (!lerCabecalho(file, &cabecalho)) return 0;

    long pos = cabecalho.pos_cabeca_livros;
    while (pos != -1) {
        fseek(file, pos, SEEK_SET);
        if (fread(livro, sizeof(Livro), 1, file) != 1) {
            perror("Erro de leitura");
            return 0;
        }
        if (livro->codigo == codigo) return 1;
        pos = livro->proxima_pos;
    }
    return 0;
}

/**
 * @brief Busca um livro pelo título (case-insensitive).
 * @param file   Ponteiro para arquivo.
 * @param titulo Título a buscar.
 * @param livro  Ponteiro para receber dados do livro.
 * @return int 1 se encontrado, 0 caso contrário.
 *
 * @pré file aberto.
 * @pós livro preenchido se retorno for 1.
 */
int buscarLivroPorTitulo(FILE* file, char* titulo, Livro* livro) {
    Cabecalho cabecalho;
    if (!lerCabecalho(file, &cabecalho)) return 0;

    long pos = cabecalho.pos_cabeca_livros;
    while (pos != -1) {
        fseek(file, pos, SEEK_SET);
        if (fread(livro, sizeof(Livro), 1, file) != 1) {
            perror("Erro de leitura");
            return 0;
        }
        if (string_compare_insensitive(livro->titulo, titulo) == 0) return 1;
        pos = livro->proxima_pos;
    }
    return 0;
}

/**
 * @brief Calcula e exibe total de livros e exemplares.
 * @param file Ponteiro para arquivo.
 *
 * @pré file aberto.
 * @pós exibe no stdout os totais.
 */
void calcularTotalLivros(FILE* file) {
    Cabecalho cabecalho;
    if (!lerCabecalho(file, &cabecalho)) {
        printf("Erro ao ler cabeçalho\n");
        return;
    }
    printf("Total de livros cadastrados: %d\n", cabecalho.total_livros);

    int total_exemplares = 0;
    Livro livro;
    long pos = cabecalho.pos_cabeca_livros;
    while (pos != -1) {
        fseek(file, pos, SEEK_SET);
        if (fread(&livro, sizeof(Livro), 1, file) != 1) {
            perror("Erro de leitura");
            break;
        }
        total_exemplares += livro.exemplares;
        pos = livro.proxima_pos;
    }
    printf("Total de exemplares na biblioteca: %d\n", total_exemplares);
}

/**
 * @brief Imprime os dados de um livro.
 * @param livro Ponteiro para o livro a ser impresso.
 *
 * @pré livro preenchido.
 * @pós exibe no stdout dados formatados.
 */
void imprimirLivro(Livro* livro) {
    printf("\n--- DADOS DO LIVRO ---\n");
    printf("Código: %d\n", livro->codigo);
    printf("Título: %s\n", livro->titulo);
    printf("Autor: %s\n", livro->autor);
    printf("Editora: %s\n", livro->editora);
    printf("Edição: %d\n", livro->edicao);
    printf("Ano: %d\n", livro->ano);
    printf("Exemplares: %d\n", livro->exemplares);
    printf("----------------------\n\n");
}

/**
 * @brief Lista todos os livros cadastrados.
 * @param file Ponteiro para arquivo.
 *
 * @pré file aberto.
 * @pós exibe no stdout lista de livros ou mensagem se vazio.
 */
void listarTodosLivros(FILE* file) {
    Cabecalho cabecalho;
    if (!lerCabecalho(file, &cabecalho)) {
        printf("Erro ao ler cabeçalho\n");
        return;
    }
    if (cabecalho.total_livros == 0) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }
    printf("\n=== LISTA DE TODOS OS LIVROS ===\n");

    Livro livro;
    long pos = cabecalho.pos_cabeca_livros;
    while (pos != -1) {
        fseek(file, pos, SEEK_SET);
        if (fread(&livro, sizeof(Livro), 1, file) != 1) {
            perror("Erro de leitura");
            break;
        }
        printf("Código: %d | Título: %s | Autor: %s | Exemplares: %d\n",
               livro.codigo, livro.titulo, livro.autor, livro.exemplares);
        pos = livro.proxima_pos;
    }
    printf("===============================\n\n");
}

// ==================== FUNÇÕES AUXILIARES PARA EMPRÉSTIMOS ====================

/**
 * @brief Verifica disponibilidade de um livro
 * @return Número de exemplares disponíveis, -1 se não encontrado
 */
int verificar_livro_disponivel(FILE* file, int codigo) {
    Livro livro;
    if (buscarLivroPorCodigo(file, codigo, &livro)) {
        return livro.exemplares;
    }
    return -1;
}

/**
 * @brief Atualiza exemplares de um livro
 * @param delta Valor a adicionar (pode ser negativo)
 * @return 1 em sucesso, 0 em falha
 */
int atualizar_exemplares_livro(FILE* file, int codigo, int delta) {
    Cabecalho cab;
    if (!lerCabecalho(file, &cab)) return 0;

    long pos = cab.pos_cabeca_livros;
    while (pos != -1) {
        Livro livro;
        fseek(file, pos, SEEK_SET);
        if (fread(&livro, sizeof(Livro), 1, file) != 1) {
            perror("Erro de leitura");
            return 0;
        }
        if (livro.codigo == codigo) {
            livro.exemplares += delta;
            fseek(file, pos, SEEK_SET);
            if (fwrite(&livro, sizeof(Livro), 1, file) != 1) {
                perror("Erro ao atualizar livro");
                return 0;
            }
            fflush(file);
            return 1;
        }
        pos = livro.proxima_pos;
    }
    return 0;
}

/**
 * @brief Obtém título de um livro
 * @return 1 em sucesso, 0 em falha
 */
int obter_titulo_livro(FILE* file, int codigo, char* titulo) {
    Livro livro;
    if (buscarLivroPorCodigo(file, codigo, &livro)) {
        strncpy(titulo, livro.titulo, 150);
        titulo[150] = '\0';
        return 1;
    }
    return 0;
}
