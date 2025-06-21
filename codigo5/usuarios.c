#include <stdio.h>
#include <stdlib.h>
#include "usuarios.h"
#include "arquivo.h"

// ==================== FUNÇÕES DE USUÁRIOS ====================

int cadastrarUsuario(FILE* file, Usuario* usuario) {
    Cabecalho cabecalho;

    if (!lerCabecalho(file, &cabecalho)) {
        printf("Erro ao ler cabeçalho\n");
        return 0;
    }

    // Posiciona no final do arquivo para inserir
    fseek(file, cabecalho.pos_topo_usuarios, SEEK_SET);

    // Se é o primeiro usuário da lista
    if (cabecalho.pos_cabeca_usuarios == -1) {
        cabecalho.pos_cabeca_usuarios = cabecalho.pos_topo_usuarios;
        usuario->proxima_pos = -1;
    } else {
        // Insere no início da lista
        usuario->proxima_pos = cabecalho.pos_cabeca_usuarios;
        cabecalho.pos_cabeca_usuarios = cabecalho.pos_topo_usuarios;
    }

    // Escreve o usuário
    if (fwrite(usuario, sizeof(Usuario), 1, file) != 1) {
        printf("Erro ao escrever usuário\n");
        return 0;
    }

    // Atualiza cabeçalho
    cabecalho.pos_topo_usuarios += sizeof(Usuario);
    cabecalho.total_usuarios++;

    if (!escreverCabecalho(file, &cabecalho)) {
        printf("Erro ao atualizar cabeçalho\n");
        return 0;
    }

    return 1;
}
