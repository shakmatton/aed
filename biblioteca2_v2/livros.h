#ifndef LIVROS_H
#define LIVROS_H

#include "estruturas.h"

// Protótipos das funções de manipulação de livros
void cadastrar_livro();
void imprimir_dados_livro();
void listar_todos_livros();
void calcular_total();
void remover_livro();
void imprimir_lista_livres();
void imprimir_arvore_niveis();

// Funções auxiliares
void trim_string(char *str);
int buscar_livro(int codigo);
void inserir_na_arvore(int codigo_novo, int pos_novo);
void percorrer_in_ordem(int pos);
void remover_da_arvore(int codigo);
void imprimir_por_niveis_aux(int pos, int nivel);
int calcular_altura_arvore(int pos);
int encontrar_menor(int pos);
int remover_no_arvore(int pos_atual, int pos_pai, int eh_filho_esquerdo, int codigo);

#endif
