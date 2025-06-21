#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carregamento.h"
#include "estruturas.h"
#include "livros.h"
#include "usuarios.h"

// ==================== FUNÇÕES DE CARREGAMENTO ====================

void removerEspacos(char* str) {
    int inicio = 0, fim = strlen(str) - 1;

    // Remove espaços do início
    while (str[inicio] == ' ' || str[inicio] == '\t') {
        inicio++;
    }

    // Remove espaços do fim
    while (fim >= inicio && (str[fim] == ' ' || str[fim] == '\t' || str[fim] == '\n' || str[fim] == '\r')) {
        fim--;
    }

    // Move string para o início
    int i;
    for (i = 0; i <= fim - inicio; i++) {
        str[i] = str[inicio + i];
    }
    str[i] = '\0';
}

int processarLivro(FILE* file, char* linha) {
    Livro livro;
    char* token;

    // Pula o "L;"
    token = strtok(linha, ";");

    // Código
    token = strtok(NULL, ";");
    if (!token) return 0;
    removerEspacos(token);
    livro.codigo = atoi(token);

    // Título
    token = strtok(NULL, ";");
    if (!token) return 0;
    removerEspacos(token);
    strncpy(livro.titulo, token, sizeof(livro.titulo) - 1);
    livro.titulo[sizeof(livro.titulo) - 1] = '\0';

    // Autor
    token = strtok(NULL, ";");
    if (!token) return 0;
    removerEspacos(token);
    strncpy(livro.autor, token, sizeof(livro.autor) - 1);
    livro.autor[sizeof(livro.autor) - 1] = '\0';

    // Editora
    token = strtok(NULL, ";");
    if (!token) return 0;
    removerEspacos(token);
    strncpy(livro.editora, token, sizeof(livro.editora) - 1);
    livro.editora[sizeof(livro.editora) - 1] = '\0';

    // Edição
    token = strtok(NULL, ";");
    if (!token) return 0;
    removerEspacos(token);
    livro.edicao = atoi(token);

    // Ano
    token = strtok(NULL, ";");
    if (!token) return 0;
    removerEspacos(token);
    livro.ano = atoi(token);

    // Exemplares
    token = strtok(NULL, ";");
    if (!token) return 0;
    removerEspacos(token);
    livro.exemplares = atoi(token);

    return cadastrarLivro(file, &livro);
}

int processarUsuario(FILE* file, char* linha) {
    Usuario usuario;
    char* token;

    // Pula o "U;"
    token = strtok(linha, ";");

    // Código
    token = strtok(NULL, ";");
    if (!token) return 0;
    removerEspacos(token);
    usuario.codigo = atoi(token);

    // Nome
    token = strtok(NULL, ";");
    if (!token) return 0;
    removerEspacos(token);
    strncpy(usuario.nome, token, sizeof(usuario.nome) - 1);
    usuario.nome[sizeof(usuario.nome) - 1] = '\0';

    return cadastrarUsuario(file, &usuario);
}

int carregarArquivo(FILE* file, char* nomeArquivoTexto) {
    FILE* arquivoTexto = fopen(nomeArquivoTexto, "r");
    if (!arquivoTexto) {
        printf("Erro ao abrir arquivo: %s\n", nomeArquivoTexto);
        return 0;
    }

    char linha[1000];
    int livros_carregados = 0, usuarios_carregados = 0, emprestimos_carregados = 0;

    printf("Carregando dados do arquivo %s...\n", nomeArquivoTexto);

    while (fgets(linha, sizeof(linha), arquivoTexto)) {
        // Remove quebra de linha
        linha[strcspn(linha, "\n")] = 0;

        // Pula linhas vazias
        if (strlen(linha) == 0) continue;

        // Identifica tipo de registro
        if (linha[0] == 'L' && linha[1] == ';') {
            if (processarLivro(file, linha)) {
                livros_carregados++;
            } else {
                printf("Erro ao processar livro: %s\n", linha);
            }
        } else if (linha[0] == 'U' && linha[1] == ';') {
            if (processarUsuario(file, linha)) {
                usuarios_carregados++;
            } else {
                printf("Erro ao processar usuário: %s\n", linha);
            }
        } else if (linha[0] == 'E' && linha[1] == ';') {
            // TODO: Implementar empréstimos
            printf("Empréstimo encontrado (não implementado ainda): %s\n", linha);
            emprestimos_carregados++;
        } else {
            printf("Linha inválida ignorada: %s\n", linha);
        }
    }

    fclose(arquivoTexto);

    printf("\n=== RESUMO DO CARREGAMENTO ===\n");
    printf("Livros carregados: %d\n", livros_carregados);
    printf("Usuários carregados: %d\n", usuarios_carregados);
    printf("Empréstimos encontrados: %d\n", emprestimos_carregados);
    printf("==============================\n\n");

    return 1;
}
