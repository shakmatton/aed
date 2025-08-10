#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"
#include "arquivo.h"
#include "utils.h"

/**
 * Propósito: Busca um livro pelo código na árvore
 * Pré-condições: codigo deve ser um número válido
 * Pós-condições: retorna a posição do livro ou -1 se não encontrado
 */
int buscar_livro(int codigo) {
    CabecalhoArquivo cab;
    ler_cabecalho(&cab);

    int pos_atual = cab.raiz;

    while (pos_atual != -1) {
        Livro livro;
        if (!ler_livro_posicao(pos_atual, &livro) || livro.livre) {
            break;
        }

        if (codigo == livro.codigo) {
            return pos_atual;
        } else if (codigo < livro.codigo) {
            pos_atual = livro.esquerda;
        } else {
            pos_atual = livro.direita;
        }
    }

    return -1;
}

/**
 * Propósito: Insere um novo livro na árvore binária de busca
 * Pré-condições: codigo_novo deve ser único, pos_novo deve ser válido
 * Pós-condições: livro será inserido na posição correta da árvore
 */
void inserir_na_arvore(int codigo_novo, int pos_novo) {
    CabecalhoArquivo cab;
    ler_cabecalho(&cab);

    if (cab.raiz == -1) {
        // Primeira inserção - tornar raiz
        cab.raiz = pos_novo;
        atualizar_cabecalho(&cab);
        return;
    }

    int pos_atual = cab.raiz;

    while (1) {
        Livro livro_atual;
        ler_livro_posicao(pos_atual, &livro_atual);

        if (codigo_novo < livro_atual.codigo) {
            if (livro_atual.esquerda == -1) {
                livro_atual.esquerda = pos_novo;
                escrever_livro_posicao(pos_atual, &livro_atual);
                break;
            } else {
                pos_atual = livro_atual.esquerda;
            }
        } else {
            if (livro_atual.direita == -1) {
                livro_atual.direita = pos_novo;
                escrever_livro_posicao(pos_atual, &livro_atual);
                break;
            } else {
                pos_atual = livro_atual.direita;
            }
        }
    }
}

/**
 * Propósito: Encontra o menor elemento de uma subárvore
 * Pré-condições: pos deve ser uma posição válida
 * Pós-condições: retorna a posição do menor elemento
 */
int encontrar_menor(int pos) {
    if (pos == -1) return -1;

    Livro livro;
    ler_livro_posicao(pos, &livro);

    while (livro.esquerda != -1) {
        pos = livro.esquerda;
        ler_livro_posicao(pos, &livro);
    }

    return pos;
}

/**
 * Propósito: Remove um nó específico da árvore
 * Pré-condições: parâmetros devem ser válidos
 * Pós-condições: nó será removido e estrutura mantida
 */
int remover_no_arvore(int pos_atual, int pos_pai, int eh_filho_esquerdo, int codigo) {
    if (pos_atual == -1) return -1;

    Livro livro_atual;
    ler_livro_posicao(pos_atual, &livro_atual);

    if (livro_atual.codigo == codigo) {
        int nova_pos = -1;

        if (livro_atual.esquerda == -1 && livro_atual.direita == -1) {
            // Caso 1: Nó folha
            nova_pos = -1;
        } else if (livro_atual.esquerda == -1) {
            // Caso 2: Só tem filho direito
            nova_pos = livro_atual.direita;
        } else if (livro_atual.direita == -1) {
            // Caso 2: Só tem filho esquerdo
            nova_pos = livro_atual.esquerda;
        } else {
            // Caso 3: Tem dois filhos
            int pos_sucessor = encontrar_menor(livro_atual.direita);
            Livro sucessor;
            ler_livro_posicao(pos_sucessor, &sucessor);

            // Copiar dados do sucessor para o nó atual
            livro_atual.codigo = sucessor.codigo;
            strcpy(livro_atual.titulo, sucessor.titulo);
            strcpy(livro_atual.autor, sucessor.autor);
            strcpy(livro_atual.editora, sucessor.editora);
            livro_atual.edicao = sucessor.edicao;
            livro_atual.ano = sucessor.ano;
            livro_atual.exemplares = sucessor.exemplares;
            livro_atual.preco = sucessor.preco;

            escrever_livro_posicao(pos_atual, &livro_atual);

            // Remover o sucessor
            livro_atual.direita = remover_no_arvore(livro_atual.direita, pos_atual, 0, sucessor.codigo);
            escrever_livro_posicao(pos_atual, &livro_atual);
            return pos_atual;
        }

        // Atualizar pai ou raiz
        if (pos_pai == -1) {
            // Era raiz
            CabecalhoArquivo cab;
            ler_cabecalho(&cab);
            cab.raiz = nova_pos;
            atualizar_cabecalho(&cab);
        } else {
            Livro pai;
            ler_livro_posicao(pos_pai, &pai);
            if (eh_filho_esquerdo) {
                pai.esquerda = nova_pos;
            } else {
                pai.direita = nova_pos;
            }
            escrever_livro_posicao(pos_pai, &pai);
        }

        // Adicionar à lista de livres
        adicionar_registro_livre(pos_atual);

        return nova_pos;
    } else if (codigo < livro_atual.codigo) {
        livro_atual.esquerda = remover_no_arvore(livro_atual.esquerda, pos_atual, 1, codigo);
        escrever_livro_posicao(pos_atual, &livro_atual);
    } else {
        livro_atual.direita = remover_no_arvore(livro_atual.direita, pos_atual, 0, codigo);
        escrever_livro_posicao(pos_atual, &livro_atual);
    }

    return pos_atual;
}

/**
 * Propósito: Remove um livro da árvore
 * Pré-condições: codigo deve existir
 * Pós-condições: livro será removido
 */
void remover_da_arvore(int codigo) {
    CabecalhoArquivo cab;
    ler_cabecalho(&cab);

    cab.raiz = remover_no_arvore(cab.raiz, -1, 0, codigo);
}

/**
 * Propósito: Percorre a árvore em ordem e imprime os livros
 * Pré-condições: pos deve ser uma posição válida ou -1
 * Pós-condições: livros serão impressos em ordem crescente de código com preços em vírgula
 */
void percorrer_in_ordem(int pos) {
    if (pos == -1) return;

    Livro livro;
    if (!ler_livro_posicao(pos, &livro) || livro.livre) return;

    // Percorrer subárvore esquerda
    percorrer_in_ordem(livro.esquerda);

    // Processar nó atual - agora com vírgula no preço
    char preco_str[20];
    formatar_preco(livro.preco, preco_str);
    printf("Código: %d | Título: %s | Autor: %s | Exemplares: %d | Preço: R$ %s\n",
           livro.codigo, livro.titulo, livro.autor, livro.exemplares, preco_str);

    // Percorrer subárvore direita
    percorrer_in_ordem(livro.direita);
}

/**
 * Propósito: Calcula a altura da árvore
 * Pré-condições: pos deve ser uma posição válida ou -1
 * Pós-condições: retorna a altura da árvore
 */
int calcular_altura_arvore(int pos) {
    if (pos == -1) return 0;

    Livro livro;
    if (!ler_livro_posicao(pos, &livro) || livro.livre) return 0;

    int altura_esq = calcular_altura_arvore(livro.esquerda);
    int altura_dir = calcular_altura_arvore(livro.direita);

    return 1 + (altura_esq > altura_dir ? altura_esq : altura_dir);
}

/**
 * Propósito: Imprime nós de um nível específico da árvore
 * Pré-condições: pos deve ser uma posição válida ou -1, nivel >= 0
 * Pós-condições: nós do nível especificado serão impressos
 */
void imprimir_por_niveis_aux(int pos, int nivel) {
    if (pos == -1) return;

    Livro livro;
    if (!ler_livro_posicao(pos, &livro) || livro.livre) return;

    if (nivel == 0) {
        printf("%d\t", livro.codigo);
        return;
    }

    imprimir_por_niveis_aux(livro.esquerda, nivel - 1);
    imprimir_por_niveis_aux(livro.direita, nivel - 1);
}

/**
 * Propósito: Imprime a árvore por níveis
 * Pré-condições: nenhuma
 * Pós-condições: árvore será impressa nível por nível
 */
void imprimir_arvore_niveis() {
    printf("\n=== ÁRVORE POR NÍVEIS ===\n");

    CabecalhoArquivo cab;
    ler_cabecalho(&cab);

    if (cab.raiz == -1) {
        printf("Árvore vazia.\n");
        return;
    }

    int altura = calcular_altura_arvore(cab.raiz);

    for (int nivel = 0; nivel < altura; nivel++) {
		printf("\n");
        imprimir_por_niveis_aux(cab.raiz, nivel);
    }
}
