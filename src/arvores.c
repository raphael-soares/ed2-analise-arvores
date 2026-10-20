#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"
#include "contador.h"

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
  if (argc < 4 || strcmp(argv[1], "avl") != 0) {
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

    Avl *arvore = avl_criar();
    esforco_zerar();
    for (int i = 0; i < n; i++) {
      avl_adicionar(arvore, chaves[i]);
    }
    Esforco adicao = esforco;

    esforco_zerar();
    for (int i = 0; i < n; i++) {
      avl_remover(arvore, remocao[i]);
    }
    printf("%d,%llu,%llu,%llu,%llu\n", n, adicao.busca, adicao.ajuste,
           esforco.busca, esforco.ajuste);
    avl_destruir(arvore);
  }

  free(remocao);
  free(chaves);
  free(tamanhos);
  return 0;
}
