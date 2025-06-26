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
#include "carregamento.h"  // Para verifica_data e funções de validação

// ==================== FUNÇÕES DE EMPRÉSTIMOS ====================

void obter_data_atual(char* data) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    sprintf(data, "%02d/%02d/%04d",
            tm_info->tm_mday,
            tm_info->tm_mon + 1,
            tm_info->tm_year + 1900);
}

int emprestar_livro(FILE* arquivo, int codigo_usuario, int codigo_livro, char data_emprestimo[], char data_devolucao[]) {
    if (!usuario_existe(arquivo, codigo_usuario)) {
        printf("Erro: usuário %d não encontrado.\n", codigo_usuario);
        return 0;
    }

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

    Emprestimo em;
    em.codigo_usuario = codigo_usuario;
    em.codigo_livro = codigo_livro;

    // Tratamento de datas
    if (strlen(data_emprestimo) == 0) {
        obter_data_atual(em.data_emprestimo);
    }
    else if (verifica_data(data_emprestimo)) {
        strcpy(em.data_emprestimo, data_emprestimo);
    }
    else {
        printf("Data de empréstimo inválida.\n");
        return 0;
    }

    if (strlen(data_devolucao) == 0) {
        em.data_devolucao[0] = '\0';
    }
    else if (verifica_data(data_devolucao)) {
        strcpy(em.data_devolucao, data_devolucao);
    }
    else {
        printf("Data de devolução inválida.\n");
        return 0;
    }

    // Valida intervalo de datas
    if (strlen(data_devolucao) > 0 && !validar_intervalo_datas(em.data_emprestimo, em.data_devolucao)) {
        printf("Erro: Empréstimo (%s) posterior à devolução (%s)\n", em.data_emprestimo, em.data_devolucao);
        return 0;
    }

    // Configura ponteiros
    em.proxima_pos = cab.pos_cabeca_emprestimos;

    // Escreve no arquivo
    fseek(arquivo, cab.pos_topo_emprestimos, SEEK_SET);
    long pos_nova = ftell(arquivo);
    if (fwrite(&em, sizeof(Emprestimo), 1, arquivo) != 1) {
        perror("Erro ao escrever empréstimo");
        return 0;
    }

    // Atualiza cabeçalho
    cab.pos_cabeca_emprestimos = pos_nova;
    cab.pos_topo_emprestimos = ftell(arquivo);
    cab.total_emprestimos++;
    if (!escreverCabecalho(arquivo, &cab)) {
        perror("Erro ao atualizar cabeçalho");
        return 0;
    }

    // Atualiza exemplares se empréstimo ativo
    if (em.data_devolucao[0] == '\0') {
        if (!atualizar_exemplares_livro(arquivo, codigo_livro, -1)) {
            printf("Erro ao atualizar exemplares\n");
            return 0;
        }
    }

    printf("Empréstimo realizado em %s.\n", em.data_emprestimo);
    return 1;
}

int devolver_livro(FILE* arquivo, int codigo_usuario, int codigo_livro) {
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

    // Atualiza exemplares
    if (!atualizar_exemplares_livro(arquivo, codigo_livro, 1)) {
        printf("Erro ao atualizar exemplares\n");
        return 0;
    }

    printf("Devolução registrada em %s.\n", em.data_devolucao);
    return 1;
}

void listar_livros_emprestados(FILE* arquivo) {
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