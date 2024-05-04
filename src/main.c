#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./hash.c"

typedef struct {
    char codigo_ibge[10];
    char nome[40];
    float latitude;
    float longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[40];
} tmunicipio;

// to do: change this function name?
char *get_key(void *reg) {
    return (*((tmunicipio *)reg)).codigo_ibge;
}

void insere_cidade(thash cidades, tmunicipio *municipio) {
    tmunicipio *municipio_aux = malloc(sizeof(tmunicipio));
    memcpy(municipio_aux, municipio, sizeof(tmunicipio));
    hash_insere(&cidades, municipio_aux);
}

// to do: improve this function
void leitor_json(FILE *arquivo, thash cidades) {
    char linha[200];
    tmunicipio municipio;
    while (fgets(linha, 200, arquivo)) {
        if (strstr(linha, "codigo_ibge")) {
            sscanf(linha, "    \"codigo_ibge\": %[^,],", municipio.codigo_ibge);
        }
        if (strstr(linha, "nome")) {
            sscanf(linha, "    \"nome\": \"%[^\"]\",", municipio.nome);
        }
        if (strstr(linha, "latitude")) {
            sscanf(linha, "    \"latitude\": %f,", &municipio.latitude);
        }
        if (strstr(linha, "longitude")) {
            sscanf(linha, "    \"longitude\": %f,", &municipio.longitude);
        }
        if (strstr(linha, "capital")) {
            sscanf(linha, "    \"capital\": %d,", &municipio.capital);
        }
        if (strstr(linha, "codigo_uf")) {
            sscanf(linha, "    \"codigo_uf\": %d,", &municipio.codigo_uf);
        }
        if (strstr(linha, "siafi_id")) {
            sscanf(linha, "    \"siafi_id\": %d,", &municipio.siafi_id);
        }
        if (strstr(linha, "ddd")) {
            sscanf(linha, "    \"ddd\": %d,", &municipio.ddd);
        }
        if (strstr(linha, "fuso_horario")) {
            sscanf(linha, "    \"fuso_horario\": \"%[^\"]\",",
                   municipio.fuso_horario);
            insere_cidade(cidades, &municipio);
        }
    }
}

int main() {
    thash cidades;
    hash_constroi(&cidades, 12040, get_key);
    leitor_json(fopen("../public/municipios.json", "r"), cidades);

    while (1) {
        printf("--------------------\n");
        printf("1 - Buscar cidade pelo código IBGE\n");
        printf("2 - Sair\n");
        printf("--------------------\n");
        int opcao;
        printf("Digite a opção desejada: ");
        scanf("%d", &opcao);

        if (opcao == 2) {
            break;
        }

        printf("--------------------\n");
        char codigo_ibge[10];
        printf("Digite o código IBGE da cidade: ");
        scanf("%s", codigo_ibge);
        printf("--------------------\n");

        tmunicipio *municipio = hash_busca(cidades, codigo_ibge);
        if (municipio != NULL) {
            printf("Nome: %s\n", municipio->nome);
            printf("Latitude: %f\n", municipio->latitude);
            printf("Longitude: %f\n", municipio->longitude);
            printf("Capital: %d\n", municipio->capital);
            printf("Código UF: %d\n", municipio->codigo_uf);
            printf("Siafi ID: %d\n", municipio->siafi_id);
            printf("DDD: %d\n", municipio->ddd);
            printf("Fuso Horário: %s\n", municipio->fuso_horario);
        } else {
            printf("Cidade não encontrada\n");
        }
    }
}