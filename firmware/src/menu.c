#include <stdio.h>
#include "database.h"
#include "controle.h"

void abrir_menu() {

    // cria menu no terminal
    int op;
    printf("\nMenu:\n");
    printf("1. Cadastrar usuário\n");
    printf("2. Listar usuários\n");
    printf("3. Exibir Logs (admin)\n");
    printf("4. Liberar porta 1\n");
    printf("5. Liberar porta 2\n");
    printf("6. Ler estado das Portas\n");
    printf("Escolha: "); 
    fflush(stdout);  // força a saída imediata
    scanf("%d", &op);
    getchar(); // limpa buffer
    printf("\n");

    switch(op) {
        case 1: db_add_user(); break;
        case 2: db_list_users(); break;
        case 3: db_list_events(); break;
        case 4: liberar_porta(0x34); break;
        case 5: liberar_porta(0x35); break;
        case 6: ler_estado_registradores(); break;
        default: printf("Opção inválida.\n");
    }
}