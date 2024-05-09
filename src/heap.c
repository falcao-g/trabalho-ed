#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void *reg;
    double dist;
} treg;

typedef struct {
    treg *vetor;
    int tam;
    int max;
} theap;

void troca(treg *a, treg *b) {
    treg *aux = a;
    a = b;
    b = aux;
}

int pai(int n) {
    return (n - 1) / 2;
}

int filho_esq(int n) {
    return (n * 2) + 1;
}

int filho_dir(int n) {
    return (n * 2) + 2;
}

void desce(theap *pheap, int n) {
    int maior = n;
    int esq = filho_esq(n);
    int dir = filho_dir(n);

    if (esq < (*pheap).tam &&
        (*pheap).vetor[esq].dist > (*pheap).vetor[maior].dist)
        maior = esq;
    if (dir < (*pheap).tam &&
        (*pheap).vetor[dir].dist > (*pheap).vetor[maior].dist)
        maior = dir;
    double debug_esq_dist = (*pheap).vetor[esq].dist;
    double debug_dir_dist = (*pheap).vetor[dir].dist;
    double debug_maior_dist = (*pheap).vetor[maior].dist;
    if (maior != n) {
        troca(&(*pheap).vetor[n], &(*pheap).vetor[maior]);
        desce(pheap, maior);
    }
}

void constroi_heap(theap *pheap, int max) {
    pheap->vetor = (treg *)malloc(sizeof(treg) * max);
    pheap->tam = 0;
    pheap->max = max;
}

void sobe(theap *pheap, int n) {
    int p = pai(n);

    if ((*pheap).vetor[p].dist < (*pheap).vetor[n].dist) {
        troca(&(*pheap).vetor[n], &(*pheap).vetor[p]);
        sobe(pheap, p);
    }
}

void altera_prioridade(theap *pheap, int n, void *reg, double dist) {
    treg anterior = (*pheap).vetor[n];
    (*pheap).vetor[n].reg = reg;
    (*pheap).vetor[n].dist = dist;

    if (anterior.dist < dist)
        sobe(pheap, n);
    if (anterior.dist > dist)
        desce(pheap, n);
}

void heap_sort(theap *pheap) {
    int n = pheap->tam;
    for (int i = pheap->tam - 1; i > 0; i--) {
        troca(&pheap->vetor[0], &pheap->vetor[i]);
        pheap->tam -= 1;
        desce(pheap, 0);
    }
    pheap->tam = n;
}

int insere_elemento(theap *pheap, void *reg, double dist) {
    if (pheap->tam == pheap->max)
        return EXIT_FAILURE;

    pheap->vetor[pheap->tam].reg = reg;
    pheap->vetor[pheap->tam].dist = dist;
    sobe(pheap, pheap->tam);
    pheap->tam += 1;

    return EXIT_SUCCESS;
}