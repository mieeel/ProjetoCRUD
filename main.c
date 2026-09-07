#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"

void limpar_buffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

int main(){
    if (!inicializar_banco()) {
        printf("Falha ao inicializar banco de dados. Encerrando...\n");
        return 1;
    }
    int opcao = 0;

    do {
        menu();
        
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

            int res = criar_cliente(nome, cnpj, limite);
            if (res > 0) {
                printf("[SUCESSO] Cliente cadastrado com ID %d!\n", res);
            } else if (res == -2) {
                printf("[FALHA] Cadastro cancelado devido ao CNPJ invalido.\n");
            } else if (res == -3) {
                printf("[FALHA] Cadastro cancelado devido ao limite negativo.\n");
            } else {
                printf("[ERRO] Falha ao inserir no banco de dados.\n");
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
            fechar_banco();
            printf("Encerrando o sistema...\n");
        } else {
            printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 5);

    return 0;
}