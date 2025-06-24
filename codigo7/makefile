# Makefile para o Sistema de Biblioteca

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Nome do executável
TARGET = biblioteca

# Arquivos objeto
OBJS = main.o arquivo.o livros.o usuarios.o carregamento.o emprestimos.o

# Regra principal
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regras individuais para cada arquivo objeto
main.o: main.c estruturas.h arquivo.h livros.h usuarios.h carregamento.h emprestimos.h
	$(CC) $(CFLAGS) -c main.c

arquivo.o: arquivo.c arquivo.h estruturas.h
	$(CC) $(CFLAGS) -c arquivo.c

livros.o: livros.c livros.h estruturas.h arquivo.h
	$(CC) $(CFLAGS) -c livros.c

usuarios.o: usuarios.c usuarios.h estruturas.h arquivo.h
	$(CC) $(CFLAGS) -c usuarios.c

carregamento.o: carregamento.c carregamento.h estruturas.h livros.h usuarios.h arquivo.h
	$(CC) $(CFLAGS) -c carregamento.c

emprestimos.o: emprestimos.c emprestimos.h estruturas.h arquivo.h
	$(CC) $(CFLAGS) -c emprestimos.c

# Limpeza
clean:
	rm -f $(OBJS) $(TARGET)

# Limpeza completa (inclui arquivos de dados)
cleanall: clean
	rm -f biblioteca.dat

# Regra para executar
run: $(TARGET)
	./$(TARGET)

# Regras que não criam arquivos
.PHONY: all clean cleanall run