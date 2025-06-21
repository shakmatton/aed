#ifndef USUARIOS_H
#define USUARIOS_H

#include <stdio.h>
#include "estruturas.h"

// ==================== PROTÓTIPOS DAS FUNÇÕES DE USUÁRIOS ====================

/**
 * Propósito: Cadastra um novo usuário no arquivo
 * Pré-condição: arquivo aberto e dados do usuário válidos
 * Pós-condição: usuário inserido no arquivo, cabeçalho atualizado
 */
int cadastrarUsuario(FILE* file, Usuario* usuario);

#endif
