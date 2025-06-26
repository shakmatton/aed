# Sistema de Gerenciamento de Biblioteca

**Autores:** Iuri F. Cordeiro, Lukas Godoy  
**Disciplina:** Algoritmos e Estruturas de Dados  
**Data:** Junho 2025

## Sumário

1. [Visão Geral](#visão-geral)
2. [Arquitetura do Sistema](#arquitetura-do-sistema)
3. [Processo de Implementação](#processo-de-implementação)
4. [Estruturas de Dados](#estruturas-de-dados)
5. [Funcionalidades Implementadas](#funcionalidades-implementadas)
6. [Formato de Arquivos](#formato-de-arquivos)
7. [Compilação e Execução](#compilação-e-execução)
8. [Testes e Validação](#testes-e-validação)

## Visão Geral

Este projeto implementa um sistema completo de gerenciamento de biblioteca em linguagem C, utilizando listas encadeadas armazenadas em arquivos binários. O sistema foi desenvolvido para atender todos os requisitos especificados no enunciado, incluindo:

- Cadastro de livros e usuários
- Sistema de empréstimos e devoluções
- Persistência em arquivo binário
- Carregamento em lote de registros
- Interface em modo texto

## Arquitetura do Sistema

### Estrutura Modular

O projeto foi organizado em módulos específicos para facilitar manutenção e compreensão:

```
src/
├── main.c              # Interface principal e menu
├── estruturas.h        # Definições das estruturas de dados
├── arquivo.c/.h        # Manipulação de arquivos binários
├── livros.c/.h         # Operações com livros
├── usuarios.c/.h       # Operações com usuários
├── emprestimos.c/.h    # Controle de empréstimos
├── carregamento.c/.h   # Importação de dados em lote
└── Makefile           # Automação da compilação
```

### Princípios de Design

1. **Separação de Responsabilidades**: Cada módulo tem uma função específica
2. **Encapsulamento**: Funções internas são estáticas quando possível
3. **Reutilização**: Funções auxiliares são compartilhadas entre módulos
4. **Robustez**: Validação de entrada e tratamento de erros

## Processo de Implementação

### Fase 1: Definição das Estruturas de Dados

**Primeira etapa:** Definição das estruturas principais em `estruturas.h`

```c
// Cabeçalho do arquivo binário
typedef struct {
    int pos_cabeca_livros;     // Posição da primeira entrada de livros
    int pos_cabeca_usuarios;   // Posição da primeira entrada de usuários  
    int pos_cabeca_emprestimos; // Posição da primeira entrada de empréstimos
    int topo;                  // Próxima posição livre no arquivo
    int total_livros;          // Contador de livros cadastrados
    int total_usuarios;        // Contador de usuários cadastrados
    int total_emprestimos;     // Contador de empréstimos
} Cabecalho;

// Estrutura de livro
typedef struct {
    int codigo;
    char titulo[151];
    char autor[201]; 
    char editora[51];
    int edicao;
    int ano;
    int exemplares;
    int proximo;  // Posição do próximo livro na lista
} Livro;

// Estrutura de usuário
typedef struct {
    int codigo;
    char nome[51];
    int proximo;  // Posição do próximo usuário na lista
} Usuario;

// Estrutura de empréstimo
typedef struct {
    int codigo_usuario;
    int codigo_livro;
    char data_emprestimo[11];  // DD/MM/AAAA
    char data_devolucao[11];   // DD/MM/AAAA ou vazio
    int proximo;  // Posição do próximo empréstimo na lista
} Emprestimo;
```

**Decisões de implementação:**
- Uso de arrays de tamanho fixo para strings (conforme especificação)
- Campo `proximo` em cada estrutura para implementar lista encadeada
- Cabeçalho centralizado para controle das listas

### Fase 2: Sistema de Arquivos Binários

**Segunda etapa:** Implementação das funções de manipulação de arquivos em `arquivo.c`

```c
// Função para abrir/criar arquivo binário
FILE* abrir_arquivo_binario(const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "r+b");
    if (arquivo == NULL) {
        // Arquivo não existe, criar novo
        arquivo = fopen(nome_arquivo, "w+b");
        if (arquivo != NULL) {
            // Inicializar cabeçalho
            Cabecalho cab = {-1, -1, -1, sizeof(Cabecalho), 0, 0, 0};
            fwrite(&cab, sizeof(Cabecalho), 1, arquivo);
        }
    }
    return arquivo;
}

// Função para ler cabeçalho
int ler_cabecalho(FILE* arquivo, Cabecalho* cab) {
    fseek(arquivo, 0, SEEK_SET);
    return fread(cab, sizeof(Cabecalho), 1, arquivo) == 1;
}

// Função para escrever cabeçalho  
int escrever_cabecalho(FILE* arquivo, const Cabecalho* cab) {
    fseek(arquivo, 0, SEEK_SET);
    return fwrite(cab, sizeof(Cabecalho), 1, arquivo) == 1;
}
```

**Características implementadas:**
- Criação automática do arquivo se não existir
- Inicialização do cabeçalho com valores padrão
- Funções de leitura/escrita do cabeçalho centralizadas

### Fase 3: Operações com Livros

**Terceira etapa:** Implementação das funcionalidades de livros em `livros.c`

#### Inserção de Livros
```c
int inserir_livro(FILE* arquivo, const Livro* livro) {
    Cabecalho cab;
    if (!ler_cabecalho(arquivo, &cab)) return 0;
    
    // Verificar se código já existe
    if (buscar_livro_por_codigo(arquivo, livro->codigo) != -1) {
        return 0; // Código já existe
    }
    
    // Inserir na posição topo
    fseek(arquivo, cab.topo, SEEK_SET);
    Livro novo_livro = *livro;
    novo_livro.proximo = cab.pos_cabeca_livros;
    
    if (fwrite(&novo_livro, sizeof(Livro), 1, arquivo) != 1) return 0;
    
    // Atualizar cabeçalho
    cab.pos_cabeca_livros = cab.topo;
    cab.topo += sizeof(Livro);
    cab.total_livros++;
    
    return escrever_cabecalho(arquivo, &cab);
}
```

#### Busca por Código
```c
int buscar_livro_por_codigo(FILE* arquivo, int codigo) {
    Cabecalho cab;
    if (!ler_cabecalho(arquivo, &cab)) return -1;
    
    int pos_atual = cab.pos_cabeca_livros;
    while (pos_atual != -1) {
        fseek(arquivo, pos_atual, SEEK_SET);
        Livro livro;
        if (fread(&livro, sizeof(Livro), 1, arquivo) != 1) break;
        
        if (livro.codigo == codigo) return pos_atual;
        pos_atual = livro.proximo;
    }
    return -1;
}
```

#### Busca por Título (Case-Insensitive)
```c
int buscar_livro_por_titulo(FILE* arquivo, const char* titulo) {
    Cabecalho cab;
    if (!ler_cabecalho(arquivo, &cab)) return -1;
    
    int pos_atual = cab.pos_cabeca_livros;
    while (pos_atual != -1) {
        fseek(arquivo, pos_atual, SEEK_SET);
        Livro livro;
        if (fread(&livro, sizeof(Livro), 1, arquivo) != 1) break;
        
        if (strcasecmp(livro.titulo, titulo) == 0) return pos_atual;
        pos_atual = livro.proximo;
    }
    return -1;
}
```

### Fase 4: Sistema de Usuários

**Quarta etapa:** Implementação similar para usuários em `usuarios.c`

```c
int inserir_usuario(FILE* arquivo, const Usuario* usuario) {
    Cabecalho cab;
    if (!ler_cabecalho(arquivo, &cab)) return 0;
    
    // Verificar código único
    if (buscar_usuario_por_codigo(arquivo, usuario->codigo) != -1) {
        return 0;
    }
    
    // Inserir no início da lista
    fseek(arquivo, cab.topo, SEEK_SET);
    Usuario novo_usuario = *usuario;
    novo_usuario.proximo = cab.pos_cabeca_usuarios;
    
    if (fwrite(&novo_usuario, sizeof(Usuario), 1, arquivo) != 1) return 0;
    
    // Atualizar cabeçalho
    cab.pos_cabeca_usuarios = cab.topo;
    cab.topo += sizeof(Usuario);
    cab.total_usuarios++;
    
    return escrever_cabecalho(arquivo, &cab);
}
```

### Fase 5: Sistema de Empréstimos

**Quinta etapa:** Implementação do controle de empréstimos em `emprestimos.c`

#### Função de Empréstimo
```c
int emprestar_livro(FILE* arquivo, int codigo_usuario, int codigo_livro) {
    // Verificar se usuário existe
    int pos_usuario = buscar_usuario_por_codigo(arquivo, codigo_usuario);
    if (pos_usuario == -1) return ERRO_USUARIO_NAO_ENCONTRADO;
    
    // Verificar se livro existe e tem exemplares
    int pos_livro = buscar_livro_por_codigo(arquivo, codigo_livro);
    if (pos_livro == -1) return ERRO_LIVRO_NAO_ENCONTRADO;
    
    fseek(arquivo, pos_livro, SEEK_SET);
    Livro livro;
    fread(&livro, sizeof(Livro), 1, arquivo);
    
    if (livro.exemplares <= 0) return ERRO_SEM_EXEMPLARES;
    
    // Verificar se usuário já tem este livro emprestado
    if (verificar_emprestimo_ativo(arquivo, codigo_usuario, codigo_livro)) {
        return ERRO_LIVRO_JA_EMPRESTADO;
    }
    
    // Criar registro de empréstimo
    Emprestimo emp;
    emp.codigo_usuario = codigo_usuario;
    emp.codigo_livro = codigo_livro;
    obter_data_atual(emp.data_emprestimo);
    strcpy(emp.data_devolucao, ""); // Vazio = não devolvido
    
    // Inserir empréstimo
    Cabecalho cab;
    ler_cabecalho(arquivo, &cab);
    
    fseek(arquivo, cab.topo, SEEK_SET);
    emp.proximo = cab.pos_cabeca_emprestimos;
    fwrite(&emp, sizeof(Emprestimo), 1, arquivo);
    
    // Atualizar cabeçalho
    cab.pos_cabeca_emprestimos = cab.topo;
    cab.topo += sizeof(Emprestimo);
    cab.total_emprestimos++;
    escrever_cabecalho(arquivo, &cab);
    
    // Decrementar exemplares disponíveis
    livro.exemplares--;
    fseek(arquivo, pos_livro, SEEK_SET);
    fwrite(&livro, sizeof(Livro), 1, arquivo);
    
    return SUCESSO;
}
```

#### Função de Devolução
```c
int devolver_livro(FILE* arquivo, int codigo_usuario, int codigo_livro) {
    // Encontrar empréstimo ativo
    int pos_emprestimo = buscar_emprestimo_ativo(arquivo, codigo_usuario, codigo_livro);
    if (pos_emprestimo == -1) return ERRO_EMPRESTIMO_NAO_ENCONTRADO;
    
    // Atualizar data de devolução
    fseek(arquivo, pos_emprestimo, SEEK_SET);
    Emprestimo emp;
    fread(&emp, sizeof(Emprestimo), 1, arquivo);
    
    obter_data_atual(emp.data_devolucao);
    
    fseek(arquivo, pos_emprestimo, SEEK_SET);
    fwrite(&emp, sizeof(Emprestimo), 1, arquivo);
    
    // Incrementar exemplares disponíveis
    int pos_livro = buscar_livro_por_codigo(arquivo, codigo_livro);
    fseek(arquivo, pos_livro, SEEK_SET);
    Livro livro;
    fread(&livro, sizeof(Livro), 1, arquivo);
    
    livro.exemplares++;
    fseek(arquivo, pos_livro, SEEK_SET);
    fwrite(&livro, sizeof(Livro), 1, arquivo);
    
    return SUCESSO;
}
```

### Fase 6: Carregamento em Lote

**Sexta etapa:** Implementação do parser de arquivo texto em `carregamento.c`

```c
int carregar_arquivo_texto(FILE* arquivo_bin, const char* nome_arquivo_txt) {
    FILE* arquivo_txt = fopen(nome_arquivo_txt, "r");
    if (!arquivo_txt) return 0;
    
    char linha[1000];
    int linhas_processadas = 0;
    int erros = 0;
    
    while (fgets(linha, sizeof(linha), arquivo_txt)) {
        // Remover quebra de linha
        linha[strcspn(linha, "\n")] = 0;
        
        if (strlen(linha) == 0) continue; // Pular linhas vazias
        
        char tipo = linha[0];
        switch (tipo) {
            case 'L':
                if (!processar_linha_livro(arquivo_bin, linha)) erros++;
                break;
            case 'U':
                if (!processar_linha_usuario(arquivo_bin, linha)) erros++;
                break;
            case 'E':
                if (!processar_linha_emprestimo(arquivo_bin, linha)) erros++;
                break;
            default:
                printf("Linha ignorada (tipo desconhecido): %s\n", linha);
                erros++;
        }
        linhas_processadas++;
    }
    
    fclose(arquivo_txt);
    printf("Processamento concluído: %d linhas, %d erros\n", linhas_processadas, erros);
    return 1;
}

// Função auxiliar para processar linha de livro
int processar_linha_livro(FILE* arquivo, const char* linha) {
    char campos[8][256];
    int num_campos = dividir_campos(linha, campos, 8);
    
    if (num_campos != 8) return 0;
    
    Livro livro;
    livro.codigo = atoi(trim(campos[1]));
    strcpy(livro.titulo, trim(campos[2]));
    strcpy(livro.autor, trim(campos[3]));
    strcpy(livro.editora, trim(campos[4]));
    livro.edicao = atoi(trim(campos[5]));
    livro.ano = atoi(trim(campos[6]));
    livro.exemplares = atoi(trim(campos[7]));
    
    return inserir_livro(arquivo, &livro);
}
```

### Fase 7: Interface do Usuário

**Sétima etapa:** Implementação do menu principal em `main.c`

```c
int main() {
    FILE* arquivo = abrir_arquivo_binario("biblioteca.dat");
    if (!arquivo) {
        printf("Erro ao abrir arquivo da biblioteca!\n");
        return 1;
    }
    
    int opcao;
    do {
        exibir_menu();
        printf("Digite sua opção: ");
        scanf("%d", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case 1: menu_cadastrar_livro(arquivo); break;
            case 2: menu_imprimir_livro(arquivo); break;
            case 3: menu_listar_livros(arquivo); break;
            case 4: menu_buscar_titulo(arquivo); break;
            case 5: menu_calcular_total(arquivo); break;
            case 6: menu_cadastrar_usuario(arquivo); break;
            case 7: menu_emprestar_livro(arquivo); break;
            case 8: menu_devolver_livro(arquivo); break;
            case 9: menu_listar_emprestimos(arquivo); break;
            case 10: menu_carregar_arquivo(arquivo); break;
            case 11: menu_mostrar_info_arquivo(arquivo); break;
            case 0: printf("Encerrando sistema...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    
    fclose(arquivo);
    return 0;
}
```

## Estruturas de Dados

### Lista Encadeada em Arquivo Binário

A implementação utiliza o conceito de lista encadeada onde:
- Cada nó contém um campo `proximo` indicando a posição do próximo elemento
- O cabeçalho mantém a posição do primeiro elemento de cada lista
- Valor -1 indica fim da lista

### Gerenciamento de Memória

- **Topo**: Próxima posição livre no final do arquivo
- **Inserção**: Sempre no topo, atualizando ponteiros
- **Busca**: Percorre a lista sequencialmente

## Funcionalidades Implementadas

### ✅ Funcionalidades Obrigatórias

#### Gerenciamento de Livros
- **Cadastrar livro**: Validação de código único, inserção na lista
- **Imprimir dados do livro**: Busca por código e exibição completa
- **Listar todos os livros**: Percorre lista exibindo resumo
- **Busca por título**: Implementação case-insensitive
- **Calcular total**: Contador mantido no cabeçalho

#### Gerenciamento de Usuários
- **Cadastrar usuário**: Validação de código único

#### Sistema de Empréstimos
- **Emprestar livro**: Validações múltiplas, controle de exemplares
- **Devolver livro**: Atualização de data, incremento de exemplares
- **Listar empréstimos**: Exibe apenas empréstimos ativos

#### Importação
- **Carregar arquivo**: Parser robusto com tratamento de erros

### ✅ Funcionalidades Adicionais

- **Informações do arquivo**: Estatísticas do sistema
- **Validações robustas**: Tratamento de casos extremos
- **Interface amigável**: Mensagens claras e formatação

## Formato de Arquivos

### Arquivo Binário (biblioteca.dat)

```
[Cabeçalho - 28 bytes]
[Dados das listas encadeadas]
```

### Arquivo Texto (formato de entrada)

```
L;codigo;titulo;autor;editora;edicao;ano;exemplares
U;codigo;nome  
E;codigo_usuario;codigo_livro;data_emprestimo;data_devolucao
```

**Características do parser:**
- Remove espaços em branco automáticamente
- Suporta linhas vazias e comentários
- Tratamento de erros linha por linha

## Compilação e Execução

### Usando Makefile (Recomendado)

```bash
# Compilar
make

# Executar
make run

# Limpar arquivos objeto
make clean

# Limpar tudo (incluindo dados)
make cleanall
```

### Compilação Manual

```bash
gcc -Wall -Wextra -std=c99 -g -c *.c
gcc -Wall -Wextra -std=c99 -g -o biblioteca *.o
```

### Flags de Compilação Utilizadas

- `-Wall -Wextra`: Habilita avisos importantes
- `-std=c99`: Padrão C99 para compatibilidade
- `-g`: Informações de debug

## Testes e Validação

### Casos de Teste Implementados

#### Teste de Cadastro
```
1. Cadastrar livros com códigos únicos ✅
2. Tentar cadastrar livro com código duplicado ✅
3. Cadastrar usuários com códigos únicos ✅
4. Tentar cadastrar usuário com código duplicado ✅
```

#### Teste de Empréstimos
```
1. Emprestar livro disponível ✅
2. Tentar emprestar sem exemplares disponíveis ✅
3. Tentar emprestar para usuário inexistente ✅
4. Devolver livro emprestado ✅
5. Tentar devolver livro não emprestado ✅
```

#### Teste de Busca
```
1. Buscar livro por código existente ✅
2. Buscar livro por código inexistente ✅
3. Buscar livro por título (case-insensitive) ✅
4. Buscar livro por título inexistente ✅
```

#### Teste de Carregamento
```
1. Carregar arquivo texto válido ✅
2. Carregar arquivo com linhas inválidas ✅
3. Carregar arquivo inexistente ✅
4. Processar campos com espaços extras ✅
```

### Validações Implementadas

- **Integridade de dados**: Verificação de códigos únicos
- **Consistência**: Controle de exemplares disponíveis
- **Robustez**: Tratamento de entradas inválidas
- **Persistência**: Manutenção de estado entre execuções

## Documentação do Código

Todas as funções foram documentadas seguindo o padrão:

```c
/**
 * Propósito: [Descrição da função]
 * Pré-condições: [O que deve ser verdade antes da chamada]
 * Pós-condições: [O que será verdade após a execução]
 * Parâmetros: [Descrição dos parâmetros]
 * Retorno: [Descrição do valor retornado]
 */
```

## Status Final

✅ **Todas as funcionalidades obrigatórias implementadas**  
✅ **Código documentado com pré e pós-condições**  
✅ **Testes validados e funcionando**  
✅ **Interface de usuário completa**  
✅ **Tratamento de erros robusto**  
✅ **Arquivo README.md completo**

O sistema está pronto para apresentação e atende integralmente aos requisitos especificados no enunciado do trabalho.