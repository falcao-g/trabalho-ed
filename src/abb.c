#include <stdlib.h>

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