#ifndef CLIENTE_H
#define CLIENTE_H

#define TAM_NOME 60
#define TAM_CNPJ 20
#define MAX_CLIENTES 100

typedef struct{
    int id;
    char nome[TAM_NOME];
    char cnpj[TAM_CNPJ];
    float limite_credito;
    int ativo; // 1 ativo | 0 inativo
} Cliente;

void inicializar_sistema();
int criar_cliente(const char *nome, const char *cnpj, float limite);
void listar_clientes();
Cliente* buscar_cliente_por_id(int id);
int atualizar_limite(int id, float novo_limite);
int deletar_cliente(int id);

void menu();
void salvar_em_arquivo();
void carregar_de_arquivo();

#endif