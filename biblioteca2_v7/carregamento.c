#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carregamento.h"
#include "estruturas.h"
#include "arquivo.h"
#include "arvore.h"
#include "utils.h"

/**
 * Propósito: Carrega livros de um arquivo texto
 * Pré-condições: arquivo deve estar no formato especificado
 * Pós-condições: livros válidos serão inseridos no sistema
 */
void carregar_arquivo() {
    char nome_arquivo[256];
    printf("\n=== CARREGAR ARQUIVO ===\n");
    printf("Nome do arquivo: ");
    scanf("%s", nome_arquivo);

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo %s\n", nome_arquivo);
        return;
    }

    char linha[1000];
    int livros_inseridos = 0;
    int linha_num = 0;

    printf("\nProcessando arquivo...\n");

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha_num++;

        // Remover quebra de linha
        linha[strcspn(linha, "\n")] = 0;

        if (strlen(linha) == 0) continue;

        Livro novo_livro;

        // Parsing da linha - usando strtok para dividir por ";"
        char *token = strtok(linha, ";");
        if (token == NULL) {
            printf("Linha %d: Formato inválido - código não encontrado\n", linha_num);
            continue;
        }

        trim_string(token);
        novo_livro.codigo = atoi(token);
        if (novo_livro.codigo <= 0) {
            printf("Linha %d: Código inválido (%s)\n", linha_num, token);
            continue;
        }

        token = strtok(NULL, ";");
        if (token == NULL) {
            printf("Linha %d: Formato inválido - título não encontrado\n", linha_num);
            continue;
        }
        trim_string(token);
        strncpy(novo_livro.titulo, token, MAX_TITULO);
        novo_livro.titulo[MAX_TITULO] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) {
            printf("Linha %d: Formato inválido - autor não encontrado\n", linha_num);
            continue;
        }
        trim_string(token);
        strncpy(novo_livro.autor, token, MAX_AUTOR);
        novo_livro.autor[MAX_AUTOR] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) {
            printf("Linha %d: Formato inválido - editora não encontrada\n", linha_num);
            continue;
        }
        trim_string(token);
        strncpy(novo_livro.editora, token, MAX_EDITORA);
        novo_livro.editora[MAX_EDITORA] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL) {
            printf("Linha %d: Formato inválido - edição não encontrada\n", linha_num);
            continue;
        }
        trim_string(token);
        novo_livro.edicao = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) {
            printf("Linha %d: Formato inválido - ano não encontrado\n", linha_num);
            continue;
        }
        trim_string(token);
        novo_livro.ano = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) {
            printf("Linha %d: Formato inválido - exemplares não encontrados\n", linha_num);
            continue;
        }
        trim_string(token);
        novo_livro.exemplares = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) {
            printf("Linha %d: Formato inválido - preço não encontrado\n", linha_num);
            continue;
        }
        trim_string(token);

        // Converter vírgula para ponto para o float (aceita ambos os formatos)
        for (int i = 0; token[i]; i++) {
            if (token[i] == ',') token[i] = '.';
        }
        novo_livro.preco = atof(token);

        if (novo_livro.preco < 0) {
            printf("Linha %d: Preço inválido (%s)\n", linha_num, token);
            continue;
        }

        // Verificar se código já existe
        if (buscar_livro(novo_livro.codigo) != -1) {
            printf("Linha %d: Código %d já existe, pulando...\n", linha_num, novo_livro.codigo);
            continue;
        }

        // Inicializar campos de controle
        novo_livro.esquerda = -1;
        novo_livro.direita = -1;
        novo_livro.livre = 0;
        novo_livro.proximo_livre = -1;

        // Inserir no arquivo
        int nova_pos = obter_nova_posicao();
        escrever_livro_posicao(nova_pos, &novo_livro);

        // Inserir na árvore
        inserir_na_arvore(novo_livro.codigo, nova_pos);

        // Incrementar contador
        CabecalhoArquivo cab;
        ler_cabecalho(&cab);
        cab.total_livros++;
        atualizar_cabecalho(&cab);

        livros_inseridos++;

        char preco_str[20];
        formatar_preco(novo_livro.preco, preco_str);

        printf("Linha %d: Livro '%s' (Código: %d, Preço: R$ %s) inserido com sucesso\n",
               linha_num, novo_livro.titulo, novo_livro.codigo, preco_str);
    }

    fclose(arquivo);
    printf("\n=== RESULTADO DO CARREGAMENTO ===\n");
    printf("Total de linhas processadas: %d\n", linha_num);
    printf("Livros inseridos com sucesso: %d\n", livros_inseridos);
    printf("Arquivo carregado!\n");
}
