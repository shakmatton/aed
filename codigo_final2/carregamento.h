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

/**
 * Propósito: Verifica se uma data está no formato DD/MM/AAAA e é válida no calendário.
 * Pré-condições: A string deve estar nesse formato, com valores numéricos válidos.
 * Pós-condições: Retorna 1 se a data for válida, 0 caso contrário.
 */
int verifica_data(const char* data);

/**
 * Propósito: Compara duas datas no formato DD/MM/AAAA.
 * Pré-condições: Ambas devem ser strings válidas nesse formato.
 * Pós-condições: Retorna -1 se data1 < data2, 0 se iguais, 1 se data1 > data2, -2 se qualquer data for inválida.
 */
int comparar_datas(const char* data1, const char* data2);

/**
 * Propósito: Verifica se a data de devolução não é anterior à data de empréstimo.
 * Pré-condições: Ambas as datas devem estar no formato DD/MM/AAAA (ou devolução vazia).
 * Pós-condições: Retorna 1 se o intervalo for válido ou devolução estiver em branco, 0 se inválido.
 */
int validar_intervalo_datas(const char* data_emprestimo, const char* data_devolucao);

#endif

