import requests
from requests.exceptions import RequestException

# URL do servidor
url = "http://localhost:5000"

# Função para testar a disponibilidade do servidor
def test_server():
    try:
        # Tentando fazer uma requisição GET
        response = requests.get(url)
        
        # Verificando se o status code é 200 (OK)
        if response.status_code == 200:
            print("Servidor está no ar!")
        else:
            print(f"Erro ao acessar o servidor. Status code: {response.status_code}")
    
    except RequestException as e:
        # Captura qualquer erro de requisição (ex. servidor fora do ar)
        print(f"Falha ao tentar acessar o servidor: {e}")

# Executa o teste
if __name__ == "__main__":
    test_server()
