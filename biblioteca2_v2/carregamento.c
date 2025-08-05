#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carregamento.h"
#include "estruturas.h"
#include "arquivo.h"
#include "livros.h"

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
    
    while (fgets(linha, sizeof(linha), arquivo)) {
        linha_num++;
        
        // Remover quebra de linha
        linha[strcspn(linha, "\n")] = 0;
        
        if (strlen(linha) == 0) continue;
        
        Livro novo_livro;
        
        // Parsing da linha
        char *token = strtok(linha, ";");
        if (token == NULL) continue;
        
        trim_string(token);
        novo_livro.codigo = atoi(token);
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        trim_string(token);
        strncpy(novo_livro.titulo, token, MAX_TITULO);
        novo_livro.titulo[MAX_TITULO] = '\0';
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        trim_string(token);
        strncpy(novo_livro.autor, token, MAX_AUTOR);
        novo_livro.autor[MAX_AUTOR] = '\0';
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        trim_string(token);
        strncpy(novo_livro.editora, token, MAX_EDITORA);
        novo_livro.editora[MAX_EDITORA] = '\0';
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        trim_string(token);
        novo_livro.edicao = atoi(token);
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        trim_string(token);
        novo_livro.ano = atoi(token);
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        trim_string(token);
        novo_livro.exemplares = atoi(token);
        
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        trim_string(token);
        // Converter vírgula para ponto para o float
        for (int i = 0; token[i]; i++) {
            if (token[i] == ',') token[i] = '.';
        }
        novo_livro.preco = atof(token);
        
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
    }
    
    fclose(arquivo);
    printf("Arquivo carregado com sucesso! %d livros inseridos.\n", livros_inseridos);
}
