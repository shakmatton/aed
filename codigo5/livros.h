#ifndef LIVROS_H
#define LIVROS_H

#include <stdio.h>
#include "estruturas.h"

// ==================== PROTÓTIPOS DAS FUNÇÕES DE LIVROS ====================

/**
 * Propósito: Cadastra um novo livro no arquivo
 * Pré-condição: arquivo aberto e dados do livro válidos
 * Pós-condição: livro inserido no arquivo, cabeçalho atualizado
 */
int cadastrarLivro(FILE* file, Livro* livro);

/**
 * Propósito: Busca livro por código
 * Pré-condição: arquivo aberto e código válido
 * Pós-condição: retorna 1 se encontrado (livro carregado), 0 se não encontrado
 */
int buscarLivroPorCodigo(FILE* file, int codigo, Livro* livro);

/**
 * Propósito: Imprime dados de um livro
 * Pré-condição: livro válido
 * Pós-condição: dados do livro impressos na tela
 */
void imprimirLivro(Livro* livro);

/**
 * Propósito: Lista todos os livros cadastrados
 * Pré-condição: arquivo aberto
 * Pós-condição: todos os livros impressos na tela
 */
void listarTodosLivros(FILE* file);

#endif
