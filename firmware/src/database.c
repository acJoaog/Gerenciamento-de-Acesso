#include "database.h"
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static sqlite3* db;

// função de inicialização do database
void db_init() {

    // define caminho para arquivo de dados
    if (sqlite3_open("data/acesso.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        exit(1);
    }
    
    // cria tabelas na primeira inicialiazação
    const char* create_users =
        "CREATE TABLE IF NOT EXISTS usuarios ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nome TEXT NOT NULL UNIQUE, "
        "senha TEXT NOT NULL, "
        "admin INTEGER NOT NULL);";
        
    const char* create_events =
        "CREATE TABLE IF NOT EXISTS eventos ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "usuario TEXT NOT NULL, "
        "porta INTEGER NOT NULL, "
        "acao TEXT NOT NULL, "
        "data_hora DATETIME DEFAULT CURRENT_TIMESTAMP);";
        
    char* err_msg = NULL;
    if (sqlite3_exec(db, create_users, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Erro ao criar tabela de usuários: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    if (sqlite3_exec(db, create_events, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Erro ao criar tabela de eventos: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

// função para fechar conexão ao database
void db_close() {
    sqlite3_close(db);
}

// limpar buffer
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// cadastro de usuarios
void db_add_user() {
    printf("Cadastrando Usuário.\n");
    fflush(stdout);  // força a saída imediata

    char nome[50], senha[50]; 
    int admin;

    // obtendo infos e criando user
    printf("Nome: ");
    fflush(stdout);  // força a saída imediata
    if (fgets(nome, sizeof(nome), stdin) == NULL) {
        fprintf(stderr, "Erro ao ler nome\n");
        return;
    }
    nome[strcspn(nome, "\n")] = '\0';

    printf("Senha: ");
    fflush(stdout);  // força a saída imediata
    if (fgets(senha, sizeof(senha), stdin) == NULL) {
        fprintf(stderr, "Erro ao ler senha\n");
        return;
    }
    senha[strcspn(senha, "\n")] = '\0';

    printf("É admin? (1 = sim, 0 = não): ");
    fflush(stdout);  // força a saída imediata
    if (scanf("%d", &admin) != 1) {
        fprintf(stderr, "\nEntrada inválida para admin\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // preparando querry
    const char* sql = "INSERT INTO usuarios (nome, senha, admin) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "\nErro ao preparar SQL: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    // inicializando variaveis da querry
    sqlite3_bind_text(stmt, 1, nome, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, senha, -1, SQLITE_STATIC); 
    sqlite3_bind_int(stmt, 3, admin);

    // executa e trata resultado da querry
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "\nErro ao cadastrar usuário: %s\n", sqlite3_errmsg(db));
    } else {
        printf("\nUsuário cadastrado!\n");
    }
    
    sqlite3_finalize(stmt);
}

int db_validate_user(const char* nome, const char* senha, int* is_admin) {
    const char* sql = "SELECT senha, admin FROM usuarios WHERE nome = ?;";
    sqlite3_stmt* stmt;
    int result = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "\nErro ao preparar SQL: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    if (sqlite3_bind_text(stmt, 1, nome, -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "\nErro ao bind do nome: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* senha_armazenada = (const char*)sqlite3_column_text(stmt, 0);
        *is_admin = sqlite3_column_int(stmt, 1);

        if (senha_armazenada && strcmp(senha_armazenada, senha) == 0) {
            result = 1;
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

void db_list_users() {
    const char* sql = "SELECT nome FROM usuarios;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "\nErro ao buscar lista de usuarios: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("Usuários cadastrados:\n");
    fflush(stdout);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("- %s\n", sqlite3_column_text(stmt, 0));
        fflush(stdout);
    }

    sqlite3_finalize(stmt);
}

void db_list_events() {
    char nome[50], senha[50];
    int is_admin;

    printf("Nome de administrador: ");
    fflush(stdout);
    if (fgets(nome, sizeof(nome), stdin) == NULL) {
        fprintf(stderr, "Erro ao ler nome\n");
        return;
    }
    nome[strcspn(nome, "\n")] = '\0';

    printf("Senha: ");
    fflush(stdout);
    if (fgets(senha, sizeof(senha), stdin) == NULL) {
        fprintf(stderr, "Erro ao ler senha\n");
        return;
    }
    senha[strcspn(senha, "\n")] = '\0';

    if (!db_validate_user(nome, senha, &is_admin) || !is_admin) {
        fprintf(stderr, "\nAcesso negado: usuário inválido ou sem privilégios.\n");
        return;
    }

    const char* sql = "SELECT usuario, porta, acao, data_hora FROM eventos ORDER BY data_hora DESC;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "\nErro ao preparar SQL: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\nEventos registrados:\n");
    fflush(stdout);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* user = (const char*)sqlite3_column_text(stmt, 0);
        int porta = sqlite3_column_int(stmt, 1);
        const char* acao = (const char*)sqlite3_column_text(stmt, 2);
        const char* data = (const char*)sqlite3_column_text(stmt, 3);
        
        printf("- %s %s a porta %d em %s.\n", user, acao, porta, data);
        fflush(stdout);
    }
    
    sqlite3_finalize(stmt);
}

void db_log_event(const char* usuario, int porta, const char* acao) {
    const char* sql = "INSERT INTO eventos (usuario, porta, acao) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar SQL de Log: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_text(stmt, 1, usuario, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, porta);
    sqlite3_bind_text(stmt, 3, acao, -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Erro ao executar SQL: %s\n", sqlite3_errmsg(db));
    }
    
    sqlite3_finalize(stmt);
}