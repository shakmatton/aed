#include <stdio.h>
#include <stdlib.h>
#include "livros.h"
#include "arquivo.h"

// ==================== FUNÇÕES DE LIVROS ====================

int cadastrarLivro(FILE* file, Livro* livro) {
    Cabecalho cabecalho;

    if (!lerCabecalho(file, &cabecalho)) {
        printf("Erro ao ler cabeçalho\n");
        return 0;
    }

    // Posiciona no final do arquivo para inserir
    fseek(file, cabecalho.pos_topo_livros, SEEK_SET);

    // Se é o primeiro livro da lista
    if (cabecalho.pos_cabeca_livros == -1) {
        cabecalho.pos_cabeca_livros = cabecalho.pos_topo_livros;
        livro->proxima_pos = -1;  // único livro na lista
    } else {
        // Insere no início da lista (mais simples)
        livro->proxima_pos = cabecalho.pos_cabeca_livros;
        cabecalho.pos_cabeca_livros = cabecalho.pos_topo_livros;
    }

    // Escreve o livro
    if (fwrite(livro, sizeof(Livro), 1, file) != 1) {
        printf("Erro ao escrever livro\n");
        return 0;
    }

    // Atualiza cabeçalho
    cabecalho.pos_topo_livros += sizeof(Livro);
    cabecalho.total_livros++;

    if (!escreverCabecalho(file, &cabecalho)) {
        printf("Erro ao atualizar cabeçalho\n");
        return 0;
    }

    printf("Livro cadastrado com sucesso!\n");
    return 1;
}

int buscarLivroPorCodigo(FILE* file, int codigo, Livro* livro) {
    Cabecalho cabecalho;

    if (!lerCabecalho(file, &cabecalho)) {
        return 0;
    }

    int pos_atual = cabecalho.pos_cabeca_livros;

    // Percorre a lista encadeada
    while (pos_atual != -1) {
        fseek(file, pos_atual, SEEK_SET);
        if (fread(livro, sizeof(Livro), 1, file) != 1) {
            return 0;
        }

        if (livro->codigo == codigo) {
            return 1;  // Encontrou!
        }

        pos_atual = livro->proxima_pos;  // Vai para o próximo
    }

    return 0;  // Não encontrou
}

void imprimirLivro(Livro* livro) {
    printf("\n--- DADOS DO LIVRO ---\n");
    printf("Código: %d\n", livro->codigo);
    printf("Título: %s\n", livro->titulo);
    printf("Autor: %s\n", livro->autor);
    printf("Editora: %s\n", livro->editora);
    printf("Edição: %d\n", livro->edicao);
    printf("Ano: %d\n", livro->ano);
    printf("Exemplares: %d\n", livro->exemplares);
    printf("--------------------\n\n");
}

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

    int pos_atual = cabecalho.pos_cabeca_livros;
    Livro livro;

    while (pos_atual != -1) {
        fseek(file, pos_atual, SEEK_SET);
        if (fread(&livro, sizeof(Livro), 1, file) != 1) {
            printf("Erro ao ler livro na posição %d\n", pos_atual);
            break;
        }

        printf("Código: %d | Título: %s | Autor: %s | Exemplares: %d\n",
               livro.codigo, livro.titulo, livro.autor, livro.exemplares);

        pos_atual = livro.proxima_pos;
    }

    printf("===============================\n\n");
}
