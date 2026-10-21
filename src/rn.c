#include <stdlib.h>

#include "contador.h"
#include "rn.h"

static void rn_balancear(Rn *arvore, RnNo *no);
static void rn_rotacionarEsquerda(Rn *arvore, RnNo *no);
static void rn_rotacionarDireita(Rn *arvore, RnNo *no);

Rn *rn_criar(void) {
  Rn *arvore = malloc(sizeof(Rn));
  arvore->nulo = malloc(sizeof(RnNo));
  arvore->nulo->pai = arvore->nulo;
  arvore->nulo->esquerda = arvore->nulo;
  arvore->nulo->direita = arvore->nulo;
  arvore->nulo->valor = 0;
  arvore->nulo->cor = Preto;
  arvore->raiz = arvore->nulo;

  return arvore;
}

void rn_adicionar(Rn *arvore, int valor) {
  RnNo *pai = arvore->nulo;
  RnNo *no = arvore->raiz;

  while (no != arvore->nulo) {
    esforco.busca++;
    pai = no;
    no = valor < no->valor ? no->esquerda : no->direita;
  }

  RnNo *novo = malloc(sizeof(RnNo));
  novo->valor = valor;
  novo->pai = pai;
  novo->esquerda = arvore->nulo;
  novo->direita = arvore->nulo;
  novo->cor = Vermelho;

  if (pai == arvore->nulo) {
    arvore->raiz = novo;
  } else if (valor < pai->valor) {
    pai->esquerda = novo;
  } else {
    pai->direita = novo;
  }

  rn_balancear(arvore, novo);
}

static void rn_liberar(Rn *arvore, RnNo *no) {
  if (no != arvore->nulo) {
    rn_liberar(arvore, no->esquerda);
    rn_liberar(arvore, no->direita);
    free(no);
  }
}

void rn_destruir(Rn *arvore) {
  rn_liberar(arvore, arvore->raiz);
  free(arvore->nulo);
  free(arvore);
}

static void rn_balancear(Rn *arvore, RnNo *no) {
  while (no->pai->cor == Vermelho) {
    esforco.ajuste++;
    if (no->pai == no->pai->pai->esquerda) {
      RnNo *tio = no->pai->pai->direita;

      if (tio->cor == Vermelho) {
        tio->cor = Preto;
        no->pai->cor = Preto;
        no->pai->pai->cor = Vermelho;
        esforco.ajuste += 3;
        no = no->pai->pai;
      } else {
        if (no == no->pai->direita) {
          no = no->pai;
          rn_rotacionarEsquerda(arvore, no);
        } else {
          no->pai->cor = Preto;
          no->pai->pai->cor = Vermelho;
          esforco.ajuste += 2;
          rn_rotacionarDireita(arvore, no->pai->pai);
        }
      }
    } else {
      RnNo *tio = no->pai->pai->esquerda;

      if (tio->cor == Vermelho) {
        tio->cor = Preto;
        no->pai->cor = Preto;
        no->pai->pai->cor = Vermelho;
        esforco.ajuste += 3;
        no = no->pai->pai;
      } else {
        if (no == no->pai->esquerda) {
          no = no->pai;
          rn_rotacionarDireita(arvore, no);
        } else {
          no->pai->cor = Preto;
          no->pai->pai->cor = Vermelho;
          esforco.ajuste += 2;
          rn_rotacionarEsquerda(arvore, no->pai->pai);
        }
      }
    }
  }
  arvore->raiz->cor = Preto;
  esforco.ajuste++;
}

static void rn_rotacionarEsquerda(Rn *arvore, RnNo *no) {
  esforco.ajuste++;
  RnNo *direita = no->direita;
  no->direita = direita->esquerda;

  if (direita->esquerda != arvore->nulo) {
    direita->esquerda->pai = no;
  }

  direita->pai = no->pai;

  if (no->pai == arvore->nulo) {
    arvore->raiz = direita;
  } else if (no == no->pai->esquerda) {
    no->pai->esquerda = direita;
  } else {
    no->pai->direita = direita;
  }

  direita->esquerda = no;
  no->pai = direita;
}

static void rn_rotacionarDireita(Rn *arvore, RnNo *no) {
  esforco.ajuste++;
  RnNo *esquerda = no->esquerda;
  no->esquerda = esquerda->direita;

  if (esquerda->direita != arvore->nulo) {
    esquerda->direita->pai = no;
  }

  esquerda->pai = no->pai;

  if (no->pai == arvore->nulo) {
    arvore->raiz = esquerda;
  } else if (no == no->pai->esquerda) {
    no->pai->esquerda = esquerda;
  } else {
    no->pai->direita = esquerda;
  }

  esquerda->direita = no;
  no->pai = esquerda;
}
