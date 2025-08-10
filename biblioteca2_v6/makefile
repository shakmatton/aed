# Makefile para Sistema de Gerenciamento de Livros
# Versão Modularizada e Balanceada

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Nome do executável
TARGET = livros

# Arquivos objeto
OBJS = main.o arquivo.o livros.o arvore.o carregamento.o utils.o

# alvo padrão
all: $(TARGET)

# Regra principal
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regras para cada arquivo objeto
main.o: main.c estruturas.h arquivo.h livros.h arvore.h carregamento.h
	$(CC) $(CFLAGS) -c main.c

arquivo.o: arquivo.c arquivo.h estruturas.h
	$(CC) $(CFLAGS) -c arquivo.c

livros.o: livros.c livros.h arquivo.h arvore.h utils.h estruturas.h
	$(CC) $(CFLAGS) -c livros.c

arvore.o: arvore.c arvore.h arquivo.h utils.h estruturas.h
	$(CC) $(CFLAGS) -c arvore.c

carregamento.o: carregamento.c carregamento.h estruturas.h arquivo.h arvore.h utils.h
	$(CC) $(CFLAGS) -c carregamento.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

# Regra para compilar e executar
run: $(TARGET)
	./$(TARGET)

# Limpeza dos arquivos compilados
clean:
	rm -f $(OBJS) $(TARGET)

# Regras que não correspondem a arquivos
.PHONY: all run clean
