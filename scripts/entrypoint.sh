#!/bin/sh

# garante que as regras udev sejam aplicadas
udevadm control --reload-rules
udevadm trigger

# verifica se o dispositivo está disponível
if [ -e /dev/ttyUSB0 ]; then
    chmod 666 /dev/ttyUSB0
fi

# executa web server
cd /server
. venv/bin/activate
python3 server.py > /dev/null 2>&1 &

# executa o aplicativo principal
cd ../firmware
exec ./main 