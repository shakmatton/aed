# Sistema de Biblioteca

Este é um sistema de gerenciamento de biblioteca desenvolvido em C, que utiliza estruturas de dados em arquivos binários para armazenar informações sobre livros, usuários e empréstimos.

## Estrutura do Projeto

O projeto foi modularizado em vários arquivos para facilitar manutenção e organização:

### Arquivos de Cabeçalho (.h)
- **`estruturas.h`** - Define todas as estruturas de dados (Cabecalho, Livro, Usuario, Emprestimo)
- **`arquivo.h`** - Protótipos das funções de manipulação de arquivos
- **`livros.h`** - Protótipos das funções relacionadas aos livros
- **`usuarios.h`** - Protótipos das funções relacionadas aos usuários
- **`carregamento.h`** - Protótipos das funções de carregamento de dados
- **`emprestimos.h`** - Protótipos das funções relacionadas aos empréstimos

### Arquivos de Implementação (.c)
- **`main.c`** - Programa principal com menu e interface do usuário
- **`arquivo.c`** - Implementação das funções de manipulação de arquivo (abertura, cabeçalho)
- **`livros.c`** - Implementação das funções de livros (cadastro, busca, listagem)
- **`usuarios.c`** - Implementação das funções de usuários
- **`carregamento.c`** - Implementação das funções de carregamento de dados de arquivo texto
- **`emprestimos.c`** - Implementação das funções de empréstimos e devoluções

### Arquivos Auxiliares
- **`Makefile`** - Para compilação automatizada
- **`README.md`** - Este arquivo de documentação

## Compilação

### Usando Make (Recomendado)
```bash
make
```

### Compilação Manual
```bash
gcc -Wall -Wextra -std=c99 -g -c main.c
gcc -Wall -Wextra -std=c99 -g -c arquivo.c
gcc -Wall -Wextra -std=c99 -g -c livros.c
gcc -Wall -Wextra -std=c99 -g -c usuarios.c
gcc -Wall -Wextra -std=c99 -g -c carregamento.c
gcc -Wall -Wextra -std=c99 -g -c emprestimos.c
gcc -Wall -Wextra -std=c99 -g -o biblioteca main.o arquivo.o livros.o usuarios.o carregamento.o emprestimos.o
```

## Execução

```bash
./biblioteca
```

Ou usando o Makefile:
```bash
make run
```

## Funcionalidades

### Gerenciamento de Livros
1. **Cadastrar livro** - Permite adicionar novos livros ao sistema
2. **Buscar livro por código** - Encontra um livro específico pelo seu código
3. **Buscar livro por título** - Localiza livros pelo título (busca case-insensitive)
4. **Listar todos os livros** - Mostra todos os livros cadastrados
5. **Calcular total de livros** - Exibe estatísticas de livros e exemplares

### Gerenciamento de Usuários
6. **Cadastrar usuário** - Registra novos usuários no sistema

### Sistema de Empréstimos
7. **Emprestar livro** - Realiza empréstimo de livro para usuário
8. **Devolver livro** - Processa devolução de livro emprestado
9. **Listar livros emprestados** - Mostra todos os empréstimos ativos

### Utilitários
10. **Carregar arquivo texto** - Importa dados de um arquivo texto formatado
11. **Mostrar informações do arquivo** - Exibe estatísticas do arquivo de dados (funcionalidade adicional)

## Formato do Arquivo de Dados

O sistema utiliza listas encadeadas armazenadas em arquivo binário. O arquivo possui:
- **Cabeçalho**: Controla posições e contadores das listas
- **Registros**: Livros, usuários e empréstimos organizados em listas encadeadas

## Formato do Arquivo Texto para Carregamento

### Livros
```
L;codigo;titulo;autor;editora;edicao;ano;exemplares
```

### Usuários
```
U;codigo;nome
```

### Empréstimos
```
E;codigo_usuario;codigo_livro;data_emprestimo;data_devolucao
```

## Exemplo de Arquivo de Entrada

```
L;1;Dom Casmurro;Machado de Assis;Ática;3;1997;5
L;2;O Cortiço;Aluísio Azevedo;Moderna;2;1995;3
L;3;1984;George Orwell;Companhia das Letras;1;2009;4
U;1;João Silva
U;2;Maria Santos
U;3;Pedro Oliveira
E;1;1;2024-01-15;2024-01-29
E;2;3;2024-02-01;2024-02-15
```

## Comandos do Makefile

- `make` ou `make all` - Compila o projeto
- `make clean` - Remove arquivos objeto e executável
- `make cleanall` - Remove tudo, incluindo arquivos de dados
- `make run` - Compila e executa o programa

## Arquitetura

O sistema utiliza:
- **Listas encadeadas** para organizar os dados em memória secundária
- **Arquivo binário** para persistência dos dados
- **Modularização** para organização e manutenibilidade do código
- **Cabeçalho de controle** para gerenciar ponteiros das listas encadeadas
- **Busca linear** através das listas para localização de registros

## Principais Características

- **Persistência de dados**: Todos os dados são salvos em arquivo binário
- **Busca flexível**: Busca por código numérico e por título (case-insensitive)
- **Controle de empréstimos**: Sistema completo de empréstimo e devolução
- **Estatísticas**: Cálculo automático de totais de livros e exemplares
- **Importação de dados**: Carregamento em lote via arquivo texto
- **Interface amigável**: Menu intuitivo com opções numeradas

## Vantagens da Modularização

1. **Manutenibilidade**: Cada módulo tem responsabilidade específica
2. **Reutilização**: Funções podem ser reutilizadas em outros projetos
3. **Testabilidade**: Cada módulo pode ser testado independentemente
4. **Legibilidade**: Código mais organizado e fácil de entender
5. **Compilação**: Apenas arquivos modificados precisam ser recompilados

## Estrutura de Dados

### Cabeçalho do Arquivo
- Posições das listas encadeadas (livros, usuários, empréstimos)
- Contadores de registros
- Posição do topo do arquivo (para novos registros)

### Listas Encadeadas
- **Livros**: Código, título, autor, editora, edição, ano, exemplares
- **Usuários**: Código, nome
- **Empréstimos**: Código do usuário, código do livro, datas

## Status de Implementação

### ✅ Funcionalidades Obrigatórias (Completas)
- [x] **Cadastrar livro** - Inserção completa de dados dos livros
- [x] **Imprimir dados do livro** - Busca e exibição por código
- [x] **Listar todos os livros** - Listagem com código, título, autor e exemplares
- [x] **Busca por título** - Localização de livros pelo título
- [x] **Calcular total** - Contagem total de livros cadastrados
- [x] **Cadastrar usuário** - Registro de usuários no sistema
- [x] **Emprestar livro** - Sistema completo de empréstimos com controle de exemplares
- [x] **Devolver livro** - Processo de devolução com atualização de exemplares
- [x] **Listar livros emprestados** - Relatório de empréstimos ativos
- [x] **Carregar arquivo** - Importação em lote via arquivo texto

### 🎯 Melhorias Futuras (Opcionais)
- [ ] Busca de usuários por nome
- [ ] Relatórios detalhados com filtros de data
- [ ] Validações mais robustas de entrada
- [ ] Sistema de multas por atraso
- [ ] Backup e restauração de dados
- [ ] Interface gráfica (GUI)
- [ ] Histórico completo de empréstimos/devoluções