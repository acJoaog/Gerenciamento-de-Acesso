#include "menu.h"
#include "database.h"

int main() {
    db_init(); // inicializa database

    while (1) {
        abrir_menu(); // inicializa menu
    }

    db_close(); // fecha conexão com SQlite
    return 0;
}