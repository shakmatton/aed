#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuarios.h"
#include "arquivo.h"
#include "estruturas.h"

/**
 * Propósito: Cadastra um novo usuário no arquivo
 * Pré-condição: arquivo aberto e dados do usuário válidos
 * Pós-condição: usuário inserido no arquivo, cabeçalho atualizado.
 */
int cadastrarUsuario(FILE* file, Usuario* usuario) {
    Cabecalho cabecalho;
    if (!lerCabecalho(file, &cabecalho)) {
        printf("Erro ao ler cabeçalho\n");
        return 0;
    }

    // Verifica duplicata de código de usuário
    long pos = cabecalho.pos_cabeca_usuarios;
    Usuario aux;
    while (pos != -1) {
        fseek(file, pos, SEEK_SET);
        if (fread(&aux, sizeof(Usuario), 1, file) != 1) {
            perror("Erro de leitura");
            return 0;
        }
        if (aux.codigo == usuario->codigo) {
            printf("Erro: Usuário com código %d já existe.\n", usuario->codigo);
            return 0;
        }
        pos = aux.proxima_pos;
    }

    // Insere no arquivo
    fseek(file, cabecalho.pos_topo_usuarios, SEEK_SET);

    if (cabecalho.pos_cabeca_usuarios == -1) {
        // Primeiro usuário
        cabecalho.pos_cabeca_usuarios = cabecalho.pos_topo_usuarios;
        usuario->proxima_pos = -1;
    } else {
        // Insere no início da lista existente
        usuario->proxima_pos = cabecalho.pos_cabeca_usuarios;
        cabecalho.pos_cabeca_usuarios = cabecalho.pos_topo_usuarios;
    }

    if (fwrite(usuario, sizeof(Usuario), 1, file) != 1) {
        perror("Erro ao escrever usuário");
        return 0;
    }

    cabecalho.pos_topo_usuarios += sizeof(Usuario);
    cabecalho.total_usuarios++;
    if (!escreverCabecalho(file, &cabecalho)) {
        printf("Erro ao atualizar cabeçalho\n");
        return 0;
    }

    printf("Usuário cadastrado com sucesso!\n");
    return 1;
}

/*
 * Propósito: Verifica se um usuário existe.
 * Pré-condição: arquivo não-nulo aberto e código válido do usuário.
 * Pós-condição: confirmação de existência de usuário válido.
 */
int usuario_existe(FILE* file, int codigo_usuario) {
    Cabecalho cabecalho;
    if (!lerCabecalho(file, &cabecalho)) return 0;

    long pos = cabecalho.pos_cabeca_usuarios;
    Usuario usuario;
    while (pos != -1) {
        fseek(file, pos, SEEK_SET);
        if (fread(&usuario, sizeof(Usuario), 1, file) != 1) {
            perror("Erro de leitura");
            return 0;
        }
        if (usuario.codigo == codigo_usuario) return 1;
        pos = usuario.proxima_pos;
    }
    return 0;
}

/**
 * Propósito: Obtém nome do usuário.
 * Pré-condição: arquivo não-nulo aberto, código e nome válidos do usuário.
 * Pós-condição: preenchimento de nome ou string de erro.
 */
int obter_nome_usuario(FILE* file, int codigo_usuario, char* nome) {
    Cabecalho cabecalho;
    if (!lerCabecalho(file, &cabecalho)) return 0;

    long pos = cabecalho.pos_cabeca_usuarios;
    Usuario usuario;
    while (pos != -1) {
        fseek(file, pos, SEEK_SET);
        if (fread(&usuario, sizeof(Usuario), 1, file) != 1) {
            perror("Erro de leitura");
            return 0;
        }
        if (usuario.codigo == codigo_usuario) {
            strncpy(nome, usuario.nome, 50);
            nome[50] = '\0';
            return 1;
        }
        pos = usuario.proxima_pos;
    }

    strcpy(nome, "<Usuário não encontrado>");
    return 0;
}

/*
 *
 * Propósito: Lista usuários cadastrados.
 * Pré-condição: arquivo não-nulo aberto.
 * Pós-condição: código e nome dos usuários ou mensagem se não houver.
 */
void listarUsuarios(FILE* file) {
    Cabecalho cabecalho;
    if (!lerCabecalho(file, &cabecalho)) {
        printf("Erro ao ler cabeçalho\n");
        return;
    }

    if (cabecalho.total_usuarios == 0) {
        printf("Nenhum usuário cadastrado.\n");
        return;
    }

    printf("\n=== LISTA DE USUÁRIOS ===\n");
    long pos = cabecalho.pos_cabeca_usuarios;
    Usuario usuario;
    while (pos != -1) {
        fseek(file, pos, SEEK_SET);
        if (fread(&usuario, sizeof(Usuario), 1, file) != 1) {
            perror("Erro de leitura");
            break;
        }
        printf("Código: %d | Nome: %s\n", usuario.codigo, usuario.nome);
        pos = usuario.proxima_pos;
    }
    printf("========================\n\n");
}
