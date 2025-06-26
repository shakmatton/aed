#ifndef CARREGAMENTO_H
#define CARREGAMENTO_H

#include <stdio.h>
#include "estruturas.h"

/**
 * Propósito: Remove espaços em branco do início e fim de uma string
 * Pré-condição: str deve ser uma string válida
 * Pós-condição: string modificada sem espaços nas extremidades
 */
void removerEspacos(char* str);

/**
 * Propósito: Processa uma linha de livro do arquivo texto
 * Pré-condição: file deve estar aberto, linha deve estar no formato correto
 * Pós-condição: livro cadastrado no arquivo, retorna 1 se sucesso
 */
int processarLivro(FILE* file, char* linha);

/**
 * Propósito: Processa uma linha de usuário do arquivo texto
 * Pré-condição: arquivo deve estar aberto, linha deve estar no formato correto
 * Pós-condição: usuário cadastrado no arquivo, retorna 1 se sucesso
 */
int processarUsuario(FILE* file, char* linha);

/**
 * Propósito: Processa uma linha de empréstimo do arquivo texto
 * Pré-condição: arquivo deve estar aberto, linha deve estar no formato correto
 * Pós-condição: empréstimo cadastrado no arquivo, retorna 1 se sucesso
 */
int processarEmprestimo(FILE* file, char* linha);

/**
 * Propósito: Carrega dados de um arquivo texto para o arquivo binário
 * Pré-condição: arquivo deve estar aberto, nomeArquivoTexto deve ser válido
 * Pós-condição: dados carregados no arquivo binário, estatísticas exibidas
 */
int carregarArquivo(FILE* file, char* nomeArquivoTexto);

/**
 * Propósito: Verifica se uma data é válida (formato DD/MM/AAAA)
 * Pré-condição: arquivo deve estar aberto, nomeArquivoTexto deve ser válido
 * Pós-condição:
 */
int verifica_data(char* data);

/**
 * Propósito: Compara duas datas (retorna -1, 0, 1 ou -2 para erro)
 * Pré-condição:
 * Pós-condição:
 */
int comparar_datas(char* data1, char* data2);

/**
 * Propósito: Valida intervalo entre datas de empréstimo e devolução
 * Pré-condição:
 * Pós-condição:
 */
int validar_intervalo_datas(char* data_emprestimo, char* data_devolucao);



#endif // CARREGAMENTO_H

