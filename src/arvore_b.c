#include <stdlib.h>

#include "arvore_b.h"
#include "contador.h"

static BNo *b_criaNo(ArvoreB *arvore) {
  int max = arvore->ordem * 2;
  BNo *no = malloc(sizeof(BNo));

  no->pai = NULL;
  no->chaves = malloc(sizeof(int) * (max + 1));
  no->filhos = malloc(sizeof(BNo *) * (max + 2));
  no->total = 0;

  for (int i = 0; i < max + 2; i++) {
    no->filhos[i] = NULL;
  }

  return no;
}

ArvoreB *b_criar(int ordem) {
  ArvoreB *arvore = malloc(sizeof(ArvoreB));
  arvore->ordem = ordem;
  arvore->raiz = b_criaNo(arvore);

  return arvore;
}

static int b_pesquisaBinaria(BNo *no, int chave) {
  int inicio = 0, fim = no->total - 1, meio;

  while (inicio <= fim) {
    esforco.busca++;
    meio = (inicio + fim) / 2;
    if (no->chaves[meio] == chave) {
      return meio;
    } else if (no->chaves[meio] > chave) {
      fim = meio - 1;
    } else {
      inicio = meio + 1;
    }
  }

  return inicio;
}

static BNo *b_localizaNo(ArvoreB *arvore, int chave) {
  BNo *no = arvore->raiz;

  while (no != NULL) {
    int i = b_pesquisaBinaria(no, chave);

    if (no->filhos[i] == NULL) {
      return no;
    }
    no = no->filhos[i];
  }

  return NULL;
}

static void b_adicionaChaveNo(BNo *no, BNo *direita, int chave) {
  int i = b_pesquisaBinaria(no, chave);

  for (int j = no->total - 1; j >= i; j--) {
    no->chaves[j + 1] = no->chaves[j];
    no->filhos[j + 2] = no->filhos[j + 1];
    esforco.ajuste++;
  }

  no->chaves[i] = chave;
  no->filhos[i + 1] = direita;
  esforco.ajuste++;

  no->total++;
}

static int b_transbordo(ArvoreB *arvore, BNo *no) {
  return no->total > arvore->ordem * 2;
}

static BNo *b_divideNo(ArvoreB *arvore, BNo *no) {
  int meio = no->total / 2;
  BNo *novo = b_criaNo(arvore);
  novo->pai = no->pai;

  for (int i = meio + 1; i < no->total; i++) {
    novo->filhos[novo->total] = no->filhos[i];
    novo->chaves[novo->total] = no->chaves[i];
    esforco.ajuste++;
    if (novo->filhos[novo->total] != NULL) {
      novo->filhos[novo->total]->pai = novo;
    }

    novo->total++;
  }

  novo->filhos[novo->total] = no->filhos[no->total];
  if (novo->filhos[novo->total] != NULL) {
    novo->filhos[novo->total]->pai = novo;
  }
  no->total = meio;

  return novo;
}

static void b_adicionaChaveRecursivo(ArvoreB *arvore, BNo *no, BNo *novo, int chave) {
  b_adicionaChaveNo(no, novo, chave);

  if (b_transbordo(arvore, no)) {
    int promovido = no->chaves[arvore->ordem];
    BNo *direita = b_divideNo(arvore, no);

    if (no->pai == NULL) {
      BNo *raiz = b_criaNo(arvore);
      raiz->filhos[0] = no;
      // no slide esta chamada usa "pai", que não existe aqui: a chave vai para a raiz nova
      b_adicionaChaveNo(raiz, direita, promovido);

      no->pai = raiz;
      direita->pai = raiz;
      arvore->raiz = raiz;
    } else {
      b_adicionaChaveRecursivo(arvore, no->pai, direita, promovido);
    }
  }
}

void b_adicionar(ArvoreB *arvore, int chave) {
  BNo *no = b_localizaNo(arvore, chave);

  b_adicionaChaveRecursivo(arvore, no, NULL, chave);
}

static void b_emprestaDaEsquerda(BNo *pai, int k, BNo *irmao, BNo *no) {
  for (int j = no->total; j > 0; j--) {
    no->chaves[j] = no->chaves[j - 1];
    esforco.ajuste++;
  }
  for (int j = no->total + 1; j > 0; j--) {
    no->filhos[j] = no->filhos[j - 1];
  }

  no->chaves[0] = pai->chaves[k - 1];
  esforco.ajuste++;
  no->filhos[0] = irmao->filhos[irmao->total];
  if (no->filhos[0] != NULL) {
    no->filhos[0]->pai = no;
  }
  no->total++;

  pai->chaves[k - 1] = irmao->chaves[irmao->total - 1];
  esforco.ajuste++;
  irmao->total--;
}

static void b_emprestaDaDireita(BNo *pai, int k, BNo *no, BNo *irmao) {
  no->chaves[no->total] = pai->chaves[k];
  esforco.ajuste++;
  no->filhos[no->total + 1] = irmao->filhos[0];
  if (irmao->filhos[0] != NULL) {
    irmao->filhos[0]->pai = no;
  }
  no->total++;

  pai->chaves[k] = irmao->chaves[0];
  esforco.ajuste++;
  for (int j = 0; j < irmao->total - 1; j++) {
    irmao->chaves[j] = irmao->chaves[j + 1];
    esforco.ajuste++;
  }
  for (int j = 0; j < irmao->total; j++) {
    irmao->filhos[j] = irmao->filhos[j + 1];
  }
  irmao->total--;
}

void b_remover(ArvoreB *arvore, int chave) {
  BNo *no = arvore->raiz;
  int i = 0;

  while (no != NULL) {
    i = b_pesquisaBinaria(no, chave);
    if (i < no->total && no->chaves[i] == chave) {
      break;
    }
    no = no->filhos[i];
  }

  if (no == NULL) {
    return;
  }

  if (no->filhos[0] != NULL) {
    BNo *folha = no->filhos[i];
    esforco.busca++;
    while (folha->filhos[0] != NULL) {
      folha = folha->filhos[folha->total];
      esforco.busca++;
    }

    no->chaves[i] = folha->chaves[folha->total - 1];
    esforco.ajuste++;
    no = folha;
    i = folha->total - 1;
  }

  for (int j = i; j < no->total - 1; j++) {
    no->chaves[j] = no->chaves[j + 1];
    esforco.ajuste++;
  }
  no->total--;

  while (no->pai != NULL && no->total < arvore->ordem) {
    BNo *pai = no->pai;
    int k = 0;
    esforco.busca++;
    while (pai->filhos[k] != no) {
      k++;
      esforco.busca++;
    }

    BNo *esquerda = k > 0 ? pai->filhos[k - 1] : NULL;
    BNo *direita = k < pai->total ? pai->filhos[k + 1] : NULL;

    if (esquerda != NULL && esquerda->total > arvore->ordem) {
      b_emprestaDaEsquerda(pai, k, esquerda, no);
      break;
    }
    if (direita != NULL && direita->total > arvore->ordem) {
      b_emprestaDaDireita(pai, k, no, direita);
      break;
    }
    break;
  }
}

static void b_liberar(BNo *no) {
  if (no != NULL) {
    if (no->filhos[0] != NULL) {
      for (int i = 0; i <= no->total; i++) {
        b_liberar(no->filhos[i]);
      }
    }
    free(no->chaves);
    free(no->filhos);
    free(no);
  }
}

void b_destruir(ArvoreB *arvore) {
  b_liberar(arvore->raiz);
  free(arvore);
}
