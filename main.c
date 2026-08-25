#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"

void limpar_buffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

int main(){
    inicializar_sistema();

    criar_cliente("Ambev S.A", "07.526.557/0001-00", 5000000.0f);
    criar_cliente("Vale S.A.", "33.592.510/0001-54", 12000000.0f);

    int opcao = 0;

    do {
        printf("\n--- SISTEMA ITAU ATACADO (CRUD) ---\n");
        printf("1. Cadastrar novo cliente (Create)\n");
        printf("2. Listar todos os clientes (Read)\n");
        printf("3. Atualizar limite de credito (Update)\n");
        printf("4. Excluir cliente (Delete)\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        if (opcao == 1) {
            char nome[TAM_NOME], cnpj[TAM_CNPJ];
            float limite;

            printf("Nome da empresa: ");
            scanf(" %[^\n]", nome);
            printf("CNPJ: ");
            scanf(" %[^\n]", cnpj);
            printf("Limite de credito inicial (R$): ");
            scanf("%f", &limite);

            int id = criar_cliente(nome, cnpj, limite);
            if (id > 0) {
                printf("[SUCESSO] Cliente cadastrado com ID %d!\n", id);
            } else {
                printf("[ERRO] Nao foi possivel cadastrar (capacidade cheia).\n");
            }

        } else if (opcao == 2) {
            listar_clientes();

        } else if (opcao == 3) {
            int id;
            float novo_limite;

            printf("Informe o ID do cliente que deseja atualizar: ");
            scanf("%d", &id);
            printf("Informe o novo limite de credito (R$): ");
            scanf("%f", &novo_limite);

            if (atualizar_limite(id, novo_limite)) {
                printf("[SUCESSO] Limite atualizado com sucesso!\n", id);
            } else {
                printf("[ERRO] Cliente com ID %d nao encontrado.\n", id);
            }

        } else if (opcao == 4) {
            int id;
            printf("Informe o ID do cliente que deseja excluir: ");
            scanf("%d", &id);

            if (deletar_cliente(id)) {
                printf("[SUCESSO] Cliente ID %d removido com sucesso!\n", id);
            } else {
                printf("[ERRO] Cliente com ID %d nao encontrado.\n", id);
            }

        } else if (opcao == 5) {
            printf("Encerrando o sistema...\n");
        } else {
            printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 5);

    return 0;
}