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
    python3 \
    py3-pip \ 
    python3-dev \
    && rm -rf /var/cache/apk/*

# define o diretorio principal
WORKDIR /firmware

RUN mkdir /firmware/src

# builda projeto na pasta principal
COPY ../firmware/src/ /firmware/src/

# compilando arquivos C
RUN mkdir -p /firmware/data && \
    gcc -o /firmware/main /firmware/src/*.c \
    -I/usr/include \
    -L/usr/lib \
    -lmodbus -lsqlite3 -lsodium \
    -Wall -Os

# copiando configs
COPY ../rules/99-usb-serial.rules /etc/udev/rules.d/

# configurando ambiente py para server HTTP
# RUN mkdir server

RUN python3 -m venv ../server/venv && \
    /server/venv/bin/pip install -r ../requirements/python.txt 

COPY ../server/ ../server

# copia e executa o entrypoint para incializar a aplicação
COPY ../scripts/entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
    
# expondo porta
EXPOSE 5000

ENTRYPOINT ["/entrypoint.sh"]