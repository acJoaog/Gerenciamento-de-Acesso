#include <stdio.h>
#include <modbus/modbus.h>
#include <assert.h>  // para assert
#include <unistd.h>  // para usleep (delay)

#define MAX_ATTEMPTS 5  // numero max de tentativas de conexão

// função para testar a conexão com o slave
int test_modbus_connection(modbus_t *mb) {
    int connection_status = modbus_connect(mb);
    assert(connection_status != -1 && "Erro ao conectar no slave.");
    return connection_status;
}

// função principal do teste
int main() {
    const char *port = "/dev/ttyUSB0";
    int slave_id = 1;
    
    modbus_t *mb = modbus_new_rtu(port, 9600, 'N', 8, 1);
    assert(mb != NULL && "Erro ao criar o contexto Modbus.");
    
    modbus_set_slave(mb, slave_id);
    
    for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        printf("Tentativa %d de conexão...\n", attempt);
        
        if (test_modbus_connection(mb) != -1) {
            printf("Conexão bem-sucedida!\n");
            break;  // Se a conexão for bem-sucedida, saímos do loop
        } else if (attempt == MAX_ATTEMPTS) {
            printf("Máximo de tentativas alcançado. Falha na conexão.\n");
        } else {
            printf("Tentando novamente...\n");
        }
        
        sleep(2);  // Aguardar 2 segundos antes de tentar novamente
    }

    modbus_close(mb);
    modbus_free(mb);

    return 0;
}
