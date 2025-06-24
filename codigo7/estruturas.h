#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

// ==================== ESTRUTURAS ====================

/**
 * @brief Cabeçalho do arquivo binário, controla listas encadeadas e contadores.
 *
 * pos_cabeca_*: posição do primeiro registro de cada lista (-1 se vazia).
 * pos_topo_*  : próxima posição livre no arquivo (onde gravar o próximo registro).
 * total_*     : contadores de registros já gravados.
 */
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


/**
 * @brief Registro de um Livro.
 *
 * codigo       : identificador único do livro.
 * titulo, autor, editora: strings com tamanho fixo.
 * edicao, ano, exemplares: dados numéricos.
 * proxima_pos  : deslocamento para o próximo registro na lista (-1 se último).
 */
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


/**
 * @brief Registro de um Usuário.
 *
 * codigo     : identificador único do usuário.
 * nome       : string com tamanho máximo de 50 caracteres.
 * proxima_pos: deslocamento para o próximo registro na lista (-1 se último).
 */
typedef struct {
    int codigo;
    char nome[51];
    int proxima_pos;  // posição do próximo usuário na lista (-1 se for o último)
} Usuario;


/**
 * @brief Registro de um Empréstimo.
 *
 * codigo_usuario : código do usuário que tomou o livro.
 * codigo_livro   : código do livro emprestado.
 * data_emprestimo: string "DD/MM/AAAA" da data de empréstimo.
 * data_devolucao : string "DD/MM/AAAA" da data de devolução ("" se não houve).
 * proxima_pos    : deslocamento para o próximo registro na lista (-1 se último).
 */
typedef struct {
    int codigo_usuario;
    int codigo_livro;
    char data_emprestimo[11];
    char data_devolucao[11];
    int proxima_pos;  // posição do próximo empréstimo na lista (-1 se for o último)
} Emprestimo;

#endif // ESTRUTURAS_H
