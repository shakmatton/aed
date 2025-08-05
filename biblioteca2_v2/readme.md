# Sistema de Gerenciamento de Livros
## Trabalho 2 - Algoritmos e Estruturas de Dados

### Componentes do Grupo
- [Seu Nome Aqui]
- [Nome do Colega 1 - se houver]
- [Nome do Colega 2 - se houver]

### Descrição do Sistema
Este sistema implementa um gerenciador de cadastro de livros utilizando árvore binária de busca armazenada em arquivo binário. O sistema é modularizado em diferentes arquivos para melhor organização e manutenibilidade.

### Estrutura Modular do Projeto

#### Arquivos de Cabeçalho (.h)
- `estruturas.h` - Definições das estruturas Livro e CabecalhoArquivo
- `arquivo.h` - Protótipos para operações de arquivo binário
- `livros.h` - Protótipos para operações com livros
- `carregamento.h` - Protótipos para carregamento em lote

#### Arquivos de Implementação (.c)
- `main.c` - Programa principal e menu
- `arquivo.c` - Implementação das operações de arquivo
- `livros.c` - Implementação das operações com livros
- `carregamento.c` - Implementação do carregamento em lote

#### Outros Arquivos
- `Makefile` - Script de compilação modularizada
- `livros_teste.txt` - Arquivo de exemplo para teste

### Compilação e Execução

#### Compilação usando Makefile (Recomendado)
```bash
make
```

#### Compilação Manual
```bash
gcc -Wall -Wextra -std=c99 -g main.c arquivo.c livros.c carregamento.c -o livros
```

#### Execução
```bash
./livros
```

#### Limpeza
```bash
make clean
```

### Funcionalidades Implementadas

#### 1. Cadastrar Livro
**Funcionalidade:** Insere um novo livro na árvore binária de busca
**Implementação:** Módulo `livros.c` - função `cadastrar_livro()`
**Como usar:** 
- Opção 1 do menu
- Inserir código único, título, autor, editora, edição, ano, exemplares e preço
- Sistema verifica duplicatas automaticamente

#### 2. Imprimir Dados do Livro
**Funcionalidade:** Busca e exibe informações completas de um livro
**Implementação:** Módulo `livros.c` - função `imprimir_dados_livro()`
**Como usar:**
- Opção 2 do menu
- Informar código do livro
- Sistema percorre árvore binária para localização

#### 3. Listar Todos os Livros
**Funcionalidade:** Lista livros em ordem crescente de código (percorrimento in-ordem)
**Implementação:** Módulo `livros.c` - função `listar_todos_livros()` e `percorrer_in_ordem()`
**Como usar:**
- Opção 3 do menu
- Sistema percorre árvore recursivamente em ordem

#### 4. Calcular Total de Livros
**Funcionalidade:** Exibe contador total mantido no cabeçalho
**Implementação:** Módulo `livros.c` - função `calcular_total()`
**Como usar:**
- Opção 4 do menu
- Lê contador do cabeçalho do arquivo

#### 5. Remover Livro
**Funcionalidade:** Remove livro da árvore mantendo propriedades da BST
**Implementação:** Módulo `livros.c` - função `remover_livro()` e `remover_da_arvore()`
**Como usar:**
- Opção 5 do menu
- Informar código do livro
- Sistema trata os 3 casos de remoção (folha, 1 filho, 2 filhos)

#### 6. Carregar Arquivo
**Funcionalidade:** Carregamento em lote de livros via arquivo texto
**Implementação:** Módulo `carregamento.c` - função `carregar_arquivo()`
**Como usar:**
- Opção 6 do menu
- Informar nome do arquivo
- Formato: codigo;titulo;autor;editora;edicao;ano;exemplares;preco

#### 7. Imprimir Lista de Registros Livres
**Funcionalidade:** Mostra posições reutilizáveis no arquivo
**Implementação:** Módulo `livros.c` - função `imprimir_lista_livres()`
**Como usar:**
- Opção 7 do menu
- Percorre lista encadeada de registros livres

#### 8. Imprimir Árvore por Níveis
**Funcionalidade:** Visualiza estrutura da árvore nível a nível
**Implementação:** Módulo `livros.c` - função `imprimir_arvore_niveis()`
**Como usar:**
- Opção 8 do menu
- Imprime códigos organizados por nível da árvore

### Arquitetura do Sistema

#### Gerenciamento de Arquivo Binário (`arquivo.c`)
- Cabeçalho na posição 0 com metadados da árvore
- Registros sequenciais com controle de posições livres
- Lista encadeada para reaproveitamento de espaço

#### Árvore Binária de Busca (`livros.c`)
- Organizada por código do livro (chave única)
- Inserção iterativa para eficiência
- Remoção com tratamento dos 3 casos clássicos
- Percorrimentos in-ordem e por níveis

#### Carregamento em Lote (`carregamento.c`)
- Parser robusto com tratamento de espaços
- Validação de formato e duplicatas
- Conversão automática de vírgulas decimais

### Estruturas de Dados

#### Livro
```c
typedef struct {
    int codigo;                // Chave primária
    char titulo[151];          // Título do livro
    char autor[201];           // Autor(es)
    char editora[51];          // Editora
    int edicao;               // Número da edição
    int ano;                  // Ano de publicação
    int exemplares;           // Quantidade disponível
    float preco;              // Preço unitário
    int esquerda, direita;    // Ponteiros para filhos
    int livre;                // Flag de registro livre
    int proximo_livre;        // Lista encadeada de livres
} Livro;
```

#### CabecalhoArquivo
```c
typedef struct {
    int raiz;           // Posição da raiz
    int topo;           // Próxima posição livre
    int cabeca_livres;  // Cabeça da lista de livres
    int total_livros;   // Contador total
} CabecalhoArquivo;
```

### Exemplo de Árvore por Níveis
Para a árvore da Figura 1 do enunciado, a saída seria:
```
20
6 25
3 11 22 50
9 15 33
7 40
```

### Vantagens da Modularização
- **Manutenibilidade:** Cada módulo tem responsabilidade específica
- **Reutilização:** Funções podem ser reutilizadas entre módulos
- **Testabilidade:** Cada módulo pode ser testado independentemente
- **Legibilidade:** Código mais organizado e fácil de entender
- **Compilação:** Recompilação seletiva apenas de módulos alterados

### Dependências entre Módulos
```
main.c -> estruturas.h, arquivo.h, livros.h, carregamento.h
arquivo.c -> estruturas.h
livros.c -> estruturas.h, arquivo.h
carregamento.c -> estruturas.h, arquivo.h, livros.h
```

### Arquivo de Teste
O arquivo `livros_teste.txt` contém dados de exemplo no formato correto:
```
7;Memorias Postumas de Bras Cubas;Machado de Assis;Bookman;4;2022;5;27,90
11;A insustentavel leveza do ser;Milan Kundera;Abril;3;2015;7;35,99
...
```

### Correções Implementadas
- **Funcionamento:** Corrigidas as operações de inserção e busca
- **Modularização:** Código dividido em módulos especializados
- **Interface:** Removidas mensagens desnecessárias do menu
- **Organização:** Estrutura mais limpa e profissional

### Testagem Recomendada
1. Compile o projeto: `make`
2. Execute: `./livros`
3. Teste cadastro manual (opção 1)
4. Teste busca (opção 2)
5. Carregue arquivo de teste (opção 6)
6. Liste todos os livros (opção 3)
7. Visualize árvore por níveis (opção 8)
8. Teste remoção (opção 5)
9. Verifique registros livres (opção 7)