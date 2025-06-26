#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>
#include "estruturas.h"

// ==================== PROTÓTIPOS DAS FUNÇÕES DE ARQUIVO ====================

/**
 * Propósito: Abre arquivo binário para leitura/escrita, criando se não existir
 * Pré-condição: nome do arquivo válido
 * Pós-condição: retorna ponteiro para arquivo ou NULL se erro
 */
FILE* abrirArquivoBinario(char arquivo[]);

/**
 * Propósito: Lê o cabeçalho do arquivo
 * Pré-condição: arquivo aberto e válido
 * Pós-condição: retorna 1 se sucesso, 0 se erro
 */
int lerCabecalho(FILE* file, Cabecalho* cabecalho);

/**
 * Propósito: Escreve o cabeçalho no arquivo
 * Pré-condição: arquivo aberto e cabeçalho válido
 * Pós-condição: retorna 1 se sucesso, 0 se erro
 */
int escreverCabecalho(FILE* file, Cabecalho* cabecalho);

/**
 * Propósito: Imprime informações do cabeçalho (para debug)
 * Pré-condição: cabeçalho válido
 * Pós-condição: informações impressas na tela
 */
void imprimirCabecalho(Cabecalho* cabecalho);

#endif
