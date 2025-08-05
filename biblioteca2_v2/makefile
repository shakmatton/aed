# Makefile para o Sistema de Gerenciamento de Livros (Modularizado)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = livros
SOURCES = main.c arquivo.c livros.c carregamento.c
HEADERS = estruturas.h arquivo.h livros.h carregamento.h
OBJECTS = $(SOURCES:.c=.o)

# Regra principal
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Regras para arquivos objeto específicos
main.o: main.c estruturas.h arquivo.h livros.h carregamento.h
	$(CC) $(CFLAGS) -c main.c -o main.o

arquivo.o: arquivo.c estruturas.h arquivo.h
	$(CC) $(CFLAGS) -c arquivo.c -o arquivo.o

livros.o: livros.c estruturas.h arquivo.h livros.h
	$(CC) $(CFLAGS) -c livros.c -o livros.o

carregamento.o: carregamento.c estruturas.h arquivo.h livros.h carregamento.h
	$(CC) $(CFLAGS) -c carregamento.c -o carregamento.o

# Regra genérica para arquivos objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpeza
clean:
	rm -f $(OBJECTS) $(TARGET) livros.dat

# Regra para execução
run: $(TARGET)
	./$(TARGET)

# Regras que não geram arquivos
.PHONY: clean run
