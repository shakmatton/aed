#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "emprestimos.h"
#include "arquivo.h"

/**
 * Propósito: Obtém a data atual no formato DD/MM/AAAA
 * Pré-condições: buffer deve ter pelo menos 11 caracteres
 * Pós-condições: buffer preenchido com data atual
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
 * Propósito: Verifica se um usuário existe no sistema
 * Pré-condições: arquivo deve estar aberto
 * Pós-condições: retorna 1 se usuário existe, 0 caso contrário
 */
int usuario_existe(FILE *arquivo, int codigo_usuario) {
    Cabecalho cabecalho;
    Usuario usuario;
    long pos_atual;

    // Lê o cabeçalho
    fseek(arquivo, 0, SEEK_SET);
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    // Percorre a lista de usuários
    pos_atual = cabecalho.pos_cabeca_usuarios;

    while (pos_atual != -1) {
        fseek(arquivo, pos_atual, SEEK_SET);
        fread(&usuario, sizeof(Usuario), 1, arquivo);

        if (usuario.codigo == codigo_usuario) {
            return 1; // Usuário encontrado
        }

        pos_atual = usuario.proxima_pos;
    }

    return 0; // Usuário não encontrado
}

/**
 * Propósito: Verifica se um livro existe e tem exemplares disponíveis
 * Pré-condições: arquivo deve estar aberto
 * Pós-condições: retorna número de exemplares disponíveis, -1 se livro não existe
 */
int verificar_livro_disponivel(FILE *arquivo, int codigo_livro) {
    Cabecalho cabecalho;
    Livro livro;
    long pos_atual;

    // Lê o cabeçalho
    fseek(arquivo, 0, SEEK_SET);
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    // Percorre a lista de livros
    pos_atual = cabecalho.pos_cabeca_livros;

    while (pos_atual != -1) {
        fseek(arquivo, pos_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);

        if (livro.codigo == codigo_livro) {
            return livro.exemplares; // Retorna número de exemplares
        }

        pos_atual = livro.proxima_pos;
    }

    return -1; // Livro não encontrado
}

/**
 * Propósito: Atualiza o número de exemplares de um livro
 * Pré-condições: arquivo deve estar aberto, livro deve existir
 * Pós-condições: número de exemplares atualizado no arquivo
 */
int atualizar_exemplares_livro(FILE *arquivo, int codigo_livro, int incremento) {
    Cabecalho cabecalho;
    Livro livro;
    long pos_atual;

    // Lê o cabeçalho
    fseek(arquivo, 0, SEEK_SET);
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    // Percorre a lista de livros
    pos_atual = cabecalho.pos_cabeca_livros;

    while (pos_atual != -1) {
        fseek(arquivo, pos_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);

        if (livro.codigo == codigo_livro) {
            livro.exemplares += incremento;

            // Reescreve o livro no arquivo
            fseek(arquivo, pos_atual, SEEK_SET);
            fwrite(&livro, sizeof(Livro), 1, arquivo);

            return 1; // Sucesso
        }

        pos_atual = livro.proxima_pos;
    }

    return 0; // Livro não encontrado
}

/**
 * Propósito: Registra um novo empréstimo de livro
 * Pré-condições: arquivo deve estar aberto, codigo_usuario e codigo_livro devem existir
 * Pós-condições: empréstimo registrado no arquivo, exemplares disponíveis decrementados
 */
int emprestar_livro(FILE *arquivo, int codigo_usuario, int codigo_livro) {
    Cabecalho cabecalho;
    Emprestimo emprestimo;
    int exemplares_disponiveis;

    // Verifica se o usuário existe
    if (!usuario_existe(arquivo, codigo_usuario)) {
        printf("Erro: Usuário com código %d não encontrado.\n", codigo_usuario);
        return 0;
    }

    // Verifica se o livro existe e tem exemplares disponíveis
    exemplares_disponiveis = verificar_livro_disponivel(arquivo, codigo_livro);
    if (exemplares_disponiveis == -1) {
        printf("Erro: Livro com código %d não encontrado.\n", codigo_livro);
        return 0;
    }

    if (exemplares_disponiveis <= 0) {
        printf("Não há exemplares disponíveis para empréstimo.\n");
        return 0;
    }

    // Lê o cabeçalho
    fseek(arquivo, 0, SEEK_SET);
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    // Preenche os dados do empréstimo
    emprestimo.codigo_usuario = codigo_usuario;
    emprestimo.codigo_livro = codigo_livro;
    obter_data_atual(emprestimo.data_emprestimo);
    strcpy(emprestimo.data_devolucao, ""); // Vazio indica que não foi devolvido
    emprestimo.proxima_pos = cabecalho.pos_cabeca_emprestimos;

    // Posiciona no final do arquivo
    fseek(arquivo, cabecalho.pos_topo_emprestimos, SEEK_SET);
    long nova_posicao = ftell(arquivo);

    // Escreve o empréstimo
    fwrite(&emprestimo, sizeof(Emprestimo), 1, arquivo);

    // Atualiza o cabeçalho
    cabecalho.pos_cabeca_emprestimos = nova_posicao;
    cabecalho.pos_topo_emprestimos = ftell(arquivo);
    cabecalho.total_emprestimos++;

    fseek(arquivo, 0, SEEK_SET);
    fwrite(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    // Decrementa o número de exemplares do livro
    atualizar_exemplares_livro(arquivo, codigo_livro, -1);

    printf("Empréstimo registrado com sucesso!\n");
    printf("Data do empréstimo: %s\n", emprestimo.data_emprestimo);

    return 1;
}

/**
 * Propósito: Registra a devolução de um livro emprestado
 * Pré-condições: arquivo deve estar aberto, empréstimo deve existir
 * Pós-condições: data de devolução atualizada, exemplares disponíveis incrementados
 */
int devolver_livro(FILE *arquivo, int codigo_usuario, int codigo_livro) {
    Cabecalho cabecalho;
    Emprestimo emprestimo;
    long pos_atual;

    // Lê o cabeçalho
    fseek(arquivo, 0, SEEK_SET);
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    // Percorre a lista de empréstimos
    pos_atual = cabecalho.pos_cabeca_emprestimos;

    while (pos_atual != -1) {
        fseek(arquivo, pos_atual, SEEK_SET);
        fread(&emprestimo, sizeof(Emprestimo), 1, arquivo);

        // Verifica se é o empréstimo procurado e se ainda não foi devolvido
        if (emprestimo.codigo_usuario == codigo_usuario &&
            emprestimo.codigo_livro == codigo_livro &&
            strlen(emprestimo.data_devolucao) == 0) {

            // Registra a data de devolução
            obter_data_atual(emprestimo.data_devolucao);

            // Reescreve o empréstimo no arquivo
            fseek(arquivo, pos_atual, SEEK_SET);
            fwrite(&emprestimo, sizeof(Emprestimo), 1, arquivo);

            // Incrementa o número de exemplares do livro
            atualizar_exemplares_livro(arquivo, codigo_livro, 1);

            printf("Devolução registrada com sucesso!\n");
            printf("Data da devolução: %s\n", emprestimo.data_devolucao);

            return 1;
        }

        pos_atual = emprestimo.proxima_pos;
    }

    printf("Erro: Empréstimo não encontrado ou livro já foi devolvido.\n");
    return 0;
}

/**
 * Propósito: Obtém o nome de um usuário pelo código
 * Pré-condições: arquivo deve estar aberto
 * Pós-condições: nome copiado para buffer, retorna 1 se encontrado
 */
int obter_nome_usuario(FILE *arquivo, int codigo_usuario, char *nome) {
    Cabecalho cabecalho;
    Usuario usuario;
    long pos_atual;

    // Lê o cabeçalho
    fseek(arquivo, 0, SEEK_SET);
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    // Percorre a lista de usuários
    pos_atual = cabecalho.pos_cabeca_usuarios;

    while (pos_atual != -1) {
        fseek(arquivo, pos_atual, SEEK_SET);
        fread(&usuario, sizeof(Usuario), 1, arquivo);

        if (usuario.codigo == codigo_usuario) {
            strcpy(nome, usuario.nome);
            return 1;
        }

        pos_atual = usuario.proxima_pos;
    }

    strcpy(nome, "Usuário não encontrado");
    return 0;
}

/**
 * Propósito: Obtém o título de um livro pelo código
 * Pré-condições: arquivo deve estar aberto
 * Pós-condições: título copiado para buffer, retorna 1 se encontrado
 */
int obter_titulo_livro(FILE *arquivo, int codigo_livro, char *titulo) {
    Cabecalho cabecalho;
    Livro livro;
    long pos_atual;

    // Lê o cabeçalho
    fseek(arquivo, 0, SEEK_SET);
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    // Percorre a lista de livros
    pos_atual = cabecalho.pos_cabeca_livros;

    while (pos_atual != -1) {
        fseek(arquivo, pos_atual, SEEK_SET);
        fread(&livro, sizeof(Livro), 1, arquivo);

        if (livro.codigo == codigo_livro) {
            strcpy(titulo, livro.titulo);
            return 1;
        }

        pos_atual = livro.proxima_pos;
    }

    strcpy(titulo, "Livro não encontrado");
    return 0;
}

/**
 * Propósito: Lista todos os livros atualmente emprestados (não devolvidos)
 * Pré-condições: arquivo deve estar aberto
 * Pós-condições: lista impressa na tela com código do usuário, nome, código do livro, título e data de empréstimo
 */
void listar_livros_emprestados(FILE *arquivo) {
    Cabecalho cabecalho;
    Emprestimo emprestimo;
    long pos_atual;
    char nome_usuario[51];
    char titulo_livro[151];
    int encontrou_emprestimos = 0;

    // Lê o cabeçalho
    fseek(arquivo, 0, SEEK_SET);
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo);

    printf("\n=== LIVROS EMPRESTADOS (NÃO DEVOLVIDOS) ===\n");
    printf("%-8s %-20s %-8s %-30s %-12s\n",
           "Cód.User", "Nome Usuário", "Cód.Livro", "Título", "Data Empr.");
    printf("------------------------------------------------------------------------\n");

    // Percorre a lista de empréstimos
    pos_atual = cabecalho.pos_cabeca_emprestimos;

    while (pos_atual != -1) {
        fseek(arquivo, pos_atual, SEEK_SET);
        fread(&emprestimo, sizeof(Emprestimo), 1, arquivo);

        // Verifica se o livro ainda não foi devolvido
        if (strlen(emprestimo.data_devolucao) == 0) {
            // Obtém nome do usuário e título do livro
            obter_nome_usuario(arquivo, emprestimo.codigo_usuario, nome_usuario);
            obter_titulo_livro(arquivo, emprestimo.codigo_livro, titulo_livro);

            // Trunca strings se necessário para exibição
            if (strlen(nome_usuario) > 20) {
                nome_usuario[17] = '.';
                nome_usuario[18] = '.';
                nome_usuario[19] = '.';
                nome_usuario[20] = '\0';
            }

            if (strlen(titulo_livro) > 30) {
                titulo_livro[27] = '.';
                titulo_livro[28] = '.';
                titulo_livro[29] = '.';
                titulo_livro[30] = '\0';
            }

            printf("%-8d %-20s %-8d %-30s %-12s\n",
                   emprestimo.codigo_usuario,
                   nome_usuario,
                   emprestimo.codigo_livro,
                   titulo_livro,
                   emprestimo.data_emprestimo);

            encontrou_emprestimos = 1;
        }

        pos_atual = emprestimo.proxima_pos;
    }

    if (!encontrou_emprestimos) {
        printf("Nenhum livro emprestado no momento.\n");
    }

    printf("------------------------------------------------------------------------\n");
}