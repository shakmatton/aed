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

### Arquivos de Implementação (.c)
- **`main.c`** - Programa principal com menu e interface do usuário
- **`arquivo.c`** - Implementação das funções de manipulação de arquivo (abertura, cabeçalho)
- **`livros.c`** - Implementação das funções de livros (cadastro, busca, listagem)
- **`usuarios.c`** - Implementação das funções de usuários
- **`carregamento.c`** - Implementação das funções de carregamento de dados de arquivo texto

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
gcc -Wall -Wextra -std=c99 -g -o biblioteca main.o arquivo.o livros.o usuarios.o carregamento.o
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

1. **Cadastrar livro** - Permite adicionar novos livros ao sistema
2. **Buscar livro por código** - Encontra um livro específico pelo seu código
3. **Listar todos os livros** - Mostra todos os livros cadastrados
4. **Mostrar informações do arquivo** - Exibe estatísticas do arquivo de dados
5. **Carregar arquivo texto** - Importa dados de um arquivo texto formatado

## Formato do Arquivo de Dados

O sistema utiliza listas encadeadas armazenadas em arquivo binário. O arquivo possui:
- **Cabeçalho**: Controla posições e contadores
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

### Empréstimos (a implementar)
```
E;codigo_usuario;codigo_livro;data_emprestimo;data_devolucao
```

## Exemplo de Arquivo de Entrada
```
L;1;Dom Casmurro;Machado de Assis;Ática;3;1997;5
L;2;O Cortiço;Aluísio Azevedo;Moderna;2;1995;3
U;1;João Silva
U;2;Maria Santos
```

## Comandos do Makefile

- `make` ou `make all` - Compila o projeto
- `make clean` - Remove arquivos objeto e executável
- `make cleanall` - Remove tudo, incluindo arquivos de dados
- `make run` - Compila e executa o programa

## Arquitetura

O sistema utiliza:
- **Listas encadeadas** para organizar os dados
- **Arquivo binário** para persistência
- **Modularização** para organização do código
- **Cabeçalho de controle** para gerenciar as listas

## Vantagens da Modularização

1. **Manutenibilidade**: Cada módulo tem responsabilidade específica
2. **Reutilização**: Funções podem ser reutilizadas em outros projetos
3. **Testabilidade**: Cada módulo pode ser testado independentemente
4. **Legibilidade**: Código mais organizado e fácil de entender
5. **Compilação**: Apenas arquivos modificados precisam ser recompilados

## Próximas Implementações

- [ ] Sistema completo de empréstimos
- [ ] Busca de usuários
- [ ] Relatórios de empréstimos
- [ ] Validações mais robustas
- [ ] Interface melhorada
