#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#define MAX_TITULO 150
#define MAX_AUTOR 200
#define MAX_EDITORA 50
#define NOME_ARQUIVO "livros.dat"

// Estrutura para armazenar informações do livro
typedef struct {
    int codigo;
    char titulo[MAX_TITULO + 1];
    char autor[MAX_AUTOR + 1];
    char editora[MAX_EDITORA + 1];
    int edicao;
    int ano;
    int exemplares;
    float preco;
    int esquerda;      // posição do filho esquerdo no arquivo
    int direita;       // posição do filho direito no arquivo
    int livre;         // indica se o registro está livre (1) ou ocupado (0)
    int proximo_livre; // próxima posição livre na lista encadeada
} Livro;

// Estrutura do cabeçalho do arquivo
typedef struct {
    int raiz;           // posição da raiz da árvore
    int topo;           // primeira posição livre no fim do arquivo
    int cabeca_livres;  // cabeça da lista de registros livres
    int total_livros;   // total de livros cadastrados
} CabecalhoArquivo;

#endif
