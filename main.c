#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./abb.c"
#include "./hash.c"

#define ROXO "\e[38;5;92m"
#define PADRAO "\e[0m"

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
        return;
    }
    printf("%sCódigo IBGE: %s%s\n", ROXO, PADRAO, municipio->codigo_ibge);
    printf("%sNome: %s%s\n", ROXO, PADRAO, municipio->nome);
    printf("%sLatitude: %s%f\n", ROXO, PADRAO, municipio->latitude);
    printf("%sLongitude: %s%f\n", ROXO, PADRAO, municipio->longitude);
    printf("%sCapital: %s%d\n", ROXO, PADRAO, municipio->capital);
    printf("%sCódigo UF: %s%d\n", ROXO, PADRAO, municipio->codigo_uf);
    printf("%sSiafi ID: %s%d\n", ROXO, PADRAO, municipio->siafi_id);
    printf("%sDDD: %s%d\n", ROXO, PADRAO, municipio->ddd);
    printf("%sFuso Horário: %s%s\n", ROXO, PADRAO, municipio->fuso_horario);
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
    theap vizinhos;

    leitor_json(fopen("./data/municipios.json", "r"), hash_cod, hash_nome,
                &arv);

    tmunicipio *pesquisa;
    char codigo_ibge[10];
    int n;
    int opcao;
    while (1) {
        printf("--------------------\n");
        printf("1 - Buscar cidade pelo código IBGE\n");
        printf("2 - Buscar N vizinhos pelo código IBGE\n");
        printf("3 - Buscar N vizinhos pelo nome da cidade\n");
        printf("0 - Sair\n");
        printf("--------------------\n");
        printf("Digite a opção desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("--------------------\n");
                printf("Digite o código IBGE da cidade: ");
                scanf("%s", codigo_ibge);
                printf("--------------------\n");

                pesquisa = hash_busca(hash_cod, codigo_ibge);
                imprime_municipio(pesquisa);
                break;

            case 2:
                printf("--------------------\n");
                printf("Digite o código IBGE da cidade: ");
                scanf("%s", codigo_ibge);
                printf("Digite o número de vizinhos: ");
                scanf("%d", &n);

                while (n > 5569 || n < 1) {
                    printf("Digite um número de vizinhos válido: ");
                    scanf("%d", &n);
                }
                printf("--------------------\n");

                pesquisa = hash_busca(hash_cod, codigo_ibge);

                if (pesquisa == NULL) {
                    printf("Cidade não encontrada\n");
                    break;
                }

                constroi_heap(&vizinhos, n);
                abb_busca_vizinhos(&arv, pesquisa, &vizinhos);
                heap_sort(&vizinhos);

                for (int i = 0; i < n; i++) {
                    printf("%dº - %s\n", i + 1,
                           ((tmunicipio *)vizinhos.vetor[i].reg)->codigo_ibge);
                }
                heap_apaga(&vizinhos);
                break;

            case 3:
                printf("--------------------\n");
                char nome[100];
                printf("Digite o nome da cidade: ");
                scanf(" %[^\n]s", nome);
                printf("Digite o número de vizinhos: ");
                scanf("%d", &n);

                while (n > 5569 || n < 1) {
                    printf("Digite um número de vizinhos válido: ");
                    scanf("%d", &n);
                }
                printf("--------------------\n");

                theap vizinhos;
                constroi_heap(&vizinhos, n);
                tmunicipio **pesquisa_v =
                    (tmunicipio **)hash_busca_repetidos(hash_nome, nome);

                if (pesquisa_v[0] == NULL) {
                    printf("Cidade não encontrada\n");
                    break;
                }

                int selecao = 0;
                int repetida = 1;
                if (pesquisa_v[1] != NULL) {
                    printf(
                        "Foram encontradas mais de uma cidade com esse nome\n");
                    printf("Selecione o munícipio desejado:\n");
                    int i;
                    for (i = 0; pesquisa_v[i] != NULL; i++) {
                        printf("%d - ", repetida);
                        imprime_municipio(pesquisa_v[i]);
                        printf("--------------------\n");
                        repetida += 1;
                    }
                    scanf("%d", &selecao);
                    selecao -= 1;
                    while (selecao < 0 || selecao > i) {
                        printf("Digite um número válido: ");
                        scanf("%d", &selecao);
                        selecao -= 1;
                    }
                    printf("--------------------\n");
                }

                abb_busca_vizinhos(&arv, pesquisa_v[selecao], &vizinhos);
                heap_sort(&vizinhos);

                for (int i = 0; i < n; i++) {
                    if (i + 1 != 1)
                        printf("%dº ", i + 1);
                    printf("vizinho mais próximo: \n");
                    char *codigo =
                        ((tmunicipio *)vizinhos.vetor[i].reg)->codigo_ibge;
                    imprime_municipio(hash_busca(hash_cod, codigo));
                    printf("%sDistância: %s%.2fkm\n", ROXO, PADRAO,
                           sqrt(vizinhos.vetor[i].dist) * 100);
                    if (i != n - 1)
                        printf("--------------------\n");
                }
                heap_apaga(&vizinhos);
                break;

            default:
                hash_apaga(&hash_cod);
                abb_apaga(arv.raiz);
                exit(0);
                break;
        }
    }
}