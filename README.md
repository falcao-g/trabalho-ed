<h1 align='center'> Trabalho Prático ED 1 </h1>

<p align='center'>
<img alt="C" src="https://img.shields.io/badge/C-555555?style=flat&logo=c&logoColor=white">
</p>

<p align="center">
    <b>Edson Takashi Matsubara | Estruturas de Dados</b>
</p>

## 📚 Descrição

O desafio consistia basicamente em implementar 2 tabelas hash e K-D Tree, dividido em 3 tarefas:

### Tarefa 1: (consulta cidade)
Dado o codigo_ibge retorne as informações desta cidade, construa uma tabela hash com tratamento de colisões utilizando um hash duplo para armazenar todas as cidades brasileiras do link https://github.com/kelvins/municipios-brasileiros/blob/main/json/municipios.json, a chave desta hash deve ser o codigo_ibge. Ao consultar um codigo_ibge seu código deverá retornar todas as informações desta cidade presente no banco de dados.

### Tarefa 2 (vizinhos mais próximo de uma cidade dado o seu codigo_ibge)
Dado o código_ibge, você deverá retornar o codigo_ibge das N cidades vizinhas mais próximas, onde N é um parâmetro do usuário. Para a busca pela vizinhança construa uma K-D Tree utilizando uma ABB, alternando entre latitude e longitude.

### Tarefa 3 (combinar tarefa 1 e 2)
Dado o nome de uma cidade retornar todas as informações das N cidades vizinhas mais próximas, crie uma outra tabela hash que dado o nome da cidade retorne o codigo_ibge. De posse do codigo_ibge utilize o código da tarefa 2 para retornar os vizinhos mais próximos e o código da tarefa 1 para retornar as informações das cidades vizinhas, cidades com nomes iguais uma lista de nomes é retornada ao usuário ele deve escolher qual ele quer.

## 🚀 Funcionalidades

- [x] Tarefa 1 - Todas as informações de uma cidade dado o código IBGE
- [x] Tarefa 2 - Código IBGE das N cidades vizinhas mais próximas
- [x] Tarefa 3 - Todas as informações das N cidades vizinhas mais próximas dado o nome de uma cidade + a distância em km entre elas (aproximada)

## 📝 Como rodar

Para compilar o programa, execute o comando abaixo:

```bash
gcc main.c -o main
```

Em seguida, execute o programa com o comando:

```bash
./main
```
