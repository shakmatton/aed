#ifndef UTILS_H
#define UTILS_H

// Funções utilitárias para entrada de dados
int read_int(const char *prompt);
float read_float(const char *prompt);
void trim_string(char *str);

// Funções utilitárias para formatação
void formatar_preco(float preco, char *buffer);

#endif
