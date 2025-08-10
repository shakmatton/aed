#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

/**
 * Propósito: Leitura segura de inteiro com prompt único
 * Pré-condições: prompt deve ser uma string válida
 * Pós-condições: retorna um inteiro válido digitado pelo usuário
 */
int read_int(const char *prompt) {
    int val;
    int resultado;

    while (1) {
        printf("%s", prompt);
        fflush(stdout);

        resultado = scanf("%d", &val);

        if (resultado == 1) {
            // Leitura bem-sucedida
            return val;
        } else {
            // Leitura falhou - limpar buffer e tentar novamente
            printf("Entrada inválida. Digite apenas dígitos (ex.: 123).\n");

            // Limpar todo o buffer até encontrar \n
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}

/**
 * Propósito: Lê um valor float do usuário, aceitando vírgula ou ponto
 * Pré-condições: prompt deve ser uma string válida
 * Pós-condições: retorna um float válido digitado pelo usuário
 */
float read_float(const char *prompt) {
    char input[50];
    float val;
    int resultado;

    while (1) {
        printf("%s", prompt);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) != NULL) {
            // Remove quebra de linha
            input[strcspn(input, "\n")] = 0;

            // Trim espaços
            trim_string(input);

            // Converter vírgula para ponto para processamento
            for (int i = 0; input[i]; i++) {
                if (input[i] == ',') {
                    input[i] = '.';
                    break;
                }
            }

            resultado = sscanf(input, "%f", &val);

            if (resultado == 1 && val >= 0) {
                return val;
            } else {
                printf("Entrada inválida. Digite um número válido.\n");
            }
        } else {
            printf("Erro na leitura. Tente novamente.\n");
        }
    }
}

/**
 * Propósito: Remove espaços em branco do início e fim de uma string
 * Pré-condições: str deve ser uma string válida
 * Pós-condições: str terá espaços removidos do início e fim
 */
void trim_string(char *str) {
    if (str == NULL) return;

    // Remove espaços do final
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }

    // Remove espaços do início
    int inicio = 0;
    while (str[inicio] && isspace(str[inicio])) {
        inicio++;
    }

    if (inicio > 0) {
        int i = 0;
        while (str[inicio + i]) {
            str[i] = str[inicio + i];
            i++;
        }
        str[i] = '\0';
    }
}

/**
 * Propósito: Converte um float para string com vírgula como separador decimal
 * Pré-condições: preco deve ser um valor válido, buffer deve ter tamanho suficiente
 * Pós-condições: buffer conterá o preço formatado com vírgula
 */
void formatar_preco(float preco, char *buffer) {
    sprintf(buffer, "%.2f", preco);

    // Converter ponto para vírgula
    for (int i = 0; buffer[i]; i++) {
        if (buffer[i] == '.') {
            buffer[i] = ',';
            break;
        }
    }
}
