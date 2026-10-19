#ifndef AVL_H
#define AVL_H

typedef struct AvlNo {
  struct AvlNo *pai;
  struct AvlNo *esquerda;
  struct AvlNo *direita;
  int valor;
  int altura;
} AvlNo;

typedef struct {
  AvlNo *raiz;
} Avl;

Avl *avl_criar(void);
int avl_vazia(Avl *arvore);
void avl_adicionar(Avl *arvore, int valor);
AvlNo *avl_localizar(AvlNo *no, int valor);
void avl_percorrer(AvlNo *no, void (*callback)(int));

#endif
