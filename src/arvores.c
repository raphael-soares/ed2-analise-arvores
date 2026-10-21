#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"
#include "contador.h"
#include "rn.h"

typedef struct {
  const char *nome;
  void *(*criar)(void);
  void (*adicionar)(void *, int);
  void (*remover)(void *, int);
  void (*destruir)(void *);
} Estrutura;

static void *criar_avl(void) { return avl_criar(); }
static void adicionar_avl(void *a, int v) { avl_adicionar(a, v); }
static void remover_avl(void *a, int v) { avl_remover(a, v); }
static void destruir_avl(void *a) { avl_destruir(a); }

static void *criar_rn(void) { return rn_criar(); }
static void adicionar_rn(void *a, int v) { rn_adicionar(a, v); }
static void remover_rn(void *a, int v) { rn_remover(a, v); }
static void destruir_rn(void *a) { rn_destruir(a); }

static const Estrutura estruturas[] = {
    {"avl", criar_avl, adicionar_avl, remover_avl, destruir_avl},
    {"rn", criar_rn, adicionar_rn, remover_rn, destruir_rn},
};

static void embaralhar(int *v, int n) {
  for (int i = n - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int t = v[i];
    v[i] = v[j];
    v[j] = t;
  }
}

static int uso(void) {
  fprintf(stderr, "uso: arvores <estrutura> <semente> <n> [n ...]\n");
  return 2;
}

int main(int argc, char **argv) {
  if (argc < 4) {
    return uso();
  }

  const Estrutura *estrutura = NULL;
  for (size_t i = 0; i < sizeof(estruturas) / sizeof(estruturas[0]); i++) {
    if (strcmp(argv[1], estruturas[i].nome) == 0) {
      estrutura = &estruturas[i];
    }
  }
  if (estrutura == NULL) {
    return uso();
  }

  int quantos = argc - 3;
  int *tamanhos = malloc(sizeof(int) * quantos);
  int maior = 0;
  for (int i = 0; i < quantos; i++) {
    tamanhos[i] = atoi(argv[i + 3]);
    if (tamanhos[i] <= 0) {
      free(tamanhos);
      return uso();
    }
    if (tamanhos[i] > maior) {
      maior = tamanhos[i];
    }
  }

  srand((unsigned)strtoul(argv[2], NULL, 10));
  int *chaves = malloc(sizeof(int) * maior);
  int *remocao = malloc(sizeof(int) * maior);

  for (int t = 0; t < quantos; t++) {
    int n = tamanhos[t];
    for (int i = 0; i < n; i++) {
      chaves[i] = i + 1;
    }
    embaralhar(chaves, n);
    memcpy(remocao, chaves, sizeof(int) * n);
    embaralhar(remocao, n);

    void *arvore = estrutura->criar();
    esforco_zerar();
    for (int i = 0; i < n; i++) {
      estrutura->adicionar(arvore, chaves[i]);
    }
    Esforco adicao = esforco;

    esforco_zerar();
    for (int i = 0; i < n; i++) {
      estrutura->remover(arvore, remocao[i]);
    }
    printf("%d,%llu,%llu,%llu,%llu\n", n, adicao.busca, adicao.ajuste,
           esforco.busca, esforco.ajuste);
    estrutura->destruir(arvore);
  }

  free(remocao);
  free(chaves);
  free(tamanhos);
  return 0;
}
