#include <ModbusRTU.h>
ModbusRTU mb;

void setup() {
    Serial.begin(9600, SERIAL_8N1);

    mb.begin(&Serial);

    mb.slave(0x01); // ID do dispositivo

    mb.addCoil(0x34); // Porta 1 (coil no endereço 0x34)
    mb.addCoil(0x35); // Porta 2 (coil no endereço 0x35)

    pinMode(D1, OUTPUT); // LED no pino D1
    pinMode(D2, OUTPUT); // LED no pino D2 

    digitalWrite(D2, LOW);
    digitalWrite(D1, LOW);
}

void loop() {
    mb.task();

    digitalWrite(D1, mb.Coil(0x34)); // atualiza LED conforme coil 0x34
    digitalWrite(D2, mb.Coil(0x35)); // atualiza LED conforme coil 0x35
}