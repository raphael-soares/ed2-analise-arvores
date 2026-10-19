#include <stdlib.h>

#include "avl.h"
#include "contador.h"

static void avl_balanceamento(Avl *arvore, AvlNo *no);
static int avl_altura(AvlNo *no);
static int avl_fb(AvlNo *no);
static AvlNo *avl_rsd(Avl *arvore, AvlNo *no);
static AvlNo *avl_rse(Avl *arvore, AvlNo *no);
static AvlNo *avl_rdd(Avl *arvore, AvlNo *no);
static AvlNo *avl_rde(Avl *arvore, AvlNo *no);

static int avl_max(int a, int b) { return a > b ? a : b; }

Avl *avl_criar(void) {
  Avl *arvore = malloc(sizeof(Avl));
  arvore->raiz = NULL;

  return arvore;
}

int avl_vazia(Avl *arvore) { return arvore->raiz == NULL; }

void avl_adicionar(Avl *arvore, int valor) {
  AvlNo *no = arvore->raiz;

  while (no != NULL) {
    esforco.busca++;
    if (valor > no->valor) {
      if (no->direita != NULL) {
        no = no->direita;
      } else {
        break;
      }
    } else {
      if (no->esquerda != NULL) {
        no = no->esquerda;
      } else {
        break;
      }
    }
  }

  AvlNo *novo = malloc(sizeof(AvlNo));
  novo->valor = valor;
  novo->pai = no;
  novo->esquerda = NULL;
  novo->direita = NULL;
  novo->altura = 1;

  if (no == NULL) {
    arvore->raiz = novo;
  } else {
    if (valor > no->valor) {
      no->direita = novo;
    } else {
      no->esquerda = novo;
    }

    avl_balanceamento(arvore, no);
  }
}

AvlNo *avl_localizar(AvlNo *no, int valor) {
  while (no != NULL) {
    if (no->valor == valor) {
      return no;
    }

    no = valor < no->valor ? no->esquerda : no->direita;
  }

  return NULL;
}

void avl_percorrer(AvlNo *no, void (*callback)(int)) {
  if (no != NULL) {
    avl_percorrer(no->esquerda, callback);
    callback(no->valor);
    avl_percorrer(no->direita, callback);
  }
}

static void avl_balanceamento(Avl *arvore, AvlNo *no) {
  while (no != NULL) {
    esforco.ajuste++;
    no->altura = avl_max(avl_altura(no->esquerda), avl_altura(no->direita)) + 1;
    int fator = avl_fb(no);

    if (fator > 1) {
      if (avl_fb(no->esquerda) > 0) {
        avl_rsd(arvore, no);
      } else {
        avl_rdd(arvore, no);
      }
    } else if (fator < -1) {
      if (avl_fb(no->direita) < 0) {
        avl_rse(arvore, no);
      } else {
        avl_rde(arvore, no);
      }
    }

    no = no->pai;
  }
}

static int avl_altura(AvlNo *no) { return no != NULL ? no->altura : 0; }

static int avl_fb(AvlNo *no) {
  int esquerda = 0, direita = 0;

  if (no->esquerda != NULL) {
    esquerda = no->esquerda->altura;
  }

  if (no->direita != NULL) {
    direita = no->direita->altura;
  }

  return esquerda - direita;
}

static AvlNo *avl_rse(Avl *arvore, AvlNo *no) {
  esforco.ajuste++;
  AvlNo *pai = no->pai;
  AvlNo *direita = no->direita;

  if (direita->esquerda != NULL) {
    direita->esquerda->pai = no;
  }

  no->direita = direita->esquerda;
  no->pai = direita;

  direita->esquerda = no;
  direita->pai = pai;

  if (pai == NULL) {
    arvore->raiz = direita;
  } else {
    if (pai->esquerda == no) {
      pai->esquerda = direita;
    } else {
      pai->direita = direita;
    }
  }

  no->altura = avl_max(avl_altura(no->esquerda), avl_altura(no->direita)) + 1;
  direita->altura =
      avl_max(avl_altura(direita->esquerda), avl_altura(direita->direita)) + 1;

  return direita;
}

static AvlNo *avl_rsd(Avl *arvore, AvlNo *no) {
  esforco.ajuste++;
  AvlNo *pai = no->pai;
  AvlNo *esquerda = no->esquerda;

  if (esquerda->direita != NULL) {
    esquerda->direita->pai = no;
  }

  no->esquerda = esquerda->direita;
  no->pai = esquerda;

  esquerda->direita = no;
  esquerda->pai = pai;

  if (pai == NULL) {
    arvore->raiz = esquerda;
  } else {
    if (pai->esquerda == no) {
      pai->esquerda = esquerda;
    } else {
      pai->direita = esquerda;
    }
  }

  no->altura = avl_max(avl_altura(no->esquerda), avl_altura(no->direita)) + 1;
  esquerda->altura =
      avl_max(avl_altura(esquerda->esquerda), avl_altura(esquerda->direita)) + 1;

  return esquerda;
}

static AvlNo *avl_rde(Avl *arvore, AvlNo *no) {
  no->direita = avl_rsd(arvore, no->direita);
  return avl_rse(arvore, no);
}

static AvlNo *avl_rdd(Avl *arvore, AvlNo *no) {
  no->esquerda = avl_rse(arvore, no->esquerda);
  return avl_rsd(arvore, no);
}
