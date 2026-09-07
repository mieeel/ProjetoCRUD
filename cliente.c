#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "cliente.h"

static sqlite3 *db = NULL;

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

int inicializar_banco(){
    // abre ou cria o arquivo "banco_clientes.db"
    int res = sqlite3_open("banco_clientes.db", &db);
    if(res != SQLITE_OK){
        printf("[ERRO] nao foi possivel abrir o banco de dados\n");
        return 0;
    }
    // Comando SQL para criar a tabela caso ela não exista
    const char *sql_create = 
        "CREATE TABLE IF NOT EXISTS clientes ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nome TEXT NOT NULL, "
        "cnpj TEXT NOT NULL, "
        "limite_credito REAL NOT NULL);";

    char *err_msg = NULL;
    res = sqlite3_exec(db, sql_create, 0, 0, &err_msg);

    if(res != SQLITE_OK){
    printf("[ERRO SQL] erro ao criar tabela: %s\n", err_msg);
    sqlite3_free(err_msg);
    return 0;
    }

    return 1;
}

void fechar_banco(){
    if(db != NULL){
        sqlite3_close(db);
        db = NULL;
    }
}

// CREATE - INSERT INTO
int criar_cliente(const char *nome, const char *cnpj, float limite){
    if(!validar_formato_cnpj(cnpj)){
        printf("[ERRO] formato de cnpj invalido, use XX.XXX.XXX/XXXX-XX\n");
        return -2; // código de erro de cnpj
    }
    if(limite < 0){
        printf("[ERRO] limite negativo\n");
        return -3; // código de erro de limite
    }
    
    const char *sql = "INSERT INTO clientes (nome, cnpj, limite_credito) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;

    // prepara a instrução sql
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK){
        return -1;
    }

    // associa os parametros com segurança (evita sql injection)
    sqlite3_bind_text(stmt, 1, nome, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, cnpj, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, limite);

    int status = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if(status == SQLITE_DONE){
        // retorna o id gerado
        return (int)sqlite3_last_insert_rowid(db);
    }

    return -1;
}

// READ - SELECT
void listar_clientes(){
    const char *sql = "SELECT id, nome, cnpj, limite_credito FROM clientes;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("[ERRO SQL] nao foi possivel consultar os clientes\n");
        return;
    }

    printf("\n=== LISTA DE CLIENTES (BANCO DE DADOS SQLITE) ===\n");
    int encontrados = 0;

    // percorre cada linha retornada pela consulta
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *nome = sqlite3_column_text(stmt, 1);
        const unsigned char *cnpj = sqlite3_column_text(stmt, 2);
        double limite = sqlite3_column_double(stmt, 3);

        printf("ID: %d | Nome: %-20s | CNPJ: %-18s | Limite: R$ %.2f\n", id, nome, cnpj, limite);
        encontrados++;
    }

    if (encontrados == 0) {
        printf("nenhum cliente cadastrado no banco de dados.\n");
    }
    printf("==================================================\n");

    sqlite3_finalize(stmt);
}

int atualizar_limite(int id, float novo_limite) {
    const char *sql = "UPDATE clientes SET limite_credito = ? WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return 0;

    sqlite3_bind_double(stmt, 1, novo_limite);
    sqlite3_bind_int(stmt, 2, id);

    sqlite3_step(stmt);
    int alterados = sqlite3_changes(db); // Quantas linhas foram modificadas
    sqlite3_finalize(stmt);

    return alterados > 0;
}

int deletar_cliente(int id) {
    const char *sql = "DELETE FROM clientes WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return 0;

    sqlite3_bind_int(stmt, 1, id);

    sqlite3_step(stmt);
    int alterados = sqlite3_changes(db);
    sqlite3_finalize(stmt);

    return alterados > 0;
}

void menu(){
    printf("\n--- SISTEMA MIGUEL (SQLITE) ---\n");
    printf("1. Cadastrar novo cliente (Create)\n");
    printf("2. Listar todos os clientes (Read)\n");
    printf("3. Atualizar limite de credito (Update)\n");
    printf("4. Excluir cliente (Delete)\n");
    printf("5. Sair\n");
    printf("Escolha uma opcao: ");
}