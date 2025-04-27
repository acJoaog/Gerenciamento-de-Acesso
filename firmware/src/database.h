#ifndef DATABASE_H
#define DATABASE_H

void db_init();
void db_close();
void db_add_user();
void db_list_users();
void db_list_events();
int db_validate_user(const char* nome, const char* senha, int* is_admin);
void db_log_event(const char* usuario, int porta, const char* acao);

#endif