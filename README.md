# Gerenciamento-de-Acesso

> Projeto em C para Linux embarcado que gerencia o acesso a portas físicas por membros de uma empresa.

---

## Sumário

- [Sobre o Projeto](#sobre-o-projeto)
- [Tecnologias](#tecnologias)
- [Pré-requisitos](#pré-requisitos)
- [Como rodar o projeto](#como-rodar-o-projeto)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Comandos Úteis](#comandos-úteis)
- [Licença](#licença)

---

## Sobre o Projeto

Este projeto cria um ambiente de desenvolvimento baseado em Docker com Alpine Linux para:

- Instalação de dependências de build em C.
- Compilação do código-fonte localizado em `/firmware/src/`.
- Configuração automática de dispositivos USB via udev.
- Execução de uma aplicação embarcada utilizando Modbus e SQLite.

---

## Tecnologias

- **Alpine Linux** (imagem base)
- **GCC** (compilador C)
- **SQLite3**
- **Libmodbus**
- **udev**
- **Flask**
- **Docker**

---

## Pré-requisitos

Antes de começar, você vai precisar ter instalado:

- [Docker](https://docs.docker.com/get-docker/) (versão recomendada: 24.0+)

---

## Como rodar o projeto

Clone o repositório:

```bash
git clone https://github.com/acJoaog/Gerenciamento-de-Acesso.git
cd Gerenciamento-de-Acesso
```

Construa a imagem Docker:

```bash
docker build -f docker/Dockerfile -t gerenciamento_acesso .
```

Execute o container:

> **Nota:** A flag `--privileged` é necessária para acesso total a dispositivos USB e configuração do udev.

```bash
docker run -it -p 5000:5000 --device=/dev/ttyUSB0 gerenciamento_acesso --privileged
```
> **Nota:** Se estiver simulando um dispositvo no Windows deve habilitar acesso da portas COM com o WSL para comunicação com o container.

Instale o usbipd no Windows (via PowerShell como administrador):

```bash
winget install --interactive --exact dorssel.usbipd-win
```

Conecte o dispositivo ao WSL:

```bash
usbipd list
```

Conecte o dispositivo (substitua BUSID pelo ID correto):

```bash
usbipd attach --wsl --busid <BUSID>
```

---

## Estrutura do Projeto

```
/firmware
  ├── src/                  # Código-fonte C (.c, .h)
  ├── data/                 # Pasta para dados persistentes
/scripts
  └── entrypoint.sh          # Script de inicialização da aplicação
/rules
  └── 99-usb-serial.rules    # Regras udev para dispositivos USB
/docker
  └── Dockerfile                  # Definição da imagem Docker
/server
  ├── server.py             # Script de inicialização do Flask
  ├── templates/            # Diretorio de modelos html
    └── eventos.html
```

---

## Comandos Úteis

Acessar o terminal da aplicação:

```bash
docker attach run nome-do-container
```

Acessar o container em modo shell:

```bash
docker exec -it nome-do-container /bin/sh
```

Compilar novamente o projeto manualmente dentro do container:

```bash
gcc -o /firmware/main /firmware/src/*.c -I/usr/include -L/usr/lib -lmodbus -lsqlite3 -lsodium -Wall -Os
```

Acessar o servidor Flask na porta 5000:

```bash
curl http://localhost:5000/
```

---

## Vídeo de demonstração

https://youtu.be/g8MNxNJZjlc

---