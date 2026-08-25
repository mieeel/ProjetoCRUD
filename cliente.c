#include <stdio.h>
#include <string.h>
#include "cliente.h"

static Cliente banco_clientes[MAX_CLIENTES];
static int total_clientes = 0;
static int proximo_id = 1;

void inicializar_sistema(){
    total_clientes = 0;
    proximo_id = 1;
}
int criar_cliente(const char *nome, const char *cnpj, float limite){
    if(total_clientes >= MAX_CLIENTES){
        return -1;
    }
    Cliente novo;
    novo.id = proximo_id++;
    strncpy(novo.nome, nome, TAM_NOME - 1);
    novo.nome[TAM_NOME - 1] = '\0';
    strncpy(novo.cnpj, cnpj, TAM_CNPJ - 1);
    novo.cnpj[TAM_CNPJ - 1] = '\0';
    novo.limite_credito = limite;
    novo.ativo = 1;

    banco_clientes[total_clientes] = novo;
    total_clientes++;

    return novo.id;
}
void listar_clientes(){
    int encontrados = 0;
    for(int i = 0; i < total_clientes; i++){
        if(banco_clientes[i].ativo){
            printf("ID: %d | NOME: %-20s | CNPJ: %-18s | LIMITE: R$%.2f\n", banco_clientes[i].id, banco_clientes[i].nome, banco_clientes[i].cnpj, banco_clientes[i].limite_credito);
            encontrados++;
        }
    }
    if(encontrados == 0){
        printf("nenhum cliente ativo cadastrado\n");
    }
    printf("\n");
}
Cliente* buscar_cliente_por_id(int id){
    for(int i = 0; i < total_clientes; i++){
        if(banco_clientes[i].id == id && banco_clientes[i].ativo){
            return &banco_clientes[i];
        }
    }
    return NULL;
}
int atualizar_limite(int id, float novo_limite){
    Cliente *c = buscar_cliente_por_id(id);
    if(c != NULL){
        c->limite_credito = novo_limite;
        return 1; // sucesso
    }
    return 0; // nao encontrado
}
int deletar_cliente(int id){
    Cliente *c = buscar_cliente_por_id(id);
    if(c != NULL){
        c->ativo = 0;
        return 1; // sucesso
    }
    return 0; // nao encontrado
}
void menu(){
    printf("\n--- SISTEMA ITAU ATACADO (CRUD) ---\n");
    printf("1. Cadastrar novo cliente (Create)\n");
    printf("2. Listar todos os clientes (Read)\n");
    printf("3. Atualizar limite de credito (Update)\n");
    printf("4. Excluir cliente (Delete)\n");
    printf("5. Sair\n");
    printf("Escolha uma opcao: ");
}
void salvar_em_arquivo(){
    FILE *arquivo = fopen("clientes.txt","w");
    if(arquivo == NULL){
        printf("[ERRO] nao foi possivel abrir o arquivo para escrita\n");
        return;
    }

    fprint(arquivo, "%d\n", proximo_id);
    for(int i = 0; i < total_clientes; i++){
        fprint(arquivo, "%d;%s;%s;%.2f;%d\n", banco_clientes[i].id, banco_clientes[i].nome, banco_clientes[i].cnpj, banco_clientes[i].limite_credito, banco_clientes[i].ativo);
    }

    fclose(arquivo);
}
void carregar_de_arquivo(){
    FILE *arquivo = fopen("clientes.txt","r");
    if(arquivo == NULL){
        return; // arquivo ainda não existe
    }

    total_clientes = 0;

    if(fscanf(arquivo, "%d\n", &proximo_id) != 1){
        proximo_id = 1;
    }

    while(total_clientes < MAX_CLIENTES && fscanf(arquivo, "%d;%59[^;];%19[^;];%f;%d\n"))
}