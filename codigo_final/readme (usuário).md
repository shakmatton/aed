# Sistema de Gerenciamento de Biblioteca
## Manual do Usuário

**Autores do Grupo:**
- Iuri F. Cordeiro
- Lukas Godoy

---

## Como Usar o Sistema

### Instalação e Execução

1. **Compilar o programa:**
   ```bash
   make
   ```

2. **Executar o sistema:**
   ```bash
   ./biblioteca
   ```

O programa abrirá um menu com as opções disponíveis.

---

## Opções do Sistema

### 1. Cadastrar Livro
**Como usar:** Escolha a opção 1 no menu principal.

O sistema solicitará:
- Código do livro (número único)
- Título do livro
- Nome do(s) autor(es)
- Nome da editora
- Número da edição
- Ano de publicação
- Quantidade de exemplares disponíveis

**Exemplo:**
```
Código: 7
Título: Dom Casmurro
Autor: Machado de Assis
Editora: Abril
Edição: 3
Ano: 2020
Exemplares: 5
```

### 2. Imprimir Dados do Livro
**Como usar:** Escolha a opção 2 no menu principal.

Digite o código do livro que deseja consultar. O sistema mostrará todas as informações do livro (título, autor, editora, edição, ano e exemplares disponíveis).

### 3. Listar Todos os Livros
**Como usar:** Escolha a opção 3 no menu principal.

O sistema exibirá uma lista com todos os livros cadastrados, mostrando:
- Código do livro
- Título
- Autor
- Quantidade de exemplares disponíveis

### 4. Buscar Livro por Título
**Como usar:** Escolha a opção 4 no menu principal.

Digite o título do livro que procura. O sistema não diferencia maiúsculas de minúsculas, então "dom casmurro" encontrará "Dom Casmurro".

### 5. Calcular Total de Livros
**Como usar:** Escolha a opção 5 no menu principal.

O sistema mostrará quantos livros estão cadastrados no total.

### 6. Cadastrar Usuário
**Como usar:** Escolha a opção 6 no menu principal.

O sistema solicitará:
- Código do usuário (número único)
- Nome completo do usuário

**Exemplo:**
```
Código: 15
Nome: João da Silva
```

### 7. Emprestar Livro
**Como usar:** Escolha a opção 7 no menu principal.

Digite:
- Código do usuário que está pegando o livro
- Código do livro a ser emprestado

O sistema automaticamente:
- Registra a data atual como data do empréstimo
- Diminui em 1 o número de exemplares disponíveis
- Verifica se há exemplares disponíveis (se não houver, mostra mensagem de erro)

### 8. Devolver Livro
**Como usar:** Escolha a opção 8 no menu principal.

Digite:
- Código do usuário que está devolvendo
- Código do livro a ser devolvido

O sistema automaticamente:
- Registra a data atual como data de devolução
- Aumenta em 1 o número de exemplares disponíveis

### 9. Listar Livros Emprestados
**Como usar:** Escolha a opção 9 no menu principal.

O sistema mostrará todos os empréstimos ativos (ainda não devolvidos), exibindo:
- Código do usuário
- Nome do usuário
- Código do livro
- Título do livro
- Data do empréstimo

### 10. Carregar Arquivo de Dados
**Como usar:** Escolha a opção 10 no menu principal.

Digite o nome do arquivo texto que contém os dados para importar em lote.

**Formato do arquivo:**
- Para livros: `L;código;título;autor;editora;edição;ano;exemplares`
- Para usuários: `U;código;nome`
- Para empréstimos: `E;código_usuário;código_livro;data_empréstimo;data_devolução`

**Exemplo de arquivo:**
```
L;7;Dom Casmurro;Machado de Assis;Abril;3;2020;5
L;11;O Cortiço;Aluísio Azevedo;Ática;2;2018;3
U;15;João da Silva
U;20;Maria Santos
E;15;7;01/06/2025;
```

**Observações:**
- Campos com espaços extras no início ou fim são automaticamente limpos
- Se a data de devolução estiver vazia, significa que o livro ainda não foi devolvido
- Linhas inválidas são ignoradas com mensagem de erro

### 0. Sair do Sistema
**Como usar:** Escolha a opção 0 no menu principal para encerrar o programa.

---

## Mensagens do Sistema

### Mensagens de Sucesso
- "Livro cadastrado com sucesso!"
- "Usuário cadastrado com sucesso!"
- "Empréstimo realizado com sucesso!"
- "Devolução realizada com sucesso!"

### Mensagens de Erro Comuns
- "Código já existe!" - Quando tenta cadastrar livro ou usuário com código já usado
- "Livro não encontrado!" - Quando o código do livro não existe
- "Usuário não encontrado!" - Quando o código do usuário não existe
- "Não há exemplares disponíveis" - Quando tenta emprestar livro sem estoque
- "Empréstimo não encontrado!" - Quando tenta devolver livro que não foi emprestado

---

## Dicas Importantes

1. **Códigos únicos:** Cada livro e usuário deve ter um código diferente de todos os outros.

2. **Arquivo de dados:** Sempre salve seus arquivos de importação com extensão `.txt` e use o formato correto com ponto-e-vírgula separando os campos.

3. **Empréstimos:** Um usuário pode emprestar vários livros, mas não pode emprestar o mesmo livro duas vezes sem devolver antes.

4. **Dados persistentes:** Todas as informações ficam salvas no arquivo `biblioteca.dat` e são mantidas entre execuções do programa.

5. **Backup:** Faça backup do arquivo `biblioteca.dat` regularmente para não perder seus dados.

---

## Resolução de Problemas

**Problema:** O programa não inicia
**Solução:** Verifique se compilou corretamente com `make` e se tem permissão para executar

**Problema:** "Erro ao abrir arquivo"
**Solução:** Verifique se tem permissão de escrita na pasta onde está executando o programa

**Problema:** Dados não aparecem após reiniciar
**Solução:** Verifique se o arquivo `biblioteca.dat` não foi deletado ou movido

**Problema:** Erro ao carregar arquivo texto
**Solução:** Verifique se o arquivo existe e se está no formato correto com campos separados por ponto-e-vírgula