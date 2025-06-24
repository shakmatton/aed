# Sistema de Biblioteca

**Autores:**

- Iuri F. Cordeiro
- Lukas Godoy

## Sumário

1. [Visão Geral](#visão-geral)
2. [Instalação](#instalação)
3. [Uso](#uso)
4. [Estrutura do Projeto](#estrutura-do-projeto)
5. [Formato de Dados](#formato-de-dados)
6. [Status de Implementação](#status-de-implementação)
7. [Funcionalidades](#funcionalidades)
8. [Funcionalidades Adicionais](#funcionalidades-adicionais)
9. [Comandos do Makefile](#comandos-do-makefile)

---

## Visão Geral

Este é um sistema de gerenciamento de biblioteca desenvolvido em C, que utiliza listas encadeadas armazenadas em arquivos binários para persistir informações de livros, usuários e empréstimos.

### Objetivos

- Gerenciar cadastro de livros e usuários.
- Controlar empréstimos e devoluções, mantendo histórico e atualizando disponibilidade.
- Importar registros em lote de arquivos texto.

---

## Instalação

### Dependências

- GCC ≥ 4.9
- Make

### Compilação

```bash
# Usando Makefile (recomendado)
make

# Compilação manual
gcc -Wall -Wextra -std=c99 -g -c main.c arquivo.c livros.c usuarios.c carregamento.c emprestimos.c
gcc -Wall -Wextra -std=c99 -g -o biblioteca main.o arquivo.o livros.o usuarios.o carregamento.o emprestimos.o
```

---

## Uso

```bash
# Executar o programa compilado
a ./biblioteca
```

O programa exibirá um menu em modo texto com as opções disponíveis.

---

## Estrutura do Projeto

### Arquivos de Cabeçalho (.h)

- **estruturas.h**: Definição das estruturas `Cabecalho`, `Livro`, `Usuario`, `Emprestimo`.
- **arquivo.h**: Protótipos das funções de manipulação de arquivos binários (abertura, cabeçalho, leitura e escrita).
- **livros.h**: Protótipos das funções relacionadas a operações de livros (inserção, busca, listagem).
- **usuarios.h**: Protótipos das funções relacionadas a usuários.
- **carregamento.h**: Protótipos das funções de importação em lote de arquivo texto.
- **emprestimos.h**: Protótipos das funções de empréstimos e devoluções.

### Arquivos de Implementação (.c)

- **main.c**: Função principal; apresenta menu e interface de usuário.
- **arquivo.c**: Implementação de manipulação de arquivos binários.
- **livros.c**: Implementação das operações de livros.
- **usuarios.c**: Implementação das operações de usuários.
- **carregamento.c**: Implementação de importação de registros de arquivo texto.
- **emprestimos.c**: Implementação de empréstimos e devoluções.

### Arquivos Auxiliares

- **Makefile**: Automação da compilação e execução.
- **README.md**: Documentação do projeto.

---

## Formato de Dados

### Arquivo Binário

O sistema utiliza listas encadeadas armazenadas em arquivo binário. O arquivo inicia com um registro de cabeçalho contendo:

- Posições de início das listas de livros, usuários e empréstimos.
- Contadores de registros.
- Posição do topo (próxima posição livre).

### Arquivo Texto para Carregamento

Cada linha do arquivo texto começa com um identificador de registro e campos separados por ponto-e-vírgula (`;`). Espaços em branco no início ou fim de cada campo são eliminados.

| Prefixo | Significado | Campos                                                            |
| ------- | ----------- | ----------------------------------------------------------------- |
| `L;`    | Livro       | código; título; autor; editora; edição; ano; exemplares           |
| `U;`    | Usuário     | código; nome                                                      |
| `E;`    | Empréstimo  | código\_usuario; código\_livro; data\_emprestimo; data\_devolucao |

#### Exemplo de Arquivo de Entrada

```txt
L;7;Memórias Póstumas de Brás Cubas;Machado de Assis;Bookman;4;2022;5
L;11;A Insustentável Leveza do Ser;Milan Kundera;Abril;3;2015;7
U;7;Maria dos Santos
E;7;13;11/03/2025;25/03/2025
E;22;4;15/05/2025;
```

---

## Status de Implementação

- ✅ Funcionalidades obrigatórias completas.
- ✅ Documentação das funções com pré-condições e pós-condições em cada módulo.

---

## Funcionalidades

1. **Gerenciamento de Livros**

   - Cadastrar livro
   - Imprimir dados do livro (por código)
   - Buscar livro por título (case-insensitive)
   - Listar todos os livros (código, título, autor, exemplares)
   - Calcular total de livros cadastrados

2. **Gerenciamento de Usuários**

   - Cadastrar usuário

3. **Sistema de Empréstimos**

   - Emprestar livro (registra data atual e decrementa exemplares)
   - Devolver livro (registra data atual e incrementa exemplares)
   - Listar empréstimos ativos

4. **Importação em Lote**

   - Carregar registros de livros, usuários e empréstimos de arquivo texto

---

## Funcionalidades Adicionais

- Mostrar informações internas do arquivo binário (estatísticas de cabeçalho).

---

## Comandos do Makefile

- `make` ou `make all`   : Compila o projeto
- `make clean`          : Remove arquivos objeto e executável
- `make cleanall`       : Remove tudo, inclusive arquivos de dados gerados
- `make run`            : Compila e executa o programa
