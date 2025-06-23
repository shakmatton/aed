#ifndef CARREGAMENTO_H
#define CARREGAMENTO_H

#include <stdio.h>
#include "estruturas.h"

/**
 * Propósito: Remove espaços em branco do início e fim de uma string
 * Pré-condições: str deve ser uma string válida
 * Pós-condições: string modificada sem espaços nas extremidades
 */
void removerEspacos(char* str);

/**
 * Propósito: Processa uma linha de livro do arquivo texto
 * Pré-condições: file deve estar aberto, linha deve estar no formato correto
 * Pós-condições: livro cadastrado no arquivo, retorna 1 se sucesso
 */
int processarLivro(FILE* file, char* linha);

/**
 * Propósito: Processa uma linha de usuário do arquivo texto
 * Pré-condições: file deve estar aberto, linha deve estar no formato correto
 * Pós-condições: usuário cadastrado no arquivo, retorna 1 se sucesso
 */
int processarUsuario(FILE* file, char* linha);

/**
 * Propósito: Processa uma linha de empréstimo do arquivo texto
 * Pré-condições: file deve estar aberto, linha deve estar no formato correto
 * Pós-condições: empréstimo cadastrado no arquivo, retorna 1 se sucesso
 */
int processarEmprestimo(FILE* file, char* linha);

/**
 * Propósito: Carrega dados de um arquivo texto para o arquivo binário
 * Pré-condições: file deve estar aberto, nomeArquivoTexto deve ser válido
 * Pós-condições: dados carregados no arquivo binário, estatísticas exibidas
 */
int carregarArquivo(FILE* file, char* nomeArquivoTexto);

#endif // CARREGAMENTO_H
