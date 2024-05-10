#include <math.h>
#include <stdlib.h>

#include "heap.c"

typedef struct _tnode {
    void *reg;
    struct _tnode *esq;
    struct _tnode *dir;
} tnode;

typedef struct _abb {
    tnode *raiz;
    double (*cmp)(void *, void *, int);
    double (*dist)(void *, void *);
} tarv;

void abb_constroi(tarv *parv, double (*cmp)(void *, void *, int),
                  double (*dist)(void *, void *)) {
    parv->raiz = NULL;
    parv->cmp = cmp;
    parv->dist = dist;
}

void abb_insere_node(tarv *parv, tnode **ppnode, void *reg, int eixo) {
    if (*ppnode == NULL) {
        *ppnode = malloc(sizeof(tnode));
        (*ppnode)->reg = reg;
        (*ppnode)->esq = NULL;
        (*ppnode)->dir = NULL;
    } else if (parv->cmp((*ppnode)->reg, reg, eixo) > 0) {
        abb_insere_node(parv, &((*ppnode)->esq), reg, eixo + 1);
    } else {
        abb_insere_node(parv, &((*ppnode)->dir), reg, eixo + 1);
    }
}

void abb_insere(tarv *parv, void *reg) {
    return abb_insere_node(parv, &parv->raiz, reg, 0);
}

void *abb_busca_node(tarv *parv, tnode *pnode, void *reg, int eixo) {
    tnode *ret;
    if (pnode == NULL) {
        ret = NULL;
    } else if (parv->cmp(pnode->reg, reg, eixo) > 0) { /* esquerda*/
        ret = abb_busca_node(parv, pnode->esq, reg, eixo + 1);
    } else if (parv->cmp(pnode->reg, reg, eixo) < 0) { /*direita*/
        ret = abb_busca_node(parv, pnode->dir, reg, eixo + 1);
    } else {
        ret = pnode->reg;
    }
    return ret;
}

void *abb_busca(tarv *parv, void *reg) {
    return abb_busca_node(parv, parv->raiz, reg, 0);
}

void abb_busca_node_vizinho(tarv *parv, tnode *pnode, void *reg, int eixo,
                            theap *vizinhos) {
    if (pnode == NULL) {
        return;
    }

    double dist = parv->dist(pnode->reg, reg);
    if (dist > 0) {
        int ret = insere_elemento(vizinhos, pnode->reg, dist);
        if (ret == EXIT_FAILURE && dist < vizinhos->vetor[0].dist) {
            altera_prioridade(vizinhos, 0, pnode->reg, dist);
        }
    }

    tnode *proximo = NULL;
    tnode *outro = NULL;

    if (parv->cmp(pnode->reg, reg, eixo) > 0) {
        proximo = pnode->esq;
        outro = pnode->dir;
    } else {
        proximo = pnode->dir;
        outro = pnode->esq;
    }

    abb_busca_node_vizinho(parv, proximo, reg, eixo + 1, vizinhos);

    if (pow(parv->cmp(pnode->reg, reg, eixo), 2) < vizinhos->vetor[0].dist) {
        abb_busca_node_vizinho(parv, outro, reg, eixo + 1, vizinhos);
    }
}

void abb_busca_vizinhos(tarv *parv, void *reg, theap *vizinhos) {
    abb_busca_node_vizinho(parv, parv->raiz, reg, 0, vizinhos);
}

void abb_apaga(tnode *pnode) {
    if (pnode == NULL) {
        return;
    }
    abb_apaga(pnode->esq);
    abb_apaga(pnode->dir);
    free(pnode);
}