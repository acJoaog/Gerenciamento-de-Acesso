FROM alpine:latest

# Instala dependências no Alpine
RUN apk update && \
    apk add --no-cache \
    build-base \
    sqlite-dev \
    sqlite \
    libmodbus \
    libmodbus-dev \
    curl \
    nano \
    net-tools \
    udev \
    usbutils \
    libsodium-dev \
    minicom \
    screen \
    linux-headers \
    && rm -rf /var/cache/apk/*

# define o diretorio principal
WORKDIR /firmware

RUN mkdir /firmware/src

# builda projeto na pasta principal
COPY ../firmware/src/ /firmware/src/

# compilando arquivos C
RUN mkdir -p /firmware/data && \
    gcc -o main src/main.c src/menu.c src/controle.c src/database.c \
    -lmodbus -lsqlite3 -lsodium && \
    chmod +x main

# copiando configs
COPY ../rules/99-usb-serial.rules /etc/udev/rules.d/

# copia e executa o entrypoint para incializar a aplicação
COPY ../scripts/entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]