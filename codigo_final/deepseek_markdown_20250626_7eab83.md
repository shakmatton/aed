# Sistema de Gerenciamento de Biblioteca
**Autores:** Iuri F. Cordeiro, Lukas Godoy
## Sumário
- Visão Geral
- Instalação
- Uso
- Estrutura do Projeto
- Formato de Dados
- Status de Implementação
- Funcionalidades
- Funcionalidades Adicionais
- Comandos do Makefile
## Visão Geral
Este é um sistema de gerenciamento de biblioteca desenvolvido em C, que utiliza listas encadeadas armazenadas em arquivos binários para persistir informações de livros, usuários e empréstimos.
### Objetivos
- Gerenciar cadastro de livros e usuários.
- Controlar empréstimos e devoluções, mantendo histórico e atualizando disponibilidade.
- Importar registros em lote de arquivos texto.
## Instalação
### Dependências
- GCC (versão 4.9 ou superior)
- Make
### Compilação
```bash
# Usando Makefile (recomendado)
make
# Compilação manual
gcc -Wall -Wextra -std=c99 -g -c main.c arquivo.c livros.c usuarios.c carregamento.c emprestimos.c
gcc -Wall -Wextra -std=c99 -g -o biblioteca main.o arquivo.o livros.o usuarios.o carregamento.o emprestimos.o
```
## Uso
```bash
./biblioteca
```
O programa exibirá um menu em modo texto com as opções disponíveis.
## Estrutura do Projeto
### Arquivos de Cabeçalho (.h)
- `estruturas.h`: Definição das estruturas `Cabecalho`, `Livro`, `Usuario`, `Emprestimo`.
- `arquivo.h`: Protótipos das funções de manipulação de arquivos binários (abertura, cabeçalho, leitura e escrita).
- `livros.h`: Protótipos das funções relacionadas a operações de livros (inserção, busca, listagem).
- `usuarios.h`: Protótipos das funções relacionadas a usuários.
- `carregamento.h`: Protótipos das funções de importação em lote de arquivo texto.
- `emprestimos.h`: Protótipos das funções de empréstimos e devoluções.
### Arquivos de Implementação (.c)
- `main.c`: Função principal; apresenta menu e interface de usuário.
- `arquivo.c`: Implementação de manipulação de arquivos binários.
- `livros.c`: Implementação das operações de livros.
- `usuarios.c`: Implementação das operações de usuários.
- `carregamento.c`: Implementação de importação de registros de arquivo texto.
- `emprestimos.c`: Implementação de empréstimos e devoluções.
### Arquivos Auxiliares
- `Makefile`: Automação da compilação e execução.
- `README.md`: Documentação do projeto.
## Formato de Dados
### Arquivo Binário
O sistema utiliza listas encadeadas armazenadas em arquivo binário. O arquivo inicia com um registro de cabeçalho contendo:
- Posições de início das listas de livros, usuários e empréstimos.
- Contadores de registros.
- Posição do topo (próxima posição livre).
### Arquivo Texto para Carregamento
Cada linha do arquivo texto começa com um identificador de registro e campos separados por ponto-e-vírgula (;). Espaços em branco no início ou fim de cada campo são eliminados.
| Prefixo | Significado | Campos |
|---------|-------------|--------|
| `L;`    | Livro       | código; título; autor; editora; edição; ano; exemplares |
| `U;`    | Usuário     | código; nome |
| `E;`    | Empréstimo  | código_usuario; código_livro; data_emprestimo; data_devolucao |
**Exemplo de Arquivo de Entrada:**
```
L;7;Memórias Póstumas de Brás Cubas;Machado de Assis;Bookman;4;2022;5
L;11;A Insustentável Leveza do Ser;Milan Kundera;Abril;3;2015;7
U;7;Maria dos Santos
E;7;13;11/03/2025;25/03/2025
E;22;4;15/05/2025;
```
## Status de Implementação
✅ Funcionalidades obrigatórias completas.
✅ Documentação das funções com pré-condições e pós-condições em cada módulo.
## Funcionalidades
### Gerenciamento de Livros
- **Cadastrar livro**: Insere um novo livro no sistema.
- **Imprimir dados do livro (por código)**: Mostra todas as informações de um livro a partir de seu código.
- **Buscar livro por título (case-insensitive)**: Localiza um livro pelo título, ignorando maiúsculas e minúsculas.
- **Listar todos os livros**: Exibe código, título, autor e quantidade de exemplares de todos os livros.
- **Calcular total de livros cadastrados**: Mostra o número total de livros no sistema.
### Gerenciamento de Usuários
- **Cadastrar usuário**: Insere um novo usuário no sistema.
### Sistema de Empréstimos
- **Emprestar livro**: Registra um empréstimo com a data atual e decrementa o número de exemplares disponíveis. Se não houver exemplares, exibe mensagem de erro.
- **Devolver livro**: Registra a devolução com a data atual e incrementa o número de exemplares disponíveis.
- **Listar empréstimos ativos**: Mostra os empréstimos que ainda não foram devolvidos, com códigos de usuário e livro, nomes, títulos e datas.
### Importação em Lote
- **Carregar registros**: Importa livros, usuários e empréstimos de um arquivo texto.
## Funcionalidades Adicionais
- **Mostrar informações do cabeçalho**: Exibe as estatísticas do cabeçalho do arquivo binário (posições iniciais, contadores, topo).
## Comandos do Makefile
- `make` ou `make all`: Compila o projeto.
- `make clean`: Remove arquivos objeto.
- `make cleanall`: Remove arquivos objeto, executável e os arquivos de dados gerados (`livros.bin`, `usuarios.bin`, `emprestimos.bin`).
- `make run`: Compila e executa o programa.
