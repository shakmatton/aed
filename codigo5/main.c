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
void mostrarMenu() {
    printf("\n=== SISTEMA DE BIBLIOTECA ===\n");
    printf("1. Cadastrar livro\n");
    printf("2. Buscar livro por código\n");
    printf("3. Listar todos os livros\n");
    printf("4. Cadastrar usuário\n");
    printf("5. Emprestar livro\n");
    printf("6. Devolver livro\n");
    printf("7. Listar livros emprestados\n");
    printf("8. Mostrar informações do arquivo\n");
    printf("9. Carregar arquivo texto\n");
    printf("0. Sair\n");
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

    do {
        mostrarMenu();
        scanf("%d", &opcao);
        getchar(); // limpa o buffer

        switch (opcao) {
            case 1:
                printf("\n--- CADASTRAR LIVRO ---\n");
                printf("Código: ");
                scanf("%d", &livro.codigo);
                getchar();

                printf("Título: ");
                fgets(livro.titulo, sizeof(livro.titulo), stdin);
                livro.titulo[strcspn(livro.titulo, "\n")] = 0; // remove \n

                printf("Autor: ");
                fgets(livro.autor, sizeof(livro.autor), stdin);
                livro.autor[strcspn(livro.autor, "\n")] = 0;

                printf("Editora: ");
                fgets(livro.editora, sizeof(livro.editora), stdin);
                livro.editora[strcspn(livro.editora, "\n")] = 0;

                printf("Edição: ");
                scanf("%d", &livro.edicao);

                printf("Ano: ");
                scanf("%d", &livro.ano);

                printf("Exemplares: ");
                scanf("%d", &livro.exemplares);

                cadastrarLivro(arquivo, &livro);
                break;

            case 2:
                printf("\n--- BUSCAR LIVRO ---\n");
                printf("Digite o código do livro: ");
                scanf("%d", &codigo);

                if (buscarLivroPorCodigo(arquivo, codigo, &livro)) {
                    imprimirLivro(&livro);
                } else {
                    printf("Livro com código %d não encontrado.\n", codigo);
                }
                break;

            case 3:
                listarTodosLivros(arquivo);
                break;

            case 4:
                printf("\n--- CADASTRAR USUÁRIO ---\n");
                printf("Código: ");
                scanf("%d", &usuario.codigo);
                getchar();

                printf("Nome: ");
                fgets(usuario.nome, sizeof(usuario.nome), stdin);
                usuario.nome[strcspn(usuario.nome, "\n")] = 0; // remove \n

                cadastrarUsuario(arquivo, &usuario);
                break;

            case 5:
                printf("\n--- EMPRESTAR LIVRO ---\n");
                printf("Código do usuário: ");
                scanf("%d", &codigo_usuario);

                printf("Código do livro: ");
                scanf("%d", &codigo_livro);

                emprestar_livro(arquivo, codigo_usuario, codigo_livro);
                break;

            case 6:
                printf("\n--- DEVOLVER LIVRO ---\n");
                printf("Código do usuário: ");
                scanf("%d", &codigo_usuario);

                printf("Código do livro: ");
                scanf("%d", &codigo_livro);

                devolver_livro(arquivo, codigo_usuario, codigo_livro);
                break;

            case 7:
                listar_livros_emprestados(arquivo);
                break;

            case 8:
                {
                    Cabecalho cabecalho;
                    if (lerCabecalho(arquivo, &cabecalho)) {
                        imprimirCabecalho(&cabecalho);
                    }
                }
                break;

            case 9:
                printf("\n--- CARREGAR ARQUIVO ---\n");
                printf("Digite o nome do arquivo texto: ");
                fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
                nomeArquivo[strcspn(nomeArquivo, "\n")] = 0; // remove \n

                carregarArquivo(arquivo, nomeArquivo);
                break;

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