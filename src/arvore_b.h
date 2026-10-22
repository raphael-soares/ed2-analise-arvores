#ifndef ARVORE_B_H
#define ARVORE_B_H

typedef struct BNo {
  int total;
  int *chaves;
  struct BNo **filhos;
  struct BNo *pai;
} BNo;

typedef struct {
  BNo *raiz;
  int ordem;
} ArvoreB;

ArvoreB *b_criar(int ordem);
void b_adicionar(ArvoreB *arvore, int chave);
void b_destruir(ArvoreB *arvore);

#endif
