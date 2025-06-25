/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"
#include "arquivo.h"
#include "livros.h"
#include "usuarios.h"
#include "carregamento.h"
#include "emprestimos.h"

// ==================== MENU E MAIN ====================

/**
 * @brief Exibe o menu principal de opções.
 */
void mostrarMenu() {
    printf("\n=== SISTEMA DE BIBLIOTECA ===\n\n");
    
    printf("Operações com Livros:\n\n");
    
    printf("1. Cadastrar livro\n");
    printf("2. Imprimir dados do livro\n");
    printf("3. Listar todos os livros\n");
    printf("4. Buscar livro por título\n");
    printf("5. Calcular total de livros\n\n");

    printf("Operações com Usuários e Empréstimos:\n\n");

    printf("6. Cadastrar usuário\n");
    printf("7. Emprestar livro\n");
    printf("8. Devolver livro\n");
    printf("9. Listar livros emprestados\n");
    printf("10. Carregar arquivo texto\n");
    //printf("11. Mostrar informações do arquivo (opção extra)\n");
    printf("\n0. Sair\n\n");
    printf("Escolha uma opção: ");
}

int main() {
    FILE* arquivo = abrirArquivoBinario("biblioteca.dat");
    if (!arquivo) {
        printf("Erro ao abrir arquivo da biblioteca!\n");
        return 1;
    }

    int opcao;
    Livro livro;
    Usuario usuario;
    int codigo, codigo_usuario, codigo_livro;
    char nomeArquivo[100];
    char titulo[151];

    do {
        mostrarMenu();
        if (scanf("%d", &opcao) != 1) {
            // Entrada inválida
            scanf("%*s");
            opcao = -1;
        }
        getchar(); // limpa o buffer

        switch (opcao) {
            case 1:
                printf("\n--- CADASTRAR LIVRO ---\n");
                printf("Código: ");
                scanf("%d", &livro.codigo);
                getchar();

                printf("Título: ");
                fgets(livro.titulo, sizeof(livro.titulo), stdin);
                livro.titulo[strcspn(livro.titulo, "\n")] = '\0';

                printf("Autor: ");
                fgets(livro.autor, sizeof(livro.autor), stdin);
                livro.autor[strcspn(livro.autor, "\n")] = '\0';

                printf("Editora: ");
                fgets(livro.editora, sizeof(livro.editora), stdin);
                livro.editora[strcspn(livro.editora, "\n")] = '\0';

                printf("Edição: ");
                scanf("%d", &livro.edicao);

                printf("Ano: ");
                scanf("%d", &livro.ano);

                printf("Exemplares: ");
                scanf("%d", &livro.exemplares);

                cadastrarLivro(arquivo, &livro);
                break;

            case 2:
                printf("\n--- IMPRIMIR DADOS DO LIVRO ---\n");
                printf("Digite o código do livro: ");
                scanf("%d", &codigo);

                if (buscarLivroPorCodigo(arquivo, codigo, &livro)) {
                    imprimirLivro(&livro);
                } else {
                    printf("Livro com código %d não encontrado.\n", codigo);
                }
                break;

            case 3:
                printf("\n--- LISTAR TODOS OS LIVROS ---\n");
                listarTodosLivros(arquivo);
                break;

            case 4:
                printf("\n--- BUSCAR LIVRO POR TÍTULO ---\n");
                printf("Digite o título do livro: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';

                if (buscarLivroPorTitulo(arquivo, titulo, &livro)) {
                    imprimirLivro(&livro);
                } else {
                    printf("Livro com título \"%s\" não encontrado.\n", titulo);
                }
                break;

            case 5:
                printf("\n--- TOTAL DE LIVROS CADASTRADOS ---\n");
                calcularTotalLivros(arquivo);
                break;

            case 6:
                printf("\n--- CADASTRAR USUÁRIO ---\n");
                printf("Código: ");
                scanf("%d", &usuario.codigo);
                getchar();

                printf("Nome: ");
                fgets(usuario.nome, sizeof(usuario.nome), stdin);
                usuario.nome[strcspn(usuario.nome, "\n")] = '\0';

                cadastrarUsuario(arquivo, &usuario);
                break;

            case 7:
                printf("\n--- EMPRESTAR LIVRO ---\n");
                printf("Código do usuário: ");
                scanf("%d", &codigo_usuario);

                printf("Código do livro: ");
                scanf("%d", &codigo_livro);

                emprestar_livro(arquivo, codigo_usuario, codigo_livro, "", "");
                break;

            case 8:
                printf("\n--- DEVOLVER LIVRO ---\n");
                printf("Código do usuário: ");
                scanf("%d", &codigo_usuario);

                printf("Código do livro: ");
                scanf("%d", &codigo_livro);

                devolver_livro(arquivo, codigo_usuario, codigo_livro);
                break;

            case 9:
                printf("\n--- TOTAL DE LIVROS EMPRESTADOS ---\n");
                listar_livros_emprestados(arquivo);
                break;

            case 10:
                printf("\n--- CARREGAR ARQUIVO ---\n");
                printf("Digite o nome do arquivo texto a ser carregado: ");
                fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
                nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0';

                carregarArquivo(arquivo, nomeArquivo);
                break;

            // descomentar a opção 11 em MostrarMenu() para exibí-la na tela
            case 11: {
                printf("\n--- MOSTRAR DADOS DO ARQUIVO BINÁRIO ---\n");
                Cabecalho cab;
                if (lerCabecalho(arquivo, &cab)) {
                    imprimirCabecalho(&cab);
                }
                break;
            }

            case 0:
                printf("Saindo do sistema...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    fclose(arquivo);
    return 0;
}
