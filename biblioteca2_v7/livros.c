#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "livros.h"
#include "arquivo.h"
#include "arvore.h"
#include "utils.h"

/**
 * Propósito: Cadastra um novo livro no sistema
 * Pré-condições: nenhuma
 * Pós-condições: livro será inserido se código não existir
 */
void cadastrar_livro() {
    Livro novo_livro;

    printf("\n=== CADASTRAR LIVRO ===\n");

    // Leitura do código com validação
    novo_livro.codigo = read_int("Código: ");

    // Verificar se código já existe
    if (buscar_livro(novo_livro.codigo) != -1) {
        printf("Erro: Já existe um livro com código %d\n", novo_livro.codigo);
        return;
    }

    getchar(); // consumir \n restante do read_int

    printf("Título: ");
    fgets(novo_livro.titulo, sizeof(novo_livro.titulo), stdin);
    trim_string(novo_livro.titulo);

    printf("Autor: ");
    fgets(novo_livro.autor, sizeof(novo_livro.autor), stdin);
    trim_string(novo_livro.autor);

    printf("Editora: ");
    fgets(novo_livro.editora, sizeof(novo_livro.editora), stdin);
    trim_string(novo_livro.editora);

    novo_livro.edicao = read_int("Edição: ");
    novo_livro.ano = read_int("Ano: ");
    novo_livro.exemplares = read_int("Exemplares: ");

    // Limpar buffer antes de ler o preço
    getchar(); // consumir \n restante do read_int

    // Usar a nova função para ler o preço
    novo_livro.preco = read_float("Preço: ");

    // Inicializar campos de controle
    novo_livro.esquerda = -1;
    novo_livro.direita = -1;
    novo_livro.livre = 0;
    novo_livro.proximo_livre = -1;

    // Obter posição e inserir
    int nova_pos = obter_nova_posicao();
    escrever_livro_posicao(nova_pos, &novo_livro);

    // Inserir na árvore
    inserir_na_arvore(novo_livro.codigo, nova_pos);

    // Incrementar contador
    CabecalhoArquivo cab;
    ler_cabecalho(&cab);
    cab.total_livros++;
    atualizar_cabecalho(&cab);

    printf("Livro cadastrado com sucesso!\n");
}

/**
 * Propósito: Imprime os dados de um livro específico
 * Pré-condições: nenhuma
 * Pós-condições: dados do livro serão exibidos ou mensagem de erro
 */
void imprimir_dados_livro() {
    int codigo;
    printf("\n=== IMPRIMIR DADOS DO LIVRO ===\n");
    codigo = read_int("Código do livro: ");

    int pos = buscar_livro(codigo);
    if (pos == -1) {
        printf("Livro com código %d não encontrado.\n", codigo);
        return;
    }

    Livro livro;
    ler_livro_posicao(pos, &livro);

    char preco_str[20];
    formatar_preco(livro.preco, preco_str);

    printf("\n--- DADOS DO LIVRO ---\n");
    printf("Código: %d\n", livro.codigo);
    printf("Título: %s\n", livro.titulo);
    printf("Autor: %s\n", livro.autor);
    printf("Editora: %s\n", livro.editora);
    printf("Edição: %d\n", livro.edicao);
    printf("Ano: %d\n", livro.ano);
    printf("Exemplares: %d\n", livro.exemplares);
    printf("Preço: R$ %s\n", preco_str);
}

/**
 * Propósito: Lista todos os livros cadastrados
 * Pré-condições: nenhuma
 * Pós-condições: todos os livros serão listados em ordem crescente
 */
void listar_todos_livros() {
    printf("\n=== LISTA DE TODOS OS LIVROS ===\n");

    CabecalhoArquivo cab;
    ler_cabecalho(&cab);

    if (cab.raiz == -1) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }

    percorrer_in_ordem(cab.raiz);
}

/**
 * Propósito: Calcula e exibe o total de livros cadastrados
 * Pré-condições: nenhuma
 * Pós-condições: total de livros será exibido
 */
void calcular_total() {
    CabecalhoArquivo cab;
    ler_cabecalho(&cab);

    printf("\n=== TOTAL DE LIVROS ===\n");
    printf("Total de livros cadastrados: %d\n", cab.total_livros);
}

/**
 * Propósito: Remove um livro do sistema
 * Pré-condições: nenhuma
 * Pós-condições: livro será removido se código existir
 */
void remover_livro() {
    int codigo;
    printf("\n=== REMOVER LIVRO ===\n");
    codigo = read_int("Código do livro a remover: ");

    if (buscar_livro(codigo) == -1) {
        printf("Livro com código %d não encontrado.\n", codigo);
        return;
    }

    remover_da_arvore(codigo);
    printf("Livro removido com sucesso!\n");
}

/**
 * Propósito: Imprime a lista de registros livres
 * Pré-condições: nenhuma
 * Pós-condições: posições livres serão exibidas
 */
void imprimir_lista_livres() {
    printf("\n=== LISTA DE REGISTROS LIVRES ===\n");

    CabecalhoArquivo cab;
    ler_cabecalho(&cab);

    if (cab.cabeca_livres == -1) {
        printf("Não há registros livres.\n");
        return;
    }

    printf("Posições de registros livres: ");
    int pos_atual = cab.cabeca_livres;
    int flag=0;

    while (pos_atual != -1) {
        if(flag==1) printf(", ");
		else flag = 1;
        printf("%d", pos_atual);

        Livro livro_livre;
        if (!ler_livro_posicao(pos_atual, &livro_livre)) break;
        pos_atual = livro_livre.proximo_livre;
    }

    printf("\n");
}
