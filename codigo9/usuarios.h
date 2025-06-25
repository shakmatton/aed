#ifndef USUARIOS_H
#define USUARIOS_H

#include <stdio.h>
#include "estruturas.h"

// ==================== PROTÓTIPOS DAS FUNÇÕES DE USUÁRIOS ====================

/**
 * Propósito: Cadastra um novo usuário no arquivo
 * Pré-condição: arquivo aberto e dados do usuário válidos
 * Pós-condição: usuário inserido no arquivo, cabeçalho atualizado.
 */
int cadastrarUsuario(FILE* file, Usuario* usuario);

/*
 * Propósito: Verifica se um usuário existe.
 * Pré-condição: arquivo não-nulo aberto e código válido do usuário.
 * Pós-condição: confirmação de existência de usuário válido.
 */
int usuario_existe(FILE* file, int codigo_usuario);

/**
 * Propósito: Obtém nome do usuário.
 * Pré-condição: arquivo não-nulo aberto, código e nome válidos do usuário.
 * Pós-condição: preenchimento de nome ou string de erro.
 */
int obter_nome_usuario(FILE* file, int codigo_usuario, char* nome);

/*
 *
 * Propósito: Lista usuários cadastrados.
 * Pré-condição: arquivo não-nulo aberto.
 * Pós-condição: código e nome dos usuários ou mensagem se não houver.
 */
void listarUsuarios(FILE* file);

#endif

