#ifndef CLIENTE_H
#define CLIENTE_H

#include "sqlite3.h"

#define TAM_NOME 60
#define TAM_CNPJ 20


typedef struct{
    int id;
    char nome[TAM_NOME];
    char cnpj[TAM_CNPJ];
    float limite_credito;
} Cliente;

// inicializa a conexão com o banco de dados e cria a tabela se não existir
int inicializar_banco();
void fechar_banco();

// operações do crud direto no banco de dados
int criar_cliente(const char *nome, const char *cnpj, float limite);
void listar_clientes();
int atualizar_limite(int id, float novo_limite);
int deletar_cliente(int id);

void menu();
int validar_formato_cnpj(const char *cnpj);

#endif