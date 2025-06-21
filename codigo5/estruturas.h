#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

// ==================== ESTRUTURAS ====================

// Cabeçalho do arquivo - armazena informações de controle
typedef struct {
    int pos_cabeca_livros;      // primeira posição da lista de livros (-1 se vazia)
    int pos_topo_livros;        // próxima posição livre para inserir livro
    int pos_cabeca_usuarios;    // primeira posição da lista de usuários (-1 se vazia)
    int pos_topo_usuarios;      // próxima posição livre para inserir usuário
    int pos_cabeca_emprestimos; // primeira posição da lista de empréstimos (-1 se vazia)
    int pos_topo_emprestimos;   // próxima posição livre para inserir empréstimo
    int total_livros;           // contador total de livros
    int total_usuarios;         // contador total de usuários
    int total_emprestimos;      // contador total de empréstimos
} Cabecalho;

// Estrutura do Livro
typedef struct {
    int codigo;
    char titulo[151];
    char autor[201];
    char editora[51];
    int edicao;
    int ano;
    int exemplares;
    int proxima_pos;  // posição do próximo livro na lista (-1 se for o último)
} Livro;

// Estrutura do Usuário
typedef struct {
    int codigo;
    char nome[51];
    int proxima_pos;  // posição do próximo usuário na lista (-1 se for o último)
} Usuario;

// Estrutura do Empréstimo
typedef struct {
    int codigo_usuario;
    int codigo_livro;
    char data_emprestimo[11];
    char data_devolucao[11];
    int proxima_pos;  // posição do próximo empréstimo na lista (-1 se for o último)
} Emprestimo;

#endif
