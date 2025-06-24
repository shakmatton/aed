/* emprestimos.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estruturas.h"
#include "arquivo.h"
#include "usuarios.h"
#include "livros.h"
#include "emprestimos.h"

// ==================== FUNÇÕES DE EMPRÉSTIMOS ====================

/**
 * @brief Obtém a data atual no formato DD/MM/AAAA.
 *
 * @param data Buffer de ao menos 11 bytes para receber a string.
 * @pré data não é NULL e tem espaço suficiente.
 * @pós data preenchida com a data atual, terminada em '\0'.
 */
void obter_data_atual(char *data) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    sprintf(data, "%02d/%02d/%04d",
            tm_info->tm_mday,
            tm_info->tm_mon + 1,
            tm_info->tm_year + 1900);
}

/**
 * @brief Registra um empréstimo de livro.
 *
 * @param arquivo        Ponteiro para o arquivo aberto.
 * @param codigo_usuario Código do usuário.
 * @param codigo_livro   Código do livro.
 * @return int           1 em sucesso, 0 em falha.
 *
 * @pré arquivo aberto, usuário e livro existem.
 * @pós Empréstimo adicionado na lista e exemplares decrementados.
 */
int emprestar_livro(FILE *arquivo, int codigo_usuario, int codigo_livro) {
    // Verifica se usuário existe
    if (!usuario_existe(arquivo, codigo_usuario)) {
        printf("Erro: usuário %d não encontrado.\n", codigo_usuario);
        return 0;
    }

    // Busca o livro para verificar existência
    Livro livro;
    if (!buscarLivroPorCodigo(arquivo, codigo_livro, &livro)) {
        printf("Erro: livro %d não encontrado.\n", codigo_livro);
        return 0;
    }

    if (livro.exemplares <= 0) {
        printf("Não há exemplares disponíveis.\n");
        return 0;
    }

    Cabecalho cab;
    if (!lerCabecalho(arquivo, &cab)) return 0;

    // Prepara o empréstimo
    Emprestimo em;
    em.codigo_usuario = codigo_usuario;
    em.codigo_livro = codigo_livro;
    obter_data_atual(em.data_emprestimo);
    em.data_devolucao[0] = '\0'; // Empréstimo ativo

    // Configura ponteiros para inserir no início da lista
    em.proxima_pos = cab.pos_cabeca_emprestimos;

    // Escreve na área de empréstimos
    fseek(arquivo, cab.pos_topo_emprestimos, SEEK_SET);
    long pos_nova = ftell(arquivo);
    if (fwrite(&em, sizeof(Emprestimo), 1, arquivo) != 1) {
        perror("Erro ao escrever empréstimo");
        return 0;
    }

    // Atualiza cabeçalho: novo empréstimo é a nova cabeça e atualiza topo
    cab.pos_cabeca_emprestimos = pos_nova;
    cab.pos_topo_emprestimos = ftell(arquivo);
    cab.total_emprestimos++;
    if (!escreverCabecalho(arquivo, &cab)) {
        perror("Erro ao atualizar cabeçalho");
        return 0;
    }

    // Atualiza exemplares do livro
    if (!atualizar_exemplares_livro(arquivo, codigo_livro, -1)) {
        printf("Erro ao atualizar exemplares\n");
        return 0;
    }

    printf("Empréstimo realizado em %s.\n", em.data_emprestimo);
    return 1;
}

/**
 * @brief Registra a devolução de um livro emprestado.
 *
 * @param arquivo        Ponteiro para o arquivo aberto.
 * @param codigo_usuario Código do usuário.
 * @param codigo_livro   Código do livro.
 * @return int           1 em sucesso, 0 em falha.
 *
 * @pré arquivo aberto.
 * @pós Data de devolução preenchida e exemplares incrementados.
 */
int devolver_livro(FILE *arquivo, int codigo_usuario, int codigo_livro) {
    Cabecalho cab;
    if (!lerCabecalho(arquivo, &cab)) return 0;

    Emprestimo em;
    long pos = cab.pos_cabeca_emprestimos;
    int encontrado = 0;
    while (pos != -1) {
        fseek(arquivo, pos, SEEK_SET);
        if (fread(&em, sizeof(Emprestimo), 1, arquivo) != 1) {
            perror("Erro de leitura");
            break;
        }
        if (em.codigo_usuario == codigo_usuario &&
            em.codigo_livro == codigo_livro &&
            em.data_devolucao[0] == '\0') {

            // Marca como devolvido
            obter_data_atual(em.data_devolucao);
            fseek(arquivo, pos, SEEK_SET);
            if (fwrite(&em, sizeof(Emprestimo), 1, arquivo) != 1) {
                perror("Erro ao atualizar empréstimo");
                return 0;
            }
            encontrado = 1;
            break;
        }
        pos = em.proxima_pos;
    }

    if (!encontrado) {
        printf("Erro: empréstimo não encontrado ou já devolvido.\n");
        return 0;
    }

    // Atualiza exemplares do livro
    if (!atualizar_exemplares_livro(arquivo, codigo_livro, 1)) {
        printf("Erro ao atualizar exemplares\n");
        return 0;
    }

    printf("Devolução registrada em %s.\n", em.data_devolucao);
    return 1;
}

/**
 * @brief Lista todos os empréstimos ativos (não devolvidos).
 *
 * @param arquivo Ponteiro para o arquivo aberto.
 *
 * @pré arquivo aberto.
 * @pós Exibe na tela todos os empréstimos ativos formatados.
 */
void listar_livros_emprestados(FILE *arquivo) {
    Cabecalho cab;
    if (!lerCabecalho(arquivo, &cab)) return;

    Emprestimo em;
    char nome[51], titulo[151];
    printf("\n=== EMPRÉSTIMOS ATIVOS ===\n");
    printf("%-8s %-20s %-8s %-30s %-12s\n", "Cód.U", "Usuário", "Cód.L", "Título", "Empréstimo");
    printf("--------------------------------------------------------------------------------\n");

    long pos = cab.pos_cabeca_emprestimos;
    int encontrou = 0;
    while (pos != -1) {
        fseek(arquivo, pos, SEEK_SET);
        if (fread(&em, sizeof(Emprestimo), 1, arquivo) != 1) {
            perror("Erro de leitura");
            break;
        }
        if (em.data_devolucao[0] == '\0') {
            obter_nome_usuario(arquivo, em.codigo_usuario, nome);
            obter_titulo_livro(arquivo, em.codigo_livro, titulo);
            printf("%-8d %-20s %-8d %-30s %-12s\n",
                   em.codigo_usuario, nome, em.codigo_livro, titulo, em.data_emprestimo);
            encontrou = 1;
        }
        pos = em.proxima_pos;
    }
    if (!encontrou)
        printf("Nenhum empréstimo ativo.\n");

    printf("--------------------------------------------------------------------------------\n");
}
