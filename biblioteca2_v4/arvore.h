#ifndef ARVORE_H
#define ARVORE_H

#include "estruturas.h"

// Operações básicas da árvore
int buscar_livro(int codigo);
void inserir_na_arvore(int codigo_novo, int pos_novo);
void remover_da_arvore(int codigo);

// Percorrimentos da árvore
void percorrer_in_ordem(int pos);
void imprimir_arvore_niveis();

// Funções auxiliares da árvore
int calcular_altura_arvore(int pos);
int encontrar_menor(int pos);
int remover_no_arvore(int pos_atual, int pos_pai, int eh_filho_esquerdo, int codigo);
void imprimir_por_niveis_aux(int pos, int nivel);

#endif
