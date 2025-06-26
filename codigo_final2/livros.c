#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "livros.h"
#include "arquivo.h"
#include "estruturas.h"

/**
 * Propósito: Compara duas strings ignorando maiúsculas/minúsculas
 * Pré-condição: ambas as strings devem ser válidas e terminadas em '\0'
 * Pós-condição: retorna 0 se forem iguais (ignorando caixa), 1 se diferentes
 */
int compara_string(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower(*a) != tolower(*b)) return 1;
        a++;
        b++;
    }
    return *a != *b;
}

/**
 * Propósito: Cadastra um novo livro no arquivo
 * Pré-condição: arquivo aberto e dados do livro válidos
 * Pós-condição: livro inserido no arquivo, cabeçalho atualizado
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
 * Propósito: Busca livro por código
 * Pré-condição: arquivo aberto
 * Pós-condição: retorna 1 se encontrado (livro carregado), 0 se não encontrado
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
 * Propósito: Busca livro por título
 * Pré-condição: arquivo aberto e título válido
 * Pós-condição: retorna 1 se encontrado (livro carregado), 0 se não encontrado
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
        if (compara_string(livro->titulo, titulo) == 0) return 1;
        pos = livro->proxima_pos;
    }
    return 0;
}

/**
 * Propósito: Calcula e exibe o total de livros cadastrados
 * Pré-condição: arquivo aberto
 * Pós-condição: total de livros e exemplares impressos na tela
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
 * Propósito: Imprime dados de um livro
 * Pré-condição: livro válido
 * Pós-condição: dados do livro impressos na tela
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
 * Propósito: Lista todos os livros cadastrados
 * Pré-condição: arquivo aberto
 * Pós-condição: todos os livros impressos na tela
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

/**
 * Propósito: Verifica a disponibilidade de exemplares de um livro
 * Pré-condição: arquivo aberto
 * Pós-condição: retorna número de exemplares disponíveis ou -1 se não encontrado
 */
int verificar_livro_disponivel(FILE* file, int codigo) {
    Livro livro;
    if (buscarLivroPorCodigo(file, codigo, &livro)) {
        return livro.exemplares;
    }
    return -1;
}

/**
 * Propósito: Atualiza a quantidade de exemplares de um livro
 * Pré-condição: arquivo aberto
 * Pós-condição: número de exemplares atualizado no arquivo, se encontrado
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
 * Propósito: Obtém o título de um livro dado seu código
 * Pré-condição: arquivo aberto
 * Pós-condição: título copiado para o ponteiro fornecido, retorna 1 se sucesso
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
