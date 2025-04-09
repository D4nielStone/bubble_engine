import requests

def gerar_codigo(prompt, model='codellama:7b-instruct-q4_0'):
    url = 'http://localhost:11434/api/generate'
    payload = {
        'model': model,
        'prompt': prompt,
        'stream': False
    }

    try:
        response = requests.post(url, json=payload)
        response.raise_for_status()
        resultado = response.json()
        return resultado.get('response', 'Nenhuma resposta gerada.')
    except requests.exceptions.RequestException as e:
        return f"Erro na requisição: {e}"

if __name__ == '__main__':
    entrada = input("Digite o que deseja que o modelo gere (ex: 'crie uma função em Python que ordena uma lista'): ")
    resposta = gerar_codigo(entrada)
    print("\nResposta do CodeLlama:\n")
    print(resposta)

