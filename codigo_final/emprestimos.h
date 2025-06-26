#ifndef EMPRESTIMOS_H
#define EMPRESTIMOS_H

#include "estruturas.h"

/**
 * Propósito: Registra um novo empréstimo de livro
 * Pré-condições: arquivo deve estar aberto, codigo_usuario e codigo_livro devem existir
 * Pós-condições: empréstimo registrado no arquivo, exemplares disponíveis decrementados
 */
int emprestar_livro(FILE* arquivo, int codigo_usuario, int codigo_livro, char data_emprestimo[], char data_devolucao[]);

/**
 * Propósito: Registra a devolução de um livro emprestado
 * Pré-condições: arquivo deve estar aberto, empréstimo deve existir
 * Pós-condições: data de devolução atualizada, exemplares disponíveis incrementados
 */
int devolver_livro(FILE *arquivo, int codigo_usuario, int codigo_livro);

/**
 * Propósito: Lista todos os livros atualmente emprestados (não devolvidos)
 * Pré-condições: arquivo deve estar aberto
 * Pós-condições: lista impressa na tela com código do usuário, nome, código do livro, título e data de empréstimo
 */
void listar_livros_emprestados(FILE *arquivo);

/**
 * Propósito: Verifica se um usuário existe no sistema
 * Pré-condições: arquivo deve estar aberto
 * Pós-condições: retorna 1 se usuário existe, 0 caso contrário
 */
int usuario_existe(FILE *arquivo, int codigo_usuario);

/**
 * Propósito: Obtém a data atual no formato DD/MM/AAAA
 * Pré-condições: buffer deve ter pelo menos 11 caracteres
 * Pós-condições: buffer preenchido com data atual
 */
void obter_data_atual(char *data);



#endif // EMPRESTIMOS_H