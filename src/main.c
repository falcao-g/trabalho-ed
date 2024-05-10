#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./abb.c"
#include "./hash.c"

#define ANSI_PURPLE "\e[38;5;92m"
#define ANSI_RESET "\e[0m"

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

char *pega_codigo(void *reg) {
    return ((tmunicipio *)reg)->codigo_ibge;
}

char *pega_nome(void *reg) {
    return ((tmunicipio *)reg)->nome;
}

void insere_cidade(thash hash_cod, thash hash_nome, tarv *arv,
                   tmunicipio *municipio) {
    tmunicipio *municipio_aux = malloc(sizeof(tmunicipio));
    memcpy(municipio_aux, municipio, sizeof(tmunicipio));
    hash_insere(&hash_cod, municipio_aux);
    hash_insere(&hash_nome, municipio_aux);
    abb_insere(arv, municipio_aux);
}

void leitor_json(FILE *arquivo, thash hash_cod, thash hash_nome, tarv *arv) {
    char linha[150];
    tmunicipio municipio;
    while (fgets(linha, 150, arquivo)) {
        if (strstr(linha, "codigo_ibge")) {
            sscanf(linha, "    \"codigo_ibge\": %[^,],", municipio.codigo_ibge);
        } else if (strstr(linha, "nome")) {
            sscanf(linha, "    \"nome\": \"%[^\"]\",", municipio.nome);
        } else if (strstr(linha, "latitude")) {
            sscanf(linha, "    \"latitude\": %f,", &municipio.latitude);
        } else if (strstr(linha, "longitude")) {
            sscanf(linha, "    \"longitude\": %f,", &municipio.longitude);
        } else if (strstr(linha, "capital")) {
            sscanf(linha, "    \"capital\": %d,", &municipio.capital);
        } else if (strstr(linha, "codigo_uf")) {
            sscanf(linha, "    \"codigo_uf\": %d,", &municipio.codigo_uf);
        } else if (strstr(linha, "siafi_id")) {
            sscanf(linha, "    \"siafi_id\": %d,", &municipio.siafi_id);
        } else if (strstr(linha, "ddd")) {
            sscanf(linha, "    \"ddd\": %d,", &municipio.ddd);
        } else if (strstr(linha, "fuso_horario")) {
            sscanf(linha, "    \"fuso_horario\": \"%[^\"]\",",
                   municipio.fuso_horario);
            insere_cidade(hash_cod, hash_nome, arv, &municipio);
        }
    }
}

void imprime_municipio(tmunicipio *municipio) {
    if (municipio == NULL) {
        printf("Cidade não encontrada\n");
    }
    printf("%sCódigo IBGE: %s%s\n", ANSI_PURPLE, ANSI_RESET,
           municipio->codigo_ibge);
    printf("%sNome: %s%s\n", ANSI_PURPLE, ANSI_RESET, municipio->nome);
    printf("%sLatitude: %s%f\n", ANSI_PURPLE, ANSI_RESET, municipio->latitude);
    printf("%sLongitude: %s%f\n", ANSI_PURPLE, ANSI_RESET,
           municipio->longitude);
    printf("%sCapital: %s%d\n", ANSI_PURPLE, ANSI_RESET, municipio->capital);
    printf("%sCódigo UF: %s%d\n", ANSI_PURPLE, ANSI_RESET,
           municipio->codigo_uf);
    printf("%sSiafi ID: %s%d\n", ANSI_PURPLE, ANSI_RESET, municipio->siafi_id);
    printf("%sDDD: %s%d\n", ANSI_PURPLE, ANSI_RESET, municipio->ddd);
    printf("%sFuso Horário: %s%s\n", ANSI_PURPLE, ANSI_RESET,
           municipio->fuso_horario);
}

double cmp(void *a, void *b, int eixo) {
    if (eixo % 2 == 0) {
        return ((tmunicipio *)a)->latitude - ((tmunicipio *)b)->latitude;
    }
    return ((tmunicipio *)a)->longitude - ((tmunicipio *)b)->longitude;
}

double dist(void *a, void *b) {
    double dx = cmp(a, b, 0);
    double dy = cmp(a, b, 1);
    return dx * dx + dy * dy;
}

int main() {
    thash hash_cod;
    hash_constroi(&hash_cod, 12040, pega_codigo);
    thash hash_nome;
    hash_constroi(&hash_nome, 12040, pega_nome);
    tarv arv;
    abb_constroi(&arv, cmp, dist);

    leitor_json(fopen("../data/municipios.json", "r"), hash_cod, hash_nome,
                &arv);

    tmunicipio *pesquisa;
    while (1) {
        printf("--------------------\n");
        printf("1 - Buscar cidade pelo código IBGE\n");
        printf("2 - Buscar cidade pelo nome\n");
        printf("3 - Buscar N vizinhos mais próximos de uma cidade\n");
        printf("0 - Sair\n");
        printf("--------------------\n");
        int opcao;
        printf("Digite a opção desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("--------------------\n");
                char codigo_ibge[10];
                printf("Digite o código IBGE da cidade: ");
                scanf("%s", codigo_ibge);
                printf("--------------------\n");

                pesquisa = hash_busca(hash_cod, codigo_ibge);
                imprime_municipio(pesquisa);
                break;

            case 2:
                printf("--------------------\n");
                char nome[100];
                printf("Digite o nome da cidade: ");
                scanf(" %[^\n]s", nome);
                printf("--------------------\n");

                pesquisa = hash_busca(hash_nome, nome);
                imprime_municipio(pesquisa);
                // tmunicipio *municipio3 = hash_busca2(hash_nome, nome);
                // printf("%s\n", (*(tmunicipio **)municipio3 + 9)->nome);
                // if (municipio3 != NULL) {
                //     for (int i = 0; i < 3; i++) {
                //         imprime_municipio(*(tmunicipio **)municipio3 + i);
                //     }
                // } else {
                //     printf("Cidade não encontrada\n");
                // }
                break;

            case 3:
                // tem que fazer a busca por nome primeiro
                printf("--------------------\n");
                char nome2[100];
                int n;
                printf("Digite o nome da cidade: ");
                scanf(" %[^\n]s", nome2);
                printf("Digite o número de vizinhos: ");
                scanf("%d", &n);

                while (n > 5569 || n < 1) {
                    printf("Digite um número de vizinhos válido: ");
                    scanf("%d", &n);
                }
                printf("--------------------\n");

                theap vizinhos;
                constroi_heap(&vizinhos, n);

                tmunicipio *municipio3 = hash_busca(hash_nome, nome2);
                abb_busca_vizinhos(&arv, municipio3, &vizinhos);
                heap_sort(&vizinhos);
                for (int i = 0; i < n; i++) {
                    if (i + 1 != 1)
                        printf("%dº ", i + 1);
                    printf("vizinho mais próximo: \n");
                    imprime_municipio(vizinhos.vetor[i].reg);
                    printf("%sDistância: %s%.2fkm\n", ANSI_PURPLE, ANSI_RESET,
                           sqrt(vizinhos.vetor[i].dist) * 100);
                    if (i != n - 1)
                        printf("--------------------\n");
                }
                heap_apaga(&vizinhos);
                break;

            default:
                hash_apaga(&hash_cod);
                abb_apaga(arv.raiz);
                free(pesquisa);
                exit(0);
                break;
        }
    }
}