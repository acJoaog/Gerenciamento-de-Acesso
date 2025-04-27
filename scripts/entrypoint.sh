#!/bin/sh

# garante que as regras udev sejam aplicadas
udevadm control --reload-rules
udevadm trigger

# verifica se o dispositivo está disponível
if [ -e /dev/ttyUSB0 ]; then
    chmod 666 /dev/ttyUSB0
fi

# executa o aplicativo principal
exec ./main