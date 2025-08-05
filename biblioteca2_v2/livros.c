#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "livros.h"
#include "arquivo.h"

/**
 * Propósito: Remove espaços em branco do início e fim de uma string
 * Pré-condições: str deve ser uma string válida
 * Pós-condições: str terá espaços removidos do início e fim
 */
void trim_string(char *str) {
    if (str == NULL) return;
    
    // Remove espaços do final
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }
    
    // Remove espaços do início
    int inicio = 0;
    while (str[inicio] && isspace(str[inicio])) {
        inicio++;
    }
    
    if (inicio > 0) {
        int i = 0;
        while (str[inicio + i]) {
            str[i] = str[inicio + i];
            i++;
        }
        str[i] = '\0';
    }
}

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
 * Propósito: Cadastra um novo livro no sistema
 * Pré-condições: nenhuma
 * Pós-condições: livro será inserido se código não existir
 */
void cadastrar_livro() {
    Livro novo_livro;
    
    printf("\n=== CADASTRAR LIVRO ===\n");
    printf("Código: ");
    scanf("%d", &novo_livro.codigo);
    getchar(); // consumir \n
    
    // Verificar se código já existe
    if (buscar_livro(novo_livro.codigo) != -1) {
        printf("Erro: Já existe um livro com código %d\n", novo_livro.codigo);
        return;
    }
    
    printf("Título: ");
    fgets(novo_livro.titulo, sizeof(novo_livro.titulo), stdin);
    trim_string(novo_livro.titulo);
    
    printf("Autor: ");
    fgets(novo_livro.autor, sizeof(novo_livro.autor), stdin);
    trim_string(novo_livro.autor);
    
    printf("Editora: ");
    fgets(novo_livro.editora, sizeof(novo_livro.editora), stdin);
    trim_string(novo_livro.editora);
    
    printf("Edição: ");
    scanf("%d", &novo_livro.edicao);
    
    printf("Ano: ");
    scanf("%d", &novo_livro.ano);
    
    printf("Exemplares: ");
    scanf("%d", &novo_livro.exemplares);
    
    printf("Preço: ");
    scanf("%f", &novo_livro.preco);
    
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
    printf("Código do livro: ");
    scanf("%d", &codigo);
    
    int pos = buscar_livro(codigo);
    if (pos == -1) {
        printf("Livro com código %d não encontrado.\n", codigo);
        return;
    }
    
    Livro livro;
    ler_livro_posicao(pos, &livro);
    
    printf("\n--- DADOS DO LIVRO ---\n");
    printf("Código: %d\n", livro.codigo);
    printf("Título: %s\n", livro.titulo);
    printf("Autor: %s\n", livro.autor);
    printf("Editora: %s\n", livro.editora);
    printf("Edição: %d\n", livro.edicao);
    printf("Ano: %d\n", livro.ano);
    printf("Exemplares: %d\n", livro.exemplares);
    printf("Preço: R$ %.2f\n", livro.preco);
}

/**
 * Propósito: Percorre a árvore em ordem e imprime os livros
 * Pré-condições: pos deve ser uma posição válida ou -1
 * Pós-condições: livros serão impressos em ordem crescente de código
 */
void percorrer_in_ordem(int pos) {
    if (pos == -1) return;
    
    Livro livro;
    if (!ler_livro_posicao(pos, &livro) || livro.livre) return;
    
    // Percorrer subárvore esquerda
    percorrer_in_ordem(livro.esquerda);
    
    // Processar nó atual
    printf("Código: %d | Título: %s | Autor: %s | Exemplares: %d\n", 
           livro.codigo, livro.titulo, livro.autor, livro.exemplares);
    
    // Percorrer subárvore direita
    percorrer_in_ordem(livro.direita);
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
    cab.total_livros--;
    atualizar_cabecalho(&cab);
}

/**
 * Propósito: Remove um livro do sistema
 * Pré-condições: nenhuma
 * Pós-condições: livro será removido se código existir
 */
void remover_livro() {
    int codigo;
    printf("\n=== REMOVER LIVRO ===\n");
    printf("Código do livro a remover: ");
    scanf("%d", &codigo);
    
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
    
    while (pos_atual != -1) {
        printf("%d ", pos_atual);
        
        Livro livro_livre;
        if (!ler_livro_posicao(pos_atual, &livro_livre)) break;
        pos_atual = livro_livre.proximo_livre;
    }
    
    printf("\n");
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
        printf("%d\n", livro.codigo);
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
        imprimir_por_niveis_aux(cab.raiz, nivel);
    }
}
