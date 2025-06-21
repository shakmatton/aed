#ifndef CARREGAMENTO_H
#define CARREGAMENTO_H

#include <stdio.h>

// ==================== PROTÓTIPOS DAS FUNÇÕES DE CARREGAMENTO ====================

/**
 * Propósito: Remove espaços em branco do início e fim de uma string
 * Pré-condição: string válida
 * Pós-condição: string sem espaços nas extremidades
 */
void removerEspacos(char* str);

/**
 * Propósito: Processa uma linha de livro do arquivo texto
 * Pré-condição: linha válida no formato L;codigo;titulo;autor;editora;edicao;ano;exemplares
 * Pós-condição: livro cadastrado no sistema
 */
int processarLivro(FILE* file, char* linha);

/**
 * Propósito: Processa uma linha de usuário do arquivo texto
 * Pré-condição: linha válida no formato U;codigo;nome
 * Pós-condição: usuário cadastrado no sistema
 */
int processarUsuario(FILE* file, char* linha);

/**
 * Propósito: Carrega dados de um arquivo texto para o sistema
 * Pré-condição: arquivo binário aberto, nome do arquivo texto válido
 * Pós-condição: dados carregados no sistema
 */
int carregarArquivo(FILE* file, char* nomeArquivoTexto);

#endif
