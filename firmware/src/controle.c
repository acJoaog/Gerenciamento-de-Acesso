#include "database.h"

#include <stdio.h>
#include <modbus/modbus.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define MODBUS_SLAVE_ID 0x01        // endereço do escravo

#define PORT_OPEN 0xFF              
#define PORT_CLOSED 0x00            
#define SERIAL_PORT "/dev/ttyUSB0"  // porta serial (USB)
#define SLAVE_ESP8266 0x01          // ID do ESP8266
#define BAUD_RATE 9600              // taxa de comunicação

void liberar_porta(int PORT_REGISTER){

    printf("Para abrir a porta faça login.\n");
    fflush(stdout);  // força a saída imediata

    // variaveis para autenticação
    char nome[50], senha[50];
    int is_admin;
    int port_id = 0;

    // traduzindo endereço para numero da porta

    if(PORT_REGISTER == 0x34){
        port_id = 1;
    }

    if(PORT_REGISTER == 0x35){
        port_id = 2;
    }

    // verificando credenciais
    printf("Nome de usuario: ");
    fflush(stdout);  // força a saída imediata
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Senha: ");
    fflush(stdout);  // força a saída imediata
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = 0;

    // tratando resultado

    // acesso negado
    if (!db_validate_user(nome, senha, &is_admin)) {
        printf("Acesso negado: usuário inválido.\n");
        return;
    }

    // acesso liberado

    // configurando modbus
    modbus_t *ctx = modbus_new_rtu(SERIAL_PORT, 9600, 'N', 8, 1);
    if (ctx == NULL) {
        fprintf(stderr, "Falha ao criar contexto Modbus\n");
        return;
    }

    modbus_set_slave(ctx, SLAVE_ESP8266); 

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Falha na conexão: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return;
    }

    // envia comando para abrir Porta 1 (coil 0x34 = 0xFF) ou Porta 2 (coil 0x35 = 0xFF)
    if (modbus_write_bit(ctx, PORT_REGISTER, PORT_OPEN) == -1) {
        fprintf(stderr, "Falha ao escrever: %s\n", modbus_strerror(errno));
    } else {
        printf("Abrindo porta!\n");
        db_log_event(nome, port_id, "abriu");
    }

    // aguarda 5 segundos
    sleep(5);

    // envia comando para fechar Porta 1 (coil 0x34 = 0x00) ou Porta 2 (coil 0x35 = 0x00)
    if (modbus_write_bit(ctx, PORT_REGISTER, PORT_CLOSED) == -1) {
        fprintf(stderr, "Falha ao escrever: %s\n", modbus_strerror(errno));
    } else {
        printf("Fechando porta!\n");
        db_log_event(nome, port_id, "fechou");
    }

    modbus_close(ctx);
    modbus_free(ctx);
}