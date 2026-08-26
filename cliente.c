#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "cliente.h"

static Cliente banco_clientes[MAX_CLIENTES];
static int total_clientes = 0;
static int proximo_id = 1;

void inicializar_sistema(){
    total_clientes = 0;
    proximo_id = 1;
}
int criar_cliente(const char *nome, const char *cnpj, float limite){
    if(!validar_formato_cnpj(cnpj)){
        printf("[ERRO] formato de cnpj invalido, use XX.XXX/XXXX-XX\n");
        return -2; // código de erro de cnpj
    }
    if(limite < 0){
        printf("[ERRO] limite negativo\n");
        return -3; // código de erro de limite
    }
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

    salvar_em_arquivo();
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
        salvar_em_arquivo();
        return 1; // sucesso
    }
    return 0; // nao encontrado
}
int deletar_cliente(int id){
    Cliente *c = buscar_cliente_por_id(id);
    if(c != NULL){
        c->ativo = 0;
        salvar_em_arquivo();
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

    fprintf(arquivo, "%d\n", proximo_id);
    for(int i = 0; i < total_clientes; i++){
        if(banco_clientes[i].ativo == 1){
        fprintf(arquivo, "%d;%s;%s;%.2f;%d\n", banco_clientes[i].id, banco_clientes[i].nome, banco_clientes[i].cnpj, banco_clientes[i].limite_credito, banco_clientes[i].ativo);
        }
    }

    fclose(arquivo);
}
void carregar_de_arquivo(){
    FILE *arquivo = fopen("clientes.txt","r");
    if(arquivo == NULL){
        return; // arquivo ainda não existe
    }

    char linha[256];
    if(fgets(linha, sizeof(linha), arquivo) != NULL){
        proximo_id = atoi(linha);
    }
    
    total_clientes = 0;

    while(fgets(linha, sizeof(linha), arquivo) != NULL && total_clientes < MAX_CLIENTES){
        char *str_id = strtok(linha, ";");
        char *nome = strtok(NULL, ";");
        char *cnpj = strtok(NULL, ";");
        char *str_limite = strtok(NULL, ";");
        char *str_ativo = strtok(NULL, ";\n");


        if(str_id && nome && cnpj && str_limite && str_ativo){
            banco_clientes[total_clientes].id = atoi(str_id);
            strncpy(banco_clientes[total_clientes].nome, nome, TAM_NOME - 1);
            banco_clientes[total_clientes].nome[TAM_NOME - 1] = '\0';

            strncpy(banco_clientes[total_clientes].cnpj, cnpj, TAM_CNPJ - 1);
            banco_clientes[total_clientes].cnpj[TAM_CNPJ - 1] = '\0';

            banco_clientes[total_clientes].limite_credito = atof(str_limite);
            banco_clientes[total_clientes].ativo = atoi(str_ativo);

            total_clientes++;
        }
    }
    fclose(arquivo);
}
int validar_formato_cnpj(const char *cnpj){
    if(strlen(cnpj) != 18){
        return 0; // tam invalido
    }
    if(cnpj[2] != '.' || cnpj[6] != '.' || cnpj[10] != '/' || cnpj[15] != '-'){
        return 0; // pontuacao invalida
    }
    int posicoes_numeros[14] = {0, 1, 3, 4, 5, 7, 8, 9, 11, 12, 13, 14, 16, 17};
    for(int i = 0; i < 14; i++){
        int pos = posicoes_numeros[i];
        if(!isdigit(cnpj[pos])){
            return 0; // nao e digito
        }
    }

    return 1; // cnpj valido
}