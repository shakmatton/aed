#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "estruturas.h"

// Protótipos das funções de arquivo
void inicializar_arquivo();
int ler_cabecalho(CabecalhoArquivo *cab);
void atualizar_cabecalho(CabecalhoArquivo *cab);
int ler_livro_posicao(int pos, Livro *livro);
int escrever_livro_posicao(int pos, Livro *livro);
int obter_nova_posicao();
void adicionar_registro_livre(int pos);

#endif
