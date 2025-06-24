/* carregamento.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carregamento.h"
#include "estruturas.h"
#include "livros.h"
#include "usuarios.h"
#include "emprestimos.h"
#include "arquivo.h"

// ==================== FUNÇÕES DE CARREGAMENTO ====================

/**
 * @brief Remove espaços em branco no início e no fim de uma string.
 */
void removerEspacos(char* str) {
    int inicio = 0, fim = (int)strlen(str) - 1;
    while (str[inicio] == ' ' || str[inicio] == '\t') inicio++;
    while (fim >= inicio && (str[fim] == ' ' || str[fim] == '\t' || str[fim] == '\n' || str[fim] == '\r')) fim--;
    int i;
    for (i = 0; i <= fim - inicio; i++) {
        str[i] = str[inicio + i];
    }
    str[i] = '\0';
}

/**
 * @brief Processa uma linha de livro no arquivo de lote.
 */
int processarLivro(FILE* file, char* linha) {
    Livro livro;
    char* token;

    strtok(linha, ";"); // descarta "L;"

    // Campos obrigatórios
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); livro.codigo = atoi(token);
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); strncpy(livro.titulo, token, sizeof(livro.titulo)-1); livro.titulo[sizeof(livro.titulo)-1] = '\0';
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); strncpy(livro.autor, token, sizeof(livro.autor)-1); livro.autor[sizeof(livro.autor)-1] = '\0';
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); strncpy(livro.editora, token, sizeof(livro.editora)-1); livro.editora[sizeof(livro.editora)-1] = '\0';
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); livro.edicao = atoi(token);
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); livro.ano = atoi(token);
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); livro.exemplares = atoi(token);

    // Chama função comum (já trata duplicatas)
    return cadastrarLivro(file, &livro);
}

/**
 * @brief Processa uma linha de usuário no arquivo de lote.
 */
int processarUsuario(FILE* file, char* linha) {
    Usuario usuario;
    char* token;

    strtok(linha, ";"); // descarta "U;"

    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); usuario.codigo = atoi(token);
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); strncpy(usuario.nome, token, sizeof(usuario.nome)-1); usuario.nome[sizeof(usuario.nome)-1] = '\0';

    return cadastrarUsuario(file, &usuario);
}

/**
 * @brief Processa uma linha de empréstimo no arquivo de lote.
 *        Registra empréstimo no início da lista e atualiza exemplares.
 */
int processarEmprestimo(FILE* file, char* linha) {
    Emprestimo emprestimo;
    Cabecalho cabecalho;
    char* token;

    strtok(linha, ";"); // descarta "E;"

    // Extrai campos
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); emprestimo.codigo_usuario = atoi(token);
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); emprestimo.codigo_livro = atoi(token);
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); strncpy(emprestimo.data_emprestimo, token, sizeof(emprestimo.data_emprestimo)-1); emprestimo.data_emprestimo[sizeof(emprestimo.data_emprestimo)-1] = '\0';
    token = strtok(NULL, ";");
    if (token && strlen(token) > 0) {
        removerEspacos(token);
        strncpy(emprestimo.data_devolucao, token, sizeof(emprestimo.data_devolucao)-1);
        emprestimo.data_devolucao[sizeof(emprestimo.data_devolucao)-1] = '\0';
    } else {
        emprestimo.data_devolucao[0] = '\0'; // ativo
    }

    // Verifica se usuário e livro existem
    if (!usuario_existe(file, emprestimo.codigo_usuario)) {
        printf("Erro: Usuário %d não existe\n", emprestimo.codigo_usuario);
        return 0;
    }

    Livro livro;
    if (!buscarLivroPorCodigo(file, emprestimo.codigo_livro, &livro)) {
        printf("Erro: Livro %d não existe\n", emprestimo.codigo_livro);
        return 0;
    }

    // Lê cabeçalho atual
    fseek(file, 0, SEEK_SET);
    if (fread(&cabecalho, sizeof(Cabecalho), 1, file) != 1) {
        perror("Erro ao ler cabeçalho");
        return 0;
    }

    // Insere no início da lista de empréstimos
    emprestimo.proxima_pos = cabecalho.pos_cabeca_emprestimos;
    fseek(file, cabecalho.pos_topo_emprestimos, SEEK_SET);
    long nova_pos = ftell(file);
    if (fwrite(&emprestimo, sizeof(Emprestimo), 1, file) != 1) {
        perror("Erro ao escrever empréstimo");
        return 0;
    }

    // Atualiza cabeçalho
    cabecalho.pos_cabeca_emprestimos = nova_pos;
    cabecalho.pos_topo_emprestimos = ftell(file);
    cabecalho.total_emprestimos++;
    fseek(file, 0, SEEK_SET);
    if (fwrite(&cabecalho, sizeof(Cabecalho), 1, file) != 1) {
        perror("Erro ao atualizar cabeçalho");
        return 0;
    }
    fflush(file);

    // Atualiza exemplares se empréstimo ainda ativo
    if (emprestimo.data_devolucao[0] == '\0') {
        if (!atualizar_exemplares_livro(file, emprestimo.codigo_livro, -1)) {
            printf("Erro ao atualizar exemplares do livro %d\n", emprestimo.codigo_livro);
            return 0;
        }
    }

    return 1;
}

/**
 * @brief Lê e processa todo o arquivo de lote texto.
 */
int carregarArquivo(FILE* file, char* nomeArquivoTexto) {
    FILE* arquivoTexto = fopen(nomeArquivoTexto, "r");
    if (!arquivoTexto) {
        printf("Erro ao abrir arquivo: %s\n", nomeArquivoTexto);
        return 0;
    }

    char linha[1000];
    int l=0, u=0, e=0;
    printf("Carregando dados do arquivo %s...\n", nomeArquivoTexto);

    while (fgets(linha, sizeof(linha), arquivoTexto)) {
        linha[strcspn(linha, "\n")] = '\0';
        if (strlen(linha) == 0) continue;
        if (linha[0]=='L' && linha[1]==';') {
            if (processarLivro(file, linha)) l++;
        }
        else if (linha[0]=='U' && linha[1]==';') {
            if (processarUsuario(file, linha)) u++;
        }
        else if (linha[0]=='E' && linha[1]==';') {
            if (processarEmprestimo(file, linha)) e++;
        }
        else {
            printf("Linha inválida ignorada: %s\n", linha);
        }
    }
    fclose(arquivoTexto);

    printf("\n=== RESUMO DO CARREGAMENTO ===\n");
    printf("Livros: %d | Usuários: %d | Empréstimos: %d\n", l, u, e);
    printf("==============================\n\n");
    return 1;
}
