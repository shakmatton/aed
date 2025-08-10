#include <stdio.h>
#include <stdlib.h>
#include "arquivo.h"

/**
 * Propósito: Lê o cabeçalho do arquivo
 * Pré-condições: cab deve ser um ponteiro válido para CabecalhoArquivo
 * Pós-condições: cab conterá os dados do cabeçalho ou valores padrão se arquivo não existir
 */
int ler_cabecalho(CabecalhoArquivo *cab) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb");
    if (arquivo == NULL) {
        // Arquivo não existe, inicializar valores padrão
        cab->raiz = -1;
        cab->topo = 1; // posição 0 é reservada para o cabeçalho
        cab->cabeca_livres = -1;
        cab->total_livros = 0;
        return 0;
    }
    
    int resultado = fread(cab, sizeof(CabecalhoArquivo), 1, arquivo);
    fclose(arquivo);
    return resultado;
}

/**
 * Propósito: Atualiza o cabeçalho no arquivo
 * Pré-condições: cab deve conter dados válidos do cabeçalho
 * Pós-condições: cabeçalho será atualizado no arquivo
 */
void atualizar_cabecalho(CabecalhoArquivo *cab) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "r+b");
    if (arquivo == NULL) {
        arquivo = fopen(NOME_ARQUIVO, "wb");
    }
    
    if (arquivo != NULL) {
        fseek(arquivo, 0, SEEK_SET);
        fwrite(cab, sizeof(CabecalhoArquivo), 1, arquivo);
        fclose(arquivo);
    }
}

/**
 * Propósito: Lê um livro de uma posição específica do arquivo
 * Pré-condições: pos >= 1 e livro deve ser um ponteiro válido
 * Pós-condições: livro conterá os dados lidos ou retornará 0 se erro
 */
int ler_livro_posicao(int pos, Livro *livro) {
    if (pos < 1) return 0;
    
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb");
    if (arquivo == NULL) return 0;
    
    long offset = sizeof(CabecalhoArquivo) + (pos - 1) * sizeof(Livro);
    fseek(arquivo, offset, SEEK_SET);
    int resultado = fread(livro, sizeof(Livro), 1, arquivo);
    fclose(arquivo);
    
    return resultado;
}

/**
 * Propósito: Escreve um livro em uma posição específica do arquivo
 * Pré-condições: pos >= 1 e livro deve conter dados válidos
 * Pós-condições: livro será escrito na posição especificada
 */
int escrever_livro_posicao(int pos, Livro *livro) {
    if (pos < 1) return 0;
    
    FILE *arquivo = fopen(NOME_ARQUIVO, "r+b");
    if (arquivo == NULL) {
        arquivo = fopen(NOME_ARQUIVO, "wb");
        if (arquivo == NULL) return 0;
        
        // Escrever cabeçalho inicial se arquivo novo
        CabecalhoArquivo cab;
        ler_cabecalho(&cab);
        fwrite(&cab, sizeof(CabecalhoArquivo), 1, arquivo);
    }
    
    long offset = sizeof(CabecalhoArquivo) + (pos - 1) * sizeof(Livro);
    fseek(arquivo, offset, SEEK_SET);
    int resultado = fwrite(livro, sizeof(Livro), 1, arquivo);
    fclose(arquivo);
    
    return resultado;
}

/**
 * Propósito: Obtém uma nova posição para inserir um livro
 * Pré-condições: arquivo deve estar inicializado
 * Pós-condições: retorna uma posição livre disponível
 */
int obter_nova_posicao() {
    CabecalhoArquivo cab;
    ler_cabecalho(&cab);
    
    // Se há posições livres na lista, usar uma delas
    if (cab.cabeca_livres != -1) {
        Livro livro_livre;
        ler_livro_posicao(cab.cabeca_livres, &livro_livre);
        int pos_retorno = cab.cabeca_livres;
        cab.cabeca_livres = livro_livre.proximo_livre;
        atualizar_cabecalho(&cab);
        return pos_retorno;
    }
    
    // Caso contrário, usar o topo
    int nova_pos = cab.topo;
    cab.topo++;
    atualizar_cabecalho(&cab);
    return nova_pos;
}

/**
 * Propósito: Adiciona um registro à lista de registros livres
 * Pré-condições: pos deve ser uma posição válida
 * Pós-condições: posição será adicionada à lista de livres
 */
void adicionar_registro_livre(int pos) {
    CabecalhoArquivo cab;
    ler_cabecalho(&cab);
    
    Livro livro_livre;
    livro_livre.livre = 1;
    livro_livre.proximo_livre = cab.cabeca_livres;
    cab.cabeca_livres = pos;
    cab.total_livros--;
    
    escrever_livro_posicao(pos, &livro_livre);
    atualizar_cabecalho(&cab);
}

/**
 * Propósito: Inicializa o arquivo binário se não existir
 * Pré-condições: nenhuma
 * Pós-condições: arquivo será criado com cabeçalho inicial
 */
void inicializar_arquivo() {
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb");
    if (arquivo == NULL) {
        // Arquivo não existe, criar
        CabecalhoArquivo cab;
        cab.raiz = -1;
        cab.topo = 1;
        cab.cabeca_livres = -1;
        cab.total_livros = 0;
        atualizar_cabecalho(&cab);
    } else {
        fclose(arquivo);
    }
}
