#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carregamento.h"
#include "estruturas.h"
#include "livros.h"
#include "usuarios.h"
#include "emprestimos.h"
#include "arquivo.h"

/**
 * Propósito: Verifica se uma data está no formato DD/MM/AAAA e é válida no calendário.
 * Pré-condições: A string deve estar nesse formato, com valores numéricos válidos.
 * Pós-condições: Retorna 1 se a data for válida, 0 caso contrário.
 */
int verifica_data(const char* data) {
    char data_copia[11];
    strcpy(data_copia, data);
    char* token;
    int dia, mes, ano;
    
    token = strtok(data_copia, "/"); 
    if (!token) return 0; 
    dia = atoi(token);
    
    token = strtok(NULL, "/"); 
    if (!token) return 0; 
    mes = atoi(token);
    
    token = strtok(NULL, "/"); 
    if (!token) return 0; 
    ano = atoi(token);
    
    // Validação básica de ano
    if (ano < 1900 || ano > 2100) return 0;
    
    // Validação de mês
    if (mes < 1 || mes > 12) return 0;
    
    // Validação de dia
    if (dia < 1) return 0;
    
    // Verificação de fevereiro com ano bissexto
    if (mes == 2) {
        int bissexto = (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0));
        int max_dias = bissexto ? 29 : 28;
        if (dia > max_dias) return 0;
    }
    // Meses com 30 dias
    else if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
        return 0;
    }
    // Meses com 31 dias
    else if (dia > 31) {
        return 0;
    }

    return 1;
}

/**
 * Propósito: Compara duas datas no formato DD/MM/AAAA.
 * Pré-condições: Ambas devem ser strings válidas nesse formato.
 * Pós-condições: Retorna -1 se data1 < data2, 0 se iguais, 1 se data1 > data2, -2 se qualquer data for inválida.
 */
int comparar_datas(const char* data1, const char* data2) {
    int dia1, mes1, ano1;
    int dia2, mes2, ano2;
    
    if (sscanf(data1, "%d/%d/%d", &dia1, &mes1, &ano1) != 3) return -2;
    if (sscanf(data2, "%d/%d/%d", &dia2, &mes2, &ano2) != 3) return -2;
    
    // Comparação por ano
    if (ano1 < ano2) return -1;
    if (ano1 > ano2) return 1;
    
    // Mesmo ano, compara mês
    if (mes1 < mes2) return -1;
    if (mes1 > mes2) return 1;
    
    // Mesmo mês, compara dia
    if (dia1 < dia2) return -1;
    if (dia1 > dia2) return 1;
    
    return 0; // Datas iguais
}

/**
 * Propósito: Verifica se a data de devolução não é anterior à data de empréstimo.
 * Pré-condições: Ambas as datas devem estar no formato DD/MM/AAAA (ou devolução vazia).
 * Pós-condições: Retorna 1 se o intervalo for válido ou devolução estiver em branco, 0 se inválido.
 */
int validar_intervalo_datas(const char* data_emprestimo, const char* data_devolucao) {
    // Se devolução está vazia, sempre válido
    if (data_devolucao[0] == '\0') return 1;
    
    int comparacao = comparar_datas(data_emprestimo, data_devolucao);
    
    // Datas inválidas ou empréstimo após devolução
    if (comparacao == -2 || comparacao > 0) {
        return 0;
    }
    
    return 1;
}

/**
 * Propósito: Remove espaços em branco do início e fim de uma string
 * Pré-condições: str deve ser uma string válida
 * Pós-condições: string modificada sem espaços nas extremidades
 */
void removerEspacos(char* str) {
    if (!str || !*str) return;
    
    int inicio = 0;
    while (str[inicio] == ' ' || str[inicio] == '\t') 
        inicio++;
    
    int fim = (int)strlen(str) - 1;
    while (fim >= inicio && (str[fim] == ' ' || str[fim] == '\t' || str[fim] == '\n' || str[fim] == '\r')) 
        fim--;
    
    int i;
    for (i = 0; i <= fim - inicio; i++) {
        str[i] = str[inicio + i];
    }
    str[i] = '\0';
}

/**
 * Propósito: Processa uma linha de livro do arquivo texto
 * Pré-condições: file deve estar aberto, linha deve estar no formato correto
 * Pós-condições: livro cadastrado no arquivo, retorna 1 se sucesso
 */
int processarLivro(FILE* file, char* linha) {
    Livro livro;
    char* token;

    strtok(linha, ";"); // descarta "L;"

    // Campos obrigatórios
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); livro.codigo = atoi(token);
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); strncpy(livro.titulo, token, sizeof(livro.titulo)-1); livro.titulo[sizeof(livro.titulo)-1] = '\0';
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); strncpy(livro.autor, token, sizeof(livro.autor)-1); livro.autor[sizeof(livro.autor)-1] = '\0';
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); strncpy(livro.editora, token, sizeof(livro.editora)-1); livro.editora[sizeof(livro.editora)-1] = '\0';
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); livro.edicao = atoi(token);
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); livro.ano = atoi(token);
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); livro.exemplares = atoi(token);

    return cadastrarLivro(file, &livro);
}

/**
 * Propósito: Processa uma linha de usuário do arquivo texto
 * Pré-condições: file deve estar aberto, linha deve estar no formato correto
 * Pós-condições: usuário cadastrado no arquivo, retorna 1 se sucesso
 */
int processarUsuario(FILE* file, char* linha) {
    Usuario usuario;
    char* token;

    strtok(linha, ";"); // descarta "U;"

    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); usuario.codigo = atoi(token);
    token = strtok(NULL, ";"); if (!token) return 0; removerEspacos(token); strncpy(usuario.nome, token, sizeof(usuario.nome)-1); usuario.nome[sizeof(usuario.nome)-1] = '\0';

    return cadastrarUsuario(file, &usuario);
}

/**
 * Propósito: Processa uma linha de empréstimo do arquivo texto
 * Pré-condições: file deve estar aberto, linha deve estar no formato correto
 * Pós-condições: empréstimo cadastrado no arquivo, retorna 1 se sucesso
 */
int processarEmprestimo(FILE* file, char* linha) {
    Emprestimo emprestimo;
    Cabecalho cabecalho;
    char* token;
    char linha_copia[1000];
    
    strncpy(linha_copia, linha, sizeof(linha_copia)-1);
    linha_copia[sizeof(linha_copia)-1] = '\0';

    strtok(linha_copia, ";"); // descarta "E;"

    // Extrai campos
    token = strtok(NULL, ";"); 
    if (!token) return 0; 
    removerEspacos(token); 
    emprestimo.codigo_usuario = atoi(token);
    
    token = strtok(NULL, ";"); 
    if (!token) return 0; 
    removerEspacos(token); 
    emprestimo.codigo_livro = atoi(token);
    
    token = strtok(NULL, ";"); 
    if (!token) return 0; 
    removerEspacos(token); 
    strncpy(emprestimo.data_emprestimo, token, sizeof(emprestimo.data_emprestimo)-1); 
    emprestimo.data_emprestimo[sizeof(emprestimo.data_emprestimo)-1] = '\0';
    
    token = strtok(NULL, ";");
    if (token && strlen(token) > 0) {
        removerEspacos(token);
        strncpy(emprestimo.data_devolucao, token, sizeof(emprestimo.data_devolucao)-1);
        emprestimo.data_devolucao[sizeof(emprestimo.data_devolucao)-1] = '\0';
    } else {
        emprestimo.data_devolucao[0] = '\0'; // ativo
    }

    // Validação de datas
    if (!verifica_data(emprestimo.data_emprestimo)) {
        printf("Erro: Data de empréstimo inválida: %s\n", emprestimo.data_emprestimo);
        return 0;
    }

    if (emprestimo.data_devolucao[0] != '\0' && !verifica_data(emprestimo.data_devolucao)) {
        printf("Erro: Data de devolução inválida: %s\n", emprestimo.data_devolucao);
        return 0;
    }

    // Valida intervalo de datas
    if (!validar_intervalo_datas(emprestimo.data_emprestimo, emprestimo.data_devolucao)) {
        printf("Erro: Data de empréstimo (%s) posterior à devolução (%s)\n",
               emprestimo.data_emprestimo, 
               emprestimo.data_devolucao[0] ? emprestimo.data_devolucao : "<vazia>");
        return 0;
    }

    // Verifica se usuário e livro existem
    if (!usuario_existe(file, emprestimo.codigo_usuario)) {
        printf("Erro: Usuário %d não existe\n", emprestimo.codigo_usuario);
        return 0;
    }

    Livro livro;
    if (!buscarLivroPorCodigo(file, emprestimo.codigo_livro, &livro)) {
        printf("Erro: Livro %d não existe\n", emprestimo.codigo_livro);
        return 0;
    }

    // Lê cabeçalho atual
    fseek(file, 0, SEEK_SET);
    if (fread(&cabecalho, sizeof(Cabecalho), 1, file) != 1) {
        perror("Erro ao ler cabeçalho");
        return 0;
    }

    // Insere no início da lista de empréstimos
    emprestimo.proxima_pos = cabecalho.pos_cabeca_emprestimos;
    fseek(file, cabecalho.pos_topo_emprestimos, SEEK_SET);
    long nova_pos = ftell(file);
    if (fwrite(&emprestimo, sizeof(Emprestimo), 1, file) != 1) {
        perror("Erro ao escrever empréstimo");
        return 0;
    }

    // Atualiza cabeçalho
    cabecalho.pos_cabeca_emprestimos = nova_pos;
    cabecalho.pos_topo_emprestimos = ftell(file);
    cabecalho.total_emprestimos++;
    fseek(file, 0, SEEK_SET);
    if (fwrite(&cabecalho, sizeof(Cabecalho), 1, file) != 1) {
        perror("Erro ao atualizar cabeçalho");
        return 0;
    }
    fflush(file);

    // Atualiza exemplares se empréstimo ainda ativo
    if (emprestimo.data_devolucao[0] == '\0') {
        if (!atualizar_exemplares_livro(file, emprestimo.codigo_livro, -1)) {
            printf("Erro ao atualizar exemplares do livro %d\n", emprestimo.codigo_livro);
            return 0;
        }
    }

    return 1;
}

/**
 * Propósito: Carrega dados de um arquivo texto para o arquivo binário
 * Pré-condições: file deve estar aberto, nomeArquivoTexto deve ser válido
 * Pós-condições: dados carregados no arquivo binário, estatísticas exibidas
 */
int carregarArquivo(FILE* file, char* nomeArquivoTexto) {
    FILE* arquivoTexto = fopen(nomeArquivoTexto, "r");
    if (!arquivoTexto) {
        printf("Erro ao abrir arquivo: %s\n", nomeArquivoTexto);
        return 0;
    }

    char linha[1000];
    int l = 0, u = 0, e = 0;
    printf("Carregando dados do arquivo %s...\n", nomeArquivoTexto);

    while (fgets(linha, sizeof(linha), arquivoTexto)) {
        linha[strcspn(linha, "\n")] = '\0';
        if (strlen(linha) == 0) continue;
        
        if (strncmp(linha, "L;", 2) == 0) { 
            if (processarLivro(file, linha)) l++; 
            else printf("Falha ao processar livro: %s\n", linha);
        }
        else if (strncmp(linha, "U;", 2) == 0) { 
            if (processarUsuario(file, linha)) u++; 
            else printf("Falha ao processar usuário: %s\n", linha);
        }
        else if (strncmp(linha, "E;", 2) == 0) { 
            if (processarEmprestimo(file, linha)) e++; 
            else printf("Falha ao processar empréstimo: %s\n", linha);
        }
        else { 
            printf("Linha inválida ignorada: %s\n", linha); 
        }
    }
    fclose(arquivoTexto);

    printf("\n=== RESUMO DO CARREGAMENTO ===\n");
    printf("Livros: %d | Usuários: %d | Empréstimos: %d\n", l, u, e);
    printf("==============================\n\n");
    return 1;
}